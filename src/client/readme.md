g++ client.cpp main.cpp threadpool.cpp -std=c++14 `pkg-config --cflags libndn-cxx` `pkg-config --libs libndn-cxx` -o Client

Segmentation fault:HOW TO DEBUG? GDB METHOD
g++ -g client.cpp main.cpp threadpool.cpp -std=c++14 `pkg-config --cflags libndn-cxx` `pkg-config --libs libndn-cxx`

gdb a.out core

(gdb) r  ---to continue

(gdb) q  ---to quit
