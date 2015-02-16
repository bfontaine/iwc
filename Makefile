# iwc Makefile
#
# most of this was copied from https://github.com/bfontaine/homer

CC        ?= gcc
PREFIX    ?= /usr/local
BINPREFIX ?= $(PREFIX)/bin
MANPREFIX ?= $(PREFIX)/share/man/man1

SRC:=src

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

MANS=$(wildcard man/*.md)
MAN_PAGES=$(MANS:.md=.1)

BIN:=iwc

CFLAGS=-Wall -Wextra -Wundef -Wpointer-arith -std=gnu99
LDFLAGS=

RONN=ronn

ifdef TRAVIS
RONN=\#
endif

all: $(BIN) docs

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC)/cli.o: $(SRC)/version.h

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

docs: $(MAN_PAGES)

man/%.1: man/%.md
	$(RONN) -rw --manual "iwc Manual" --pipe $< > $@

man/%.html: man/%.md
	$(RONN) -w5 -s toc --manual "iwc Manual" --pipe $< > $@

clean:
	find . -name '*.o' -delete
	$(RM) $(BIN)
	$(RM) -r *.tmp

install: $(BIN) docs
	@mkdir -p $(BINPREFIX)
	@mkdir -p $(MANPREFIX)
	cp -f $(BIN) $(BINPREFIX)/$(BIN)
	cp -f man/*.1 $(MANPREFIX)/

uninstall:
	rm -f $(BINPREFIX)/$(BIN)
	$(foreach MAN, $(MAN_PAGES), \
		rm -f $(MANPREFIX)/$(notdir $(MAN)))

ghpages:
	git checkout gh-pages
	git show master:man/iwc.html > index.html
	git commit -m "saving man page to GitHub docs" index.html
	git push origin gh-pages
	git checkout @{-1}
