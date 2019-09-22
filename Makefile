DEST=$(HOME)/bin

precise-time: precise-time.c
	$(CC) -o $@ -W -Wall precise-time.c

clean:
	rm -f precise-time

install: precise-time
	mkdir -p $(DEST)
	install precise-time $(DEST)
