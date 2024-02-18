cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(fgfwdumper)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(plugin_name "fgfwdumper.skprx")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

add_executable(fgfwdumper
  fgfwdumper.c
)

set_target_properties(fgfwdumper
  PROPERTIES LINK_FLAGS
  -nostdlib
)

target_link_libraries(fgfwdumper
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

vita_create_self(${plugin_name} fgfwdumper
  CONFIG ${CMAKE_SOURCE_DIR}/fgfwdumper.yml
  UNSAFE
)

vita_create_stubs(stubs fgfwdumper ${CMAKE_CURRENT_SOURCE_DIR}/fgfwdumper.yml KERNEL)