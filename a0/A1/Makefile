CC=g++
#g++ -std=c++11 -O2 -mavx2 -c classify.cpp 
CFLAGS=-std=c++11 -O2 -fopenmp -g
GPROF=
# sources=main.cpp classify.cpp
sources_orig=main.cpp classify.cpp
sources=main_v1.cpp classify_v1.cpp

objects_orig=$(sources_orig:.cpp=.o)
objects=$(sources:.cpp=.o)

classify:$(objects_orig)
	$(CC) $(CFLAGS) $(GPROF) $^ -o $@
classify_v1:$(objects)
	$(CC) $(CFLAGS) $(GPROF) $^ -o $@


clean:
	rm -f $(objects) $(objects_orig) classify gmon.out
run: classify
	./classify rfile dfile 1009072 4 10
run1: classify_v1
	./classify_v1 rfile dfile 1009072 4 10
runp:
	perf stat ./classify rfile dfile 1009072 4 10
runpr:
	perf record ./classify rfile dfile 1009072 4 1
	perf report -i perf.data
rung: 
	valgrind --tool=cachegrind --branch-sim=yes  ./classify rfile dfile 1009072 4 1
runcallgrind:
	valgrind --tool=callgrind ./classify rfile dfile 1009072 4 1
gprof: classify
	gprof classify gmon.out > analysis_gprof.txt

#classify.o: classify.h
#	touch classify.o
#	$(CC) $(CFLAGS) -c $<
#main.o: classify.h
#	$(CC) $(CFLAGS) -c $<

%.o: %.cpp classify.h
	$(CC) $(CFLAGS)  $(GPROF) -c $<
