FLAGS += -D v050

SOURCES = $(wildcard src/*.cpp)

include ../../plugin.mk


dist: all
	mkdir -p dist/MSM
	cp LICENSE* dist/MSM/
	cp plugin.* dist/MSM/
	cp -R res dist/MSM/
	cp -R Help dist/MSM/