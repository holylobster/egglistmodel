
bin_PROGRAMS = demo

demo_SOURCES = \
	demo.c \
	egg-app-info-model.h \
	egg-app-info-model.c

demo_CFLAGS = \
	-Wall \
	$(GIO_UNIX_CFLAGS) \
	$(GTK_CFLAGS) \
	-I$(top_srcdir)/src

demo_LDADD = \
	$(top_builddir)/src/libegglistmodel.la \
	$(GIO_UNIX_LIBS) \
	$(GTK_LIBS)
