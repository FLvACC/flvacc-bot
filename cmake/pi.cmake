# See: https://github.com/abhiTronix/raspberry-pi-cross-compilers/wiki/Cross-Compiler-CMake-Usage-Guide-with-rsynced-Raspberry-Pi-64-bit-OS
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(tools $ENV{HOME}/tools/cross-pi-gcc-14.2.0-64)
set(rootfs_dir $ENV{HOME}/rootfs)

set(CMAKE_FIND_ROOT_PATH ${rootfs_dir})
set(CMAKE_SYSROOT ${rootfs_dir})

set(CMAKE_LIBRARY_ARCHITECTURE aarch64-linux-gnu)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE} -L${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -static-libstdc++ -static-libgcc")

## Compiler Binary
SET(BIN_PREFIX ${tools}/bin/aarch64-linux-gnu)

SET (CMAKE_C_COMPILER ${BIN_PREFIX}-gcc)
SET (CMAKE_CXX_COMPILER ${BIN_PREFIX}-g++)
SET (CMAKE_LINKER ${BIN_PREFIX}-ld
            CACHE STRING "Set the cross-compiler tool LD" FORCE)
SET (CMAKE_AR ${BIN_PREFIX}-ar
            CACHE STRING "Set the cross-compiler tool AR" FORCE)
SET (CMAKE_NM ${BIN_PREFIX}-nm
            CACHE STRING "Set the cross-compiler tool NM" FORCE)
SET (CMAKE_OBJCOPY ${BIN_PREFIX}-objcopy
            CACHE STRING "Set the cross-compiler tool OBJCOPY" FORCE)
SET (CMAKE_OBJDUMP ${BIN_PREFIX}-objdump
            CACHE STRING "Set the cross-compiler tool OBJDUMP" FORCE)
SET (CMAKE_RANLIB ${BIN_PREFIX}-ranlib
            CACHE STRING "Set the cross-compiler tool RANLIB" FORCE)
SET (CMAKE_STRIP ${BIN_PREFIX}-strip
    CACHE STRING "Set the cross-compiler tool STRIP" FORCE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(VCPKG_TARGET_TRIPLET arm64-linux)

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
