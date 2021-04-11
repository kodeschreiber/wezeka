BUILD_TARGETS=$(wildcard src/*)
ALL_BINS=$(wildcard build/bin/*)
ALL_DOCS=$(wildcard docs/*)
BUILD_ROOT=$(shell pwd)

.PHONY: all
all: build-all


### BUILD SECTION ###
.PHONY: build-all
build-all: build-bins build-docs build-bbroot

.PHONY: build-bins
build-bins:
	mkdir -p ./build/bin ./build/etc/bash_completion.d
	cd ./src/skel; \
	$(MAKE) PREFIX="$(BUILD_ROOT)/build"
	cd ./src/c; \
	$(MAKE) PREFIX="$(BUILD_ROOT)/build" CC=$(CC) CFLAGS=$(CFLAGS)
	cp ./src/wzk.sh ./build/etc/bash_completion.d/
	chmod +x $(BUILD_ROOT)/build/bin/*

.PHONY: build-docs
build-docs:
	mkdir -p ./build/share/wzk
	cp ./docs/* ./build/share/wzk/

.PHONY: build-bbroot
build-bbroot: build-bins
	if [ -z $(ARCH) ]; then "Please specifiy an architecture 'make ARCH=...'"; exit 1; fi
	mkdir -p $(BUILD_ROOT)/build/bbroot
	cd $(BUILD_ROOT)/build/bbroot; \
	if [ ! -f ./busybox ]; then wget "https://busybox.net/downloads/binaries/1.31.0-defconfig-multiarch-musl/busybox-$(ARCH)"; fi; \
	mv ./busybox* ./busybox; \
	chmod +x ./busybox; \
	./busybox --list | while read app; do ln -sf ./busybox ./$$app; done
	cat ./build/bin/wzk-ifio | sed 's@#!/bin/bash@#!/bin/ash@' > $(BUILD_ROOT)/build/bbroot/wzk-ifio
	install -m 555 ./build/bin/wzk-colorize $(BUILD_ROOT)/build/bbroot/
	chmod +x $(BUILD_ROOT)/build/bbroot/wzk-ifio $(BUILD_ROOT)/build/bbroot/wzk-colorize
	cd $(BUILD_ROOT)/build/bbroot; \
	tar zcf $(BUILD_ROOT)/build/bbroot.tar.gz ./*


### INSTALL SECTION ###
.PHONY: install-bins
install-bins:
	mkdir -p $(PREFIX)/bin
	for file in $(ALL_BINS); do install -m 555 $$file $(PREFIX)/bin; done
	mkdir -p $(PREFIX)/etc/bash_completion.d
	install -m 555 ./build/etc/bash_completion.d/wzk.sh $(PREFIX)/etc/bash_completion.d
	mkdir -p $(PREFIX)/share/wzk
	install -m 444 ./build/bbroot.tar.gz $(PREFIX)/share/wzk

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
