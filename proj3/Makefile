CC=gcc #매크로 정의
CCFLAGS=
#add C source files here
SRCS=main.c

TARGET=mips-sim #타겟
OBJS := $(patsubst %.c,%.o,$(SRCS))
all: $(TARGET)
%.o:%.c
	$(CC) $(CCFLAGS) $< -c -o $@
$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@
.PHONY=clean
clean:
	rm -f $(OBJS) $(TARGET)