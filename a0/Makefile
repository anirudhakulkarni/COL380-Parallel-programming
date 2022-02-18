CC=g++ -fopenmp
#g++ -std=c++11 -O2 -mavx2 -c classify.cpp 
CFLAGS=-std=c++11 -O2 -pg

# sources=main.cpp classify.cpp
sources=main.cpp classify.cpp
objects=$(sources:.cpp=.o)

classify:$(objects)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@
v1:
	make
	./classify rfile dfile 1009072 4 10
run: classify
	./classify rfile dfile 1009072 4 10
runp:
	perf stat ./classify rfile dfile 1009072 4 10
runpr:
	perf record ./classify rfile dfile 1009072 4 1
	perf report -i perf.data
rung: 
	valgrind --tool=cachegrind ./classify rfile dfile 1009072 4 1
#classify.o: classify.h
#	touch classify.o
#	$(CC) $(CFLAGS) -c $<
#main.o: classify.h
#	$(CC) $(CFLAGS) -c $<

%.o: %.cpp classify.h
	$(CC) $(CFLAGS) -c $<
g++ -fopenmp -pg -pg -std=c++11 -O2 main.o classify.o -o classify 