cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER g++)
project(SilverProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output dir
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Source files (Now looking into both src/ and external/)
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.c"
)

add_executable(silver ${SOURCES})

# Include directories (Added the external directory to the path)
target_include_directories(silver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assimp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/freetype
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine_events
    ${CMAKE_CURRENT_SOURCE_DIR}/external
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

# --- Assets & Runtime File Management ---

# Copy runtime DLLs to bin
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libassimp-6.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)

# Copy asset folders to bin
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fonts" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/models" DESTINATION ${EXECUTABLE_OUTPUT_PATH})

# Copy single-file image textures to bin
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/steamhappy.png" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/icon.png" DESTINATION ${EXECUTABLE_OUTPUT_PATH})