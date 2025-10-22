.PHONY: all clean

TARGET = ./bin/run
CC     = gcc
CFLAGS = -g -Iinclude
SRCDIR = src
SRC = $(SRCDIR)/*.c

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@echo "Build done."

clean:
	@echo "Cleaning up..."
	rm -f  vgcore*
