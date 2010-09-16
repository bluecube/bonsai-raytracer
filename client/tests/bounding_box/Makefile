CFILES = bounding_box.c
CLIENT_CFILES = bounding_box.c ray.c vector-sse.c vector-nosse.c random.c
OFILES = $(CFILES:.c=.o)
CLIENT_OFILES = $(addprefix ../../,$(CLIENT_CFILES:.c=.o))
DEPFILES = $(CFILES:.c=.d)
PROG = bounding_box_test

override CFLAGS += -Wall -std=gnu99 -D_ISOC99_SOURCE -I../../ -msse2
override LDFLAGS += -Wall -lm

override CFLAGS += -O0 -g

all: $(PROG)

$(PROG): $(OFILES) $(CLIENT_OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(CLIENT_OFILES):
	$(MAKE) -C ../../ $(notdir $@)

clean:
	rm -f $(OFILES) $(DEPFILES)

%.d: %.c
	$(CC) $(CFLAGS) -MM -MT $@ -MT $(@:.d=.o) -MF $@ $<

-include $(DEPFILES)

