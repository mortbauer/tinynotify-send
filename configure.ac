AC_PREREQ([2.60])
AC_INIT([libtinynotify], [0.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 no-dependencies -Wall])

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
LT_INIT([disable-static])

PKG_CHECK_MODULES([DBUS], [dbus-1])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
