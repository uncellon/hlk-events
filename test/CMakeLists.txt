set(BINARY ${CMAKE_PROJECT_NAME}Test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

target_include_directories(
    ${BINARY} PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(${BINARY} PUBLIC ${PROJECT_NAME})