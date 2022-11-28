dump_h5: dump_h5.cpp
	g++ dump_h5.cpp -o bin/dump_h5 -IHighFive/include -I/usr/include/hdf5/serial -L/usr/lib/x86_64-linux-gnu/hdf5/serial/ -lhdf5 --std=c++17
