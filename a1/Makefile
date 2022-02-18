all:
	g++ -fPIC -g -c -Wall psort.cpp -fopenmp
	g++ -shared -Wl,-soname,libsort.so -o libsort.so psort.o -lc

clean:
	rm *.so *.o out

run:
	g++ -fopenmp -g -Wall psort.cpp driver.cpp -o out
	./out input_1000000.txt 10
	./out input_10.txt 10
	./out input_100.txt 10
	./out input_10000.txt 10
rung: 

	valgrind --tool=cachegrind --branch-sim=yes ./out input_1000000.txt 100

runb:
	g++ -fopenmp -g -Wall test.cpp driver_bin.cpp -o out
	./out input_4294967295.bin 8
runt:
	g++ -fopenmp -g -Wall test.cpp driver_bin.cpp -o out
	./out input_100000000.bin 8
	./out input_4294967295.bin 24
runt2:
	g++ -fopenmp -g -Wall test.cpp driver.cpp -o out
	./out input_10.txt 10
	./out input_100.txt 10
	./out input_10000.txt 10
	./out input_100000.txt 10
	./out input_1000000.txt 10
	./out input_10000000.txt 10
	./out input_100000000.txt 10
f:
	./out input_1000000.bin 10
	./out input_100000000.bin 10
