#include <iostream>
#include <ios>
#include "stdlib.h"
#include "SystemUtils.h"
#include "Packet.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include "PcapFileDevice.h"
#include "itch/itch_parser.h"

//for measurement
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    pcpp::IFileReaderDevice* reader = pcpp::IFileReaderDevice::getReader("../pcaps/itch.pcap");

    if (reader == nullptr)
    {
        std::cerr << "Cannot determine reader for file type" << std::endl;
        return 1;
    }

    if (!reader->open())
    {
        std::cerr << "Cannot open pcap file for reading" << std::endl;
        return 1;
    }

    ITCHParser itchParser;

    auto t1 = high_resolution_clock::now();
    pcpp::RawPacket rawPacket;
    while (reader->getNextPacket(rawPacket))
    {
        pcpp::Packet parsedPacket(&rawPacket);
        for (pcpp::Layer* curLayer = parsedPacket.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
        {         
            if((curLayer->getProtocol() == pcpp::UDP)) {
                uint8_t* buffer = curLayer->getLayerPayload();
                auto size = curLayer->getLayerPayloadSize();

                itchParser.parse(buffer, size);
            }
            
        }
    }
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "total parsing duration: "<< ms_double.count() << "ms\n";

    // close the file reader, we don't need it anymore
    reader->close();

    return 0;
}