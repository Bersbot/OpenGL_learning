add_subdirectory (glfw)
add_subdirectory (glad)
add_subdirectory (glm)

<<<<<<< HEAD
target_include_directories (${PROJECT_NAME} PUBLIC glfw/include)
target_include_directories (${PROJECT_NAME} PUBLIC glad/include)
target_include_directories (${PROJECT_NAME} PUBLIC glm/glm)
=======
target_include_directories (${PROJECT_NAME} PUBLIC glfw/include/)
target_include_directories (${PROJECT_NAME} PUBLIC glad/include/)
target_include_directories (${PROJECT_NAME} PUBLIC glm/glm/)
>>>>>>> d9a1444 (Add README with build instructions)
