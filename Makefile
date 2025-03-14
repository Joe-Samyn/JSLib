

# Compiler
GCC := gcc
AR := ar

# Compiler Flags
INCLUDES := -Iinclude
C_FLAGS := -Wall

DEBUG := -g 

# Directories
BUILD := build
TARGET := jlib.a
SRC := src
TESTS := tests

# Files
SRC_FILES := $(wildcard ${SRC}/*.c)
TEST_FILES := $(wildcard ${TESTS}/*.c)

OBJ_FILES := $(patsubst ${SRC}/%.c, ${BUILD}/${SRC}/%.o, ${SRC_FILES})
TEST_OBJ_FILES := $(patsubst ${TESTS}/%.c, ${BUILD}/${TESTS}/%.o, ${TEST_FILES})

debug: ${BUILD}/${TARGET}
	@echo "Building ${TARGET}"

# Build static library target
${BUILD}/${TARGET}: ${OBJ_FILES}
	echo "Building target..."
	${AR} rcs $@ $<

${BUILD}/${SRC}/%.o: ${SRC}/%.c
	@mkdir -p build
	@mkdir -p build/src
	${GCC} ${C_FLAGS} ${INCLUDES} ${DEBUG} -c $< -o $@

clean:
	@echo "Cleaning build directory.."
	@rm -rf build/src
	@rm -rf build/tests
	@rm -rf build/*
	@echo "Creating new build directory and subdirectories"
	@mkdir -p build/tests
	@mkdir -p build/src
