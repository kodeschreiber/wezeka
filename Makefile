PREFIX=/usr
CC=gcc
STARGETS=wzk-sekt wzk-proxy wzk-factor wzk-dict wzk-profile wzk-isolate
CTARGETS=wzk-chaser wzk-argparse wzk-blkctl

wzk-psc: ./bin/psc.skel
	mkdir -p "$(PREFIX)/libexec"
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)/libexec" ./bin/psc.awk
	cat $< | awk -v ctx="./bin" -f ./bin/psc.awk | sed "s@%prefix%@$(PREFIX)@g" > $@
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)/bin" $@

$(STARGETS): wzk-%: ./bin/%.skel wzk-psc
	export CONTEXT='./bin'; \
	wzk-psc $< | sed '2,$s/#\!\/bin\/bash/REMALL/;/REMALL/d' > $@
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)/bin" $@

$(CTARGETS): wzk-%: ./bin/%.c
	$(CC) -o $@ $<
	/usr/bin/install -o 0 -g 0 -m 555 -t "$(PREFIX)/bin" $@

factor: wzk-chaser wzk-sekt wzk-factor

blkctl: wzk-blkctl
	rm -f $(PREFIX)/bin/wzk-mkblk $(PREFIX)/bin/wzk-mntblk $(PREFIX)/bin/wzk-umntblk
	ln -s $(PREFIX)/bin/wzk-blkctl $(PREFIX)/bin/wzk-mkblk
	ln -s $(PREFIX)/bin/wzk-blkctl $(PREFIX)/bin/wzk-mntblk
	ln -s $(PREFIX)/bin/wzk-blkctl $(PREFIX)/bin/wzk-umntblk

wzk: wzk-proxy
	rm -f $(PREFIX)/bin/wzk
	ln -s $(PREFIX)/bin/wzk-proxy $(PREFIX)/bin/wzk

autofill: ./bin/wzk_autofill.sh
	cat $< | sed "s@%prefix%@$(PREFIX)@g" > ./wzk_autofill.sh
	cp ./wzk_autofill.sh /etc/bash_completion.d/

all: factor blkctl wzk autofill $(STARGETS) $(CTARGETS)

clean:
	rm -f ./wzk*
