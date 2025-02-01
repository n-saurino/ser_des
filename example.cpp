#include <vector>
#include <array>
#include <thread>
#include <atomic>
#include <boost/lockfree/queue.hpp>
#include <cstddef>
#include <cstring>

// Define record size.
constexpr size_t RECORD_SIZE = 28;

// Define a type for a raw market data message.
using RawMessage = std::array<std::byte, RECORD_SIZE>;

struct MarketDataRecord {
    char symbol[8];
    double price;
    uint32_t volume;
    uint64_t timestamp;
};

// Assume deserialize() converts a RawMessage to MarketDataRecord.
MarketDataRecord deserialize(const RawMessage& raw) {
    MarketDataRecord record;
    std::memcpy(record.symbol, raw.data(), 8);
    std::memcpy(&record.price, raw.data() + 8, 8);
    std::memcpy(&record.volume, raw.data() + 16, 4);
    std::memcpy(&record.timestamp, raw.data() + 20, 8);
    return record;
}

// Global lock-free queue for raw messages.
boost::lockfree::queue<RawMessage> rawQueue(1024);

// Receiver thread function.
void receiverThread() {
    // Set up multicast socket and read data...
    std::vector<std::byte> incomingBuffer;  // Assume data is appended here.
    
    while (/* running flag */) {
        // Read data from the socket and append to incomingBuffer.
        // ...
        
        // Process complete messages.
        while (incomingBuffer.size() >= RECORD_SIZE) {
            RawMessage raw;
            std::memcpy(raw.data(), incomingBuffer.data(), RECORD_SIZE);
            
            // Push the raw message into the queue.
            while (!rawQueue.push(raw)) {
                // Optionally, yield if the queue is full.
            }
            
            // Remove the processed bytes from incomingBuffer.
            incomingBuffer.erase(incomingBuffer.begin(), incomingBuffer.begin() + RECORD_SIZE);
        }
    }
}

// Worker thread function.
void workerThread() {
    RawMessage raw;
    while (/* running flag */) {
        if (rawQueue.pop(raw)) {
            // Deserialize raw message.
            MarketDataRecord record = deserialize(raw);
            // Process the market data record (e.g., update order book, run strategy).
        } else {
            // Optionally, yield or sleep briefly.
        }
    }
}

int main() {
    // Start the receiver thread.
    std::thread receiver(receiverThread);
    
    // Start a pool of worker threads.
    const int numWorkers = 4;
    std::vector<std::thread> workers;
    for (int i = 0; i < numWorkers; ++i) {
        workers.emplace_back(workerThread);
    }
    
    // (Wait for a shutdown signal and update the running flag accordingly.)
    
    // Join threads.
    receiver.join();
    for (auto& worker : workers) {
        worker.join();
    }
    
    return 0;
}
