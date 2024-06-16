#include <simba/sbe_reader.h>
#include <simba/simba.h>

namespace NSimba {

using namespace NSBEReader;

std::vector<TSBEMessage> TSimbaParser::NextFrame(std::span<const uint8_t> frame) {
    auto reader = TSBEReader(frame);
    auto mdpHeader = reader.Read<TMarketDataPacketHeader>();
    std::vector<TSBEMessage> result;
    if (mdpHeader.MsgFlags.IsIncrementalPacket()) {
        reader.Read<TIncrementalPacketHeader>();
        while (reader.Inner.HasMoreData()) {
            result.push_back(reader.Read<TSBEMessage>());
        }
    } else {
        result.push_back(reader.Read<TSBEMessage>());
    }
    return result;
}

}  // namespace NSimba
