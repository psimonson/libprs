CC=gcc
CFLAGS+=-std=c11 -Wall -Wextra -Wno-unused-function -g -O0 -fPIC
LDFLAGS+=

SRCDIR=$(shell basename $(shell pwd))
VERSION=1.0
TARNAME=$(SRCDIR)-$(VERSION)

SOURCE=$(wildcard *.c)
OBJECT=$(SOURCE:%.c=%.c.o)
TARGET=$(SRCDIR).a

DESTDIR?=
PREFIX?=/usr/local

HEADER_TARGETS=\
	byte_order.h\
	abuffer.h\
	tpool.h\
	debug.h\
	clist.h\
	ctest.h\
	vector.h\
	ui.h\
	ui_menu.h\
	ui_util.h\
	network.h\
	netutil.h

.PHONY: all install remove clean distclean dist
all: $(TARGET)

install: all
	@echo -n "Installing project... "
	@(mkdir -p $(DESTDIR)/$(PREFIX)/include/prs2 && \
	cp $(HEADER_TARGETS) $(DESTDIR)/$(PREFIX)/include/prs2 && \
	chmod 644 $(DESTDIR)/$(PREFIX)/include/prs2/*.h && \
	mkdir -p $(DESTDIR)/$(PREFIX)/lib && \
	cp $(TARGET) $(DESTDIR)/$(PREFIX)/lib && \
	chmod 644 $(DESTDIR)/$(PREFIX)/lib/$(TARGET)) && \
	echo "done!" || echo "failed!"

remove:
	@echo -n "Removing installation... "
	@(rm -rf $(DESTDIR)/$(PREFIX)/include/prs2 && \
	rm -f $(DESTDIR)/$(PREFIX)/lib/$(TARGET)) && \
	echo "done!" || echo "failed!"

clean:
	@echo -n "Cleaning project... "
	@rm -f $(OBJECT) $(TARGET) && echo "done!" || echo "failed!"

dist: distclean
	@echo "Building distribution: $(TARNAME)..."
	@(cd .. && tar cv --exclude=.git ./$(SRCDIR) | xz -9 > $(TARNAME).tar.xz) && echo "Finished building distribution." || echo "Failed to build distribution."

distclean:
	@echo "Cleaning distribution... "
	@$(MAKE) clean
	@rm -f *.bak && echo "done!" || echo "failed!"

$(TARGET): $(OBJECT)
	@echo -n "Building project: $(TARGET) "
	@ar rcs $@ $^ && echo "- [DONE]" || echo "- [FAIL]"

%.c.o: %.c
	@echo "Compiling source file: $< => $@"
	@$(CC) $(CFLAGS) -c $< -o $@

