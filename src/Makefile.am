
noinst_LIBRARIES = libegglistmodel.a

libegglistmodel_a_SOURCES = \
	gio-patch.h \
	glistmodel.h \
	glistmodel.c \
	gliststore.h \
	gliststore.c \
	egglistmodel.h \
	eggsortedhash.c \
	eggsortedhash.h \
	eggsetliststore.c \
	eggsetliststore.h \
	eggkeyedliststore.c \
	eggkeyedliststore.h

libegglistmodel_a_CFLAGS = \
	-Wall \
	-include gio-patch.h \
	$(EGGLISTMODEL_CFLAGS)
