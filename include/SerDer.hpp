#pragma once
#include <vector>
#include <cstddef>
#include <cstring> // for std::memcpy
#include <cstdint>

/*
You are tasked with designing a serializer/deserializer protocol for a 
high-frequency market data feed at TXSE. The goal is to efficiently convert 
market data records to and from a compact binary format for fast transmission
and storage.
*/

// this struct contains 28 bytes of data based on data types of it's
// member variables
struct MarketDataRecord{
    // Fixed-length symbol (e.g., up to 8 ASCII characters; pad with spaces if needed)
    char symbol[8];  
    // Price as a double (8 bytes)
    double price{};  
    // Volume as a 32-bit unsigned integer (4 bytes)
    uint32_t volume{};  
    // Timestamp as a 64-bit unsigned integer representing microseconds since epoch (8 bytes)
    uint64_t timestamp{};  
};

class SerDer
{
public:
    SerDer(std::size_t buffer_size): buffer_size_{buffer_size}{
        buffer_.resize(buffer_size_);
    }

    ~SerDer(){

    }

    // serialize(record): Converts a market data record into a compact binary 
    // format (e.g., a std::vector<uint8_t>).
    const std::vector<std::byte>& Serialize(const MarketDataRecord& mdr){
        std::memcpy(&buffer_[0], mdr.symbol, 8);
        std::memcpy(&buffer_[8], &mdr.price, 8);
        std::memcpy(&buffer_[16], &mdr.volume, 4);
        std::memcpy(&buffer_[20], &mdr.timestamp, 8);
        return buffer_;
    }
    
    // deserialize(data): Given a binary representation, reconstructs the 
    // original market data record.
    MarketDataRecord Deserialize(const std::vector<std::byte>& binary_data){
        MarketDataRecord mdr{};
        std::memcpy(mdr.symbol, &binary_data[0], 8);
        std::memcpy(&mdr.price, &binary_data[8], 8);
        std::memcpy(&mdr.volume, &binary_data[16], 4);
        std::memcpy(&mdr.timestamp, &binary_data[20], 8);        
        return mdr;
    }
    
    void ClearBuffer(){
        buffer_.clear();
        buffer_.resize(buffer_size_);
    }

private:
    std::vector<std::byte> buffer_{};
    std::size_t buffer_size_{28};    
};
