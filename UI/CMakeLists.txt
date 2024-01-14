cmake_minimum_required(VERSION 3.16)

project(UI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)


if(WIN32)
    set(ENGINE_LIB_EXT ".lib")
else()
    set(ENGINE_LIB_EXT ".a")
endif()

# Specify the resource file
set(UI_RESOURCES qrc_resource_file.qrc)

# Check the Qt version and add the resource file accordingly
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_resources(UI_RESOURCES ${UI_RESOURCES})
elseif(QT_VERSION_MAJOR EQUAL 5)
    qt5_add_resources(UI_RESOURCES ${UI_RESOURCES})
else()
    message(FATAL_ERROR "Unsupported Qt version: ${QT_VERSION_MAJOR}")
endif()

qt_add_executable(UI
    MANUAL_FINALIZATION
    src/main.cpp
    src/mainwindow/mainwindow.cpp include/mainwindow/mainwindow.h
    src/mainwindow/mainwindow.ui
    include/qsfmlcanvas.h src/qsfmlcanvas.cpp
    include/simulationcanvas.h src/simulationcanvas.cpp
    ${UI_RESOURCES}
    include/creaturetracker.h
    src/creaturetracker.cpp
    include/mainwindow/graph_manager.h
    src/mainwindow/graph_manager.cpp




)

target_link_libraries(UI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)

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

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(UI PRIVATE
    ${ENGINE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(UI PRIVATE Engine)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.evolution.UI)
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

# Define the source and destination of the configuration file
set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/settings.json")
set(CONFIG_DEST "${CMAKE_BINARY_DIR}/UI")

# Copy the file as a pre-build step
add_custom_command(
    TARGET UI PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONFIG_FILE}
        ${CONFIG_DEST}
    COMMENT "Copying settings.json to build directory"
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(UI)
endif()

