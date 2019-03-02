CFLAGS = -Wall -std=c++11 -I/usr/local/include -L/usr/local/lib -L/usr/local/lib64
LFLAGS = -lglfw3 -lGLEW -lopenal -lGL -lXrandr -lXinerama -lassimp -lXcursor -lXi -lXxf86vm -lX11 -pthread -lrt -lm -ldl -Wl,-rpath,/usr/local/lib64 -Wl,-rpath,/usr/local/lib
CXX = g++

a.out: main.o Mesh.o Model.o
	$(CXX) main.o Mesh.o Model.o $(CFLAGS) $(LFLAGS)

Mesh.o: Mesh.cpp Mesh.h
	$(CXX) -Wall -std=c++14 -c Mesh.cpp

Model.o: Model.cpp Model.h
	$(CXX) -Wall -std=c++14 -c Model.cpp

main.o: main.cpp Window.h Shader.h DrawFigure.h Model.h Mesh.o
	$(CXX) -Wall -std=c++14 -c main.cpp

clean:
	$(RM) *.o
