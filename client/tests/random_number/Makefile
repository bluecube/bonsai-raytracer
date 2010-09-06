CFILES = random_number.c
CLIENT_CFILES = random.c
OFILES = $(CFILES:.c=.o)
CLIENT_OFILES = $(CLIENT_CFILES:.c=.o)
DEPFILES = $(CFILES:.c=.d)
PROG = random_number_test

override CFLAGS += -Wall -std=gnu99 -D_ISOC99_SOURCE -I../../
override LDFLAGS += -Wall -lm

override CFLAGS += -O0 -g

all: $(PROG)

$(PROG): $(OFILES) $(addprefix ../../,$(CLIENT_OFILES))
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(CLIENT_OFILES):
	$(MAKE) -C ../../ $@

clean:
	rm -f $(OFILES) $(DEPFILES)

%.d: %.c
	$(CC) $(CFLAGS) -MM -MT $@ -MT $(@:.d=.o) -MF $@ $<

-include $(DEPFILES)

