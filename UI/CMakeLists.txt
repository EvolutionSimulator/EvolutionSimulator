cmake_minimum_required(VERSION 3.16)

project(UI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(WIN32)
    set(ENGINE_LIB_EXT ".lib")
else()
    set(ENGINE_LIB_EXT ".a")
endif()

qt_add_executable(UI
    MANUAL_FINALIZATION
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
    qsfmlcanvas.h qsfmlcanvas.cpp
    simulationcanvas.h simulationcanvas.cpp

)

target_link_libraries(UI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# SFML Integration
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Link SFML libraries
target_link_libraries(UI PRIVATE sfml-graphics)

# Copy OpenAL DLL for Windows
if(WIN32)
    add_custom_command(
        TARGET UI
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:UI>
        VERBATIM)
endif()

add_subdirectory("../Engine" "${CMAKE_BINARY_DIR}/Engine")
target_include_directories(UI PRIVATE
    "../Engine/include"
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(UI PRIVATE Engine)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.UI)
endif()
set_target_properties(UI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS UI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UI)
endif()
