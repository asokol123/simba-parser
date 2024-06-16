# Building

```bash
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make
```

# Running
```bash
./build/pcap ./path/to/pcap/file > output.json
```

Output will be multiline json file