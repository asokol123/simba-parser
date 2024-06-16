#include "reader.h"

#include <array>

namespace NReader {
template <CBaseReader TBaseReader>
void TReader<TBaseReader>::Read(std::span<uint8_t> buffer) {
    Inner_.ReadImpl(buffer);
    Offset_ += buffer.size();
}

template <CBaseReader TBaseReader>
void TReader<TBaseReader>::Read(std::span<char> buffer) {
    std::span<uint8_t> byteBuffer(reinterpret_cast<uint8_t*>(buffer.data()), buffer.size());
    Read(byteBuffer);
}

template <CBaseReader TBaseReader>
bool TReader<TBaseReader>::HasMoreData() {
    return Inner_.HasMoreDataImpl();
}

template <CBaseReader TBaseReader>
void TReader<TBaseReader>::SkipNBytes(size_t n) {
    Inner_.SkipNBytesImpl(n);
    Offset_ += n;
}

template <CBaseReader TBaseReader>
uint8_t TReader<TBaseReader>::ReadUint8() {
    std::array<uint8_t, 1> buffer;
    Read(buffer);

    return buffer[0];
}

template <CBaseReader TBaseReader>
char TReader<TBaseReader>::ReadChar() {
    std::array<char, 1> buffer;
    Read(buffer);

    return buffer[0];
}

template <CBaseReader TBaseReader>
double TReader<TBaseReader>::ReadDouble() {
    union {
        std::array<uint8_t, 8> buffer;
        double doubleRepresentation;
    } u;
    Read(u.buffer);

    return u.doubleRepresentation;
}

template <CBaseReader TBaseReader>
uint16_t TReader<TBaseReader>::ReadUint16(EEndianess endianess) {
    std::array<uint8_t, 2> buffer;
    Read(buffer);

    if (endianess == EEndianess::Little) {
        return (static_cast<uint16_t>(buffer[1]) << 8) | static_cast<uint16_t>(buffer[0]);
    } else {
        return (static_cast<uint16_t>(buffer[0]) << 8) | static_cast<uint16_t>(buffer[1]);
    }
}

template <CBaseReader TBaseReader>
uint32_t TReader<TBaseReader>::ReadUint32(EEndianess endianess) {
    std::array<uint8_t, 4> buffer;
    Read(buffer);

    if (endianess == EEndianess::Little) {
        return (static_cast<uint32_t>(buffer[3]) << 24) |
               (static_cast<uint32_t>(buffer[2]) << 16) |
               (static_cast<uint32_t>(buffer[1]) << 8) |
               static_cast<uint32_t>(buffer[0]);
    } else {
        return (static_cast<uint32_t>(buffer[0]) << 24) |
               (static_cast<uint32_t>(buffer[1]) << 16) |
               (static_cast<uint32_t>(buffer[2]) << 8) |
               static_cast<uint32_t>(buffer[3]);
    }
}

template <CBaseReader TBaseReader>
uint64_t TReader<TBaseReader>::ReadUint64(EEndianess endianess) {
    std::array<uint8_t, 8> buffer;
    Read(buffer);

    if (endianess == EEndianess::Little) {
        return (static_cast<uint64_t>(buffer[7]) << 56) |
               (static_cast<uint64_t>(buffer[6]) << 48) |
               (static_cast<uint64_t>(buffer[5]) << 40) |
               (static_cast<uint64_t>(buffer[4]) << 32) |
               (static_cast<uint64_t>(buffer[3]) << 24) |
               (static_cast<uint64_t>(buffer[2]) << 16) |
               (static_cast<uint64_t>(buffer[1]) << 8) |
               static_cast<uint64_t>(buffer[0]);
    } else {
        return (static_cast<uint64_t>(buffer[0]) << 56) |
               (static_cast<uint64_t>(buffer[1]) << 48) |
               (static_cast<uint64_t>(buffer[2]) << 40) |
               (static_cast<uint64_t>(buffer[3]) << 32) |
               (static_cast<uint64_t>(buffer[4]) << 24) |
               (static_cast<uint64_t>(buffer[5]) << 16) |
               (static_cast<uint64_t>(buffer[6]) << 8) |
               static_cast<uint64_t>(buffer[7]);
    }
}

template <CBaseReader TBaseReader>
size_t TReader<TBaseReader>::Offset() const {
    return Offset_;
}

template <CBaseReader TBaseReader>
Impl::ReadCountDebugGuard<TReader<TBaseReader>> TReader<TBaseReader>::CreateGuard(size_t expectedCount, const char* msg) {
    return Impl::ReadCountDebugGuard(*this, expectedCount, msg);
}

namespace Impl {

TFileReader::TFileReader(std::ifstream file) : File_(std::move(file)) {}

void TFileReader::ReadImpl(std::span<uint8_t> buffer) {
    File_.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    if (File_.gcount() != static_cast<std::streamsize>(buffer.size())) {
        throw std::runtime_error("Failed to read whole buffer");
    }
}

bool TFileReader::HasMoreDataImpl() {
    return File_.peek() != std::ifstream::traits_type::eof();
}

void TFileReader::SkipNBytesImpl(size_t n) {
    File_.seekg(n, std::ios_base::cur);
}

TArrayRefReader::TArrayRefReader(std::span<const uint8_t> data) : Data_(data) {}

void TArrayRefReader::ReadImpl(std::span<uint8_t> buffer) {
    if (buffer.size() > Data_.size()) {
        throw std::runtime_error("Read exceeds available data");
    }
    std::copy(Data_.begin(), Data_.begin() + buffer.size(), buffer.begin());
    Data_ = Data_.subspan(buffer.size());
}

bool TArrayRefReader::HasMoreDataImpl() {
    return !Data_.empty();
}

void TArrayRefReader::SkipNBytesImpl(size_t n) {
    if (n > Data_.size()) {
        throw std::runtime_error("Skip exceeds available data");
    }
    Data_ = Data_.subspan(n);
}

}  // namespace Impl

template class NReader::TReader<NReader::Impl::TArrayRefReader>;
template class NReader::TReader<NReader::Impl::TFileReader>;

}  // namespace NReader
