cmake_minimum_required (VERSION 3.7)

set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION}")
set(CMAKE_BUILD_TYPE "${CONFIGURATION}")

set(CMAKE_C_COMPILER_WORKS 1)
set(BUILD_UEFI true)

add_definitions(-DUEFI)

include(ProcessorCount)
ProcessorCount(pc)
math(EXPR pc2 "${pc}*2")

set(CMAKE_C_FLAGS "/EHa /GF /Gy /GS- /Zi /utf-8 /wd4100 /wd4197 /wd4206 /MP${pc2}")
set(CMAKE_C_FLAGS_DEBUG "/Od /Ob0 /Oi /GR -D_DEBUG")
set(CMAKE_C_FLAGS_RELEASE "/Ox /Ot /Ob2 /Oi /Oy -DNDEBUG")

set(CMAKE_STATIC_LINKER_FLAGS "/ignore:4221")

set(CMAKE_EXE_LINKER_FLAGS "/OPT:ICF /OPT:REF /INCREMENTAL:NO /DEBUG /MAP /MERGE:.rdata=.text /NODEFAULTLIB /SUBSYSTEM:EFI_APPLICATION")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/RELEASE /LTCG")

set(CMAKE_SHARED_LINKER_FLAGS "/OPT:ICF /OPT:REF /INCREMENTAL:NO /DEBUG /MAP /MERGE:.rdata=.text /NODEFAULTLIB /SUBSYSTEM:EFI_APPLICATION")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/RELEASE /LTCG")

if("${PLATFORM}" STREQUAL "Win32")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
endif()

set(IL2C_LIBRARY_NAME_BASE "il2c-msvc-uefi-${PLATFORM}")
set(IL2C_LIBRARY_NAME "lib${IL2C_LIBRARY_NAME_BASE}.lib")

set(TARGET_LIBRARY_NAME "lib${IL2C_LIBRARY_NAME_BASE}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/../include)
link_directories(${CMAKE_CURRENT_LIST_DIR}/../lib/${CONFIGURATION}/)
