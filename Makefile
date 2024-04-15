SRCDIR = src

BINDIR = bin

CC = gcc

PPDIR = $(SRCDIR)/preprocessor
SOLDIR = $(SRCDIR)/solver

all:
	mkdir -p $(BINDIR)
	$(CC) $(PPDIR)/*.c -o $(BINDIR)/pp
	$(CC) $(SOLDIR)/*.c -o $(BINDIR)/solver

preprocessor:
	mkdir -p $(BINDIR)
	$(CC) $(PPDIR)/*.c -o $(BINDIR)/pp

solver:
	mkdir -p $(BINDIR)
	$(CC) $(SOLDIR)/*.c -o $(BINDIR)/solver

clean:
	rm -rf $(BINDIR)

