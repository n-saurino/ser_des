# Implement a Market Data Feed Serializer/Deserializer

## Problem Description

Design and implement a serializer/deserializer protocol for a high-frequency market data feed at TXSE. The implementation must convert market data records to and from a compact binary format, optimizing for efficient transmission and storage while maintaining ultra-low latency requirements.

## Data Structure

```cpp
struct MarketDataRecord {
    char symbol[8];     // Fixed-length symbol (up to 8 ASCII characters, pad with spaces)
    double price;       // Price as a double (8 bytes)
    uint32_t volume;    // Volume as a 32-bit unsigned integer (4 bytes)
    uint64_t timestamp; // Timestamp as microseconds since epoch (8 bytes)
};
```

## Required Functions

```cpp
#include <vector>
#include <cstdint>
#include <cstring>

// Serializes a MarketDataRecord into a compact binary representation
std::vector<uint8_t> serialize(const MarketDataRecord& record);

// Deserializes a compact binary representation into a MarketDataRecord
MarketDataRecord deserialize(const std::vector<uint8_t>& data);
```

## Technical Requirements

### Performance
- Low Latency: Minimize dynamic memory allocations and unnecessary copying
- Use fixed-size buffers
- Binary (not text-based) serialization
- O(1) time complexity for both operations

### Memory
- Compact binary format
- Pack fields in natural order
- Use little-endian encoding
- Fixed format: exactly 28 bytes per record (8 + 8 + 4 + 8)
- Contiguous memory buffers

### Correctness
- Deterministic format: `deserialize(serialize(record))` must return original record
- Proper handling of symbol padding (right-padded with spaces)
- Correct endianness handling

## Example Usage

```cpp
// Create a sample record
MarketDataRecord record;
std::memset(record.symbol, ' ', sizeof(record.symbol));  // Fill with spaces
std::memcpy(record.symbol, "AAPL", 4);  // Symbol is "AAPL"
record.price = 150.25;
record.volume = 1000;
record.timestamp = 1610000000123456ULL;

// Serialize and deserialize
std::vector<uint8_t> data = serialize(record);
MarketDataRecord restored = deserialize(data);

// Restored record should be identical to original
assert(std::memcmp(&record, &restored, sizeof(MarketDataRecord)) == 0);
```

## Constraints

1. Performance Requirements
   - Constant time operations O(1)
   - Minimal dynamic memory allocation
   - Efficient memory usage

2. Format Requirements
   - Little-endian byte order for numeric values
   - Fixed 28-byte format per record
   - Symbols must be exactly 8 bytes (space-padded)

3. Assumptions
   - All market data records are valid
   - Symbols are ASCII characters only
   - Maximum symbol length is 8 characters

## Follow-Up Discussion Topics

### 1. Variable-Length Field Support
- Strategies for handling optional fields
- Efficient variable-length encoding
- Impact on performance and complexity

### 2. Low-Latency Optimization
- Multi-threaded processing optimization
- Memory layout considerations
- Cache-friendly design patterns

### 3. Data Validation
- Checksum implementation strategies
- Performance impact of validation
- Error detection trade-offs

## Performance Benchmarking

Key metrics to measure:
1. Serialization latency (99th percentile)
2. Deserialization latency (99th percentile)
3. Memory allocation patterns
4. Cache performance
5. Thread scaling characteristics

## Best Practices

1. Use appropriate memory alignment
2. Implement proper error handling
3. Consider cache line optimization
4. Use compile-time checks where possible
5. Implement proper logging and monitoring
6. Consider platform-specific optimizations
