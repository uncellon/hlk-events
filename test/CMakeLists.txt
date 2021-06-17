set(TEST_PROJECT HlkEventsTest)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${TEST_PROJECT} ${TEST_SOURCES})

target_include_directories(
    ${TEST_PROJECT} PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_link_libraries(${TEST_PROJECT} PUBLIC ${PROJECT_NAME})