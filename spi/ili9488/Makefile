cc = aarch64-linux-gnu-gcc

src = $(wildcard *.c)
target = exe



all:
	$(cc)  ${src} -o ${target} -lrt  -pthread -lwiringPi -lwiringPiDev

clean:
	-rm $(target)

