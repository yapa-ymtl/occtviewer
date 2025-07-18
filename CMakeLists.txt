cmake_minimum_required(VERSION 3.16)

project(OCCTViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_RPATH "/Users/yapa/Projects/upwork/Mohammad/occt/install/lib")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Gui Core)
find_package(OpenCASCADE REQUIRED)
find_package(Qt6 REQUIRED X11Extras 5Compat)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        myglwidget.cpp
        myglwidget.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OCCTViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        myglwidget.h myglwidget.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OCCTViewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(OCCTViewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OCCTViewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(OCCTViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(OCCTViewer PRIVATE Qt6::OpenGLWidgets)
target_link_libraries(OCCTViewer PRIVATE Qt6::Gui)
target_link_libraries(OCCTViewer PRIVATE ${OpenCASCADE_LIBRARIES})
target_link_libraries(OCCTViewer PRIVATE Qt6::Core Qt6::X11Extras Qt6::5Compat)
target_link_libraries(${PROJECT_NAME} PRIVATE
    TKernel
    TKMath
    TKService
    TKV3d
    TKOpenGl
    TKGeomBase
    TKBRep
    TKTopAlgo
    TKGeomAlgo
    TKPrim
    TKBool
    TKBO
    TKShHealing
    TKXCAF
    TKXSBase
    "-framework AppKit"
    "-framework Cocoa"
    # Add others based on features you use
)


target_link_directories(OCCTViewer PRIVATE
    /Users/yapa/Projects/upwork/Mohammad/occt/install/lib
)

# Include paths
target_include_directories(OCCTViewer PRIVATE ${OpenCASCADE_INCLUDE_DIR})
target_include_directories(OCCTViewer PRIVATE
    /Users/yapa/Projects/upwork/Mohammad/occt/install/include/opencascade
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.OCCTViewer)
endif()
set_target_properties(OCCTViewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS OCCTViewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OCCTViewer)
endif()
