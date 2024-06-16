#pragma once

#include <simba/sbe_reader.h>
#include <simba/types/json.h>

namespace NSimba::NSBEReader {

struct TuInt8 {};
template <>
inline auto TSBEReader::Read<TuInt8>() {
    return Inner.ReadUint8();
}

struct TuInt8NULL {};
template <>
inline auto TSBEReader::Read<TuInt8NULL>() {
    uint8_t value = Inner.ReadUint8();
    return value == std::numeric_limits<uint8_t>::max() ? std::nullopt : std::make_optional(value);
}

struct TuInt16 {};
template <>
inline auto TSBEReader::Read<TuInt16>() {
    return Inner.ReadUint16(NReader::EEndianess::Little);
}

struct TuInt32 {};
template <>
inline auto TSBEReader::Read<TuInt32>() {
    return Inner.ReadUint32(NReader::EEndianess::Little);
}

struct TuInt32NULL {};
template <>
inline auto TSBEReader::Read<TuInt32NULL>() {
    uint32_t value = Inner.ReadUint32(NReader::EEndianess::Little);
    return value == std::numeric_limits<uint32_t>::max() ? std::nullopt : std::make_optional(value);
}

struct TuInt64 {};
template <>
inline auto TSBEReader::Read<TuInt64>() {
    return Inner.ReadUint64(NReader::EEndianess::Little);
}

struct TuInt64NULL {};
template <>
inline auto TSBEReader::Read<TuInt64NULL>() {
    uint64_t value = Inner.ReadUint64(NReader::EEndianess::Little);
    return value == std::numeric_limits<uint64_t>::max() ? std::nullopt : std::make_optional(value);
}

struct TInt32 {};
template <>
inline auto TSBEReader::Read<TInt32>() {
    int32_t result = Inner.ReadUint32(NReader::EEndianess::Little);
    return result;
}

struct TInt32NULL {};
template <>
inline auto TSBEReader::Read<TInt32NULL>() {
    int32_t value = Inner.ReadUint32(NReader::EEndianess::Little);
    return value == std::numeric_limits<int32_t>::min() ? std::nullopt : std::make_optional(value);
}

struct TInt64 {};
template <>
inline auto TSBEReader::Read<TInt64>() {
    int64_t result = Inner.ReadUint64(NReader::EEndianess::Little);
    return result;
}

struct TInt64NULL {};
template <>
inline auto TSBEReader::Read<TInt64NULL>() {
    int64_t value = Inner.ReadUint64(NReader::EEndianess::Little);
    return value == std::numeric_limits<int64_t>::min() ? std::nullopt : std::make_optional(value);
}

struct TChar {};
template <>
inline auto TSBEReader::Read<TChar>() {
    return Inner.ReadChar();
}

template <size_t N>
struct TString {
    std::array<char, N> Data_;
    TString() = default;
    TString(std::string_view data) {
        if (data.size() != N) {
            throw std::invalid_argument("invalid string size");
        }
        std::copy(data.begin(), data.end(), Data_.begin());
    }

    std::string_view GetStringView() const {
        return std::string_view(Data_.data(), N);
    }
    operator std::string_view() const {
        return GetStringView();
    }

    static TString Parse(TSBEReader* reader) {
        TString result;
        reader->Inner.Read(result.Data_);
        return result;
    }
};
using TString3 = TString<3>;
using TString4 = TString<4>;
using TString6 = TString<6>;
using TString25 = TString<25>;
using TString31 = TString<31>;
using TString256 = TString<256>;
template <size_t N>
::NJson::TJson ToJson(const TString<N>& value) {
    return ::NJson::TJson(value.GetStringView());
}

struct TDoubleNULL {};
template <>
inline auto TSBEReader::Read<TDoubleNULL>() {
    double inner = Inner.ReadDouble();
    return std::isnan(inner) ? std::nullopt : std::make_optional<double>(inner);
}

struct TSecurityIDSource {};
template <>
inline auto TSBEReader::Read<TSecurityIDSource>() {
    return '8';
}

struct TMarketID {};
template <>
inline auto TSBEReader::Read<TMarketID>() {
    auto result = TString4("MOEX");
    return result;
}

}  // namespace NSimba::NSBEReader
