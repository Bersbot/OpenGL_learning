add_subdirectory (glfw)
add_subdirectory (glad)
add_subdirectory (glm)

target_include_directories (${PROJECT_NAME} PUBLIC glfw/include)
target_include_directories (${PROJECT_NAME} PUBLIC glad/include)
target_include_directories (${PROJECT_NAME} PUBLIC glm/glm)
