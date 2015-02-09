AC_INIT(egglistmodel, 0.1)

AM_INIT_AUTOMAKE([])
AM_SILENT_RULES([yes])

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT

PKG_CHECK_MODULES(EGGLISTMODEL, gio-2.0 >= 2.43.4)
PKG_CHECK_MODULES(GTK, gtk+-3.0)
PKG_CHECK_MODULES(GIO_UNIX, gio-unix-2.0)

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    models/Makefile
    test/Makefile
])
AC_OUTPUT
