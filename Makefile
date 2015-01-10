all: bridge-finder

bridge-finder: bridge-finder.o
	g++ bridge-finder.o -o bridge-finder

bridge-finder.o: bridge-finder.cpp
	g++ -c bridge-finder.cpp

clean:
	rm -rf *o bridge-finder
