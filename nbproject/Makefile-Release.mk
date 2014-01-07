#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW_1-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/boost_wrapper.o \
	${OBJECTDIR}/color.o \
	${OBJECTDIR}/common.o \
	${OBJECTDIR}/fsysclass.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/snapshot_class.o \
	${OBJECTDIR}/t_extra.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lboost_filesystem -lboost_system

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filesystem-snapshot-creator.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filesystem-snapshot-creator.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filesystem-snapshot-creator ${OBJECTFILES} ${LDLIBSOPTIONS} -s

${OBJECTDIR}/boost_wrapper.o: boost_wrapper.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/boost_wrapper.o boost_wrapper.cpp

${OBJECTDIR}/color.o: color.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/color.o color.cpp

${OBJECTDIR}/common.o: common.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/common.o common.cpp

${OBJECTDIR}/fsysclass.o: fsysclass.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/fsysclass.o fsysclass.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/snapshot_class.o: snapshot_class.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/snapshot_class.o snapshot_class.cpp

${OBJECTDIR}/t_extra.o: t_extra.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/t_extra.o t_extra.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/filesystem-snapshot-creator.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
