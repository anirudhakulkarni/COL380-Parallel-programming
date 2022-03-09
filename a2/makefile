compile:
	mpic++ -std=c++17 -o wtf *.cpp librandomizer.a
run:
	mpirun -np 4 ./wtf data/8717_31525/edges.dat 8717 31525 0.1 5000 30 20 369
rs:
	mpirun -np 1 ./wtf data/8717_31525/edges.dat 8717 31525 0.1 5000 30 20 369

clean:
	rm wtf
all:
	rm wtf -f
	mpic++ -std=c++17 -o wtf *.cpp librandomizer.a
	mpirun -np 1 ./wtf data/8717_31525/edges.dat 8717 31525 0.1 5000 30 20 369
test:
	rm wtf -f
	mpic++ -std=c++17 -o wtf test.cpp librandomizer.a
	mpirun -np 1 ./wtf data/custom/edges.txt 10 20 0 10 9 5 369
testm:
	rm wtf -f
	mpic++ -std=c++17 -o wtf test.cpp librandomizer.a
	mpirun -np 1 ./wtf data/8717_31525/edges.dat 8717 31525 0 100 30 20 369
main:
	rm wtf -f
	mpic++ -std=c++17 -o wtf main.cpp librandomizer.a
	mpirun -np 1 ./wtf data/8717_31525/edges.dat 8717 31525 0 100 30 20 369
f2:
	# module load compiler/gcc/9.1/openmpi/4.0.2
	rm wtf -f
	mpic++ -std=c++17 -o wtf final.cpp librandomizer.a
	mpirun -np 2 ./wtf data/8717_31525/edges.dat 8717 31525 0.1 100 30 20 369
