#pragma once

#include <format>
#include <fstream>
#include <iostream>
#include <span>
#include <stdexcept>

namespace NReader {

enum class EEndianess {
    Little,
    Big
};

template <typename TReader>
concept CBaseReader = requires(TReader reader, std::span<uint8_t> buffer, size_t n) {
    { reader.ReadImpl(buffer) } -> std::same_as<void>;
    { reader.HasMoreDataImpl() } -> std::same_as<bool>;
    { reader.SkipNBytesImpl(n) } -> std::same_as<void>;
};

namespace Impl {

template <typename TDerived>
class ReadCountDebugGuard;

}  // namespace Impl

template <CBaseReader TBase>
class TReader {
   public:
    using TBaseReader = TBase;

    TReader(TBaseReader reader) : Inner_(std::move(reader)) {}

    void Read(std::span<uint8_t> buffer);
    bool HasMoreData();
    void SkipNBytes(size_t n);

    void Read(std::span<char> buffer);
    uint8_t ReadUint8();
    char ReadChar();
    uint16_t ReadUint16(EEndianess endianess);
    uint32_t ReadUint32(EEndianess endianess);
    uint64_t ReadUint64(EEndianess endianess);
    double ReadDouble();
    Impl::ReadCountDebugGuard<TReader> CreateGuard(size_t expectedCount, const char* msg);
    size_t Offset() const;

   private:
    TBaseReader Inner_;
    size_t Offset_ = 0;
};

template <typename T>
concept CReader = requires {
    requires CBaseReader<typename T::TBaseReader>;
    requires std::derived_from<T, TReader<typename T::TBaseReader>>;
};

namespace Impl {

template <typename T>
class ReadCountDebugGuard {
   public:
    ReadCountDebugGuard(T& reader, size_t expectedCount, const char* msg)
        : Reader_(reader), ExpectedCount_(expectedCount), InitialOffset_(reader.Offset()), Msg_(msg) {}

    ~ReadCountDebugGuard() {
        size_t bytesRead = Reader_.Offset() - InitialOffset_;
        if (bytesRead != ExpectedCount_) {
            std::cerr << std::format("{}: Read byte count mismatch. Expected: {}, Actual: {}", Msg_, ExpectedCount_, bytesRead) << std::endl;
            std::terminate();
        }
    }

   private:
    T& Reader_;
    size_t ExpectedCount_;
    size_t InitialOffset_;
    const char* Msg_;
};

class TFileReader {
   public:
    TFileReader(std::ifstream file);
    void ReadImpl(std::span<uint8_t> buffer);
    bool HasMoreDataImpl();
    void SkipNBytesImpl(size_t n);

   private:
    std::ifstream File_;
};

class TArrayRefReader {
   public:
    TArrayRefReader(std::span<const uint8_t> data);
    void ReadImpl(std::span<uint8_t> buffer);
    bool HasMoreDataImpl();
    void SkipNBytesImpl(size_t n);

   private:
    std::span<const uint8_t> Data_;
};

}  // namespace Impl

using TFileReader = TReader<Impl::TFileReader>;
using TArrayRefReader = TReader<Impl::TArrayRefReader>;

inline TFileReader MakeFileReader(std::ifstream file) {
    return TFileReader(Impl::TFileReader(std::move(file)));
}

inline TArrayRefReader MakeArrayRefReader(std::span<const uint8_t> data) {
    return TArrayRefReader(Impl::TArrayRefReader(data));
}

}  // namespace NReader
