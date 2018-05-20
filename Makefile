CC =  g++ -Wextra -std=c++14
OUTPUT = TripPlanner
all: main clean

main: main connection graphviewer InfoLoader menu string
	$(CC) -o $(OUTPUT) Main.cpp connection.o graphviewer.o info.o menu.o string.o

connection:
	$(CC) -c GraphViewer/connection.cpp -o connection.o

graphviewer:
	$(CC) -c GraphViewer/graphviewer.cpp -o graphviewer.o

InfoLoader:
	$(CC) -c InfoLoader.cpp -o info.o

menu:
	$(CC) -c menu.cpp -o menu.o

string:
	$(CC) -c stringSearch.cpp -o string.o

# Compilation for Dijkstra algorithms performance tests
testDijkstra: 
	$(CC) -o test_dijkstra test_dijkstra.cpp

# Compilation for String matching/aproximattion algorithms performance tests
testString: string
	$(CC) -o test_string test_str.cpp string.o

clean:
	rm *.o
