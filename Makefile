SRC=cJSON.c ListApi.c vegaJammer.c MapApi.c packFile.c cache.c vegaConfig.c vegaSniffer.c vegaTests.c dnsLookup.c main.c
OUT=bin/arpui_${TARGET}
CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99 -w

all:	$(OUT)

$(OUT): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	$(RM) $(OUT)
