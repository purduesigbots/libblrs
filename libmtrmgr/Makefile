# Universal C Makefile for MCU targets

# Path to project root (for top-level, so the project is in ./; first-level, ../; etc.)
ROOT=.
# Binary output directory
BINDIR=$(ROOT)/bin
# Subdirectories to include in the build
SUBDIRS=src

# Nothing below here needs to be modified by typical users
LIBNAME=libmtrmgr
VERSION=1.0.0
DEPOT=blrslib

TEMPLATE=$(ROOT)/$(LIBNAME)-template

# Include common aspects of this project
-include $(ROOT)/common.mk

ASMSRC:=$(wildcard *.$(ASMEXT))
ASMOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(ASMSRC:.$(ASMEXT)=.o))
HEADERS:=$(wildcard *.$(HEXT))
CSRC=$(wildcard *.$(CEXT))
COBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CSRC:.$(CEXT)=.o))
CPPSRC:=$(wildcard *.$(CPPEXT))
CPPOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CPPSRC:.$(CPPEXT)=.o))
OUT:=$(BINDIR)/$(OUTNAME)

.PHONY: all clean upload _force_look

# By default, compile program
all: $(BINDIR) $(OUT)

# Remove all intermediate object files (remove the binary directory)
clean:
	-rm -f $(OUT)
	-rm -rf $(BINDIR)

library: clean $(BINDIR) $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	-rm -f $(BINDIR)/opcontrol.o $(BINDIR)/init.o $(BINDIR)/auto.o
	$(MCUPREFIX)ar rvs $(BINDIR)/$(LIBNAME).a $(BINDIR)/*.o
	-mkdir -p $(TEMPLATE) $(TEMPLATE)/include $(TEMPLATE)/firmware
	cp $(ROOT)/include/*.h $(TEMPLATE)/include
	cp $(BINDIR)/$(LIBNAME).a $(TEMPLATE)/firmware/$(LIBNAME).a
	-rm -f $(TEMPLATE)/include/API.h $(TEMPLATE)/include/main.h
	pros conduct create-template $(LIBNAME) $(VERSION) $(DEPOT) --location $(TEMPLATE) --ignore project.pros --upgrade-files "firmware/$(LIBNAME).a include/*.h"
	@echo Need to zip $(TEMPLATE)

# Uploads program to device
upload: all
	$(UPLOAD)

# Phony force-look target
_force_look:
	@true

# Looks in subdirectories for things to make
$(SUBDIRS): %: _force_look
	@$(MAKE) --no-print-directory -C $@

# Ensure binary directory exists
$(BINDIR):
	-@mkdir -p $(BINDIR)

# Compile program
$(OUT): $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	@echo LN $(BINDIR)/*.o $(LIBRARIES) to $@
	@$(CC) $(LDFLAGS) $(BINDIR)/*.o $(LIBRARIES) -o $@
	@$(MCUPREFIX)size $(SIZEFLAGS) $(OUT)
	$(MCUPREPARE)

# Assembly source file management
$(ASMOBJ): $(BINDIR)/%.o: %.$(ASMEXT) $(HEADERS)
	@echo AS $<
	@$(AS) $(AFLAGS) -o $@ $<

# Object management
$(COBJ): $(BINDIR)/%.o: %.$(CEXT) $(HEADERS)
	@echo CC $(INCLUDE) $<
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ $<

$(CPPOBJ): $(BINDIR)/%.o: %.$(CPPEXT) $(HEADERS)
	@echo CPC $(INCLUDE) $<
	@$(CPPCC) $(INCLUDE) $(CPPFLAGS) -o $@ $<
