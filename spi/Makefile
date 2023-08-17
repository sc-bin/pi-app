cc = gcc

src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src) )

%:%.c
	$(cc)  $< -o $@ -lrt  -pthread -lwiringPi -lwiringPiDev

all:$(target)

clean:
	-rm $(target)

