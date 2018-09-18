#!../../bin/centos7-x86_64/asubtest

## You may have to change asubtest to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet("SYS","LabS-Utgard-VIP:")
epicsEnvSet("EVR","TS-EVR-1:")
epicsEnvSet("DEV1","Chop1:")
epicsEnvSet("DEV2","Chop2:")
epicsEnvSet("DEV3","Chop3:")
epicsEnvSet("DEV4","Chop4:")


cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/asubtest.dbd"
asubtest_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/modifyevrseq.db","SYS=$(SYS), EVR=$(EVR), DEV1=$(DEV1), DEV2=$(DEV2), DEV3=$(DEV3), DEV4=$(DEV4)")

cd "${TOP}/iocBoot/${IOC}"
iocInit

# Select trigger source for soft seq 0, trigger source 0, 1 means pulser 1
dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-TrigSrc-0-Sel 2

# Load sequencer setup
dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Load-Cmd 1

# Enable sequencer
dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Enable-Cmd 1

# event code 14 (14 Hz)
# 123 is the prescaler sync reset, 127 is the end of sequence, 
# option -a is array
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-EvtCode-SP 2 17 127 #6289464 ticks for 14 Hz speed 

# Defining time at which the event codes are sent
# event 127 should be set 3 ticks before maximum sequence length
# sequencer takes 12 ticks to start,should be compensated with 136ns?
#3144712 6289424 12578848 25157696
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Timestamp-SP 2 0 12578845

#caput -n LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Commit-Cmd 1



## Start any sequence programs
#seq sncxxx,"user=root"
