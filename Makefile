
# TODO: Need to clean up builds and variable
# TODO: List source files out, do not use wildcards b/c files may
# 	need to be in particular order
# TODO: Add normal build
# TODO: Get rid of all the variables, not needed

# Compiler
GCC := gcc
AR := ar

# Compiler Flags
INCLUDES := -Iinclude
C_FLAGS := -Wall

DEBUG := -g 

# Directories
BUILD := build
TARGET := libjslib.a
SRC := src
TEST_TARGET := test.exe
TESTS := tests

# Files
SRC_FILES := $(wildcard ${SRC}/*.c)
TEST_FILES := ${TESTS}/string_tests.c \
	      ${TESTS}/munit.c \
	      ${TESTS}/test_runner.c
	      

OBJ_FILES := $(patsubst ${SRC}/%.c, ${BUILD}/${SRC}/%.o, ${SRC_FILES})
TEST_OBJ_FILES := $(patsubst ${TESTS}/%.c, ${BUILD}/${TESTS}/%.o, ${TEST_FILES})

debug: ${BUILD}/${TARGET}
	@echo "Building ${TARGET}"

# Build static library target
${BUILD}/${TARGET}: ${OBJ_FILES}
	echo "Building target..."
	${AR} rcs $@ $^

${BUILD}/${SRC}/%.o: ${SRC}/%.c
	@mkdir -p build
	@mkdir -p build/src
	${GCC} ${C_FLAGS} ${INCLUDES} ${DEBUG} -c $< -o $@


test: ${BUILD}/${TEST_TARGET}
	@echo "Running tests.."
	./${BUILD}/${TEST_TARGET}

# Build Test executable from object files 
${BUILD}/${TEST_TARGET}: ${TEST_OBJ_FILES}
	@echo ${TEST_OBJ_FILES}
	${GCC} ${C_FLAGS} ${DEBUG} ${INCLUDES} -Lbuild $^ -ljslib -o $@

${BUILD}/${TESTS}/%.o: ${TESTS}/%.c
	${GCC} ${C_FLAGS} ${DEBUG} ${INCLUDES} -c $< -o $@



clean:
	@echo "Cleaning build directory.."
	@rm -rf build/src
	@rm -rf build/tests
	@rm -rf build/*
	@echo "Creating new build directory and subdirectories"
	@mkdir -p build/tests
	@mkdir -p build/src
