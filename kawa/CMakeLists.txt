add_executable(kawa-test main.cpp)

if (WIN32)
    target_link_libraries(kawa-test PRIVATE psapi)
endif()

set_target_properties(kawa-test PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/kawa"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/kawa"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/kawa"
)
