CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
DEPFILES = $(CFILES:.c=.d)

CFLAGS = -Wall -std=gnu99
LDFLAGS = -Wall -lm
PROG = bonsai-client

LDFLAGS += $(shell pkg-config --libs json)
CFLAGS += $(shell pkg-config --cflags json)

# debug flag
DEBUG=1
ifneq ($(DEBUG),1)
  $(info Compiling in optimized mode)
  CFLAGS += -O3
else
  $(info Compiling in debug mode)
  CFLAGS += -O0 -g
endif

all: $(PROG)

$(PROG): $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OFILES)

clean:
	rm -f $(OFILES) $(DEPFILES)

%.d: %.c
	$(CC) $(CFLAGS) -MM -MT $@ -MT $(@:.d=.o) -MF $@ $<

-include $(DEPFILES)

