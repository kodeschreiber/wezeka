PREFIX=/usr
CC=gcc
STARGETS=wzk-sekt wzk-mkblk wzk-proxy wzk-factor wzk-dict
CTARGETS=wzk-chaser

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

all: factor wzk-mkblk wzk-dict wzk-proxy

clean:
	rm ./wzk*
