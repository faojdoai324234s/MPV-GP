cmake_minimum_required( VERSION 3.19 )

set( library_name MPVPlayer )

project( ${library_name} CXX )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )

set( LIBMPV_HEADERS_DIR ${PROJECT_SOURCE_DIR}/include )

link_directories( ${PROJECT_SOURCE_DIR}/lib )

# Hide the console window in visual studio projects
if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

list( APPEND Qt5_DIR ${QT_DIRECTORY} )

find_package( Qt5 REQUIRED COMPONENTS Core Gui OpenGL Widgets )

if( UNIX AND NOT APPLE )
  find_package( OpenGL REQUIRED COMPONENTS EGL OpenGL )
else()
  find_package( OpenGL REQUIRED COMPONENTS OpenGL )
endif()

file( GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.hpp )

file( GLOB LIBMPV_HEADERS ${LIBMPV_HEADERS_DIR}/mpv/*.h )

add_executable( ${library_name} ${SRC_FILES} )

set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${library_name} )

if( UNIX AND NOT APPLE )
  set( LINK_LIBS
  libmpv.so
  OpenGL::EGL
  )
elseif( APPLE )
  set( LINK_LIBS
  libmpv.dylib
  )
elseif( WIN32 )
  set( LINK_LIBS
  libmpv.lib
  )
endif()

target_link_libraries( ${library_name} PRIVATE Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets
OpenGL::GL OpenGL::GLU ${LINK_LIBS} )	# Needed for QOpenGLWidget

target_include_directories( ${library_name} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${LIBMPV_HEADERS_DIR}
)
