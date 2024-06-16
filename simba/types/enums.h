#pragma once

#include <simba/macro.h>
#include <simba/types/basic.h>

namespace NSimba::NSBEReader {

M_CREATE_SIMBA_ENUM(
    TMDUpdateAction,
    TuInt8,
    (New, 0),
    (Change, 1),
    (Delete, 2));

M_CREATE_SIMBA_ENUM(
    TMDEntryType,
    TChar,
    (Bid, '0'),
    (Offer, '1'),
    (EmptyBook, 'J'));

M_CREATE_SIMBA_ENUM(
    TSecurityAltIDSource,
    TChar,
    (ISIN, '4'),
    (ExchangeSymbol, '8'));

M_CREATE_SIMBA_NULL_ENUM(
    TSecurityTradingStatus,
    TuInt8NULL,
    (TradingHalt, 2),
    (ReadyToTrade, 17),
    (NotAvailableForTrading, 18),
    (NotTradedOnThisMarket, 19),
    (UnknownOrInvalid, 20),
    (PreOpen, 21),
    (DiscreteAuctionOpen, 119),
    (DiscreteAuctionClose, 121),
    (InstrumentHalt, 122));

M_CREATE_SIMBA_NULL_ENUM(
    TTradingSessionID,
    TuInt8NULL,
    (Day, 1),
    (Morning, 3),
    (Evening, 5))

M_CREATE_SIMBA_ENUM(
    TMarketSegmentID,
    TChar,
    (Derivatives, 'D'))

M_CREATE_SIMBA_ENUM(
    TTradSesStatus,
    TuInt8,
    (Halted, 1),
    (Open, 2),
    (Closed, 3),
    (PreOpen, 4))

M_CREATE_SIMBA_NULL_ENUM(
    TTradSesEvent,
    TuInt8NULL,
    (TradingResumes, 0),
    (ChangeOfTradingSession, 1),
    (ChangeOfTradingStatus, 3))

M_CREATE_SIMBA_ENUM(
    TNegativePrices,
    TuInt8,
    (NotEligible, 0),
    (Eligible, 1))

}  // namespace NSimba::NSBEReader
