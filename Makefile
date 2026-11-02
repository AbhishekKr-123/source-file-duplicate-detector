CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
SRC=src/main.c src/file_manager.c src/file_reader.c src/normalizer.c src/tokenizer.c src/comparator.c src/similarity.c src/report.c
OUT=duplicate_detector

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT) $(OUT).exe duplicate_report.txt
