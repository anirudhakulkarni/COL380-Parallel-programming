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
	./a.out data/data_image.txt data/query_image.txt 10000 1 20
t1:
	nvcc -arch=sm_35 main.cu -o outmain
	CUDA_VISIBLE_DEVICES="1" ./outmain data/t1/data_image_a.txt data/t1/query_image_a_.txt 3 0.2 5
t2:
	nvcc -arch=sm_35 main.cu -o outmain
	CUDA_VISIBLE_DEVICES="1" ./outmain data/t2/data_image_a.txt data/t2/query_image_a_.txt 3 1 5
t3:
	nvcc -arch=sm_35 main.cu -o outmain
	CUDA_VISIBLE_DEVICES="1" ./outmain data/t3/data_image_a.txt data/t3/query_image_a_.txt 6 0.5 5
t4:
	nvcc -arch=sm_35 main.cu -o outmain
	CUDA_VISIBLE_DEVICES="1" ./outmain data/t4/data_image_a.txt data/t4/query_image_a_.txt 6 0.5 5
