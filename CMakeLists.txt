cmake_minimum_required( VERSION 3.6 )

project( bcp-prototype )

# 함수 내에서 설정한 변수값은 외부에서 적용 안됨
function( trace msg )
	string( ASCII 27 Esc )
	set( CLEAR "${Esc}[m")
	set( LightGreen "${Esc}[1;32m" )

	message( STATUS "${LightGreen}${msg}${CLEAR}" )
endfunction( trace )

set( VERSION_MAJOR 0 )
set( VERSION_MINOR 1 )
set( VERSION_PATCH 1 )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_EXTENSIONS ON )
set( CXX_STANDARD_REQUIRED ON )
set( CMAKE_VERBOSE_MAKEFILE true )

# folder names
set( BRC_LIBRARIES_NAME libraries )
set( BRC_APPS_NAME apps )
set( BRC_LIBS_NAME libs )
set( BRC_BIN_NAME bin )
set( BRC_BUILD_NAME build )
set( BRC_TEST_NAME test )

set( BRC_ROOT_PATH ${PROJECT_SOURCE_DIR} )
set( BRC_LIBRARIES_PATH ${BRC_ROOT_PATH}/${BRC_LIBRARIES_NAME} )
set( BRC_LIBS_PATH ${BRC_ROOT_PATH}/${BRC_LIBS_NAME} )
set( BRC_APPS_PATH ${BRC_ROOT_PATH}/${BRC_APPS_NAME} )
set( BRC_BUILD_PATH ${BRC_ROOT_PATH}/${BRC_BUILD_NAME} )
set( BRC_BIN_PATH ${BRC_ROOT_PATH}/${BRC_BIN_NAME} )
set( BRC_TEST_PATH ${BRC_ROOT_PATH}/${BRC_TEST_NAME} )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BRC_BIN_PATH} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BRC_LIBS_PATH} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BRC_LIBS_PATH} )

configure_file(
	${BRC_LIBRARIES_PATH}/bryllite/version.hpp.in
	${BRC_LIBRARIES_PATH}/bryllite/version.hpp
)

# Boost 1.62+ required
set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREAD ON )
find_package( Boost 1.62 REQUIRED COMPONENTS
	system
	program_options
	thread
	date_time
	filesystem
	signals
	serialization
	chrono
	unit_test_framework
	context
	locale
	iostreams
)
if( Boost_FOUND )
	include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
endif()

# include header path
include_directories( ${BRC_LIBRARIES_NAME} )

# subdirectories
add_subdirectory( ${BRC_LIBRARIES_NAME} )
add_subdirectory( ${BRC_APPS_NAME} )
add_subdirectory( ${BRC_TEST_NAME} )

