LIBNAME=libfbc
VERSION=1.0.0

# extra files (like header files)
TEMPLATEFILES = include/fbc_pid.h include/fbc.h
# basename of the source files that should be archived
TEMPLATEOBJS = fbc_pid fbc

TEMPLATE=$(ROOT)/$(LIBNAME)-template

.DEFAULT_GOAL: all

library: clean $(BINDIR) $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	$(MCUPREFIX)ar rvs $(BINDIR)/$(LIBNAME).a $(foreach f,$(TEMPLATEOBJS),$(BINDIR)/$(f).o)
	mkdir -p $(TEMPLATE) $(TEMPLATE)/firmware $(addprefix $(TEMPLATE)/, $(dir $(TEMPLATEFILES)))
	cp $(BINDIR)/$(LIBNAME).a $(TEMPLATE)/firmware/$(LIBNAME).a
	$(foreach f,$(TEMPLATEFILES),cp $(f) $(TEMPLATE)/$(f);)
	pros conduct create-template $(LIBNAME) $(VERSION) $(TEMPLATE) --ignore project.pros --upgrade-files "firmware/$(LIBNAME).a $(TEMPLATEFILES)"
	@echo Need to zip $(TEMPLATE)
