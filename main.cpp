#include <iostream>

#include "pcap.h"
#include "reader/reader.h"
#include "simba/simba.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    auto fileReader = NReader::MakeFileReader(std::move(file));
    auto udpReader = NPcap::TUDPReader(std::move(fileReader));
    NSimba::TSimbaParser parser;
    while (udpReader.Next()) {
        auto udpFrame = udpReader.GetFrame();
        auto messages = parser.NextFrame(udpFrame);
        for (const auto& message : messages) {
            std::cout << NSimba::NSBEReader::ToJson(message) << "\n";
        }
    }
}
