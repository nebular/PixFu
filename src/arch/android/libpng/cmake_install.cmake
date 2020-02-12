# Install script for directory: /Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/libpng.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpng.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpng.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpng.a")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/pnginfo.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/config.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/png.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/pngconf.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/pngdebug.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/pnglibconf.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/pngprefix.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/pngpriv.h"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/jni/pngstruct.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/png-1.6.29/cmake/png-export.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/png-1.6.29/cmake/png-export.cmake"
         "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/CMakeFiles/Export/share/png-1.6.29/cmake/png-export.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/png-1.6.29/cmake/png-export-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/png-1.6.29/cmake/png-export.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/png-1.6.29/cmake" TYPE FILE FILES "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/CMakeFiles/Export/share/png-1.6.29/cmake/png-export.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/png-1.6.29/cmake" TYPE FILE FILES "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/CMakeFiles/Export/share/png-1.6.29/cmake/png-export-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/png-1.6.29/cmake" TYPE FILE FILES
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/png-config.cmake"
    "/Volumes/git.root/projs/gaming/PixEngine_Android/lib/src/main/cpp/lib/PixEngine_core/src/arch/android/libpng/png-config-version.cmake"
    )
endif()

