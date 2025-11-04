cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(xmos-ddc)

set(APP_HW_TARGET xu316-ddc.xn)
set(APP_INCLUDES src)

set(APP_COMPILER_FLAGS -O3
                       -report
                       -g
                       -lquadflash
                       -Wno-unused-function
                       -DXUD_CORE_CLOCK=800
                       -DUSB_TILE=tile[0]
                       -fxscope
                       -DUAC_FORCE_FEEDBACK_EP=0)

set(APP_DEPENDENT_MODULES "lib_xua(v5.2.0)"
                          "lib_xud"
                          "lib_i2c")

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/sandbox)

XMOS_REGISTER_APP()
