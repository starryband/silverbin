cmake_minimum_required(VERSION 3.10)
project(SilverProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Output directory for the executable and copied assets
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 2. Source files (Looking into both src/ and external/)
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/*.c"
)

add_executable(silver ${SOURCES})

# 3. Include directories
target_include_directories(silver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assimp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/freetype
    ${CMAKE_CURRENT_SOURCE_DIR}/include/engine_events
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# 4. Tell the linker to check both /lib AND the root folder
target_link_directories(silver PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 5. Explicitly locate the specific library binaries via absolute paths
find_file(GLFW_LIB NAMES glfw3.dll glfw3dll.dll PATHS ${CMAKE_CURRENT_SOURCE_DIR} NO_DEFAULT_PATH)
find_file(ASSIMP_LIB NAMES libassimp-6.dll assimp.dll PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib NO_DEFAULT_PATH)

# FreeType is usually a static (.a) or import (.lib) file in MinGW setups, 
# so find_library is fine here. But if it's a raw DLL, change this to find_file too!
find_library(FREETYPE_LIB NAMES freetype PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib NO_DEFAULT_PATH)

# 6. Link libraries safely to the executable target
target_link_libraries(silver PRIVATE
    ${GLFW_LIB}
    ${FREETYPE_LIB}
    ${ASSIMP_LIB}
)

# 7. --- Assets & Runtime File Management ---

# Copy runtime DLLs directly to bin
file(COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libassimp-6.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}
)

# Copy asset directories straight to bin
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fonts" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/models" DESTINATION ${EXECUTABLE_OUTPUT_PATH})

# Copy individual core assets to bin
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/steamhappy.png" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/icon.png" DESTINATION ${EXECUTABLE_OUTPUT_PATH})