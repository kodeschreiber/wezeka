TARGETS=$(shell find . | grep '\.skel' | sed 's@\./@@;s/\.skel//;s/^/wzk-/' | xargs)

.PHONY: all
all: $(TARGETS)

$(TARGETS): wzk-%: ./%.skel
	cat $< | awk -v ctx="./" -f ../awk/psc.awk > $(PREFIX)/bin/$@
