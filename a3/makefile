CC= mpic++ -fopenmp 
CFLAGS=-std=c++11

compile:
	$(CC) $(CFLAGS) main.cpp -o main
compile2:
	$(CC) $(CFLAGS) main_util.cpp -o main2
runp:
	mpirun -np 2 --bind-to none ./main2 5 ./anz_dd_cp/ 1
runs:
	mpirun -np 5 --bind-to none ./main 5 ./anz_dd_cp/ 1
runb:
	mpirun -np 1 --bind-to none ./main 4 ./anz_ts/ 2
clean:
	rm -f main
all:
	$(CC) $(CFLAGS) main.cpp -o main && ./main 5