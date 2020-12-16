PREFIX=/usr/bin
TMPDIR=$(shell pwd)/.tmp

setup:
	mkdir -p $(TMPDIR)

psc: setup
	cd bin; \
	cat psc.skel | awk -f psc.awk > $(TMPDIR)/wzk-psc
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)" $(TMPDIR)/wzk-psc

sekt: setup psc
	cd bin; \
	wzk-psc ./sekt.skel > $(TMPDIR)/wzk-sekt
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)" $(TMPDIR)/wzk-sekt

proxy: setup psc
	cd bin; \
	wzk-psc ./proxy.skel > $(TMPDIR)/proxy
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)" $(TMPDIR)/proxy

chaser: setup
	cd bin; \
	$(CC) -o $(TMPDIR)/wzk-chaser chaser.c
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)" $(TMPDIR)/wzk-chaser

factor: setup psc sekt chaser
	cd bin; \
	wzk-psc ./factor.skel | sed '2,$s/#\!\/bin\/bash/REMALL/;/REMALL/d' > $(TMPDIR)/wzk-factor
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)" $(TMPDIR)/wzk-factor

teardown:
	rm -rf $(TMPDIR)
