AC_INIT(egglistmodel, 0.1)

AM_INIT_AUTOMAKE([])
AM_SILENT_RULES([yes])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

PKG_CHECK_MODULES(EGGLISTMODEL, gio-2.0 >= 2.41)

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    test/Makefile
])
AC_OUTPUT
