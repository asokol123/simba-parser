#include <reader/reader.h>

#include <cstdint>
#include <format>

namespace NPcap {

enum class ETimestampFormat {
    Nanoseconds,
    Microseconds
};

struct TPcapHeader {
    NReader::EEndianess BytesOrder;
    ETimestampFormat TimestampFormat;

    template <NReader::CReader TReader>
    static TPcapHeader Parse(TReader& reader) {
        auto guard = reader.CreateGuard(24, "PcapHeader::Parse");

        TPcapHeader header;
        uint32_t magicNumber = reader.ReadUint32(NReader::EEndianess::Big);

        switch (magicNumber) {
            case 0xA1B2C3D4:
                header.BytesOrder = NReader::EEndianess::Big;
                header.TimestampFormat = ETimestampFormat::Microseconds;
                break;
            case 0xD4C3B2A1:
                header.BytesOrder = NReader::EEndianess::Little;
                header.TimestampFormat = ETimestampFormat::Microseconds;
                break;
            case 0xA1B23C4D:
                header.BytesOrder = NReader::EEndianess::Big;
                header.TimestampFormat = ETimestampFormat::Nanoseconds;
                break;
            case 0x4D3CB2A1:
                header.BytesOrder = NReader::EEndianess::Little;
                header.TimestampFormat = ETimestampFormat::Nanoseconds;
                break;
            default:
                throw std::runtime_error(std::format("Invalid pcap file. Magic number mismatch: {:#X}", magicNumber));
        }

        reader.SkipNBytes(12);  // Skip version and reserved fields
        reader.SkipNBytes(4);   // Skip SnapLen
        uint16_t fcf = reader.ReadUint16(NReader::EEndianess::Little);
        if (fcf & (1 << 3)) {
            throw std::runtime_error("fcf f bit is set, this pcap is not supported");
        }
        reader.SkipNBytes(2);  // Skip LinkType

        return header;
    }
};

struct UDPHeader {
    uint64_t TimestampNanoseconds;
    uint16_t SourcePort;
    uint16_t DestinationPort;
    uint16_t Length;
    uint16_t Checksum;

    template <NReader::CReader TReader>
    static UDPHeader Parse(TReader& reader, const TPcapHeader& fileHeader) {
        UDPHeader packet;
        uint32_t TimestampSeconds = reader.ReadUint32(fileHeader.BytesOrder);
        uint32_t TimestampSubseconds = reader.ReadUint32(fileHeader.BytesOrder);
        if (fileHeader.TimestampFormat == ETimestampFormat::Microseconds) {
            packet.TimestampNanoseconds = static_cast<uint64_t>(TimestampSeconds) * 1000000000 + static_cast<uint64_t>(TimestampSubseconds) * 1000;
        } else {
            packet.TimestampNanoseconds = static_cast<uint64_t>(TimestampSeconds) * 1000000000 + TimestampSubseconds;
        }

        uint32_t capturedLength = reader.ReadUint32(fileHeader.BytesOrder);
        uint32_t originalLength = reader.ReadUint32(fileHeader.BytesOrder);
        if (capturedLength != originalLength) {
            throw std::runtime_error("Captured length does not match original length");
        }

        reader.SkipNBytes(14);  // Skip Ethernet header
        reader.SkipNBytes(20);  // Skip IP header

        // UDP header uses network byte order AKA big endian
        packet.SourcePort = reader.ReadUint16(NReader::EEndianess::Big);
        packet.DestinationPort = reader.ReadUint16(NReader::EEndianess::Big);
        packet.Length = reader.ReadUint16(NReader::EEndianess::Big) - 8;  // length include header length
        packet.Checksum = reader.ReadUint16(NReader::EEndianess::Big);

        if (packet.Length != originalLength - 14 - 20 - 8) {
            throw std::runtime_error(std::format("Length mismatch: {} != {}", packet.Length, originalLength - 14 - 20 - 8));
        }

        if (packet.Length > 3000) {  // in reality we would have 1500 max, 3000 just in case
            throw std::runtime_error("Packet length is too long");
        }

        return packet;
    }
};

template <NReader::CReader TReader>
class TUDPReader {
   public:
    TUDPReader(TReader reader)
        : Reader_(std::move(reader)), Header_(TPcapHeader::Parse(Reader_)) {
    }

    bool Next() {
        if (!Reader_.HasMoreData()) {
            return false;
        }
        Frame_.clear();
        UDPHeader header = UDPHeader::Parse(Reader_, Header_);
        Frame_.resize(header.Length);
        Reader_.Read(Frame_);
        return true;
    }

    std::span<const uint8_t> GetFrame() const {
        return Frame_;
    }

   private:
    TReader Reader_;
    TPcapHeader Header_;
    std::vector<uint8_t> Frame_;
};

}  // namespace NPcap
