cmake_minimum_required(VERSION 3.5)
SET(CMAKE_CXX_COMPILER /usr/bin/clang)
set (CMAKE_CXX_STANDARD 11)

project(procon_driver)

find_library(HIDAPI_LIBRARY
	NAMES hidapi hidapi-libusb REQUIRED)

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

mark_as_advanced(HIDAPI_INCLUDE_DIR HIDAPI_LIBRARY)

include_directories(
	${HIDAPI_INCLUDE_DIRS}
	"src/"
)
set(MAIN main.cpp)


MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${HIDAPI_INCLUDE_DIR} )

add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} ${HIDAPI_LIBRARIES})