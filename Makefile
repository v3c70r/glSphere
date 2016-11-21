MAIN = gl4
TOOLPATH = tools/src/
TOOLINCLUDEPATH = tools/include/

CC = g++
LIBS = -lglut -lGLU -lGL -lIL -lILU -lGLEW

$(MAIN) : $(MAIN).cpp  Mesh.cpp Mesh.h coordinate.cpp coordinate.h $(TOOLPATH)* $(TOOLINCLUDEPATH)* 
	$(CC) -o $(MAIN) -g $(MAIN).cpp Mesh.cpp coordinate.cpp $(TOOLPATH)*.cpp -I $(TOOLINCLUDEPATH) $(LIBS)

clean:
	rm -f *.o

