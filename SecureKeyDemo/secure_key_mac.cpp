#include <iostream>
#include <cstring>
#include <vector>
#include <sys/mman.h> // macOS memory locking (mlock)

// Securely wipe memory (prevents compiler optimization)
void secure_wipe(void* ptr, size_t size) {
    if (!ptr || size == 0) return;
    
    // Use volatile to prevent optimization
    volatile char* vptr = static_cast<volatile char*>(ptr);
    for (size_t i = 0; i < size; ++i) {
        vptr[i] = 0;
    }
}

// Custom allocator to lock memory (prevents swapping)
template<typename T>
struct SecureAllocator {
    using value_type = T;

    T* allocate(size_t n) {
        T* ptr = static_cast<T*>(malloc(n * sizeof(T)));
        if (ptr) {
            mlock(ptr, n * sizeof(T)); // Lock in RAM (macOS)
        }
        return ptr;
    }

    void deallocate(T* ptr, size_t n) {
        if (ptr) {
            secure_wipe(ptr, n * sizeof(T)); // Zeroize before freeing
            munlock(ptr, n * sizeof(T)); // Unlock
            free(ptr);
        }
    }
};

int main() {
    std::cout << "🔒 macOS Secure Key Erasure Demo 🔒\n";

    // 1. Create a 256-bit key (32 bytes) using secure allocator
    std::vector<uint8_t, SecureAllocator<uint8_t>> key(32, 0xAA);

    std::cout << "Key before wipe: ";
    for (auto byte : key) printf("%02x ", byte);
    std::cout << "\n";

    // 2. Explicitly wipe key (optional, since allocator does it on deallocation)
    secure_wipe(key.data(), key.size());

    std::cout << "Key after wipe: ";
    for (auto byte : key) printf("%02x ", byte);
    std::cout << "\n";

    // 3. Key is automatically deallocated & wiped when 'key' goes out of scope
    std::cout << "✅ Key securely erased from memory.\n";

    return 0;
}
