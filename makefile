CXX = g++
CXXFLAGS = -Wall -g

mytest: swarm.o mytest.cpp
	$(CXX) $(CXXFLAGS) swarm.o mytest.cpp -o mytest

swarm.o: swarm.cpp swarm.h
	$(CXX) $(CXXFLAGS) -c swarm.cpp

clean:
	rm *.o*
	rm *~

run:
	./mytest

val:
	valgrind ./mytest
