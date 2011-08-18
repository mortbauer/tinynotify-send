AC_PREREQ([2.60])
AC_INIT([libtinynotify], [0.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 no-dependencies -Wall])

AC_PROG_CC
AM_PROG_CC_C_O
AC_USE_SYSTEM_EXTENSIONS
LT_INIT([disable-static])
GTK_DOC_CHECK([1.15])

AC_CHECK_FUNCS([strdup asprintf vasprintf],, [
	AC_MSG_ERROR([One of the required library functions was not found])
])
PKG_CHECK_MODULES([DBUS], [dbus-1])

AC_ARG_ENABLE([cli],
	[AS_HELP_STRING([--disable-cli],
		[Disable CLI support])])
AS_IF([test x"$enable_cli" != x"no"], [
	AC_CHECK_FUNC([getopt],, [
		AC_MSG_WARN([getopt() not found, disabling CLI])
		enable_cli=no
	])
])
AM_CONDITIONAL([ENABLE_CLI], [test x"$enable_cli" != x"no"])

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--disable-debug],
		[Disable debugging asserts])])
AS_IF([test x"$enable_debug" = x"no"], [
	AC_DEFINE([NDEBUG], [1], [Set this to disable debugging asserts])
])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile doc/Makefile libtinynotify.pc libtinynotify-cli.pc])
AC_OUTPUT
