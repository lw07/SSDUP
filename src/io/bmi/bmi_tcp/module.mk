
ifneq (,$(BUILD_BMI_TCP))

BUILD_EPOLL = 1
DIR := src/io/bmi/bmi_tcp
LIBSRC += \
	$(DIR)/bmi-tcp.c \
	$(DIR)/sockio.c

SERVERSRC += \
	$(DIR)/bmi-tcp.c \
	$(DIR)/sockio.c

LIBBMISRC += \
	$(DIR)/bmi-tcp.c \
	$(DIR)/sockio.c

ifdef BUILD_EPOLL
LIBSRC += $(DIR)/socket-collection-epoll.c
SERVERSRC += $(DIR)/socket-collection-epoll.c
LIBBMISRC += $(DIR)/socket-collection-epoll.c
MODCFLAGS_$(DIR)/bmi-tcp.c := -D__PVFS2_USE_EPOLL__
else
LIBSRC += $(DIR)/socket-collection.c
SERVERSRC += $(DIR)/socket-collection.c
LIBBMISRC += $(DIR)/socket-collection.c
endif

endif  # BUILD_BMI_TCP
