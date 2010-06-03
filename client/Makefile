CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
DEPFILES = $(CFILES:.c=.d)
CFLAGS = -Wall -g -std=gnu99
CFLAGS += $(shell pkg-config --cflags json)
LDFLAGS = -Wall
LDFLAGS += $(shell pkg-config --libs json)
PROG = bonsai-client

all: $(PROG)

$(PROG): $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OFILES)

clean:
	rm -f $(OFILES) $(DEPFILES)

%.d: %.c
	$(CXX) -MM -MG $(CXXFLAGS) $< | sed '1 s|:| $@:|' > $@

-include $(DEPFILES)

