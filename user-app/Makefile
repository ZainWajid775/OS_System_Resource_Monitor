CC = gcc
CFLAGS = -Wall -O2

all: resmon resmon_debug

resmon: resmon.c
	$(CC) $(CFLAGS) -o resmon resmon.c

resmon_debug: resmon_debug.c
	$(CC) $(CFLAGS) -o resmon_debug resmon_debug.c

clean:
	rm -f resmon resmon_debug

install: resmon
	sudo cp resmon /usr/local/bin/
	sudo chmod +x /usr/local/bin/resmon
	sudo touch /var/log/resmon.log
	sudo chmod 666 /var/log/resmon.log

uninstall:
	sudo rm -f /usr/local/bin/resmon

.PHONY: all clean install uninstall
