#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/CuC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/CuC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c control_comm.c terminal_comm.c digital_input.c digital_output.c relay_output.c analog_output.c process_control.c i2c.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/control_comm.o ${OBJECTDIR}/terminal_comm.o ${OBJECTDIR}/digital_input.o ${OBJECTDIR}/digital_output.o ${OBJECTDIR}/relay_output.o ${OBJECTDIR}/analog_output.o ${OBJECTDIR}/process_control.o ${OBJECTDIR}/i2c.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/control_comm.o.d ${OBJECTDIR}/terminal_comm.o.d ${OBJECTDIR}/digital_input.o.d ${OBJECTDIR}/digital_output.o.d ${OBJECTDIR}/relay_output.o.d ${OBJECTDIR}/analog_output.o.d ${OBJECTDIR}/process_control.o.d ${OBJECTDIR}/i2c.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/control_comm.o ${OBJECTDIR}/terminal_comm.o ${OBJECTDIR}/digital_input.o ${OBJECTDIR}/digital_output.o ${OBJECTDIR}/relay_output.o ${OBJECTDIR}/analog_output.o ${OBJECTDIR}/process_control.o ${OBJECTDIR}/i2c.o

# Source Files
SOURCEFILES=main.c control_comm.c terminal_comm.c digital_input.c digital_output.c relay_output.c analog_output.c process_control.c i2c.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/CuC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F66J11
MP_PROCESSOR_OPTION_LD=18f66j11
MP_LINKER_DEBUG_OPTION=  -u_DEBUGSTACK
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/control_comm.o: control_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/control_comm.o.d 
	@${RM} ${OBJECTDIR}/control_comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/control_comm.o   control_comm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/control_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/control_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/terminal_comm.o: terminal_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_comm.o.d 
	@${RM} ${OBJECTDIR}/terminal_comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/terminal_comm.o   terminal_comm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/terminal_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/digital_input.o: digital_input.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/digital_input.o.d 
	@${RM} ${OBJECTDIR}/digital_input.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/digital_input.o   digital_input.c 
	@${DEP_GEN} -d ${OBJECTDIR}/digital_input.o 
	@${FIXDEPS} "${OBJECTDIR}/digital_input.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/digital_output.o: digital_output.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/digital_output.o.d 
	@${RM} ${OBJECTDIR}/digital_output.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/digital_output.o   digital_output.c 
	@${DEP_GEN} -d ${OBJECTDIR}/digital_output.o 
	@${FIXDEPS} "${OBJECTDIR}/digital_output.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/relay_output.o: relay_output.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/relay_output.o.d 
	@${RM} ${OBJECTDIR}/relay_output.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/relay_output.o   relay_output.c 
	@${DEP_GEN} -d ${OBJECTDIR}/relay_output.o 
	@${FIXDEPS} "${OBJECTDIR}/relay_output.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/analog_output.o: analog_output.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analog_output.o.d 
	@${RM} ${OBJECTDIR}/analog_output.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/analog_output.o   analog_output.c 
	@${DEP_GEN} -d ${OBJECTDIR}/analog_output.o 
	@${FIXDEPS} "${OBJECTDIR}/analog_output.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/process_control.o: process_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/process_control.o.d 
	@${RM} ${OBJECTDIR}/process_control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/process_control.o   process_control.c 
	@${DEP_GEN} -d ${OBJECTDIR}/process_control.o 
	@${FIXDEPS} "${OBJECTDIR}/process_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/i2c.o   i2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/control_comm.o: control_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/control_comm.o.d 
	@${RM} ${OBJECTDIR}/control_comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/control_comm.o   control_comm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/control_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/control_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/terminal_comm.o: terminal_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/terminal_comm.o.d 
	@${RM} ${OBJECTDIR}/terminal_comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/terminal_comm.o   terminal_comm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/terminal_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/terminal_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/digital_input.o: digital_input.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/digital_input.o.d 
	@${RM} ${OBJECTDIR}/digital_input.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/digital_input.o   digital_input.c 
	@${DEP_GEN} -d ${OBJECTDIR}/digital_input.o 
	@${FIXDEPS} "${OBJECTDIR}/digital_input.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/digital_output.o: digital_output.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/digital_output.o.d 
	@${RM} ${OBJECTDIR}/digital_output.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/digital_output.o   digital_output.c 
	@${DEP_GEN} -d ${OBJECTDIR}/digital_output.o 
	@${FIXDEPS} "${OBJECTDIR}/digital_output.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/relay_output.o: relay_output.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/relay_output.o.d 
	@${RM} ${OBJECTDIR}/relay_output.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/relay_output.o   relay_output.c 
	@${DEP_GEN} -d ${OBJECTDIR}/relay_output.o 
	@${FIXDEPS} "${OBJECTDIR}/relay_output.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/analog_output.o: analog_output.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analog_output.o.d 
	@${RM} ${OBJECTDIR}/analog_output.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/analog_output.o   analog_output.c 
	@${DEP_GEN} -d ${OBJECTDIR}/analog_output.o 
	@${FIXDEPS} "${OBJECTDIR}/analog_output.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/process_control.o: process_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/process_control.o.d 
	@${RM} ${OBJECTDIR}/process_control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/process_control.o   process_control.c 
	@${DEP_GEN} -d ${OBJECTDIR}/process_control.o 
	@${FIXDEPS} "${OBJECTDIR}/process_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	@${RM} ${OBJECTDIR}/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa- -o-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/i2c.o   i2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/CuC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/CuC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/CuC.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/CuC.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
