#pragma once

#include <simba/macro.h>
#include <simba/types/basic.h>
#include <simba/types/enums.h>
#include <simba/types/json.h>
#include <simba/types/sets.h>
#include <simba/types/struct.h>

namespace NSimba::NSBEReader {

M_CREATE_SIMBA_MESSAGE(
    1,
    THeartbeat);

M_CREATE_SIMBA_MESSAGE(
    2,
    TSequenceReset,
    (NewSeqNo, TuInt32))

M_CREATE_SIMBA_STRUCT(
    TBestPricesEntry,
    (MktBidPx, TDecimal5NULL),
    (MktOfferPx, TDecimal5NULL),
    (MktBidSize, TInt64NULL),
    (MktOfferSize, TInt64NULL),
    (SecurityID, TInt32))

M_CREATE_SIMBA_MESSAGE(
    14,
    TBestPrices,
    (Entries, TRepeatedGroup<TBestPricesEntry>))

M_CREATE_SIMBA_MESSAGE(
    4,
    TEmptyBook,
    (LastMsgSeqNumProcessed, TuInt32NULL))

M_CREATE_SIMBA_MESSAGE(
    15,
    TOrderUpdate,
    (MDEntryID, TInt64),
    (MDEntryPx, TDecimal5),
    (MDEntrySize, TInt64),
    (MDFlags, TMDFlagsSet),
    (MDFlags2, TMDFlags2Set),
    (SecurityID, TInt32),
    (RptSeq, TuInt32),
    (MDUpdateAction, TMDUpdateAction),
    (MDEntryType, TMDEntryType))

M_CREATE_SIMBA_MESSAGE(
    16,
    TOrderExecution,
    (MDEntryID, TInt64),
    (MDEntryPx, TDecimal5NULL),
    (MDEntrySize, TInt64NULL),
    (LastPx, TDecimal5),
    (LastQty, TInt64),
    (TradeID, TInt64),
    (MDFlags, TMDFlagsSet),
    (MDFlags2, TMDFlags2Set),
    (SecurityID, TInt32),
    (RptSeq, TuInt32),
    (MDUpdateAction, TMDUpdateAction),
    (MDEntryType, TMDEntryType))

M_CREATE_SIMBA_STRUCT(
    TOrderBookSnapshotEntry,
    (MDEntryID, TInt64NULL),
    (TransactTime, TuInt64),
    (MDEntryPx, TDecimal5NULL),
    (MDEntrySize, TInt64NULL),
    (TradeID, TInt64NULL),
    (MDFlags, TMDFlagsSet),
    (MDFlags2, TMDFlags2Set),
    (MDEntryType, TMDEntryType))

M_CREATE_SIMBA_MESSAGE(
    17,
    TOrderBookSnapshot,
    (SecurityID, TInt32),
    (LastMsgSeqNumProcessed, TuInt32),
    (RptSeq, TuInt32),
    (ExchangeTradingSessionID, TuInt32),
    (Entries, TRepeatedGroup<TOrderBookSnapshotEntry>))

M_CREATE_SIMBA_STRUCT(
    TSecurityDefinitionMDFeedType,
    (MDFeedType, TString25),
    (MarketDepth, TuInt32NULL),
    (MDBookType, TuInt32NULL))

M_CREATE_SIMBA_STRUCT(
    TSecurityDefinitionUnderlying,
    (UnderlyingSymbol, TString25),
    (UnderlyingBoard, TString4),
    (UnderlyingSecurityID, TInt32NULL),
    (UnderlyingFutureID, TInt32NULL))

M_CREATE_SIMBA_STRUCT(
    TSecurityDefinitionLeg,
    (LegSymbol, TString25),
    (LegSecurityID, TInt32),
    (LegRatioQty, TInt32))

M_CREATE_SIMBA_STRUCT(
    TSecurityDefinitionInstrAttrib,
    (InstrAttribType, TInt32),
    (InstrAttribValue, TString31))

M_CREATE_SIMBA_STRUCT(
    TSecurityDefinitionEvent,
    (EventType, TInt32),
    (EventDate, TuInt32),
    (EventTime, TuInt64))

M_CREATE_SIMBA_MESSAGE(
    18,
    TSecurityDefinition,
    (TotNumReports, TuInt32),
    (Symbol, TString25),
    (SecurityID, TInt32),
    (SecurityIDSource, TSecurityIDSource),
    (SecurityAltID, TString25),
    (SecurityAltIDSource, TSecurityAltIDSource),
    (SecurityType, TString4),
    (CFICode, TString6),
    (StrikePrice, TDecimal5NULL),
    (ContractMultiplier, TInt32NULL),
    (SecurityTradingStatus, TSecurityTradingStatus),
    (Currency, TString3),
    (MarketID, TMarketID),
    (MarketSegmentID, TMarketSegmentID),
    (TradingSessionID, TTradingSessionID),
    (ExchangeTradingSessionID, TInt32NULL),
    (Volatility, TDecimal5NULL),
    (HighLimitPx, TDecimal5NULL),
    (LowLimitPx, TDecimal5NULL),
    (MinPriceIncrement, TDecimal5NULL),
    (MinPriceIncrementAmount, TDecimal5NULL),
    (InitialMarginOnBuy, TDecimal2NULL),
    (InitialMarginOnSell, TDecimal2NULL),
    (InitialMarginSyntetic, TDecimal2NULL),
    (TheorPrice, TDecimal5NULL),
    (TheorPriceLimit, TDecimal5NULL),
    (UnderlyingQty, TDecimal5NULL),
    (UnderlyingCurrency, TString3),
    (MaturityDate, TuInt32NULL),
    (MaturityTime, TuInt32NULL),
    (Flags, TFlagsSet),
    (MinPriceIncrementAmountCurr, TDecimal5NULL),
    (SettlPriceOpen, TDecimal5NULL),
    (ValuationMethod, TString4),
    (RiskFreeRate, TDoubleNULL),
    (FixedSpotDiscount, TDoubleNULL),
    (ProjectedSpotDiscount, TDoubleNULL),
    (SettlCurrency, TString3),
    (NegativePrices, TNegativePrices),
    (DerivativeContractMultiplier, TInt32NULL),
    (InterestRateRiskUp, TDoubleNULL),
    (InterestRateRiskDown, TDoubleNULL),
    (RiskFreeRate2, TDoubleNULL),
    (InterestRate2RiskUp, TDoubleNULL),
    (InterestRate2RiskDown, TDoubleNULL),
    (MDFeedTypes, TRepeatedGroup<TSecurityDefinitionMDFeedType>),
    (Underlyings, TRepeatedGroup<TSecurityDefinitionUnderlying>),
    (Legs, TRepeatedGroup<TSecurityDefinitionLeg>),
    (InstrAttribs, TRepeatedGroup<TSecurityDefinitionInstrAttrib>),
    (Events, TRepeatedGroup<TSecurityDefinitionEvent>),
    (SecurityDesc, TUtf8String),
    (QuotationList, TVarString))

M_CREATE_SIMBA_MESSAGE(
    9,
    TSecurityStatus,
    (SecurityID, TInt32),
    (SecurityIDSource, TSecurityIDSource),
    (Symbol, TString25),
    (SecurityTradingStatus, TSecurityTradingStatus),
    (HighLimitPx, TDecimal5NULL),
    (LowLimitPx, TDecimal5NULL),
    (InitialMarginOnBuy, TDecimal2NULL),
    (InitialMarginOnSell, TDecimal2NULL),
    (InitialMarginSyntetic, TDecimal2NULL))

M_CREATE_SIMBA_MESSAGE(
    10,
    TSecurityDefinitionUpdateReport,
    (SecurityID, TInt32),
    (SecurityIDSource, TSecurityIDSource),
    (Volatility, TDecimal5NULL),
    (TheorPrice, TDecimal5NULL),
    (TheorPriceLimit, TDecimal5NULL))

M_CREATE_SIMBA_MESSAGE(
    11,
    TTradingSessionStatus,
    (TradSesOpenTime, TuInt64),
    (TradSesCloseTime, TuInt64),
    (TradSesIntermClearingStartTime, TuInt64NULL),
    (TradSesIntermClearingEndTime, TuInt64NULL),
    (TradingSessionID, TTradingSessionID),
    (ExchangeTradingSessionID, TInt32NULL),
    (TradSesStatus, TTradSesStatus),
    (MarketID, TMarketID),
    (MarketSegmentID, TMarketSegmentID),
    (TradSesEvent, TTradSesEvent))

M_CREATE_SIMBA_STRUCT(
    TDiscreteAuctionUnderlying,
    (UnderlyingSymbol, TVarString))

M_CREATE_SIMBA_MESSAGE(
    13,
    TDiscreteAuction,
    (TradSesOpenTime, TuInt64),
    (TradSesCloseTimeFrom, TuInt64),
    (TradSesCloseTimeTill, TuInt64),
    (AuctionID, TInt64),
    (ExchangeTradingSessionID, TInt32),
    (EventIDOpen, TInt32),
    (EventIDClose, TInt32),
    (Underlyings, TRepeatedGroup<TDiscreteAuctionUnderlying>))

M_CREATE_SIMBA_STRUCT(
    TSecurityMassStatusRelatedSym,
    (SecurityID, TInt32),
    (SecurityIDSource, TSecurityIDSource),
    (SecurityTradingStatus, TSecurityTradingStatus))

M_CREATE_SIMBA_MESSAGE(
    19,
    TSecurityMassStatus,
    (RelatedSym, TRepeatedGroup2<TSecurityMassStatusRelatedSym>))

M_CREATE_SIMBA_MESSAGE(
    1000,
    TLogon)

M_CREATE_SIMBA_MESSAGE(
    1001,
    TLogout,
    (Text, TString256))

M_CREATE_SIMBA_MESSAGE(
    1002,
    TMarketDataRequest,
    (ApplBegSeqNum, TuInt32),
    (ApplEndSeqNum, TuInt32))

M_DECLARE_SIMBA_SBE_MESSAGE(
    THeartbeat,
    TSequenceReset,
    TBestPrices,
    TEmptyBook,
    TOrderUpdate,
    TOrderExecution,
    TOrderBookSnapshot,
    TSecurityDefinition,
    TSecurityStatus,
    TSecurityDefinitionUpdateReport,
    TTradingSessionStatus,
    TDiscreteAuction,
    TSecurityMassStatus,
    TLogon,
    TLogout,
    TMarketDataRequest)
template <>
inline ::NJson::TJson ToJson(const TSBEMessage& value) {
    return std::visit([](auto&& value) { return ToJson(value); }, value);
}

}  // namespace NSimba::NSBEReader

namespace NJson {

template <>
inline TJson TJson::ToJson(const NSimba::NSBEReader::TSBEMessage& value) {
    return NSimba::NSBEReader::ToJson(value);
}

}  // namespace NJson
