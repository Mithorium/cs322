# Second one for gdb debugging
CC = g++
# CC = g++ -g -O0

all: sparsepoly

sparsepoly:	PolynomialLinkedList.h PolynomialLinkedList.cc
	$(CC) -DUSE_SPARSEPOLY_MAIN PolynomialLinkedList.cc -o SparsePoly

sampletimer:	PolynomialLinkedList.h PolynomialLinkedList.cc SampleTimer.cc
	$(CC) PolynomialLinkedList.cc SampleTimer.cc -o SampleTimer

clean:
	rm -f *.o SparsePoly SampleTimer
