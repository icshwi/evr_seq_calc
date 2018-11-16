## Load record instances
dbLoadRecords("modifyevrseq.db","SYS_EVRSEQ=$(SYS_EVRSEQ), EVR_EVRSEQ=$(EVR_EVRSEQ), DEV1=$(DEV1), DEV2=$(DEV2), DEV3=$(DEV3), DEV4=$(DEV4), NCG_SYS=$(NCG_SYS), NCG_DRV=$(NCG_DRV)")

# Alternative method of loading records, using unbuilt substitutions file
#
# Change into the development directory so that the substitution and
# template files can be found
#cd $(EVRSEQ_DB_TOP)
#dbLoadTemplate("modifyevrseq.substitutions","SYS_EVRSEQ=$(SYS_EVRSEQ), EVR_EVRSEQ=$(EVR_EVRSEQ), DEV1=$(DEV1), DEV2=$(DEV2), DEV3=$(DEV3), DEV4=$(DEV4), NCG_SYS=$(NCG_SYS), NCG_DRV=$(NCG_DRV)")
#cd $(E3_IOCSH_TOP)

# Example commands for configuring sequence
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-EvtCode-SP 2 17 127 #6289464 ticks for 14 Hz speed 
#3144712 6289424 12578848 25157696
#caput -a LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Timestamp-SP 2 0 12578845
#caput -n LabS-Utgard-VIP:TS-EVR-1:SoftSeq0-Commit-Cmd 1
