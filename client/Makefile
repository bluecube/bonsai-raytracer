CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
DEPFILES = $(CFILES:.c=.d)

CFLAGS = -Wall -g -std=gnu99
LDFLAGS = -Wall -lm
PROG = bonsai-client

LDFLAGS += $(shell pkg-config --libs json)
CFLAGS += $(shell pkg-config --cflags json)

all: $(PROG)

$(PROG): $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OFILES)

clean:
	rm -f $(OFILES) $(DEPFILES)

%.d: %.c
	$(CC) $(CFLAGS) -MM -MT $@ -MT $(@:.d=.o) -MF $@ $<

-include $(DEPFILES)

