cmake_minimum_required(VERSION 3.10)
project(MipsGraphicsBridge)

set(CMAKE_CXX_STANDARD 17)

add_library(mips_display SHARED Main.cpp MipsDisplay.cpp)

set_target_properties(mips_display PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_include_directories(mips_display PRIVATE ${CMAKE_SOURCE_DIR})

# For Windows builds, olcPixelGameEngine needs GDI+
if(WIN32)
    target_link_libraries(mips_display PRIVATE gdiplus shlwapi dwmapi)
endif()
