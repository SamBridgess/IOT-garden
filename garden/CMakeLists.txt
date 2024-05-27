project(iot-garden)

CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC})


# Find the Mosquitto package
find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)

# Include directories
include_directories(${MOSQUITTO_INCLUDE_DIRS})

# Link the Mosquitto library
target_link_libraries(${PROJECT_NAME} ${MOSQUITTO_LIBRARIES})