g++ server.cpp main.cpp -std=c++14 `pkg-config --cflags libndn-cxx` `pkg-config --libs libndn-cxx` -o Server
