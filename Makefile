bridge-finder: bridge-finder.o
	g++ -o bridge-finder bridge-finder.o

bridge-finder.o:
	g++ -c bridge-finder.cpp

clean:
	rm bridge-finder.o bridge-finder
