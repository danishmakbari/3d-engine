.PHONY: all clean install uninstall

TARGET = engine

LDIR = lib
IDIR = include
ODIR = obj
SDIR = src

CC = g++
CFLAGS = -I$(IDIR) -pipe -Wall --pedantic -march=native -O2 -c 
LDFLAGS = -L$(LDIR) -no-pie -lpthread -ldl -lm -lSDL2 -lSDL2_image -lGL -lGLEW -lfreetype -lode -llua

SRC = $(wildcard $(SDIR)/*.c)
OBJ = $(patsubst $(SDIR)%.c, $(ODIR)%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(ODIR)/*.o

