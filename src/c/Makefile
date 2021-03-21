TARGETS=$(shell find . | grep '\.c' | sed 's@\./@@;s/\.c//;s/^/wzk-/' | xargs)

.PHONY: all
all: $(TARGETS)

$(TARGETS): wzk-%: ./%.c
	$(CC) -o $(PREFIX)/bin/$@ $<
