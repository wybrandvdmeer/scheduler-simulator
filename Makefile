EXE=allocate

$(EXE): main.o Process.o Scheduler.o MemStrategy.o FirstFitMemStrategy.o PagedMemStrategy.o \
		VirtualMemStrategy.o
	g++ -Wall -o $(EXE) main.o Process.o Scheduler.o MemStrategy.o FirstFitMemStrategy.o \
		PagedMemStrategy.o VirtualMemStrategy.o

main.o: main.cpp
	g++ -c main.cpp

Process.o: Process.cpp Process.h State.h
	g++ -c Process.cpp
	
Scheduler.o: Scheduler.cpp Scheduler.h MemStrategy.h
	g++ -c Scheduler.cpp

MemStrategy.o: MemStrategy.cpp MemStrategy.h
	g++ -c MemStrategy.cpp

VirtualMemStrategy.o: VirtualMemStrategy.cpp VirtualMemStrategy.h
	g++ -c VirtualMemStrategy.cpp

FirstFitMemStrategy.o: FirstFitMemStrategy.cpp FirstFitMemStrategy.h MemStrategy.h
	g++ -c FirstFitMemStrategy.cpp

PagedMemStrategy.o: PagedMemStrategy.cpp PagedMemStrategy.h MemStrategy.h
	g++ -c PagedMemStrategy.cpp

clean:
	rm -rf *.o $(EXE)

format:
	clang-format -style=file -i *.c
