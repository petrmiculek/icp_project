PACKNAME=xlavin00_xmicul08
MAKEFILENAME=QMakefile


.PHONY: all clean cleandoc cleanpack doxygen pack

all: $(MAKEFILENAME) doxygen
	$(MAKE) -f $(MAKEFILENAME) all

run: all
	$(MAKE) -f $(MAKEFILENAME) run

$(MAKEFILENAME): icp.pro
	qmake

doxygen:
	doxygen

cleandoc:
	rm -rf doc/*

cleanpack:
	rm -f $(PACKNAME).zip

clean: cleandoc cleanpack $(MAKEFILENAME)
	$(MAKE) -f $(MAKEFILENAME) distclean

pack:
	mkdir -p doc
	zip $(PACKNAME).zip doc/
	zip -r $(PACKNAME).zip Doxyfile Makefile README.txt *.{qrc,pro} examples/* src/*

#%: $(MAKEFILENAME)
#	$(MAKE) -f $(MAKEFILENAME) $@
