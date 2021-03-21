BUILD_TARGETS=$(wildcard src/*)
ALL_BINS=$(wildcard build/bin/*)
ALL_DOCS=$(wildcard docs/*)
BUILD_ROOT=$(shell pwd)

.PHONY: all
all: build-all


### BUILD SECTION ###
.PHONY: build-all
build-all: build-bins build-docs

.PHONY: build-bins
build-bins:
	mkdir -p ./build/bin ./build/etc/bash_completion.d
	cd ./src/skel; \
	$(MAKE) PREFIX="$(BUILD_ROOT)/build"
	cd ./src/c; \
	$(MAKE) PREFIX="$(BUILD_ROOT)/build" CC=$(CC) CFLAGS=$(CFLAGS)
	cp ./src/wzk.sh ./build/etc/bash_completion.d/

.PHONY: build-docs
build-docs:
	mkdir -p ./build/share/wzk
	cp ./docs/* ./build/share/wzk/


### INSTALL SECTION ###
.PHONY: install-bins
install-bins:
	mkdir -p $(PREFIX)/bin
	for file in $(ALL_BINS); do install -m 550 $$file $(PREFIX)/bin; done
	mkdir -p $(PREFIX)/etc/bash_completion.d
	install -m 555 ./build/etc/bash_completion.d/wzk.sh $(PREFIX)/etc/bash_completion.d

.PHONY: install-docs
install-docs:
	mkdir -p $(PREFIX)/share/wzk
	for file in $(ALL_DOCS); do install -m 444 $$file $(PREFIX)/share/wzk; done

.PHONY: install
install: install-bins install-docs


### CLEAN SECTION ###
.PHONY: clean
clean:
	rm -rf ./build

.PHONY: legacy_uninstall
legacy_uninstall:
	rm -f $(PREFIX)/bin/wzk-*
	rm -f /etc/bash_completion.d/wzk*

.PHONY: uninstall
uninstall:
	rm -f  $(PREFIX)/bin/wzk-*
	rm -f  /etc/bash_completion.d/wzk.sh
	rm -rf $(PREFIX)/share/wzk
