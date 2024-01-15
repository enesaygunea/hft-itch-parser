#pragma once

#include <vector>
#include <cassert>
#include "utils/endian.h"
#include "utils/writechar.h"

#include<unistd.h> //sleep for test

#pragma pack(push, 1)
struct SecondsMessage {
    char messageType;
    uint32_t second;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct OrderBookDirectoryMessage {
    char messageType;
    uint32_t timestamp;
    uint32_t orderBookID;
    char symbol[32];
    char longName[32];
    char ISIN[12];
    uint8_t financialProduct; 
    char tradingCurrency[3]; 
    uint16_t numberOfDecimalsInPrice;
    uint16_t numberOfDecimalsInNominalValue;
    uint32_t oddLotSize;
    uint32_t roundLotSize;
    uint32_t blockLotSize;
    uint64_t nominalValue;
    uint8_t numberOfLegs;
    uint32_t underlyingOrderBookID;
    int32_t strikePrice;
    uint32_t expirationDate;
    uint16_t numberOfDecimalsInStrikePrice;
    uint8_t putOrCall;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct CombinationOrderBookLegMessage {
    char messageType;
    uint32_t timestamp;
    uint32_t combinationOrderBookID;
    uint32_t legOrderBookID;
    char legSide;
    uint32_t legRatio;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TickSizeTableEntryMessage {
    char messageType;
    uint32_t timestamp;
    uint32_t orderBookID;
    int64_t tickSize;
    int32_t priceFrom;
    int32_t priceTo;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SystemEventMessage {
    char messageType;
    uint32_t timestamp;
    char eventCode;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct OrderBookStateMessage {
    char messageType;
    uint32_t timestamp;
    uint32_t orderBookID;
    char stateName[20];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct AddOrderMessage {
    char messageType;
    uint32_t timestamp;
    uint64_t orderID;
    uint32_t orderBookID;
    char side;
    uint32_t orderBookPosition;
    uint64_t quantity;
    int32_t price;
    uint16_t orderAttributes;
    uint8_t lotType;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct AddOrderMessageWithMPID {
    char messageType;
    uint32_t timestamp;
    uint64_t orderID;
    uint32_t orderBookID;
    char side;
    uint32_t orderBookPosition;
    uint64_t quantity;
    int32_t price;
    uint16_t orderAttributes;
    uint8_t lotType;
    char participantID[7];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct OrderExecutedMessage {
    char messageType;
    uint32_t timestamp;
    uint64_t orderID;
    uint32_t orderBookID;
    char side;
    uint64_t executedQuantity;
    uint64_t matchID;
    uint32_t comboGroupID;
    char reserverd1[7];
    char reserverd2[7];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct OrderExecutedWithPriceMessage {
    char messageType;
    uint32_t timestamp;
    uint64_t orderID;
    uint32_t orderBookID;
    char side;
    uint64_t executedQuantity;
    uint64_t matchID;
    uint32_t comboGroupID;
    char reserverd1[7];
    char reserverd2[7];
    int32_t tradePrice;
    char occuredAtCross;
    char printable;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct OrderDeleteMessage {
    char messageType;
    uint32_t timestamp;
    uint64_t orderID;
    uint32_t orderBookID;
    char side;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct TradeMessage {
    char messageType;
    uint32_t timestamp;
    uint64_t matchID;
    uint32_t comboGroupID;
    char side;
    uint64_t quantity;
    uint32_t orderBookID;
    int32_t tradePrice;
    char reserverd1[7];
    char reserverd2[7];
    char printable;
    char occuredAtCross;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct EquilibriumPriceUpdate {
    char messageType;
    uint32_t timestamp;
    uint32_t orderBookID;
    uint64_t availableBidQuantityatEquilibriumPrice;
    uint64_t availableAskQuantityatEquilibriumPrice;
    int32_t equilibriumPrice;
    int32_t bestBidPrice;
    int32_t bestAskPrice;
    uint64_t bestBidQuantity;
    uint64_t bestAskQuantity;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct UnknownMessage
{
    char messageType;
};
#pragma pack(pop)

void printOrderBookStateMessage(const OrderBookStateMessage& message) {
    std::cout << "Order Book State Message: " << '\n';
    std::cout << "  Message Type: " << Common::WriteChar(message.messageType) << '\n';
    std::cout << "  Timestamp: " << message.timestamp << " ns" << '\n';
    std::cout << "  Order Book ID: " << message.orderBookID << '\n';
    std::cout << "  State Name: " << Common::WriteString(message.stateName) << '\n';;
}

void printAddOrderMessageWithMPID(const AddOrderMessageWithMPID& message) {
    std::cout << "Add Order Message With MPID: " << '\n';
    std::cout << "  Message Type: " << Common::WriteChar(message.messageType) << '\n';
    std::cout << "  Timestamp: " << message.timestamp << " ns" << '\n';
    std::cout << "  Order ID: " << message.orderID << '\n';
    std::cout << "  Order Book ID: " << message.orderBookID << '\n';
    std::cout << "  Side: " << Common::WriteChar(message.side) << '\n';
    std::cout << "  Order Book Position: " << message.orderBookPosition << '\n';
    std::cout << "  Quantity: " << message.quantity << '\n';
    std::cout << "  Price: " << message.price << '\n';
    std::cout << "  Order Attributes: " << message.orderAttributes << '\n';
    std::cout << "  Lot Type: " << static_cast<int>(message.lotType) << '\n'; // uint8_t tipi için cast gerekli
    std::cout << "  Participant ID: " << Common::WriteString(message.participantID) << '\n';
}

void printAddOrderMessage(const AddOrderMessage& message) {
    std::cout << "Add Order Message: " << '\n';
    std::cout << "  Message Type: " << Common::WriteChar(message.messageType) << '\n';
    std::cout << "  Timestamp: " << message.timestamp << " ns" << '\n';
    std::cout << "  Order ID: " << message.orderID << '\n';
    std::cout << "  Order Book ID: " << message.orderBookID << '\n';
    std::cout << "  Side: " << Common::WriteChar(message.side) << '\n';
    std::cout << "  Order Book Position: " << message.orderBookPosition << '\n';
    std::cout << "  Quantity: " << message.quantity << '\n';
    std::cout << "  Price: " << message.price << '\n';
    std::cout << "  orderAttributes: " << message.orderAttributes << '\n';
    std::cout << "  lotType: " << message.lotType << '\n';
}

void printOrderBookDirectoryMessage(const OrderBookDirectoryMessage& message) {
    std::cout << "Order Book Directory Message: " << '\n';
    std::cout << "  Message Type: " << Common::WriteChar(message.messageType) << '\n';
    std::cout << "  Timestamp: " << message.timestamp << " ns" << '\n';
    std::cout << "  Order Book ID: " << message.orderBookID << '\n';
    std::cout << "  Symbol: " << Common::iso_8859_1_to_utf8(message.symbol) << '\n';
    std::cout << "  Long Name: " << Common::WriteString(message.longName) << '\n';
    std::cout << "  ISIN: " << Common::WriteString(message.ISIN) << '\n';
    std::cout << "  Financial Product: " << +message.financialProduct << '\n';
    std::cout << "  Trading Currency: " << Common::WriteString(message.tradingCurrency) << '\n';
    std::cout << "  Number Of Decimals In Price: " << message.numberOfDecimalsInPrice << '\n';
    std::cout << "  Number Of Decimals In Nominal Value: " << message.numberOfDecimalsInNominalValue << '\n';
    std::cout << "  Odd Lot Size: " << message.oddLotSize << '\n';
    std::cout << "  Round Lot Size: " << message.roundLotSize << '\n';
    std::cout << "  Block Lot Size: " << message.blockLotSize << '\n';
    std::cout << "  Nominal Value: " << message.nominalValue << '\n';
    std::cout << "  Number Of Legs: " << +message.numberOfLegs << '\n';
    std::cout << "  Underlying Order Book ID: " << message.underlyingOrderBookID << '\n';
    std::cout << "  Strike Price: " << message.strikePrice << '\n';
    std::cout << "  Expiration Date: " << message.expirationDate << '\n';
    std::cout << "  Number Of Decimals In Strike Price: " << message.numberOfDecimalsInStrikePrice << '\n';
    std::cout << "  Put Or Call: " << +message.putOrCall << '\n';
}

class ITCHParser {
public:
    ITCHParser() : _messages(0), _errors(0)
    {
        reset();
    }

    void reset() {
        _size = 0;
        _cache.clear();
    }

    template <size_t N>
    inline size_t ReadString(const void* buffer, char (&str)[N]) {
        std::memcpy(str, buffer, N);

        return N;
    }

    bool parseSecondMessage(void* buffer, size_t size) {
        assert((size == 5) && "Invalid size of the ITCH message type 'T'");
        uint8_t* data = (uint8_t*)buffer;

        SecondsMessage message;
        message.messageType = *data++;
        data += Common::Endian::ReadBigEndian(data, message.second);

        return true;
    }

    bool parseOrderBookDirectoryMessage(void* buffer, size_t size) {
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

        //printOrderBookDirectoryMessage(message);
        //usleep(5000'000);
        return true;
    }

    bool parseCombinationOrderBookLegMessage(void* buffer, size_t size) {
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

    bool parseTickSizeTableEntryMessage(void* buffer, size_t size) {
        assert((size == 25) && "Invalid size of the ITCH message type 'L'");
        uint8_t* data = (uint8_t*)buffer;

        TickSizeTableEntryMessage message;
        message.messageType = *data++;
        data += Common::Endian::ReadBigEndian(data, message.timestamp);
        data += Common::Endian::ReadBigEndian(data, message.orderBookID);
        data += Common::Endian::ReadLittleEndian(data, message.tickSize);
        data += Common::Endian::ReadLittleEndian(data, message.priceFrom);
        data += Common::Endian::ReadLittleEndian(data, message.priceTo);

        return true;
    }

    bool parseSystemEventMessage(void* buffer, size_t size) {
        assert((size == 6) && "Invalid size of the ITCH message type 'S'");
        uint8_t* data = (uint8_t*)buffer;
        
        SystemEventMessage message;
        message.messageType = *data++;
        data += Common::Endian::ReadBigEndian(data, message.timestamp);
        message.eventCode = *data++;

        return true;
    }
    
    bool parseOrderBookStateMessage(void* buffer, size_t size) {
        assert((size == 29) && "Invalid size of the ITCH message type 'O'");
        uint8_t* data = (uint8_t*)buffer;
        
        OrderBookStateMessage message;
        message.messageType = *data++;
        data += Common::Endian::ReadBigEndian(data, message.timestamp);
        data += Common::Endian::ReadBigEndian(data, message.orderBookID);
        data += ReadString(data, message.stateName);

        printOrderBookStateMessage(message);
        //usleep(100'000);
        return true;
    }

    bool parseAddOrderMessage(void* buffer, size_t size) {
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
        data += Common::Endian::ReadLittleEndian(data, message.price);
        data += Common::Endian::ReadBigEndian(data, message.orderAttributes);
        message.lotType = *data++;

        //return onMessage(message);
        //printAddOrderMessage(message);
        //usleep(500'000);
        return true;
    }
    
    bool parseAddOrderMessageWithMPID(void* buffer, size_t size) {
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
        data += Common::Endian::ReadLittleEndian(data, message.price);
        data += Common::Endian::ReadBigEndian(data, message.orderAttributes);
        message.lotType = *data++;
        data += ReadString(data, message.participantID);

        //printAddOrderMessageWithMPID(message);

        return true;
    }

    bool parseOrderExecutedMessage(void* buffer, size_t size) {
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
    
    bool parseOrderExecutedWithPriceMessage(void* buffer, size_t size) {
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
        data += Common::Endian::ReadLittleEndian(data, message.tradePrice);
        message.occuredAtCross = *data++;
        message.printable = *data++;

        return true;
    }

    bool parseOrderDeleteMessage(void* buffer, size_t size) {
        assert((size == 18) && "Invalid size of the ITCH message type 'D'");
        uint8_t* data = (uint8_t*)buffer;

        OrderDeleteMessage message;
        message.messageType = *data++;
        data += Common::Endian::ReadBigEndian(data, message.timestamp);
        data += Common::Endian::ReadBigEndian(data, message.orderID);
        data += Common::Endian::ReadBigEndian(data, message.orderBookID);
        message.side = *data++;

        return true;
    }

    bool parseTradeMessage(void* buffer, size_t size) {
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

    bool parseEquilibriumPriceUpdate(void* buffer, size_t size) {
        assert((size == 53) && "Invalid size of the ITCH message type 'Z'");
        uint8_t* data = (uint8_t*)buffer;

        EquilibriumPriceUpdate message;
        message.messageType = *data++;
        data += Common::Endian::ReadBigEndian(data, message.timestamp);
        data += Common::Endian::ReadBigEndian(data, message.orderBookID);
        data += Common::Endian::ReadBigEndian(data, message.availableBidQuantityatEquilibriumPrice);
        data += Common::Endian::ReadBigEndian(data, message.availableAskQuantityatEquilibriumPrice);
        data += Common::Endian::ReadLittleEndian(data, message.equilibriumPrice);
        data += Common::Endian::ReadLittleEndian(data, message.bestBidPrice);
        data += Common::Endian::ReadLittleEndian(data, message.bestAskPrice);
        data += Common::Endian::ReadBigEndian(data, message.bestBidQuantity);
        data += Common::Endian::ReadBigEndian(data, message.bestAskQuantity);

        return true;
    }
    
    bool onMessage(const UnknownMessage& message) { ++_errors; return true; }

    bool parseUnknownMessage(void* buffer, size_t size)
    {
        assert((size > 0) && "Invalid size of the unknown ITCH message!");
        if (size == 0)
            return false;

        uint8_t* data = (uint8_t*)buffer;

        UnknownMessage message;
        message.messageType = *data;

        return onMessage(message);
    }

    bool parseMessage(void* buffer, size_t size)
    {
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

    bool parse(void* buffer, size_t size)
    {
        uint8_t* data = (uint8_t*)buffer;
        data += 18;

        uint16_t messageCount;
        uint16_t messageSize;
        data+= Common::Endian::ReadBigEndian(data, messageCount);
    
        while(messageCount --> 0) { /* long arrow operator :) */
            data+= Common::Endian::ReadBigEndian(data, messageSize);
            parseMessage(data, messageSize);
            data += messageSize;
        }

        return true;
    }

    size_t messages() const { return _messages; }
    size_t errors() const { return _errors; }

private:
    size_t _messages;
    size_t _errors;
    size_t _size;
    std::vector<uint8_t> _cache;
};