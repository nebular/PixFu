cmake_minimum_required(VERSION 3.4.1)
# set targetPlatform, will be passed in from gradle when this sample is completed
# openGL Supportability
# platform         status
#   (0 12)           ES2/ES3 not supported
#   [12, 18)         ES2 only; for ES3, app do dynamic load/detection
#                    this applies to the situations that:
#                        - minimum API is set to less than 18. In this case
#                          there is no ES3 header/lib support inside NDK
#                        - the built APK might be running on newer API phones
#                    with dynamic loading of ES3, the same APK would still be able
#                    to use ES3. Otherwise, app would stuck with ES2 even phone is
#                    is newer than the minimum API level (for example, Android-27 etc).
#
#   [18, 24)         ES2 & ES3
#                    If app is built to only support API-18 or later,
#                    set minimum api level to 18 is good enough, NDK supprts ES3
#                    with the right header and lib files. No need to use ES3 dynamic
#                    detection.
#   [24, infinite)   ES2 & ES3 & Vulkan

add_subdirectory(src/arch/android/libpng)

include_directories(src)

include_directories(src/core/headers)
include_directories(src/input/headers)
include_directories(src/items/headers)
include_directories(src/support)
include_directories(src/thirdparty)

include_directories(src/arch/android/libpng/jni)

include_directories(${CMAKE_SOURCE_DIR})

## PIXENGINE

add_library(pixFu SHARED
        src/core/Drawable.cpp
        src/core/LayerVao.cpp
        src/core/OpenGlUtils.cpp
        src/core/Fu.cpp
        src/core/Surface.cpp
        src/core/Texture2D.cpp

        src/input/Keyboard.cpp
        src/input/AxisController.cpp
        src/input/VirtualKeys.cpp
        src/input/Mouse.cpp

        src/items/Canvas2D.cpp
        src/items/Font.cpp

        src/support/FuOLC.cpp

        src/arch/android/platform_android.cpp
        src/arch/android/input/GyroController.cpp
        )

# Include libraries needed for gles3jni lib

target_link_libraries(pixFu
        ${OPENGL_LIB}
        android
        EGL
        png
        log
        m)
