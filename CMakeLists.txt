cmake_minimum_required(VERSION 3.16)
project(HlkEvents VERSION 1.0.5)

option(BUILD_EXAMPLES "Build examples" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

file(GLOB HEADERS include/hlk/events/*.h)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Hlk::Events ALIAS ${PROJECT_NAME})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
        PUBLIC_HEADER "${HEADERS}"
        EXPORT_NAME Events
)

target_include_directories(
    ${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/hlk/events>
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    PUBLIC_HEADER
        DESTINATION "include/hlk/events"
)

install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE Hlk::
    FILE "${PROJECT_NAME}Config.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)

# Generate local "${PROJECT_NAME}Config.cmake"
export(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Config.cmake
)
# Since the export(...) is performed after find_package(...), create an empty file to avoid find_package(...) errors
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)

# Generate config for version supporting
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install config for version supporting
install(
    FILES
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
