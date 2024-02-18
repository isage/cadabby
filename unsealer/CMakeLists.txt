cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(fgunseal)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(plugin_name "fgunseal.skprx")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

add_executable(fgunseal
  fgunseal.c
)

set_target_properties(fgunseal
  PROPERTIES LINK_FLAGS
  -nostdlib
)

target_link_libraries(fgunseal
  SceDebugForDriver_stub
  SceModulemgrForDriver_stub
  SceSysmemForDriver_stub
  SceSysclibForDriver_stub
  SceSysconForDriver_stub
  taihenForKernel_stub
  taihenModuleUtils_stub
)

vita_create_self(${plugin_name} fgunseal
  CONFIG ${CMAKE_SOURCE_DIR}/fgunseal.yml
  UNSAFE
)

vita_create_stubs(stubs fgunseal ${CMAKE_CURRENT_SOURCE_DIR}/fgunseal.yml KERNEL)