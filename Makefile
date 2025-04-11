# Get all source files except main.c
SRC_FILES := $(filter-out src/main.c, $(wildcard src/*.c))

# Build the main program
compile:
	gcc -Wall -Wextra -Werror src/main.c $(SRC_FILES) -o main.out

# Run the main program
r:
	make compile
	./main.out

# Run Valgrind
v:
	make compile
	valgrind --leak-check=full --show-leak-kinds=all ./main.out

# Compile with debugging symbols
compiledebug:
	gcc src/main.c $(SRC_FILES) -g -o main.out

# Run with GDB
d:
	make compiledebug
	gdb ./main.out

# Compile test suite
compiletest:
	gcc -Wall -Wextra -Werror test/test.c test/utils.c $(SRC_FILES) -o test.out

# Run tests
t:
	make compiletest
	./test.out

# Debug test suite
compiletestdebug:
	gcc test/test.c test/utils.c $(SRC_FILES) -g -o test.out

dt:
	make compiletestdebug
	gdb ./test.out

# Clean binaries
c:
	rm -f main.out test.out

# Format code
f:
	clang-format -i src/*.c src/*.h test/*.c test/*.h

# Format check (optional, if implemented)
cf:
	./check-format.sh
