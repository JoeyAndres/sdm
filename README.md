# sparse-distributed-memory
Sparse distributed memory, aka Kanerva Coding.

# Compilation and Installation

### Dependency
* g++-4.9 or greater.
* cmake 2.8.9 or greater.
* gmp-6.1.1
* lzip
* m4

### Installation:
```bash
sudo apt-get install lzip  # Needed by the tar --lzip
sudo apt-get install m4

# Install GNU MultiPrecision (GMP).
wget https://gmplib.org/download/gmp/gmp-6.1.1.tar.lz -P /tmp
cd /tmp
tar --lzip -xvf gmp-6.1.1.tar.lz
cd gmp-6.1.1
./configure --enable-cxx
make -j16
sudo make install

# Finally, install sdm.
cd /tmp
git clone https://github.com/JoeyAndres/sparse-distributed-memory.git
cd sparse-distributed-memory
mkdir build
cd build
cmake ..
make -j16
sudo make install
```
# Usage
In  this sample usage, we map _64-bit_ addresses to _2^11=2048_ hard locations.
The data being saved is also _64-bit_.

```c++
#include <cstdint>

#include <sdm>  // Header for sdm stuff.

constexpr size_t hardLocationBitCount = 11;  // 2^11 = 2048 hard locations.
constexpr size_t addressBitCount = 64;
constexpr size_t dataBitCount = 64;
auto sparseDistributedSystem = 
  sdm::SDMFactory<addressBitCount, hardLocationBitCount, dataBitCount>(3).get();
  
uint64_t address = 1;  // 64 bit address.
uint64_t data = address;  // 64 bit data.

// Writes data.
sparseDistributedSystem->write(address, data);

// Read data.
uint64_t readData = sparseDistributedSystem->read(address).to_ullong();
// readData = 1
```
