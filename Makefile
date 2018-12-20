src = $(wildcard src/*.c)
obj = $(src:.c=.o)

LDFLAGS = -O2

jankyserv: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj)
