CC = g++
ROOTFLAGS = `root-config --cflags --glibs`
#CC = i686-w64-mingw32-g++ #for windows

OBJ = obj/display.o obj/writetofile.o

all: versus analysis

versus: $(OBJ) src/versus.cc include/versus.h
	$(CC) -g -o darts_versus $(OBJ) src/versus.cc

obj/display.o: include/display.h src/display.cc
	$(CC) -g -c -o obj/display.o -pthread -I/sw/include/root src/display.cc

obj/writetofile.o: include/writetofile.h src/writetofile.cc
	$(CC) -g -c  -o obj/writetofile.o -pthread -I/sw/include/root src/writetofile.cc

analysis: src/analysis.cc include/analysis.h
	$(CC) -g -o analysis src/analysis.cc include/analysis.h $(ROOTFLAGS)

clean:
	rm -f $(OBJ)
