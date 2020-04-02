# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := pTherapy
G4TARGET := $(name)
G4EXLIB := true
G4WORKDIR :=.

# Includes for ROOT
CPPFLAGS += -I/sw/include/root

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

########################### ROOT #################################
# Copied from:
# GNUmakefile for IcaG4.  Jacek M. Holeczek Fri May 10 2002.
# Reference: http://root.cern.ch/root/HowtoMC.html

#ifdef ROOTSYS
  ifndef G4UI_USE_ROOT
    ROOTCPPFLAGS   = $(shell root-config --cflags)
    CPPFLAGS      += -DG4ANALYSIS_USE_ROOT $(ROOTCPPFLAGS)
    CPPFLAGS      +=-Wno-deprecated -g
    ROOTLIBS       = $(shell root-config --nonew --glibs)
    ROOTLIBS      := $(filter-out -lNew,$(ROOTLIBS))
    ROOTLIBS      += -lMinuit -lHtml
    LDLIBS        += $(ROOTLIBS)
  endif
#endif
