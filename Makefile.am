ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

SUBDIRS = doc

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libtinynotify.la
pkgconfig_DATA = libtinynotify.pc
include_HEADERS = lib/tinynotify.h

libtinynotify_la_LDFLAGS = -version-info 0:0:0
libtinynotify_la_CPPFLAGS = $(DBUS_CFLAGS)
libtinynotify_la_LIBADD = $(DBUS_LIBS)
libtinynotify_la_SOURCES = lib/tinynotify.c lib/tinynotify.h

if ENABLE_CLI
lib_LTLIBRARIES += libtinynotify-cli.la
pkgconfig_DATA += libtinynotify-cli.pc
include_HEADERS += lib/tinynotify-cli.h

libtinynotify_cli_la_LDFLAGS = -version-info 0:0:0
libtinynotify_cli_la_LIBADD = libtinynotify.la
libtinynotify_cli_la_SOURCES = lib/tinynotify-cli.c lib/tinynotify-cli.h lib/tinynotify.h

noinst_PROGRAMS = test

test_CPPFLAGS = -I$(top_srcdir)/lib
test_LDADD = libtinynotify.la libtinynotify-cli.la
test_SOURCES = src/test.c lib/tinynotify.h
endif
