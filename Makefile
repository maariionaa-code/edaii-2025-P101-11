#get all source files except main.c
SRC_FILES := $(filter-out src/main.c, $(wildcard src/*.c))

#build the main program
compile:
	gcc -Wall -Wextra -Werror src/main.c $(SRC_FILES) -o main.out

#run the main program with default dataset
r:
	make compile
	./main.out datasets/wikipedia270

#run Valgrind with the default dataset
v:
	make compile
	valgrind --leak-check=full --show-leak-kinds=all ./main.out datasets/wikipedia270

#compile with debug symbols
compiledebug:
	gcc src/main.c $(SRC_FILES) -g -o main.out

#run with GDB (default dataset)
d:
	make compiledebug
	gdb --args ./main.out datasets/wikipedia270

#compile test suite
compiletest:
	gcc -Wall -Wextra -Werror test/test.c test/utils.c $(SRC_FILES) -o test.out

#run tests
t:
	make compiletest
	./test.out

#debug test suite
compiletestdebug:
	gcc test/test.c test/utils.c $(SRC_FILES) -g -o test.out

dt:
	make compiletestdebug
	gdb ./test.out

#clean binaries
c:
	rm -f main.out test.out

#format code using clang-format
f:
	clang-format -i src/*.c src/*.h test/*.c test/*.h

#check formatting script (if implemented)
cf:
	./check-format.sh

#run all datasets automatically
runall:
	make compile
	for dataset in datasets/wikipedia*; do \
	  echo "=============================="; \
	  echo "Running with: $$dataset"; \
	  echo "=============================="; \
	  ./main.out $$dataset; \
	  echo; \
	done

#show available targets
help:
	@echo "Available targets:"
	@echo "  make compile       - Build main program"
	@echo "  make r             - Run main with default dataset"
	@echo "  make v             - Run with Valgrind"
	@echo "  make compiledebug  - Compile with debug symbols"
	@echo "  make d             - Run debugger with default dataset"
	@echo "  make t             - Run unit tests"
	@echo "  make dt            - Debug test suite"
	@echo "  make c             - Clean output binaries"
	@echo "  make f             - Format source code"
	@echo "  make runall        - Run main with all datasets"
	@echo "  make help          - Show this help"
