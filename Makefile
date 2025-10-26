.PHONY: all clean

TARGET = ./bin/run
CC     = clang
CFLAGS = -fsanitize=address -Iinclude -Wall -g -Werror 
# CFLAGS = -Iinclude -Wall -g -Werror 
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
	# rm -f ./bin/run
