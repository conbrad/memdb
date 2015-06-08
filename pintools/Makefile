all: libdebug pintool cache

cache:
	cd analysis-tools && $(MAKE)

libdebug: 
	cd memtracker && $(MAKE) Makefile.libdebug

pintool:
	cd memtracker && $(MAKE) -f makefile
	


