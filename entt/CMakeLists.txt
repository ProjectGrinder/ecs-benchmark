add_executable(entt-test main.cpp)

if (WIN32)
    target_link_libraries(entt-test PRIVATE psapi)
endif()


set_target_properties(entt-test PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/entt"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/entt"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/entt"
)
