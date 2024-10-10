CXX = gcc

CFLAGS = -Wall -ggdb -O2
LDFLAGS = 

OBJDIR = bin
SRCDIR = src
TARGET = main
SRC = $(shell find $(SRCDIR) -name "*.c")
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

.PHONY : all clean

all: $(OBJDIR) $(OBJDIR)/$(TARGET)

$(OBJDIR):
	mkdir -p $@

#dirs: $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< $(CFLAGS) 

$(OBJDIR)/$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

run: all
	$(OBJDIR)/$(TARGET)

clean:
	rm -rf $(OBJDIR)