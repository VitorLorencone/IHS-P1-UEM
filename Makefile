# Variables with OS check
FLAGS = -Wall
OBJS = main.o teste.o

ifeq ($(OS),Windows_NT)
    RM = del /F /Q
    EXT = .exe
else
    RM = rm -f
    EXT =
endif

# Default Rule (> make)
all: main$(EXT)

# Executable from .o files
main$(EXT): $(OBJS)
	gcc $(FLAGS) -o $@ $^

# .o from .c files
main.o: main.c main.h
	gcc $(FLAGS) -c $<

# Run Rule (> make run)
run: main$(EXT)
	./main$(EXT)

# Clean Rule (> make clean)
clean:
	rm -f *.o main$(EXT)
	$(RM) *.o
	$(RM) main$(EXT)