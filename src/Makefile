# Variables with OS check
# -std=c99 needed for better duktape compatibility
FLAGS = -Wall -std=c99
OBJS = main.o edit_distance.o duktape.o

# OS pick
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
	gcc -o $@ $^ -lm

# .o from .c files
main.o: main.c
	gcc $(FLAGS) -c $<

duktape.o: duktape.c duktape.h duk_config.h
	gcc $(FLAGS) -c $<

edit_distance.o: edit_distance.c edit_distance.h
	gcc $(FLAGS) -c $<

# Run Rule (> make run)
run: main$(EXT)
	./main$(EXT)

# Clean Rule (> make clean)
clean:
	$(RM) *.o
	$(RM) main$(EXT)