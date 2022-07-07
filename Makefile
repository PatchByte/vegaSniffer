SRC=cJson.c MapApi.c packFile.c cache.c vegaConfig.c vegaSniffer.c dnsLookup.c main.c
OUT=bin/arpui
CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99

all:	$(OUT)

$(OUT): $(SRC)
	mkdir bin
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	$(RM) $(OUT)
