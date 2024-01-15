#ifndef ITCH_PARSER_H
#define ITCH_PARSER_H

#include <fstream>
#include <cstring>
#include <cstdint>
#include <iomanip>

struct SecondsMessage {
    char messageType;
    uint32_t second;
};

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

struct CombinationOrderBookLegMessage {
    char messageType;
    uint32_t timestamp;
    uint32_t combinationOrderBookID;
    uint32_t legOrderBookID;
    char legSide;
    uint32_t legRatio;
};

struct TickSizeTableEntryMessage {
    char messageType;
    uint32_t timestamp;
    uint32_t orderBookID;
    int64_t tickSize;
    int32_t priceFrom;
    int32_t priceTo;
};

struct SystemEventMessage {
    char messageType;
    uint32_t timestamp;
    char eventCode;
};

struct OrderBookStateMessage {
    char messageType;
    uint32_t timestamp;
    uint32_t orderBookID;
    char stateName[20];
};

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

    friend std::ostream& operator<<(std::ostream& os, const AddOrderMessage& msg) {
        os  << std::setw(20) << std::left << msg.timestamp << ' '
            << std::setw(10) << "ADD" << ' '
            << std::setw(10) << msg.side << ' '
            << std::setw(20) << std::right << msg.orderID << ' '
            << std::setw(20) << msg.orderBookID << ' '
            << std::setw(20) << msg.quantity << ' '
            << std::setw(20) << msg.price << '\n';
        return os;
    }
};

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

struct OrderDeleteMessage {
    char messageType;
    uint32_t timestamp;
    uint64_t orderID;
    uint32_t orderBookID;
    char side;

    friend std::ostream& operator<<(std::ostream& os, const OrderDeleteMessage& msg) {
        os  << std::setw(20) << std::left << msg.timestamp << ' '
            << std::setw(10) << "DEL" << ' '
            << std::setw(10) << msg.side << ' '
            << std::setw(20) << std::right << msg.orderID << ' '
            << std::setw(20) << msg.orderBookID << ' '
            << std::setw(20) << '0'
            << std::setw(20) << '0' << '\n';
        return os;
    }
};

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

struct UnknownMessage {
    char messageType;
};

class ITCHParser {
public:
    ITCHParser(const std::string& filename);
    ~ITCHParser();

    template <size_t N>
    inline size_t ReadString(const void* buffer, char (&str)[N]) {
        std::memcpy(str, buffer, N);
        return N;
    }
    bool parseSecondMessage(void* buffer, size_t size)noexcept;
    bool parseOrderBookDirectoryMessage(void* buffer, size_t size)noexcept;
    bool parseCombinationOrderBookLegMessage(void* buffer, size_t size)noexcept;
    bool parseTickSizeTableEntryMessage(void* buffer, size_t size)noexcept;
    bool parseSystemEventMessage(void* buffer, size_t size)noexcept;
    bool parseOrderBookStateMessage(void* buffer, size_t size)noexcept;
    bool parseAddOrderMessage(void* buffer, size_t size)noexcept;
    bool parseAddOrderMessageWithMPID(void* buffer, size_t size)noexcept;
    bool parseOrderExecutedMessage(void* buffer, size_t size)noexcept;
    bool parseOrderExecutedWithPriceMessage(void* buffer, size_t size)noexcept;
    bool parseOrderDeleteMessage(void* buffer, size_t size)noexcept;
    bool parseTradeMessage(void* buffer, size_t size)noexcept;
    bool parseEquilibriumPriceUpdate(void* buffer, size_t size)noexcept;
    bool parseUnknownMessage(void* buffer, size_t size)noexcept;
    bool parseMessage(void* buffer, size_t size)noexcept;
    bool parse(void* buffer)noexcept;

    size_t errors()const noexcept { return _errors; }

private:
    bool onMessage(const UnknownMessage& message)const noexcept{ ++_errors; return true; }
    void _writeFileHeader()noexcept;
    mutable size_t _messages;
    mutable size_t _errors;
    std::ofstream _outFile;
};

#endif