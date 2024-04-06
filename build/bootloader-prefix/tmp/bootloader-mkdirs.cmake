# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/One/Desktop/document/SDK/v5.2.1/esp-idf/components/bootloader/subproject"
  "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader"
  "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader-prefix"
  "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader-prefix/tmp"
  "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader-prefix/src"
  "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/One/Desktop/document/Github/z4001-debug-monitor/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
