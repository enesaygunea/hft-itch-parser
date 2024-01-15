#include <iostream>
#include <cassert>
#include "itch/itch_parser.h"
#include "utils/endian.h"
#include "utils/writechar.h"

ITCHParser::ITCHParser(const std::string& filename) : _messages(0), _errors(0) {
    _outFile.open(filename, std::ios::binary);
    if (!_outFile.is_open()) {
        std::cerr << "Cannot open file: " << filename << "\n";
        exit(1);
    }
    _writeFileHeader();
}

ITCHParser::~ITCHParser() {
    if (_outFile.is_open()) {
        _outFile.close();
    }
}

bool ITCHParser::parseSecondMessage(void* buffer, size_t size) {
    assert((size == 5) && "Invalid size of the ITCH message type 'T'");
    uint8_t* data = (uint8_t*)buffer;

    SecondsMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.second);

    return true;
}

bool ITCHParser::parseOrderBookDirectoryMessage(void* buffer, size_t size) {
    assert((size == 129) && "Invalid size of the ITCH message type 'R'");

    uint8_t* data = (uint8_t*)buffer;
    OrderBookDirectoryMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    data += ReadString(data, message.symbol);
    data += ReadString(data, message.longName);
    data += ReadString(data, message.ISIN);
    message.financialProduct = *data++;
    data += ReadString(data, message.tradingCurrency);
    data += Common::Endian::ReadBigEndian(data, message.numberOfDecimalsInPrice);
    data += Common::Endian::ReadBigEndian(data, message.numberOfDecimalsInNominalValue);
    data += Common::Endian::ReadBigEndian(data, message.oddLotSize);
    data += Common::Endian::ReadBigEndian(data, message.roundLotSize);
    data += Common::Endian::ReadBigEndian(data, message.blockLotSize);
    data += Common::Endian::ReadBigEndian(data, message.nominalValue);
    message.numberOfLegs = *data++;
    data += Common::Endian::ReadBigEndian(data, message.underlyingOrderBookID);
    data += Common::Endian::ReadLittleEndian(data, message.strikePrice);
    data += Common::Endian::ReadBigEndian(data, message.expirationDate);
    data += Common::Endian::ReadBigEndian(data, message.numberOfDecimalsInStrikePrice);
    message.putOrCall = *data++;

    return true;
}

bool ITCHParser::parseCombinationOrderBookLegMessage(void* buffer, size_t size) {
    assert((size == 18) && "Invalid size of the ITCH message type 'M'");
    uint8_t* data = (uint8_t*)buffer;

    CombinationOrderBookLegMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.combinationOrderBookID);
    data += Common::Endian::ReadBigEndian(data, message.legOrderBookID);
    message.legSide = *data++;
    data += Common::Endian::ReadBigEndian(data, message.legRatio);

    return true;
}

bool ITCHParser::parseTickSizeTableEntryMessage(void* buffer, size_t size) {
    assert((size == 25) && "Invalid size of the ITCH message type 'L'");
    uint8_t* data = (uint8_t*)buffer;

    TickSizeTableEntryMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    data += Common::Endian::ReadBigEndian(data, message.tickSize);
    data += Common::Endian::ReadBigEndian(data, message.priceFrom);
    data += Common::Endian::ReadBigEndian(data, message.priceTo);

    return true;
}

bool ITCHParser::parseSystemEventMessage(void* buffer, size_t size) {
    assert((size == 6) && "Invalid size of the ITCH message type 'S'");
    uint8_t* data = (uint8_t*)buffer;
    
    SystemEventMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    message.eventCode = *data++;

    return true;
}

bool ITCHParser::parseOrderBookStateMessage(void* buffer, size_t size) {
    assert((size == 29) && "Invalid size of the ITCH message type 'O'");
    uint8_t* data = (uint8_t*)buffer;
    
    OrderBookStateMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    data += ReadString(data, message.stateName);

    return true;
}

bool ITCHParser::parseAddOrderMessage(void* buffer, size_t size) {
    assert((size == 37) && "Invalid size of the ITCH message type 'A'");
    uint8_t* data = (uint8_t*)buffer;

    AddOrderMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderID);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    message.side = *data++;
    data += Common::Endian::ReadBigEndian(data, message.orderBookPosition);
    data += Common::Endian::ReadBigEndian(data, message.quantity);
    data += Common::Endian::ReadBigEndian(data, message.price);
    data += Common::Endian::ReadBigEndian(data, message.orderAttributes);
    message.lotType = *data++;

    _outFile << message;
    return true;
}

bool ITCHParser::parseAddOrderMessageWithMPID(void* buffer, size_t size) {
    assert((size == 44) && "Invalid size of the ITCH message type 'F'");
    uint8_t* data = (uint8_t*)buffer;

    AddOrderMessageWithMPID message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderID);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    message.side = *data++;
    data += Common::Endian::ReadBigEndian(data, message.orderBookPosition);
    data += Common::Endian::ReadBigEndian(data, message.quantity);
    data += Common::Endian::ReadBigEndian(data, message.price);
    data += Common::Endian::ReadBigEndian(data, message.orderAttributes);
    message.lotType = *data++;
    data += ReadString(data, message.participantID);

    return true;
}

bool ITCHParser::parseOrderExecutedMessage(void* buffer, size_t size) {
    assert((size == 52) && "Invalid size of the ITCH message type 'E'");
    uint8_t* data = (uint8_t*)buffer;

    OrderExecutedMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderID);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    message.side = *data++;
    data += Common::Endian::ReadBigEndian(data, message.executedQuantity);
    data += Common::Endian::ReadBigEndian(data, message.matchID);
    data += Common::Endian::ReadBigEndian(data, message.comboGroupID);
    data += ReadString(data, message.reserverd1);
    data += ReadString(data, message.reserverd2);

    return true;
}

bool ITCHParser::parseOrderExecutedWithPriceMessage(void* buffer, size_t size) {
    assert((size == 58) && "Invalid size of the ITCH message type 'C'");
    uint8_t* data = (uint8_t*)buffer;

    OrderExecutedWithPriceMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderID);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    message.side = *data++;
    data += Common::Endian::ReadBigEndian(data, message.executedQuantity);
    data += Common::Endian::ReadBigEndian(data, message.matchID);
    data += Common::Endian::ReadBigEndian(data, message.comboGroupID);
    data += ReadString(data, message.reserverd1);
    data += ReadString(data, message.reserverd2);
    data += Common::Endian::ReadBigEndian(data, message.tradePrice);
    message.occuredAtCross = *data++;
    message.printable = *data++;

    return true;
}

bool ITCHParser::parseOrderDeleteMessage(void* buffer, size_t size) {
    assert((size == 18) && "Invalid size of the ITCH message type 'D'");
    uint8_t* data = (uint8_t*)buffer;

    OrderDeleteMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderID);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    message.side = *data++;

    _outFile << message;
    return true;
}

bool ITCHParser::parseTradeMessage(void* buffer, size_t size) {
    assert((size == 50) && "Invalid size of the ITCH message type 'P'");
    uint8_t* data = (uint8_t*)buffer;

    TradeMessage message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.matchID);
    data += Common::Endian::ReadBigEndian(data, message.comboGroupID);
    message.side = *data++;
    data += Common::Endian::ReadBigEndian(data, message.quantity);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    data += Common::Endian::ReadBigEndian(data, message.tradePrice);
    data += ReadString(data, message.reserverd1);
    data += ReadString(data, message.reserverd2);
    message.printable = *data++;
    message.occuredAtCross = *data++;

    return true;
}

bool ITCHParser::parseEquilibriumPriceUpdate(void* buffer, size_t size) {
    assert((size == 53) && "Invalid size of the ITCH message type 'Z'");
    uint8_t* data = (uint8_t*)buffer;

    EquilibriumPriceUpdate message;
    message.messageType = *data++;
    data += Common::Endian::ReadBigEndian(data, message.timestamp);
    data += Common::Endian::ReadBigEndian(data, message.orderBookID);
    data += Common::Endian::ReadBigEndian(data, message.availableBidQuantityatEquilibriumPrice);
    data += Common::Endian::ReadBigEndian(data, message.availableAskQuantityatEquilibriumPrice);
    data += Common::Endian::ReadBigEndian(data, message.equilibriumPrice);
    data += Common::Endian::ReadBigEndian(data, message.bestBidPrice);
    data += Common::Endian::ReadBigEndian(data, message.bestAskPrice);
    data += Common::Endian::ReadBigEndian(data, message.bestBidQuantity);
    data += Common::Endian::ReadBigEndian(data, message.bestAskQuantity);

    return true;
}

bool ITCHParser::parseUnknownMessage(void* buffer, size_t size) {
    assert((size > 0) && "Invalid size of the unknown ITCH message!");
    if (size == 0)
        return false;

    uint8_t* data = (uint8_t*)buffer;

    UnknownMessage message;
    message.messageType = *data;

    return onMessage(message);
}

bool ITCHParser::parseMessage(void* buffer, size_t size) {
    // Message is empty
    if (size == 0)
        return false;
    
    uint8_t* data = (uint8_t*)buffer;
    
    switch (*data) {
        case 'T':
            return parseSecondMessage(data, size);
        case 'R':
            return parseOrderBookDirectoryMessage(data, size);
        case 'M':
            return parseCombinationOrderBookLegMessage(data, size);
        case 'L':
            return parseTickSizeTableEntryMessage(data, size);
        case 'S':
            return parseSystemEventMessage(data, size);
        case 'O':
            return parseOrderBookStateMessage(data, size);;
        case 'A':
            return parseAddOrderMessage(data, size);
        case 'F':
            return parseAddOrderMessageWithMPID(data, size);
        case 'E':
            return parseOrderExecutedMessage(data, size);
        case 'C':
            return parseOrderExecutedWithPriceMessage(data, size);
        case 'D':
            return parseOrderDeleteMessage(data, size);
        case 'P':
            return parseTradeMessage(data, size);
        case 'Z':
            return parseEquilibriumPriceUpdate(data, size);
        //case 'U': will use in the future ITCH spec.
        default:
            return parseUnknownMessage(data, size);
    }
}

bool ITCHParser::parse(void* buffer) {
    uint8_t* data = (uint8_t*)buffer;
    data += 18; //for MoldUDP64 protocol header

    uint16_t messageCount;
    uint16_t messageSize;
    data+= Common::Endian::ReadBigEndian(data, messageCount);

    while(messageCount --> 0) { /* long arrow operator :) */
        data+= Common::Endian::ReadBigEndian(data, messageSize);
        if(parseMessage(data, messageSize)) ++ _messages;
        data += messageSize;
    }

    return true;
}

void ITCHParser::_writeFileHeader() {
    _outFile 
            << std::setw(20) << std::left << "Timestamp"
            << std::setw(10) << std::left << "Message Type"
            << std::setw(10) << std::left << "Side"
            << std::setw(20) << std::left << "OrderID"
            << std::setw(20) << "OrderBookID"
            << std::setw(20) << "Quantity"
            << std::setw(20) << "Price" << '\n'
            << std::setfill('-') << std::setw(150) << '-' << '\n'
            << std::setfill(' ');
}
