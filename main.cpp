#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "Packet.h"
#include "PcapFileDevice.h"
#include "itch/itch_parser.h"


int main(int argc, char* argv[])
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    if(argc < 3) {
        std::cout << "Please enter a file!";
    }

    std::string inputFile{argv[1]};
    std::string outputFile{argv[2]};

    pcpp::IFileReaderDevice* reader = pcpp::IFileReaderDevice::getReader(inputFile.data());
    ITCHParser itchParser(outputFile);

    if (reader == nullptr)
    {
        std::cerr << "Cannot determine reader for file type\n";
        return 1;
    }

    if (!reader->open())
    {
        std::cerr << "Cannot open pcap file for reading\n";
        return 1;
    }


    auto t1 = high_resolution_clock::now();
    pcpp::RawPacket rawPacket;
    while (reader->getNextPacket(rawPacket))
    {
        pcpp::Packet parsedPacket(&rawPacket);
        for (pcpp::Layer* curLayer = parsedPacket.getFirstLayer(); curLayer != nullptr; curLayer = curLayer->getNextLayer())
        {         
            if((curLayer->getProtocol() == pcpp::UDP)) {
                itchParser.parse(curLayer->getLayerPayload());
            }
            
        }
    }
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "process finished! total parsing duration: "<< ms_double.count() << "ms\n";

    reader->close();

    return 0;
}