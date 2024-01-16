# 设置编译器
CC := gcc
# 设置编译参数
CFLAGS :=  -lrt  -pthread -lwiringPi -lwiringPiDev

SOURCES := $(shell find . -maxdepth 1 -name "*.c")
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE := exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(CFLAGS)
	@echo "Generated executable: $(EXECUTABLE)"

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
	@echo "Removed executable and object files"
