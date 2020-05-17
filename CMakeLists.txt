cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{DOLCESDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{DOLCESDK}/share/dolce.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define DOLCESDK to point to your SDK path!")
  endif()
endif()

project(custom_boot_splash)
include("${DOLCESDK}/share/dolce.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable(custom_boot_splash
  src/main.c
)

target_link_libraries(custom_boot_splash
  SceSysmemForDriver_stub
  SceCpuForDriver_stub
  SceDmacmgrForDriver_stub 
  SceThreadmgrForDriver_stub
  SceDebugForDriver_stub
  SceIofilemgrForDriver_stub
  SceDisplayForDriver_stub
)

dolce_create_self(custom_boot_splash.skprx custom_boot_splash
  CONFIG exports.yml
  UNSELF
)
