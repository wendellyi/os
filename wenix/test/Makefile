CC := gcc
CC_OPTION := -c -Wall -g
CC_OUTPUT_OPTION := -o

target = main.o test_vsprintf.o

all: $(target)
	$(CC) -o test $(target)

%.o: %.c
	$(CC) $(CC_OPTION) $< $(CC_OUTPUT_OPTION) $@

.PHONY: all clean

clean:
	rm *.o -fr