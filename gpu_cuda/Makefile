
all: cuda.cu cuda.hpp
	nvcc -dc cuda.cu -o cuda.o --optimize 3
	nvcc -dlink cuda.o -o gpu_cuda.o --optimize 3

clean:
	rm -rf *.o
