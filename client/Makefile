CFILES = $(wildcard *.c)
CFILES += $(wildcard objects/*.c)
CFILES += $(wildcard surfaces/*.c)
CFILES += $(wildcard lights/*.c)
OFILES = $(CFILES:.c=.o)
DEPFILES = $(CFILES:.c=.d)
PROG = bonsai-client

override CFLAGS += -Wall -Wextra -std=gnu99 -D_ISOC99_SOURCE -march=native -I.
override LDFLAGS += -Wall -Wextra -lm

override LDFLAGS += $(shell pkg-config --libs json)
override CFLAGS += $(shell pkg-config --cflags json)

override LDFLAGS += $(shell pkg-config --libs gsl)
override CFLAGS += $(shell pkg-config --cflags gsl)

# debug flag
DEBUG=0
ifneq ($(DEBUG),1)
  $(info Compiling in optimized mode)
  override CFLAGS += -O3 -DNDEBUG
else
  $(info Compiling in debug mode)
  override CFLAGS += -O0 -g
endif

all: $(PROG)

$(PROG): $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OFILES)

clean:
	rm -f $(OFILES) $(DEPFILES)

%.d: %.c
	$(CC) $(CFLAGS) -MM -MT $@ -MT $(@:.d=.o) -MF $@ $<

-include $(DEPFILES)

