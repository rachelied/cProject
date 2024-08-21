# Compilation macros
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

# Directories
SRCDIR = sources
HDRDIR = headers
OBJDIR = obj
OUTDIR = output

# Source files
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Header files
HDRS = $(wildcard $(HDRDIR)/*.h)

# Executable name
TARGET = assembler

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compiling source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDRS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -I$(HDRDIR) -o $@

# Run target
run: $(TARGET)
	@mkdir -p $(OUTDIR)
	./$(TARGET) -o $(OUTDIR)

# Clean target
clean:
	rm -rf $(TARGET) $(OBJDIR) $(OUTDIR)

# Phony targets
.PHONY: all clean run
