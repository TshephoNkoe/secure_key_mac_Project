#include <iostream>
#include <cstring>
#include <vector>
#include <sys/mman.h> // For mlock/munlock

// Secure wipe function (volatile to prevent optimization)
void secure_wipe(void* ptr, size_t size) {
    if (!ptr || size == 0) return;
    
    volatile char* vptr = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; ++i) {
        vptr[i] = 0;
    }
}

// Secure allocator that locks memory and zeroizes on free
template<typename T>
struct SecureAllocator {
    using value_type = T;

    T* allocate(size_t n) {
        T* ptr = static_cast<T*>(malloc(n * sizeof(T)));
        if (ptr) {
            mlock(ptr, n * sizeof(T)); // Lock in RAM
        }
        return ptr;
    }

    void deallocate(T* ptr, size_t n) {
        if (ptr) {
            secure_wipe(ptr, n * sizeof(T)); // Zeroize memory
            munlock(ptr, n * sizeof(T)); // Unlock
            free(ptr);
        }
    }
};

// Example usage with a real encryption key
int main() {
    std::cout << "🔐 macOS Secure Key Handling Demo 🔐\n";
    
    // 1. Create secure container for 256-bit key
    std::vector<uint8_t, SecureAllocator<uint8_t>> encryptionKey(32);
    
    // 2. Fill with random data (simulate key generation)
    std::cout << "Generating random encryption key...\n";
    arc4random_buf(encryptionKey.data(), encryptionKey.size());
    
    // 3. Show key (for demo purposes only - don't do this in production!)
    std::cout << "Key in memory: ";
    for (auto byte : encryptionKey) printf("%02x ", byte);
    std::cout << "\n";
    
    // 4. Securely wipe when done
    std::cout << "Wiping key from memory...\n";
    secure_wipe(encryptionKey.data(), encryptionKey.size());
    encryptionKey.clear(); // Triggers secure deallocation
    
    std::cout << "✅ Key securely erased and memory released.\n";
    return 0;
}
