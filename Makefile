# Makefile for delaunay triangulator

DT = $(shell echo `pwd`/delaunay)

.PHONY: default compile check clean

default: compile

compile:
	$(MAKE) -C src compile

check:
	echo 'make check has not been written yet'

clean:
	$(RM) *~ delaunay
	$(MAKE) -C src clean
