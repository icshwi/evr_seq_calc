TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = asubtest
# asubtest.dbd will be created and installed
DBD += asubtest.dbd

# asubtest.dbd will be made up from these files:
asubtest_DBD += base.dbd

# Include dbd files from all support applications:
#asubtest_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#asubtest_LIBS += xxx

# asubtest_registerRecordDeviceDriver.cpp derives from asubtest.dbd
asubtest_SRCS += asubtest_registerRecordDeviceDriver.cpp
asubtest_SRCS += evrseq.c
# Build the main IOC entry point on workstation OSs.
asubtest_SRCS_DEFAULT += asubtestMain.cpp
asubtest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#asubtest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
asubtest_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

