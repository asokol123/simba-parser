#pragma once

#include <reader/reader.h>

#include <iostream>
#include <span>

namespace NSimba::NSBEReader {

struct TSBEReader {
   public:
    TSBEReader(std::span<const uint8_t> data) : Inner(NReader::MakeArrayRefReader(data)) {}
    ~TSBEReader() {
        if (ShouldConsumeAll_ && Inner.HasMoreData()) {
            std::cerr << "TSimbaBinaryReader: not all data was read" << std::endl;
            std::terminate();
        }
    }

    void AllowPartialRead() {
        ShouldConsumeAll_ = false;
    }

    template <typename T>
    auto Read() {
        return T::Parse(this);
    }

    NReader::TArrayRefReader Inner;
    bool ShouldConsumeAll_ = true;
};

}  // namespace NSimba::NSBEReader
