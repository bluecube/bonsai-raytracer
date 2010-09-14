CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
DEPFILES = $(CFILES:.c=.d)
PROG = bonsai-client

override CFLAGS += -Wall -std=gnu99 -D_ISOC99_SOURCE -msse2
override LDFLAGS += -Wall -lm

override LDFLAGS += $(shell pkg-config --libs json)
override CFLAGS += $(shell pkg-config --cflags json)

# debug flag
DEBUG=1
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

