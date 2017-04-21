CC=gcc

SH_CFLAGS=-shared -ldl -fPIC

SOURCES=lmhack.c

SHAREDLIB=lmhack.so
SHARED32LIB=lmhack32.so

all: $(SHAREDLIB) $(SHARED32LIB) $(SOURCES)

$(SHAREDLIB): $(SOURCES)
	$(CC) $(SH_CFLAGS) $(SOURCES) -o $@

$(SHARED32LIB): $(SOURCES)
	$(CC) $(SH_CFLAGS) -m32 $(SOURCES) -o $@

clean:
	rm -f $(SHAREDLIB) $(SHARED32LIB)
