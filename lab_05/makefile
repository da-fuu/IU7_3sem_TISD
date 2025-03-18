CC := gcc

HEADERS_DIR := inc
SRC_DIR := src
OUT_DIR := out

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

CFLAGS := -std=gnu99 -Wall -Werror -Wvla -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -I$(HEADERS_DIR)

ADD_FLAGS := -O0 -g

all : app.exe

app.exe : CFLAGS += $(ADD_FLAGS)
app.exe : $(OBJS)
	$(CC) $^ -o ./app.exe -lm

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/%.d : $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) -I$(HEADERS_DIR) -MM -MF $@ -MT $(OUT_DIR)/$*.o $<

$(OUT_DIR) : 
	mkdir -p $@

ifneq (clean, $(MAKECMDGOALS))
  include $(DEPS)
endif

clean :
	$(RM) $(OUT_DIR)/* ./*.exe

.PHONY : clean
