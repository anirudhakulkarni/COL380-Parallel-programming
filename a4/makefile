test:
	nvcc -arch=sm_35 test.cu -o a.out
	./a.out
main:
	nvcc -arch=sm_35 main.cu -o outmain
	./outmain 
perf:
	nvcc -arch=sm_35 perf.cu -o perf
	./perf
run:
	./run.sh data/data_image.txt data/query_image.txt 10 2 20
c:
	g++ test2.cpp -o a.out
	./a.out data/data_image.txt data/query_image.txt 10000 1 3