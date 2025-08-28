add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries (${PROJECT_NAME}
    glfw
    glad
    )
include_directories(${CMAKE_SOURCE_DIR}/libs/glm)
