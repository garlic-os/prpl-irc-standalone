TARGET := prplirc.so

IRCSOURCES = \
	cmds.c \
	dcc_send.c \
	irc.c \
	irc.h \
	msgs.c \
	parse.c

CC := g++
PACKAGES := purple
CFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic -MMD -fPIC \
          -DGLIB_DISABLE_DEPRECATION_WARNINGS \
          $(shell pkg-config --cflags $(PACKAGES))
CFLAGS += -isystem /usr/include/libpurple
# CFLAGS += -D_GNU_SOURCE
LDFLAGS := $(shell pkg-config --libs $(PACKAGES)) -shared


$(TARGET): $(IRCSOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET).d
