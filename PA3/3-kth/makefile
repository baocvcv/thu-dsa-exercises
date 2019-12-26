main: main.cpp
	g++ main.cpp kth_lib.cpp -o main

debug: main.cpp
	g++ main.cpp kth_lib.cpp -g -o main

test: test.cpp
	g++ kth_sample.cpp kth_lib.cpp -o test

clean:
	rm main test
	rm *.o
