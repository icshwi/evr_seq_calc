epicsEnvSet("SYS_EVRSEQ","LabS-Utgard-VIP:")
epicsEnvSet("EVR_EVRSEQ","TS-EVR-1:")
epicsEnvSet("DEV1","Chop-Drv-01:")
epicsEnvSet("DEV2","Chop-Drv-02:")
epicsEnvSet("DEV3","Chop-Drv-03:")
epicsEnvSet("DEV4","Chop-Drv-04:")
epicsEnvSet("CHICSYS_EVRSEQ", "LabS-Utgard-VIP")
epicsEnvSet("CHICID_EVRSEQ", "Chop-CHIC-01")
epicsEnvSet("DEVICE_EVR_SEQ", "$(EVR)")

## Load record instances
dbLoadRecords("modifyevrseq.db","SYS_EVRSEQ=$(SYS_EVRSEQ), EVR_EVRSEQ=$(EVR_EVRSEQ), DEV1=$(DEV1), DEV2=$(DEV2), DEV3=$(DEV3), DEV4=$(DEV4)")

# Alternative method of loading records, using unbuilt substitutions file
#
# Change into the development directory so that the substitution and
# template files can be found
#cd $(EVRSEQ_DB_TOP)
#dbLoadTemplate("modifyevrseq.substitutions","SYS_EVRSEQ=$(SYS_EVRSEQ), EVR_EVRSEQ=$(EVR_EVRSEQ), DEV1=$(DEV1), DEV2=$(DEV2), DEV3=$(DEV3), DEV4=$(DEV4)")
#cd $(E3_IOCSH_TOP)

afterInit dbpf, LabS-Utgard-VIP:Chop1:Freq-SP, 28
afterInit dbpf, LabS-Utgard-VIP:Chop2:Freq-SP, 28
afterInit dbpf, LabS-Utgard-VIP:Chop1:Tube-Pos-Delay, 10
afterInit dbpf, LabS-Utgard-VIP:Chop2:Tube-Pos-Delay, 20
afterInit dbpf, LabS-Utgard-VIP:TS-EVR-1:RF-Freq, 88051936

# Select trigger source for soft seq 0, trigger source 0, 1 means pulser 1
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-TrigSrc-0-Sel 2
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Load-Cmd 1
#dbpf LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Enable-Cmd 1
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-EvtCode-SP 2 17 127 #6289464 ticks for 14 Hz speed 
#3144712 6289424 12578848 25157696
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Timestamp-SP 2 0 12578845
#caput -n LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Commit-Cmd 1
