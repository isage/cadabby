cmake_minimum_required(VERSION 3.20)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(PLATFORM "pc" CACHE STRING "Platform to build for")
set_property(CACHE PLATFORM PROPERTY STRINGS pc vita)
message(${PLATFORM})
#set(CMAKE_FIND_PACKAGE_PREFER_CONFIG OFF)
IF(PLATFORM STREQUAL "pc")
ELSEIF(PLATFORM STREQUAL "vita")
    IF(DEFINED ENV{VITASDK})
        include("$ENV{VITASDK}/share/vita.toolchain.cmake" REQUIRED)
        include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
        set(VITA_APP_NAME "Battery discharger")
        set(VITA_TITLEID  "DSCH00001")
        set(VITA_VERSION  "01.00")
        set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
    ELSE()
        message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
    ENDIF()
ELSE()
    message(FATAL_ERROR "Wrong platform")
ENDIF()

project(discharger CXX)

set (discharger_VERSION_MAJOR 1)
set (discharger_VERSION_MINOR 1)
set (discharger_VERSION_RELEASE 0)

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_MIXER_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${PNG_PNG_INCLUDE_DIR})
include_directories("${discharger_SOURCE_DIR}/deps")

file(GLOB APP_SOURCES "src/[a-zA-Z]*.cpp")
file(GLOB CM_SOURCES "src/common/[a-zA-Z]*.cpp")
file(GLOB UT_SOURCES "src/Utils/[a-zA-Z]*.cpp")
file(GLOB INI_SOURCES "src/Utils/Ini/[a-zA-Z]*.cpp")
file(GLOB EV_SOURCES "src/Event/[a-zA-Z]*.cpp")
file(GLOB GM_SOURCES "src/Game/[a-zA-Z]*.cpp")
file(GLOB ST_SOURCES "src/State/[a-zA-Z]*.cpp")
file(GLOB GR_SOURCES "src/Graphics/[a-zA-Z]*.cpp")
file(GLOB SN_SOURCES "src/Sound/[a-zA-Z]*.cpp")

include_directories(${discharger_SOURCE_DIR})

set(SOURCES
    ${APP_SOURCES}
    ${CM_SOURCES}
    ${UT_SOURCES}
    ${INI_SOURCES}
    ${EV_SOURCES}
    ${GM_SOURCES}
    ${ST_SOURCES}
    ${GR_SOURCES}
    ${SN_SOURCES}
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("debug mode")
    add_definitions(-DTRACE_SCRIPT)
    add_definitions(-DDEBUG)
ENDIF()

add_definitions("-Wall")

add_executable(discharger ${SOURCES})

IF(PLATFORM STREQUAL "pc")
set_property(TARGET discharger PROPERTY OUTPUT_NAME discharger)
    add_definitions("-std=c++17")
    target_link_libraries(discharger ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY})

ELSEIF(PLATFORM STREQUAL "vita")
    add_definitions("-std=gnu++17")
    add_definitions("-D__VITA__")
    target_link_libraries(discharger SDL2_mixer::SDL2_mixer-static SDL2_image::SDL2_image-static SDL2::SDL2 png z)


  vita_create_self(${PROJECT_NAME}.self discharger UNSAFE)
  vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
    FILE platform/vita/sce_sys sce_sys
    FILE data data
  )

ENDIF()

