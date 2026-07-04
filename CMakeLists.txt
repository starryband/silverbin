cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER g++)
project(SilverProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output dir
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(silver ${SOURCES})

# Include directories
target_include_directories(silver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assimp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/freetype
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine_events
)

# Link directories (for .lib files)
target_link_directories(silver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

find_library(ASSIMP_LIB assimp PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_link_libraries(silver
    glfw3dll
    freetype
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libassimp-6.dll
)

# Copy runtime DLLs
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libassimp-6.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# Copy assets
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/steamhappy.png" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/icon.png" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/models" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libassimp-6.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin")