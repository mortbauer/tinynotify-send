ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

SUBDIRS = doc

pkgconfigdir = $(libdir)/pkgconfig
subincludedir = $(includedir)/tinynotify/libnotify

noinst_PROGRAMS = test

lib_LTLIBRARIES = libtinynotify.la
pkgconfig_DATA = libtinynotify.pc
subinclude_HEADERS = lib/notify.h

libtinynotify_la_LDFLAGS = -version-info 0:0:0
libtinynotify_la_CPPFLAGS = $(DBUS_CFLAGS)
libtinynotify_la_LIBADD = $(DBUS_LIBS)
libtinynotify_la_SOURCES = \
	lib/notify.c lib/notify.h

test_CPPFLAGS = -I$(top_srcdir)/lib
test_LDADD = libtinynotify.la
test_SOURCES = src/test.c
