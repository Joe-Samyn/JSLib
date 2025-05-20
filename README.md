# JSLib

A systems programming library focused on fundamental operations and cross-platform support. Built as a personal learning project to explore low-level programming concepts and system-specific implementations.

## Overview

JSLib provides basic functionality commonly needed in systems programming:

- [x] String manipulation (currently implemented)
- [ ] Memory management (in progress)
- [ ] Type conversions (planned)
- [ ] Standard I/O operations (planned)
- [ ] Assembly optimized implementations (planned)

The library aims to be:
- Educational: Clear implementations focusing on understanding systems concepts
- Portable: Supporting multiple platforms (currently macOS, with Windows, Linux, and Raspberry Pi planned)
- Practical: Usable in real projects like games and embedded systems

## Current Status

### Implemented Features
- String operations (see `include/jslib/string.h` for full API)

### In Progress
- Memory management system
- Platform-specific optimizations for macOS

### Planned Features
- Cross-platform support (Windows x86, Linux x86, Raspberry Pi)
- Assembly implementations for performance-critical operations
- Type conversion utilities
- Standard I/O operations

## Building

### Prerequisites
- Make build system
- GCC or compatible C compiler
- macOS (current requirement, more platforms coming soon)

### Build Commands
```bash
# Build the library
make

# Run tests
make test

# Clean build artifacts
make clean
```

## Platform Support

Current:
- ✅ macOS

Planned:
- Windows x86
- Linux x86
- Raspberry Pi

Note: This is a personal learning project and is not currently accepting contributions. After v1.0 release, the project may open for external contributions.

## Usage

Include the desired header:
```c
#include <jslib/string.h>
// or
#include <jslib/memory.h>
```

Link with the library:
```bash
gcc your_program.c -ljslib
```

## License

MIT License - See LICENSE file for details

## Project Goals

This project serves multiple purposes:
1. Learning and exploring systems programming concepts
2. Creating a practical utility library for personal projects
3. Understanding platform-specific optimizations
4. Exploring assembly language implementations

## Development Notes

- Currently focusing on macOS implementation
- Planning to maintain clear separation between platform-specific and generic code
- Emphasizing readability and educational value in implementations
- This is a personal learning project in active development
- External contributions will be welcomed after v1.0 release 
