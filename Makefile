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

CPPCHECK_VER:=$(shell cppcheck --version 2>/dev/null)
ifdef CPPCHECK_VER
CPPCHECK=cppcheck \
	--enable=warning,style \
	--language=c -q
else
CPPCHECK=\#
endif

all: $(BIN) docs

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC)/cli.o: $(SRC)/version.h

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

docs: $(MAN_PAGES)

man/%.1: man/%.md
	ronn -rw --manual "iwc Manual" --pipe $< > $@

man/%.html: man/%.md
	ronn -w5 -s toc --manual "iwc Manual" --pipe $< > $@

clean:
	find . -name '*.o' -delete
	rm -f $(BIN)
	rm -rf *.tmp

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
	@# copied from https://github.com/bfontaine/homer
	make man/iwc.html
	cp man/iwc.html /tmp/
	git stash
	git checkout gh-pages
	mv /tmp/iwc.html index.html
	git add index.html
	git commit -m "saving man page to GitHub docs"
	git push origin gh-pages
	git checkout master
	git stash pop || true
