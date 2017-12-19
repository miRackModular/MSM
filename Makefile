SLUG = MSM
VERSION = 0.5.4

FLAGS += -D v050

SOURCES = $(wildcard src/*.cpp)

include ../../plugin.mk


dist: all
	mkdir -p dist/$(SLUG)
	cp LICENSE* dist/$(SLUG)/
	cp plugin.* dist/$(SLUG)/
	cp -R res dist/$(SLUG)/
	cp -R Help dist/$(SLUG)/
