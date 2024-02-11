LD = ld
CC = cc
PKG_CONFIG = pkg-config
INSTALL = install
CFLAGS = -g -O2 -Wall -Wextra
LDFLAGS =
LIBS =
VLC_PLUGIN_CFLAGS := $(shell $(PKG_CONFIG) --cflags vlc-plugin)
VLC_PLUGIN_LIBS := $(shell $(PKG_CONFIG) --libs vlc-plugin)
VLC_PLUGIN_DIR := $(shell $(PKG_CONFIG) --variable=pluginsdir vlc-plugin)

plugindir = $(VLC_PLUGIN_DIR)/misc

override CC += -std=gnu99
override CPPFLAGS += -DPIC -I. -Isrc
override CFLAGS += -fPIC
override LDFLAGS += -Wl,-no-undefined,-z,defs

override CPPFLAGS += -DMODULE_STRING=\"hello\"
override CFLAGS += $(VLC_PLUGIN_CFLAGS)
override LIBS += $(VLC_PLUGIN_LIBS)

all: libhello_plugin.so

install: all
	mkdir -p -- $(DESTDIR)$(plugindir)
	$(INSTALL) --mode 0755 libhello_plugin.so $(DESTDIR)$(plugindir)

install-strip:
	$(MAKE) install INSTALL="$(INSTALL) -s"

uninstall:
	rm -f $(plugindir)/libhello_plugin.so

clean:
	rm -f -- libhello_plugin.so src/*.o

mostlyclean: clean

SOURCES = hello.c

$(SOURCES:%.c=src/%.o): $(SOURCES:%.c=src/%.c)

libhello_plugin.so: $(SOURCES:%.c=src/%.o)
	$(CC) $(LDFLAGS) -shared -o $@ $^ $(LIBS)

.PHONY: all install install-strip uninstall clean mostlyclean

