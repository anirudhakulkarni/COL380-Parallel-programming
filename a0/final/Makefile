CC=g++
#g++ -std=c++11 -O2 -mavx2 -c classify.cpp 
CFLAGS=-std=c++11 -O2 -fopenmp -pg

sources=main.cpp classify.cpp
objects=$(sources:.cpp=.o)

classify:$(objects)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@

run: classify
	./classify rfile dfile 1009072 4 3
clean:
	rm *.o
runp:
	perf stat ./classify rfile dfile 1009072 4 10
runpr:
	perf record ./classify rfile dfile 1009072 4 1
	perf report -i perf.data
rung: 
	valgrind --tool=cachegrind --branch-sim=yes  ./classify rfile dfile 1009072 4 1
gprof: 
	$(CC) $(CFLAGS) -fopenmp -pg -c main.cpp
	$(CC) $(CFLAGS) -fopenmp -pg -c classify.cpp
	$(CC) $(CFLAGS) -fopenmp -pg main.o classify.o -o classify
	make run
	gprof classify gmon.out > analysis_gprof.txt
	make clean
#classify.o: classify.h
#	touch classify.o
#	$(CC) $(CFLAGS) -c $<
#main.o: classify.h
#	$(CC) $(CFLAGS) -c $<

%.o: %.cpp classify.h
	$(CC) $(CFLAGS) -c $<
