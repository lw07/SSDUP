#
# Makefile stub for bmi_gm.
#
# Copyright (C) 2008 Pete Wyckoff <pw@osc.edu>
#
# See COPYING in top-level directory.
#

# only do any of this if configure decided to use GM
ifneq (,$(BUILD_GM))

#
# Local definitions.
#
DIR := src/io/bmi/bmi_gm
cfiles := bmi-gm-addr-list.c bmi-gm-bufferpool.c bmi-gm.c

#
# Export these to the top Makefile to tell it what to build.
#
src := $(patsubst %,$(DIR)/%,$(cfiles))
LIBSRC    += $(src)
SERVERSRC += $(src)
LIBBMISRC += $(src)

#
# Extra cflags for files in this directory.
# TODO: later on we will want the ability to modify this at configure time
#
MODCFLAGS_$(DIR) := -I -I/gm -DENABLE_GM_BUFPOOL

endif  # BUILD_GM
