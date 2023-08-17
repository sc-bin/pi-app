# 设置编译器
CC := gcc
# 设置编译参数
CFLAGS :=  -lrt  -pthread -lwiringPi -lwiringPiDev

SUBDIRS := $(shell find . -maxdepth 1 -type d ! -name ".*")

all:
	@for dir in $(SUBDIRS); do \
		if [ "$$dir" != "." ]; then \
			SOURCES=$$(find $$dir -maxdepth 1 -name "*.c"); \
			for src in $${SOURCES}; do \
				filename=$$(basename $$src .c); \
				if [ ! -f $$dir/$$filename ] || [ $$src -nt $$dir/$$filename ]; then \
					$(CC) $(CFLAGS) $$src -o $$dir/$$filename; \
					echo "Generated executable: $$dir/$$filename"; \
				fi; \
			done; \
			SUBSUBDIRS=$$(find $$dir -mindepth 1 -maxdepth 1 -type d ! -name ".*"); \
			for subdir in $${SUBSUBDIRS}; do \
				subdir_name=$$(basename $$subdir); \
				SOURCES=$$(find $$subdir -maxdepth 1 -name "*.c"); \
				if [ ! -f $$subdir/$$subdir_name ] || [ $$(find $$subdir -maxdepth 1 -name "*.c" -newer $$subdir/$$subdir_name | wc -l) != 0 ]; then \
					echo "Generated executable: $$subdir/$$subdir_name"; \
					$(CC)  $${SOURCES} -o $$subdir/$$subdir_name $(CFLAGS) ; \
				fi; \
			done; \
		fi; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		if [ "$$dir" != "." ]; then \
			SOURCES=$$(find $$dir -maxdepth 1 -name "*.c"); \
			for src in $${SOURCES}; do \
				filename=$$(basename $$src .c); \
				rm -f $$dir/$$filename; \
				echo "Removed executable: $$dir/$$filename"; \
			done; \
			SUBSUBDIRS=$$(find $$dir -mindepth 1 -maxdepth 1 -type d ! -name ".*"); \
			for subdir in $${SUBSUBDIRS}; do \
				subdir_name=$$(basename $$subdir); \
				rm -f $$subdir/$$subdir_name; \
				echo "Removed executable: $$subdir/$$subdir_name"; \
			done; \
		fi; \
	done
