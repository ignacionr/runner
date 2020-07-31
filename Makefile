LDFLAGS=`sdl2-config --libs` -lSDL2_image -lSDL2_ttf
CXXFLAGS=-fPIE -std=c++17

run: run.o
	$(CXX) run.o -o run $(LDFLAGS)

clean:
	rm *.o run
