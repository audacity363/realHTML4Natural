AIX_VERSION     =   6.1
OBJECT_MODE     =   64

CFG_VERSION     =   1.0.1
UTILITY_VERSION	=	1.0

# Allgemeine Entwicklungsverzeichnisse
DEVELOP         =   /develop_global
DEVSYS          =   $(DEVELOP)/sys_$(AIX_VERSION)/$(OBJECT_MODE)

STANDARD_PATH    	=   $(DEVSYS)/include
HEXD_PATH			= 	$(DEVSYS)/lib/HexDump_v1.1
UTILITY_PATH     	=   $(DEVSYS)/lib/Utility_v1.2
CFG_PATH        	 =  $(DEVSYS)/lib/Cfg_v1.0.1
TS_PATH           	 =  $(DEVSYS)/lib/TS_v1.0



INCDIR = -I. 	-I..\
				-I$(STANDARD_PATH)			\
				-I$(UTILITY_PATH)			\
				-I$(CFG_PATH)				\
				-I$(HEXD_PATH)              \
				-I$(TS_PATH)
                
				

CFLAGS	=	-g $(INCDIR)

#
main:		main.o	../natnetposts.o	\
			Makefile
		cc  main.o ../natnetposts.o    \
			$(TS_PATH)/libts.a $(CFG_PATH)/libcfg.a 	\
			-L$(HEXD_PATH) -l hexdump  											\
			-L$(UTILITY_PATH) -l utility			\
			-o main -bloadmap:main.bloadmap.txt
		-rm *.o			
		@echo "done"

.c.o:	
		@echo "Compiling $< to $@"
		$(CC) -c $(CFLAGS) $*.c
