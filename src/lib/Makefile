PREFIX=/usr
CC=gcc
COUTPUTS=dict.o moremath.o

$(COUTPUTS): %.o: %.c
	$(CC) -c -fPIC -o $@ $<

libwzk.so: $(CTARGETS)
	$(CC) $(COUTPUTS) -shared -o $@

clean:
	rm -f *.o *.so
