# sparse-distributed-memory
Sparse distributed memory, aka Kanerva Coding.

# Compilation and Installation

### Dependency
* g++-4.9 or greater.
* cmake 2.8.9 or greater.
* gmp-6.1.1
* lzip

### Installation:
```bash
sudo apt-get install lzip  # Needed by the tar --lzip

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
TODO(jandres): Add this once we have the basic functionality up.
