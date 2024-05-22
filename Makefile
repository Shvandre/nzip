CC=gcc
CXX=g++
CFLAGS=-std=c11 -D_POSIX_C_SOURCE=200809L --coverage -Wall -Wextra -Wl,-z,noexecstack -fstack-protector -fstack-protector-strong -fstack-protector-all
LDFLAGS=-lz --coverage
SANFLAGS=-g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
GTEST_LIBS = -lgtest -lgtest_main -pthread
CXXFLAGS = -std=c++17 -Wall -Wextra -fpermissive --coverage

# Source files
SOURCES=$(wildcard Source/*.c)
OBJS = $(SOURCES:.c=.o)
# Default build
all: build-san

# Custom target to build both executables
build: zip unzip

# Build with sanitizers
build-san: zip-san unzip-san
# zip executable
zip: $(OBJS) zip.o
	$(CC) -o $@ $^ $(LDFLAGS)

# unzip executable
unzip: $(OBJS) unzip.o
	$(CC) -o $@ $^ $(LDFLAGS)

# zip executable with sanitizers
zip-san: zip.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(SANFLAGS)

# unzip executable with sanitizers
unzip-san: unzip.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(SANFLAGS)

test: Tests.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIBS) $(LDFLAGS)
	./test

coverage: test
	gcov Source/*.c
	lcov --capture --directory Source --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	gcov Source/*.c
	echo "Graphical coverage report is in coverage/index.html"
	echo "To view the report, open coverage/index.html in a web browser or run 'xdg-open coverage/index.html'"

# To manage object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

analyze:
	@which cppcheck > /dev/null || (echo "cppcheck is not installed. Please install it and try again." && exit 1)
	cppcheck --enable=all --inconclusive --std=c99 --language=c --force --suppress=missingIncludeSystem Source/*.c unzip.c zip.c

# Clean build files
clean:
	rm -f *.o zip unzip zip-san unzip-san Source/*.o Source/*.gcno Source/*.gcda Source/*.gcov tests tests.o *.gcno *.gcda *.gcov coverage.info output.txt archive.zip test
	rm -rf coverage

.PHONY: all build build-san cleanret
