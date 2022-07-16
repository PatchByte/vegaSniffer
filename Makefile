SRC=str2int.c cJSON.c ListApi.c vegaJammer.c MapApi.c packFile.c cache.c vegaConfig.c vegaSniffer.c vegaTests.c dnsLookup.c main.c
OUT=bin/arpui
CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99

all:	$(OUT)

$(OUT): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	$(RM) $(OUT)
