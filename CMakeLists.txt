cmake_minimum_required(VERSION 3.5)
#SET(CMAKE_CXX_COMPILER /usr/bin/clang)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)
set (CMAKE_CXX_STANDARD 14)

project(procon_driver)

find_library(HIDAPI_LIBRARY
	NAMES hidapi hidapi-libusb REQUIRED)

#find_library(EVDEV_LIBRARY
#	NAMES evdev REQUIRED)

find_library(TESTLIB_LIBRARY2 evdev)

find_path(HIDAPI_INCLUDE_DIR
	NAMES hidapi.h
	PATH_SUFFIXES
	hidapi REQUIRED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HIDAPI
	DEFAULT_MSG
	HIDAPI_LIBRARY
	HIDAPI_INCLUDE_DIR)

if(HIDAPI_FOUND)
	set(HIDAPI_LIBRARIES "${HIDAPI_LIBRARY}")



	set(HIDAPI_INCLUDE_DIRS "${HIDAPI_INCLUDE_DIR}")
endif()

#add_library (libshit libevdev/libevdev/libevdev.c)


#if(EVDEV_FOUND)
#	set(EVDEV_LIBRARIES "${EVDEV_LIBRARY}")
#endif()

mark_as_advanced(HIDAPI_INCLUDE_DIR HIDAPI_LIBRARY)

include_directories(
	${HIDAPI_INCLUDE_DIRS}
	"src/"
	"libevdev/"
)
set(MAIN main.cpp)


MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${HIDAPI_INCLUDE_DIR} )

add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} ${HIDAPI_LIBRARIES}) #${TESTLIB_LIBRARY2})#libshit)# ${TESTLIB_LIBRARY2})#libshit)