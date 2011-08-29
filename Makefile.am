ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

SUBDIRS = doc

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libtinynotify-cli.la
pkgconfig_DATA = libtinynotify-cli.pc
include_HEADERS = lib/tinynotify-cli.h

libtinynotify_cli_la_LDFLAGS = -version-info 1:0:0
libtinynotify_cli_la_CPPFLAGS = $(LIBTINYNOTIFY_CFLAGS)
libtinynotify_cli_la_LIBADD = $(LIBTINYNOTIFY_LIBS)
libtinynotify_cli_la_SOURCES = lib/tinynotify-cli.c lib/tinynotify-cli.h

bin_PROGRAMS = tinynotify-send

tinynotify_send_CPPFLAGS = -I$(top_srcdir)/lib $(LIBTINYNOTIFY_CFLAGS)
tinynotify_send_LDADD = $(LIBTINYNOTIFY_LIBS) libtinynotify-cli.la
tinynotify_send_SOURCES = src/tinynotify-send.c lib/tinynotify-cli.h
