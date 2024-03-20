#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([TCMicomManager], [1.0.0], [jun@telechips.com])
AC_CONFIG_SRCDIR([src/tc-micom-manager.c])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP

AC_ARG_VAR([TCPM_CFLAGS], [bus interface])
AC_ARG_VAR([TCPM_LIBS], [bus interface])

# Checks PKG-CONFIG
PKG_CHECK_MODULES(PKGCONFIGINIT, pkgconfiginit, [],	[AC_MSG_RESULT([pkg-config initialized])])

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_join], , )

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MMAP
AC_CHECK_FUNCS([munmap])

AC_ARG_WITH([dbus],
	AS_HELP_STRING([--without-dbus], [Remove dbus interface]))

AS_IF([test "x$with_dbus" = "xno"], WITHOUT_DBUS=1, WITHOUT_DBUS=0)
AC_SUBST(HAVE_DBUS)
AM_CONDITIONAL([HAVE_DBUS], [test "x$WITHOUT_DBUS" = "x0"])
AS_IF([test "x$WITHOUT_DBUS" = "x0"], PKG_CHECK_MODULES([TCPM], [dbus-1 TcUtils]))

AC_ARG_ENABLE([sdm_demo],
		AS_HELP_STRING([--enable-sdm-demo], [Enable sdm demo]))
AS_IF([test "x$enable_sdm_demo" = "xyes"], ENABLE_SDM_DEMO=1, ENABLE_SDM_DEMO=0)
AC_SUBST(HAVE_SDM_DEMO)
AM_CONDITIONAL([HAVE_SDM_DEMO], [test "x$ENABLE_SDM_DEMO" = "x1"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
