cmake_minimum_required(VERSION 3.19)

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
  message(STATUS "Setting default vcpkg target triplet to $ENV{VCPKG_DEFAULT_TRIPLET}")
  set(VCPKG_TARGET_TRIPLET $ENV{VCPKG_DEFAULT_TRIPLET})
endif()

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(STATUS "VCPKG found: $ENV{VCPKG_ROOT}")
  message(STATUS "Using VCPKG integration")
  if(VCPKG_MANIFEST_FEATURES)
    message(STATUS "VCPKG_MANIFEST_FEATURES: ${VCPKG_MANIFEST_FEATURES}")
  endif()
elseif(DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Using toolchain: ${CMAKE_TOOLCHAIN_FILE}")
  if(CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg.cmake")
    message(STATUS "Toolchain uses VCPKG integration")
    if(VCPKG_MANIFEST_FEATURES)
      message(STATUS "VCPKG_MANIFEST_FEATURES: ${VCPKG_MANIFEST_FEATURES}")
    endif()
  endif()
else()
  message(WARNING "vcpkg not found, toolchain not defined, build might fail")
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json)
  file(READ ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json VCPKG_JSON_STRING)
  string(JSON VERSION_STRING GET ${VCPKG_JSON_STRING} version)
else()
  set(VERSION_STRING "0.0.0")
endif()

project(llama2c VERSION ${VERSION_STRING})

enable_language(C)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Install prefix" FORCE)
endif()

set(INSTALL_BIN_DIR      "bin"                   CACHE PATH "Path where exe and dll will be installed")
set(INSTALL_LIB_DIR      "lib"                   CACHE PATH "Path where lib will be installed")
set(INSTALL_INCLUDE_DIR  "include"               CACHE PATH "Path where headers will be installed ")
set(INSTALL_CMAKE_DIR    "share/${PROJECT_NAME}" CACHE PATH "Path where cmake configs will be installed")

if(WIN32)
  find_path(MMAN_DIR NAMES sys/mman.h PATH_SUFFIXES mman REQUIRED)
  find_library(MMAN_LIB NAMES mman REQUIRED)
endif()
find_library(MATH_LIBRARY m)
find_package(OpenMP COMPONENTS C)

set(CMAKE_PROJECT_NAME_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/run.c
)

if(WIN32)
  list(APPEND CMAKE_PROJECT_NAME_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/win.c
    ${CMAKE_CURRENT_LIST_DIR}/win.h
  )
endif()

add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME_SOURCES})
if(WIN32)
  target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MMAN_DIR})
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MMAN_LIB})
endif()
if(MATH_LIBRARY)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MATH_LIBRARY})
endif()
if(OpenMP_C_FOUND)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenMP::OpenMP_C)
endif()

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "${INSTALL_BIN_DIR}")

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)

install(
    PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
    DESTINATION ${INSTALL_BIN_DIR}
)
