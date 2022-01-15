## Process this file with automake to produce Makefile.in

images.h: 
	if test -e images.h; then \
		rm -rf images.h; \
	fi; \
	for file in `ls *.png`;do \
		gdk-pixbuf-csource $$file --raw --name="`echo $$file | sed -e s/.png//`" >> images.h; \
	done;

all: images.h

clean:
	rm -rf images.h
