# 编译器
CC = gcc
# 编译选项
CFLAGS = -g
# 目标文件
TARGET = main
# 库链接
LIBS = -lwsock32 -lWs2_32
# 源
SRCS = draw.c client.c \
       .\egg_ui\egg_chart.c .\egg_ui\egg_switch.c .\egg_ui\egg_ui.c \
       .\egg_ui\egg_panel.c .\egg_ui\mem_malloc.c egg_ui_test.c \
	   .\egg_ui\port.c systick.c port_u8g2.c

# 生成的对象文件
OBJS = $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 链接生成目标
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# 编译每个源文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 删除目标文件
del:
	del /Q $(OBJS)

# 清理所有文件
clean: del
	del /Q $(TARGET)

# 伪目标
.PHONY: all clean
