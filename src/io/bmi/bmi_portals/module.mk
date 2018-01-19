#
# Makefile stub for bmi_portals.
#
# Copyright (C) 2007 Pete Wyckoff <pw@osc.edu>
#
# See COPYING in top-level directory.
#

# only do any of this if configure decided to use Portals
ifneq (,$(BUILD_PORTALS))

#
# Local definitions.
#
DIR := src/io/bmi/bmi_portals
cfiles := portals.c

#
# Export these to the top Makefile to tell it what to build.
#
src := $(patsubst %,$(DIR)/%,$(cfiles))
LIBSRC    += $(src)
SERVERSRC += $(src)
LIBBMISRC += $(src)

#
# Add extra include paths and warnings just for this directory.
# No shadow, as portals headers use "index".  No unused else lots
# of warnings for Cray portals header files.
#
cflags :=
ifdef GNUC
cflags += -W -Wcast-qual -Wwrite-strings -Wno-unused
endif

#
# Extra cflags for files in this directory.
#
MODCFLAGS_$(DIR) := $(cflags)

#
# Portals headers.
#
ifneq (,)
MODCFLAGS_$(DIR)/portals.c := 
endif

endif  # BUILD_PORTALS
