## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = snapall

snapall_SOURCES = \
	snap.c \
	utils.c utils.h\
	key.c key.h\
	eggtrayicon.c eggtrayicon.h\
	configfile.c configfile.h\
	prefswin.c prefswin.h

snapall_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

