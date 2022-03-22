CC= mpic++ -fopenmp 
CFLAGS=-std=c++11

compile:
	$(CC) $(CFLAGS) main.cpp -o main
run:
	mpirun -np 2 --bind-to none ./main 6 ./anz_dd/ 3
clean:
	rm -f main
all:
	$(CC) $(CFLAGS) main.cpp -o main && ./main 5