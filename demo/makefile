
#!/bin/ksh

DCI_INCLUDE=-I${DSCI_PATH}/include
DCI_LIB=-L${DSCI_PATH}/lib/ -ldsci

PUB_LIB=-L${IBF_PATH}/bg_public/lib -lbg_public
PUB_INCLUDE=-I${IBF_PATH}/bg_public/bg_public

XML_INCLUDE=-I${IBF_PATH}/ibear_all/xml2/include
XML_LIB=-L${IBF_PATH}/ibear_all/xml2/lib -lxml2

IBEAR_INCLUDE=-I${IBEAR_PATH}/ibear/include
IBEAR_LIB=-L${IBF_PATH}/ibear_all/ibear/lib -libear

IFILE_INCLUDE=-I${IBF_PATH}/ifile_all/ifile/include
IFILE_LIB=-L${IBF_PATH}/ifile_all/ifile/lib -lifile

ICFG_INCLUDE=-I${IBF_PATH}/icfg_all/icfg/include
ICFG_LIB=-L${IBF_PATH}/icfg_all/icfg/lib -licfg

ILOG_INCLUDE=-I${IBF_PATH}/ilog_all/ilog/include
ILOG_LIB=-L${IBF_PATH}/ilog_all/ilog/lib -lilog

SDFS_INCLUDE=-I${SDFS_HOME}/include -I${DSC_PATH}/include
SDFS_LIB=-L${SDFS_HOME}/lib -lsdfs -L${DSC_PATH}/lib -ldsc

INCLUDE=-I. ${DCI_INCLUDE} ${PUB_INCLUDE} ${XML_INCLUDE} ${IBEAR_INCLUDE} ${IFILE_INCLUDE} ${ICFG_INCLUDE} ${ILOG_INCLUDE} ${SDFS_INCLUDE}
LIB=-L. ${PUB_LIB} ${IBEAR_LIB} ${DCI_LIB} ${XML_LIB} ${ICFG_LIB} ${ILOG_LIB} ${IFILE_LIB} ${SDFS_LIB}

IFLAGS=-DSS_64BIT_SERVER -D_REENTRANT -D_DSCI_ -D_DEBUG_INFO_

LIBORA=$(ORACLE_HOME)/lib -lpthread -lclntsh

.SUFFIXES:.cpp .o

CFLAGS = ${IFLAGS} ${INCLUDE}

CC=g++ -m64 -g  -lpthread  -ldl -O0 -fpermissive

OBJ = Check_BDSData.o Check_BDSMgr.o Check_BDS.o

BIN=CHKBDS

all:${BIN} install clean

.cpp.o:
	echo "Compiling the $<"
	$(CC) -c ${CFLAGS} $<
.c.o:
	echo "Compiling the $<"
	$(CC) -c ${CFLAGS} $<

clean:
	rm -f Check_BDSData.o Check_BDSMgr.o Check_BDS.o

######################################################################

${BIN}: $(OBJ)
	$(CC) ${CFLAGS} -o $@ $? ${LIB}

######################################################################

install:



