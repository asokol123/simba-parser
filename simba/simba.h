#pragma once

#include <simba/message.h>

#include <span>
#include <vector>

namespace NSimba {

using NSBEReader::TSBEMessage;

class TSimbaParser {
   public:
    TSimbaParser() = default;
    std::vector<TSBEMessage> NextFrame(std::span<const uint8_t> frame);
};

}  // namespace NSimba
