FLAGS += -D v050

SOURCES = $(wildcard src/*.cpp)

include ../../plugin.mk


dist: all
	mkdir -p dist/MS-modules
	cp LICENSE* dist/MS-modules/
	cp plugin.* dist/MS-modules/
	cp -R res dist/MS-modules/
