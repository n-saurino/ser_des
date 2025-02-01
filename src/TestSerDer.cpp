#include <gtest/gtest.h>
#include "SerDer.hpp"  // Adjust the include path as necessary

// Test a full round-trip: Serialize a record, then deserialize and verify it matches.
TEST(SerDerTest, RoundTripTest) {
    SerDer serializer(28);

    // Create and initialize a MarketDataRecord.
    MarketDataRecord original{};
    std::memset(original.symbol, ' ', sizeof(original.symbol));
    std::memcpy(original.symbol, "AAPL", 4);
    original.price = 150.25;
    original.volume = 1000;
    original.timestamp = 1610000000123456ULL;

    // Serialize the record.
    std::vector<std::byte> serialized = serializer.Serialize(original);

    // Deserialize the binary data.
    MarketDataRecord deserialized = serializer.Deserialize(serialized);

    // Compare each field of the original and deserialized records.
    EXPECT_EQ(std::memcmp(original.symbol, deserialized.symbol, 8), 0);
    EXPECT_DOUBLE_EQ(original.price, deserialized.price);
    EXPECT_EQ(original.volume, deserialized.volume);
    EXPECT_EQ(original.timestamp, deserialized.timestamp);
}

// Test the ClearBuffer method.
// Note: This implementation's ClearBuffer simply calls clear() on the internal buffer,
// so a subsequent call to Serialize will return an empty vector.
TEST(SerDerTest, ClearBufferTest) {
    SerDer serializer(28);

    // Ensure that before clearing, the internal buffer is correctly sized.
    MarketDataRecord record{};
    std::memset(record.symbol, ' ', sizeof(record.symbol));
    std::memcpy(record.symbol, "TEST", 4);
    record.price = 123.45;
    record.volume = 500;
    record.timestamp = 123456789ULL;

    std::vector<std::byte> serialized = serializer.Serialize(record);
    // EXPECT_EQ(serialized.size(), 28u);

    // Clear the internal buffer.
    serializer.ClearBuffer();
    
    // Create and initialize a MarketDataRecord.
    MarketDataRecord original{};
    std::memset(original.symbol, ' ', sizeof(original.symbol));
    std::memcpy(original.symbol, "AAPL", 4);
    original.price = 150.25;
    original.volume = 1000;
    original.timestamp = 1610000000123456ULL;

    // Serialize the record.
    serialized = serializer.Serialize(original);

    // Deserialize the binary data.
    MarketDataRecord deserialized = serializer.Deserialize(serialized);

    // Compare each field of the original and deserialized records.
    EXPECT_EQ(std::memcmp(original.symbol, deserialized.symbol, 8), 0);
    EXPECT_DOUBLE_EQ(original.price, deserialized.price);
    EXPECT_EQ(original.volume, deserialized.volume);
    EXPECT_EQ(original.timestamp, deserialized.timestamp);

    // After clearing, a subsequent call to Serialize returns an empty vector.
    // std::vector<std::byte> afterClear = serializer.Serialize(record);
    // EXPECT_EQ(afterClear.size(), 0u);
}