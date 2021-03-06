#!../../bin/centos7-x86_64/evrseqcalc

## You may have to change evrseqcalc to something else
## everywhere it appears in this file

< envPaths

epicsEnvSet("SYS","LabS-Utgard-VIP:")
epicsEnvSet("EVR","TS-EVR-1:")
epicsEnvSet("DEV1","Chop1:")
epicsEnvSet("DEV2","Chop2:")
epicsEnvSet("DEV3","Chop3:")
epicsEnvSet("DEV4","Chop4:")
epicsEnvSet("CHICSYS", "LabS-Utgard-VIP")
epicsEnvSet("CHICID", "Chop-CHIC-01")
epicsEnvSet("DEVICE", "$(EVR)")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/evrseqcalc.dbd"
evrseqcalc_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/modifyevrseq.db","SYS=$(SYS), EVR=$(EVR), DEV1=$(DEV1), DEV2=$(DEV2), DEV3=$(DEV3), DEV4=$(DEV4)")

## Load substitution file
#dbLoadTemplate("db/esschicTimestampBuffer.substitutions")
#dbLoadTemplate("db/modifyevrseq.substitutions")

cd "${TOP}/iocBoot/${IOC}"
iocInit
dbpf LabS-Utgard-VIP:Chop1:Freq-SP 28
dbpf LabS-Utgard-VIP:Chop2:Freq-SP 28
dbpf LabS-Utgard-VIP:Chop1:Tube-Pos-Delay 10
dbpf LabS-Utgard-VIP:Chop2:Tube-Pos-Delay 20
dbpf LabS-Utgard-VIP:TS-EVR-1:RF-Freq 88051936

# Select trigger source for soft seq 0, trigger source 0, 1 means pulser 1
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-TrigSrc-0-Sel 2
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Load-Cmd 1
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Enable-Cmd 1
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-EvtCode-SP 2 17 127 #6289464 ticks for 14 Hz speed 
#3144712 6289424 12578848 25157696
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Timestamp-SP 2 0 12578845
#caput -n LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Commit-Cmd 1
