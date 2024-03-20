AM_CFLAGS = \
			$(TCPM_CFLAGS) \
			-I$(top_srcdir)/inc
AM_CPPLAFGS = $(AM_CFLAGS)
LIBS = @LIBS@ $(TCPM_LIBS) -lTcIPCCtrl
DEFS = 
if HAVE_DBUS
DEFS += -DHAVE_DBUS
endif

if HAVE_SDM_DEMO
DEFS += -DHAVE_SDM_DEMO
endif
CC=@CC@ -Wall

##########################################
#			Main Program				 #
##########################################
bin_PROGRAMS = TCMicomManager

TCMicomManager_SOURCES = src/main.c \
						src/tc-micom-manager.c \
						src/virtual-key/tc-key-map.c \
						src/virtual-key/tc-virtual-key.c \
						src/app-demo/tc-app-demo.c
if HAVE_DBUS
TCMicomManager_SOURCES += src/dbus-interface/tc-micom-dbusmanager.c \
						src/dbus-interface/DBusMsgDefNames.c
endif

if HAVE_SDM_DEMO
TCMicomManager_SOURCES += src/app-demo/tc-sdm-demo.c
endif

comManager_CFLAGS = $(AM_CFLAGS)


