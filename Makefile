CC=gcc
CFLAGS=-std=c11 -DDEBUG_MODE -Wall -Wextra
LDFLAGS=-lz
SANFLAGS=-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

# Source files
SOURCES=$(wildcard Source/*.c)

# Default build
all: build

# Custom target to build both executables
build: zip unzip

# Build with sanitizers
build-san: zip-san unzip-san

# zip executable
zip: $(SOURCES) zip.c
	$(CC) -o $@ $^ $(LDFLAGS)

# unzip executable
unzip: $(SOURCES) unzip.c
	$(CC) -o $@ $^ $(LDFLAGS)

# zip executable with sanitizers
zip-san: zip.o $(SOURCES)
	$(CC) -o $@ $^ $(LDFLAGS) $(SANFLAGS)

# unzip executable with sanitizers
unzip-san: unzip.o $(SOURCES)
	$(CC) -o $@ $^ $(LDFLAGS) $(SANFLAGS)

# To manage object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

analyze:
	@which cppcheck > /dev/null || (echo "cppcheck is not installed. Please install it and try again." && exit 1)
	cppcheck --enable=all --inconclusive --std=c99 --language=c --force --suppress=missingIncludeSystem Source/*.c unzip.c zip.c

# Clean build files
clean:
	rm -f *.o zip unzip zip-san unzip-san

.PHONY: all build build-san clean
