#pragma once

#include <simba/macro.h>
#include <simba/types/basic.h>

namespace NSimba::NSBEReader {

M_CREATE_SIMBA_SET(
    TuInt16,
    TMsgFlagsSet,
    (LastFragment, 0),
    (StartOfSnapshot, 1),
    (EndOfSnapshot, 2),
    (IncrementalPacket, 3),
    (PossDupFlag, 4));

M_CREATE_SIMBA_SET(
    TuInt64,
    TMDFlagsSet,
    (Day, 0),
    (IOC, 1),
    (NonQuote, 2),
    (EndOfTransaction, 12),
    (DueToCrossCancel, 13),
    (SecondLeg, 14),
    (FOK, 19),
    (Replace, 20),
    (Cancel, 21),
    (MassCancel, 22),
    (Negotiated, 26),
    (MultiLeg, 27),
    (CrossTrade, 29),
    (NegotiatedMatchByRef, 31),
    (COD, 32),
    (ActiveSide, 41),
    (PassiveSide, 42),
    (Synthetic, 45),
    (RFS, 46),
    (SyntheticPassive, 57),
    (BOC, 60),
    (DuringDiscreteAuction, 62));

M_CREATE_SIMBA_SET(
    TuInt64,
    TMDFlags2Set,
    (Zero, 0));

M_CREATE_SIMBA_SET(
    TuInt64,
    TFlagsSet,
    (EveningOrMorningSession, 0),
    (AnonymousTrading, 4),
    (PrivateTrading, 5),
    (DaySession, 6),
    (MultiLeg, 8),
    (Collateral, 18),
    (IntradayExercise, 19));

}  // namespace NSimba::NSBEReader
