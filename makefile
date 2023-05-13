CC=g++
CFLAGS=-std=c++11 -Wall
TARGET= ./tp1.out

BUILD_DIR    = ./obj
SRC_DIR    = ./src
INCLUDE_DIR  = ./include


${BUILD_DIR}/${TARGET}: ${BUILD_DIR}/InputConfig.o ${BUILD_DIR}/Node.o ${BUILD_DIR}/NodeOperator.o ${BUILD_DIR}/NodeTerminal.o ${BUILD_DIR}/Tree.o ${BUILD_DIR}/main.o 
	${CC} ${CFLAGS} -o ./${TARGET} ${BUILD_DIR}/*.o


${BUILD_DIR}/InputConfig.o: ${INCLUDE_DIR}/InputConfig.hpp ${SRC_DIR}/InputConfig.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/InputConfig.cpp -o ${BUILD_DIR}/InputConfig.o

${BUILD_DIR}/Node.o:  ${INCLUDE_DIR}/Node.hpp ${INCLUDE_DIR}/InputConfig.hpp ${SRC_DIR}/Node.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Node.cpp -o ${BUILD_DIR}/Node.o

${BUILD_DIR}/NodeOperator.o: ${INCLUDE_DIR}/NodeOperator.hpp  ${INCLUDE_DIR}/InputConfig.hpp  ${INCLUDE_DIR}/Node.hpp ${SRC_DIR}/NodeOperator.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/NodeOperator.cpp -o ${BUILD_DIR}/NodeOperator.o

${BUILD_DIR}/NodeTerminal.o: ${INCLUDE_DIR}/NodeTerminal.hpp ${INCLUDE_DIR}/Node.hpp  ${SRC_DIR}/NodeTerminal.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/NodeTerminal.cpp -o ${BUILD_DIR}/NodeTerminal.o

${BUILD_DIR}/Tree.o: ${INCLUDE_DIR}/Tree.hpp   ${INCLUDE_DIR}/Node.hpp  ${SRC_DIR}/Tree.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/Tree.cpp -o ${BUILD_DIR}/Tree.o

${BUILD_DIR}/main.o: ${INCLUDE_DIR}/Tree.hpp ${INCLUDE_DIR}/Node.hpp ${INCLUDE_DIR}/NodeOperator.hpp  ${INCLUDE_DIR}/NodeTerminal.hpp  ${INCLUDE_DIR}/InputConfig.hpp  ${INCLUDE_DIR}/GenInfo.hpp ${SRC_DIR}/main.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/main.cpp -o ${BUILD_DIR}/main.o


clean:
	rm -f ${BUILD_DIR}/* 