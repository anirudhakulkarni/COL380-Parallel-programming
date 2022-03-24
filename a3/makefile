CC= mpic++ -fopenmp 
CFLAGS=-std=c++11

compile:
	$(CC) $(CFLAGS) main.cpp -o main
compile2:
	$(CC) $(CFLAGS) test.cpp -o main2
runp:
	mpirun -np 1 --bind-to none ./main2 5 ./anz_dd_cp/ 1
runs:
	mpirun --bind-to none ./main 5 ./converted_dd/ ./anz_dd_cp/user.txt
runb:
	mpirun --bind-to none ./main 5 ./converted/ ./converted/user2.txt
runo:
	mpirun --bind-to none ./main 15 ./converted/ ./anz_ts/user.txt pred.txt
runt:
	mpirun --bind-to none ./main2 15 ./converted/ ./anz_ts/user.txt
clean:
	rm -f main


all:
	$(CC) $(CFLAGS) main.cpp -o main && ./main 5
	# qsub -I -P col380.cs5190421 -N interactive_shell -lselect=1:ncpus=4 -lwalltime=00:30:00 /
	# lfs quota -hu $USER /home