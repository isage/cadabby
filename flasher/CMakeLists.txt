cmake_minimum_required(VERSION 3.20)

set(PLATFORM "pc" CACHE STRING "Platform to build for")
set_property(CACHE PLATFORM PROPERTY STRINGS pc vita)

if(PLATFORM STREQUAL "pc")
elseif(PLATFORM STREQUAL "vita")
    IF(DEFINED ENV{VITASDK})
        include("$ENV{VITASDK}/share/vita.toolchain.cmake" REQUIRED)
        include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)
    ELSE()
        message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
    ENDIF()

endif()

project(fgfwflasher)

set(plugin_name "fgfwflasher.skprx")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

if(PLATFORM STREQUAL "pc")
add_definitions("-D__PCEMU__")
endif()

add_executable(fgfwflasher
  fgfwflasher.c
)

if(PLATFORM STREQUAL "vita")
set_target_properties(fgfwflasher
  PROPERTIES LINK_FLAGS
  -nostdlib
)

target_link_libraries(fgfwflasher
  SceDebugForDriver_stub
  SceKernelSuspendForDriver_stub
  SceModulemgrForDriver_stub
  SceThreadmgrForDriver_stub
  SceSysmemForDriver_stub
  SceSysclibForDriver_stub
  SceSysrootForDriver_stub
  SceUsbServForDriver_stub
  SceSblACMgrForDriver_stub
  SceSysconForDriver_stub
  SceI2cForDriver_stub
  SceHdmiForDriver_stub
  ScePowerForDriver_stub
  SceDisplayForDriver_stub
  SceIdStorageForDriver_stub
  SceIofilemgrForDriver_stub
  taihenForKernel_stub
  taihenModuleUtils_stub
)

vita_create_self(${plugin_name} fgfwflasher
  CONFIG ${CMAKE_SOURCE_DIR}/fgfwflasher.yml
  UNSAFE
)

vita_create_stubs(stubs fgfwflasher ${CMAKE_CURRENT_SOURCE_DIR}/fgfwflasher.yml KERNEL)
endif()