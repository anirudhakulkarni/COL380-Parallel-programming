CC= mpic++ -fopenmp 
CFLAGS=-std=c++11

compile:
	$(CC) $(CFLAGS) main.cpp -o main
run:
	mpirun -np 1 --bind-to none ./main 5 ./anz_dd/ 1
clean:
	rm -f main
all:
	$(CC) $(CFLAGS) main.cpp -o main && ./main 5