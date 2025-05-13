
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
LIBS := -ljslib
BUILD ?= debug

# Directories
BUILD_DIR := build
SRC_DIR := src
TESTS_DIR := tests

# Targets
TARGET := ${BUILD_DIR}/${BUILD}/libjslib.a
TEST_TARGET := ${BUILD_DIR}/test.exe

# Files
SRC_FILES := ${SRC_DIR}/string.c \
			${SRC_DIR}/memory_macos.c

TEST_FILES := ${TESTS_DIR}/string_tests.c \
		  ${TESTS_DIR}/memory_tests.c \
	      ${TESTS_DIR}/munit.c \
	      ${TESTS_DIR}/test_runner.c

# Determin flags for compiler based on above variables
ifeq (${BUILD}, debug)
	C_FLAGS := -g -O0 -Wall -L${BUILD_DIR}/${BUILD}
else ifeq (${BUILD}, release)
	C_FLAGS := -O2 -Wall -L${BUILD_DIR}/${BUILD}
endif
	      

OBJ_FILES := $(patsubst ${SRC_DIR}/%.c, ${BUILD_DIR}/${SRC_DIR}/%.o, ${SRC_FILES})
TEST_OBJ_FILES := $(patsubst ${TESTS_DIR}/%.c, ${BUILD_DIR}/${TESTS_DIR}/%.o, ${TEST_FILES})

# Build library  
all: ${TARGET}

${TARGET}: ${OBJ_FILES}
	@echo "Building JSLib static libary.."
	@mkdir -p ${BUILD_DIR}/${BUILD}
	${AR} rcs $@ $^

${BUILD_DIR}/${SRC_DIR}/%.o: ${SRC_DIR}/%.c
	@echo "Compiling source files.."
	@mkdir -p ${BUILD_DIR}/${BUILD}
	@mkdir -p ${BUILD_DIR}/${SRC_DIR}
	${AR} rcs $@ $^
	${GCC} ${C_FLAGS} ${INCLUDES} -c $< -o $@


# Build and run unit tests
test: ${TEST_TARGET}

${TEST_TARGET}: ${TEST_OBJ_FILES}
	@echo "Building unit tests executable.."
	${GCC} ${C_FLAGS} ${INCLUDES} $^ ${LIBS} -o $@
	
	@echo "Running tests.."
	${MAKE} tests_run

${BUILD_DIR}/${TESTS_DIR}/%.o: ${TESTS_DIR}/%.c
	@echo "Compiling unit tests.."
	@mkdir -p ${BUILD_DIR}/${TESTS_DIR} 
	${GCC} ${C_FLAGS} ${INCLUDES} -c $< -o $@

# Run tests
tests_run: 
	./${TEST_TARGET}
	

clean:
	@echo "Cleaning build directory.."
	@rm -rf build/src
	@rm -rf build/tests
	@rm -rf build/*

