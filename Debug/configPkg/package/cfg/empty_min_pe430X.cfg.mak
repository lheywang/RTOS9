# invoke SourceDir generated makefile for empty_min.pe430X
empty_min.pe430X: .libraries,empty_min.pe430X
.libraries,empty_min.pe430X: package/cfg/empty_min_pe430X.xdl
	$(MAKE) -f C:\Users\l.heywang\Desktop\work\TP_RTOS2/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\l.heywang\Desktop\work\TP_RTOS2/src/makefile.libs clean

