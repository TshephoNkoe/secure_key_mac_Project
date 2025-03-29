# Secure Key Erasure for macOS 🔒

A C++ implementation for securely handling encryption keys in memory, with guaranteed wiping to prevent leaks.

## Features
- 🔐 **Secure Key Generation**: Uses macOS's `arc4random_buf()` for cryptographic randomness
- 🧹 **Guaranteed Wiping**: Volatile memory overwrite (prevents compiler optimizations)
- 🚫 **No Swap Leaks**: Locks keys in RAM using `mlock()`
- 🛡️ **Double Protection**: Zeroizes memory both manually and on deallocation

## How It Works
```plaintext
1. Generates 256-bit encryption key
2. Locks key in RAM (prevents swapping to disk)
3. Uses key for crypto operations
4. Securely wipes all key traces:
   - Overwrites with zeros
   - Unlocks memory
   - Verifies erasure

# Compile (requires Xcode Command Line Tools)
clang++ -std=c++17 -O2 secure_key_mac.cpp -o secure_key_demo

# Run
./secure_key_demo

# Expected Output
🔐 macOS Secure Key Handling Demo 🔐
Generating random encryption key...
Key in memory: 3f a7 12 4e ... (32 random bytes)
Wiping key from memory...
✅ Key securely erased and memory released.

Security Guarantees
Protection	Method Used
Memory scraping	Volatile zeroization
Swap file leaks	mlock()/munlock()
Compiler tricks	secure_wipe() function
Physical attacks	Combine with FileVault
Best Practices
Use with Apple's Secure Enclave for hardware-backed keys

Enable FileVault for full-disk encryption

Never log/show keys in production
