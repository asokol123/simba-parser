#pragma once

#include <simba/macro.h>
#include <simba/types/basic.h>
#include <simba/types/sets.h>

namespace NSimba::NSBEReader {

M_CREATE_SIMBA_STRUCT(
    TMarketDataPacketHeader,
    (MsgSeqNum, TuInt32),
    (MsgSize, TuInt16),
    (MsgFlags, TMsgFlagsSet),
    (SendingTime, TuInt64))

M_CREATE_SIMBA_STRUCT(
    TIncrementalPacketHeader,
    (TransactTime, TuInt64),
    (ExchangeTradingSessionID, TuInt32))

M_CREATE_SIMBA_STRUCT(
    TMessageHeader,
    (BlockLength, TuInt16),
    (TemplateId, TuInt16),
    (SchemaId, TuInt16),
    (Version, TuInt16))

M_CREATE_SIMBA_STRUCT(
    TGroupSize,
    (blockLength, TuInt16),
    (numInGroup, TuInt8))

M_CREATE_SIMBA_STRUCT(
    TGroupSize2,
    (blockLength, TuInt16),
    (numInGroup, TuInt16))

using TUtf8String = std::string;
using TVarString = std::string;
template <>
inline auto TSBEReader::Read<std::string>() {
    uint16_t length = Read<TuInt16>();
    std::string result;
    result.resize(length);
    Inner.Read(result);
    return result;
}

template <int8_t N>
struct TDecimal {
    int64_t Value;

    double AsDouble() const {
        return std::ldexp(Value, -N);
    }
};
template <int8_t N>
::NJson::TJson ToJson(const TDecimal<N>& value) {
    return std::move(::NJson::TJsonStructWriter(std::format("Decimal{}", N)).Field("Value", value.Value)).Json();
}

struct TDecimal5 {};
template <>
inline auto TSBEReader::Read<TDecimal5>() {
    return TDecimal<5>{.Value = Read<TInt64>()};
}
struct TDecimal5NULL {};
template <>
inline auto TSBEReader::Read<TDecimal5NULL>() {
    auto value = Read<TInt64NULL>();
    if (!value) {
        return std::optional<TDecimal<5>>(std::nullopt);
    }
    return std::make_optional<TDecimal<5>>(TDecimal<5>{.Value = *value});
}
struct TDecimal2NULL {};
template <>
inline auto TSBEReader::Read<TDecimal2NULL>() {
    auto value = Read<TInt64NULL>();
    if (!value) {
        return std::optional<TDecimal<2>>(std::nullopt);
    }
    return std::make_optional<TDecimal<2>>(TDecimal<2>{.Value = *value});
}

template <typename TSize, typename TElem>
struct TRepeatedGroupImpl {
    static std::vector<TElem> Parse(TSBEReader* reader) {
        auto groupSize = reader->Read<TSize>();
        std::vector<TElem> result;
        result.reserve(groupSize.numInGroup);
        for (size_t i = 0; i < groupSize.numInGroup; ++i) {
            result.push_back(reader->Read<TElem>());
        }
        return result;
    }
};

template <typename TElem>
using TRepeatedGroup = TRepeatedGroupImpl<TGroupSize, TElem>;
template <typename TElem>
using TRepeatedGroup2 = TRepeatedGroupImpl<TGroupSize2, TElem>;

}  // namespace NSimba::NSBEReader
