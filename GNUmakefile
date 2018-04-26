# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := nTPC_CASA_DAQ_proton_fast
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = /home/fuyue/Geant4/geant4.10.02-install/share/Geant4-10.2.0/geant4make
endif


.PHONY: all
all: lib bin

CPPFLAGS += `root-config --cflags`
EXTRALIBS += `root-config --nonew --libs`

include $(G4INSTALL)/config/binmake.gmk
