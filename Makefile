include config.mk

LIBNAME=$(LIBPRE)oauth2.$(LIBEXT)

%.o: %.c %.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

all: $(LIBNAME) test

$(LIBNAME): oauth2.o curl_request.o
	$(AR) $(ARFLAGS) $@ $^

LDFLAGS+=-L. -loauth2
test: test.o $(LIBNAME)
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -vf *.a *.lib *.o test

.PHONY: all test clean
