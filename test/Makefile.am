
AM_CPPFLAGS = $(EGGLISTMODEL_CFLAGS) -I$(top_srcdir)/src
LDADD = $(top_srcdir)/src/libegglistmodel.a $(EGGLISTMODEL_LIBS)

check_PROGRAMS = \
	egghashstores

egghashstores_SOURCES = egghashstores.c

check-local: $(check_PROGRAMS)
	gtester $(check_PROGRAMS)
