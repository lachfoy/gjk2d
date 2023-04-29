CXXFLAGS := -std=c++20 -g -Wall -Wextra -Wpedantic
INCLUDES := -Iinclude

LIBS := -lmingw32 -lSDL2main -lSDL2
OBJECTS := glad.o\
	Input.o\
	Renderer.o\
	Font.o\
	TextRenderer.o\
	Physics.o\
	Game.o\
	main.o

2dgame: $(OBJECTS)
	g++ $^ -o test $(LIBS)

glad.o: glad.c
	gcc $(INCLUDES) -c $^ -o $@

%.o: src/%.cpp
	g++ $(INCLUDES) $(CXXFLAGS) -c $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -fo test.exe $(OBJECTS)