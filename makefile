CSR:			GraphCSR.o Main.o
				g++ GraphCSR.o Main.o -o CSR

GraphCSR.o:		GraphCSR.cpp GraphCSR.h
				g++ -c GraphCSR.cpp

Main.o:			Main.cpp GraphCSR.h
				g++ -c Main.cpp

clean:
				rm -f *.o
				rm CSR




#
#	PA5 makefile
#	> type "make" to make "CSR"
#	> to run:	./CSR <text file of vertices> <integer vertex>
#
#	! note: possible odd results from very large input
#	! (every recipe must begin with a tab)
#