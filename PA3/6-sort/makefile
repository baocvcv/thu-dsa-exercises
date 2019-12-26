main: main.cpp
	g++ main.cpp sort_lib.cpp -o main

debug: main.cpp
	g++ main.cpp sort_lib.cpp -g -o main

test: sort_sample.cpp
	g++ sort_sample.cpp sort_lib.cpp -o test

clean:
	rm main test
