# Компилятор
CC := gcc

# Опции компиляции
CFLAGS := -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -O0 -g
INCLUDE_DIR := -I./inc/

SRCS := $(wildcard ./src/*.c)
OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:./src/%=./out/%)


app.exe : $(OBJS)
	$(CC) $^ -o $@

out/%.o : src/%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

out/%.d : src/%.c
	$(CC) $(INCLUDE_DIR) -MM $< > $@

# Список целей, которым не нужны зависимости
NODEPS := clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
  include $(OBJS:.o=.d)
endif

.PHONY : clean
clean :
	$(RM) ./out/* ./*.exe
