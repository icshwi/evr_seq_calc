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
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-TrigSrc-0-Sel 2
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Load-Cmd 1
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Enable-Cmd 1
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-EvtCode-SP 2 17 127 #6289464 ticks for 14 Hz speed 
#3144712 6289424 12578848 25157696
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Timestamp-SP 2 0 12578845
#caput -n LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Commit-Cmd 1