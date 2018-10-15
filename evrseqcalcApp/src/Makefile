TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = evrseqcalc
# evrseqcalc.dbd will be created and installed
DBD += evrseqcalc.dbd

# evrseqcalc.dbd will be made up from these files:
evrseqcalc_DBD += base.dbd
# Include dbd files from all support applications:
evrseqcalc_DBD += evrseq.dbd

# Add all the support libraries needed by this IOC
#evrseqcalc_LIBS += xxx

# evrseq_registerRecordDeviceDriver.cpp derives from evrseqcalc.dbd
evrseqcalc_SRCS += evrseqcalc_registerRecordDeviceDriver.cpp
evrseqcalc_SRCS += evrseq.c
# Build the main IOC entry point on workstation OSs.
evrseqcalc_SRCS_DEFAULT += evrseqMain.cpp
evrseqcalc_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#evrseqcalc_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
evrseqcalc_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

