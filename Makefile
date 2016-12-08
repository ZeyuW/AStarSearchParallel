all:
	g++ -fopenmp -Wall -O3 -o main search_path.cpp -std=c++11
clean:
	rm main.o
