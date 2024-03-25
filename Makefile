EXE=allocate

$(EXE): main.o Process.o Scheduler.o
	g++ -Wall -o $(EXE) main.o Process.o Scheduler.o

main.o: main.cpp
	g++ -c main.cpp

Process.o: Process.cpp Process.h State.h
	g++ -c Process.cpp
	
Scheduler.o: Scheduler.cpp Scheduler.h
	g++ -c Scheduler.cpp

clean:
	rm -rf *.o $(EXE)

format:
	clang-format -style=file -i *.c
