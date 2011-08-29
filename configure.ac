AC_PREREQ([2.60])
AC_INIT([libtinynotify-cli], [0.0.2])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 no-dependencies -Wall])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

AC_PROG_CC
AM_PROG_CC_C_O
AC_USE_SYSTEM_EXTENSIONS
LT_INIT([disable-static])
GTK_DOC_CHECK([1.15])

PKG_CHECK_MODULES([LIBTINYNOTIFY], [libtinynotify])
AC_CHECK_FUNCS([getopt],, [
	AC_MSG_ERROR([One of the required library functions was not found])
])
AC_CHECK_FUNCS([getopt_long])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile doc/Makefile libtinynotify-cli.pc])
AC_OUTPUT
