cmake_minimum_required (VERSION 3.5)

set (project ThorQ)
project (
    ${project}
    DESCRIPTION "Software to control 3rd party collars"
    LANGUAGES CXX
)

option(VERBOSE "Verbose compiler output" OFF)

# --- Configure default build ---
set (default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message (STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set (CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()
# --------------------------------

# --- Simplify build checking ---
math (EXPR VPTR_BITS "${CMAKE_SIZEOF_VOID_P}*8") # Convert 4/8 -> 32/64

if (APPLE)
    set (BUILD_OS "osx")
elseif (UNIX)
    set (BUILD_OS "linux")
elseif (WIN32)
    set (BUILD_OS "win")
endif ()
# --------------------------------

# --- Set global C++ CMake settings ---
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set (CMAKE_POLICY_DEFAULT_CMP0069 NEW)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
if (VERBOSE)
    set (CMAKE_VERBOSE_MAKEFILE ON)
endif ()
# -------------------------------

# --- Set compiler flags ---
add_definitions (-DBUILDER_STATIC_DEFINE)

# Optimization and compiler specific fixes
if (MSVC)
    # Windows minmax is retarded
    set (CMAKE_CXX_FLAGS " -DWIN32 -D_WINDOWS -W3 -GR -EHsc -DNOMINMAX -wo5054")

    if (${CMAKE_BUILD_TYPE} MATCHES "Release")
        string (APPEND CMAKE_CXX_FLAGS " /MD /DEBUG:NONE /O2 /Ob2")
    else ()
        string (APPEND CMAKE_CXX_FLAGS " /MDd /DEBUG:FULL /Od /Ob0 /Wall")
    endif ()
else ()
    # 64-bit, and threading
    set (CMAKE_CXX_FLAGS " -m64 -pthread")

    if (${CMAKE_BUILD_TYPE} MATCHES "Release")
        string (APPEND CMAKE_CXX_FLAGS " -fstack-protector -O3 -g0")
        if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            string (APPEND CMAKE_CXX_FLAGS " -s")
        endif ()
    else ()
        string (APPEND CMAKE_CXX_FLAGS " -O0 -g3 -Wall -Wextra -pedantic -Wformat=2 -Wsign-conversion -Wundef -Wredundant-decls -Wwrite-strings -Wpointer-arith -Wcast-qual -Wmissing-include-dirs -Wcast-align -Wctor-dtor-privacy -Wdisabled-optimization -Winvalid-pch -Woverloaded-virtual -Wconversion -Wswitch-enum -Wno-ctor-dtor-privacy -Wno-format-nonliteral -Wnoexcept -Wno-dangling-else -Wno-unused-local-typedefs -Wdouble-promotion -Wtrampolines -Wzero-as-null-pointer-constant -Wuseless-cast -Wvector-operation-performance -Wsized-deallocation -Wshadow -Wshift-overflow=2 -Wnull-dereference -Wduplicated-cond -Wpessimizing-move -Wredundant-move")
    endif ()
endif ()
# --------------------------

# --- OpenVR ---
if (APPLE)
    set (OPENVR_BUILD_NAME "${BUILD_OS}32") # OpenVR only has 32-bit for OSX
else ()
    set (OPENVR_BUILD_NAME "${BUILD_OS}${VPTR_BITS}")
endif ()

include_directories ()
link_directories () # eg. win64 / linux32
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/openvr/bin/${OPENVR_BUILD_NAME}/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# --------------

# ----- Qt -----
find_package (
    Qt5
    COMPONENTS
        Core
        Gui
        OpenGL
        Widgets
        Multimedia
        MultimediaWidgets
        SerialPort
        Concurrent
        Network
    REQUIRED
)
if (NOT Qt5Core_FOUND)
    message (FATAL_ERROR "No Qt installation found, aborting")
endif ()

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

message ("Found Qt ${Qt5Core_VERSION}")
# -----------------------------

# ---- dependencies ----
find_package (fmt REQUIRED CONFIG)
find_package (unofficial-sodium REQUIRED CONFIG)
# ----------------------

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

# --- Build ---
set (${project}_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sysutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pathutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/validators.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/stylesheets.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/collar/serial.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/api/client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/p2pclient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/apiobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/realtimeclient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/persistentcookiejar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/announcement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/user.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/account.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/passwordhash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/api/friendship.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/ui/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/loginwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/userlistitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/userlistwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/controlwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/settingswidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/loginwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/registerwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/recoverwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/errorlabel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/namedlineedit.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrlaunchmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovraction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovroverlay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovractionset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrinputvalue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrtrackeddevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/openvroverlaycontroller.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/maincontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/windowgeometrysaver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/windowpositionsaver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/loginwindowcontroller.cpp
)
set (${project}_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/filetype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sysutils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pathutils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/fileutils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/validators.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stylesheets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.h

    ${CMAKE_CURRENT_SOURCE_DIR}/collar/serial.h

    ${CMAKE_CURRENT_SOURCE_DIR}/api/client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/p2pclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/apiobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/realtimeclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/persistentcookiejar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/announcement.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/user.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/account.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/passwordhash.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/friendship.h

    ${CMAKE_CURRENT_SOURCE_DIR}/ui/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/loginwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/userlistitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/userlistwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/controlwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/settingswidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/loginwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/registerwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/recoverwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/errorlabel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/namedlineedit.h

    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrlaunchmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrhelpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovraction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovroverlay.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovractionset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrinputvalue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/ovrtrackeddevice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vr/openvroverlaycontroller.h

    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/maincontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/windowgeometrysaver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/windowpositionsaver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/loginwindowcontroller.h
)
set (${project}_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)

if (WIN32)
    list (APPEND ${project}_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc)
endif ()

add_executable (${project}
    main.cpp
    ${${project}_HEADERS}
    ${${project}_SOURCES}
    ${${project}_RESOURCES}
)

target_link_directories (${project} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/openvr/lib/${OPENVR_BUILD_NAME}
)

target_include_directories (${project} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${FMT_INCLUDE_DIRS}
    ${SODIUM_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/openvr/headers

    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5OpenGL_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Multimedia_INCLUDE_DIRS}
    ${Qt5SerialPort_INCLUDE_DIRS}
    ${Qt5Concurrent_INCLUDE_DIRS}
    ${Qt5MultimediaWidgets_INCLUDE_DIRS}
)

if (WIN32)
    set (WIN_LIBS ws2_32 winmm Rpcrt4 Iphlpapi)
endif ()

target_link_libraries (${project} PRIVATE
    ${WIN_LIBS}
    ${CMAKE_DL_LIBS}
    Threads::Threads
    ${Qt5Gui_LIBRARIES}
    ${Qt5Core_LIBRARIES}
    ${Qt5OpenGL_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Network_LIBRARIES}
    ${Qt5Multimedia_LIBRARIES}
    ${Qt5SerialPort_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
    ${Qt5MultimediaWidgets_LIBRARIES}

    fmt::fmt
    unofficial-sodium::sodium
    openvr_api
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property (TARGET ${project} PROPERTY WIN32_EXECUTABLE TRUE)
endif ()

message ("Building ${project} in ${CMAKE_BUILD_TYPE} mode for ${VPTR_BITS}-bit ${BUILD_OSL}, using ${CMAKE_CXX_COMPILER_ID}")
