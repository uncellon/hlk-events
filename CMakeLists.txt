cmake_minimum_required(VERSION 3.16)
project(HlkEvents VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_library(${PROJECT_NAME} INTERFACE)
add_library(Hlk::Events ALIAS ${PROJECT_NAME})

add_subdirectory(test)

file(GLOB HEADERS src/*.h)

# Specify include directories for "local" build and installed library
target_include_directories(
    ${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/hlk/events>
)

# Set public headers
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
        PUBLIC_HEADER "${HEADERS}"
        EXPORT_NAME events
)

# Install headers
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
        PUBLIC_HEADER 
            DESTINATION include/hlk/events
)

# Generate base config and install
install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE Hlk::
    FILE ${PROJECT_NAME}Config.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)

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
