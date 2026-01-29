# CMakeLists.txt (top-level)

cmake_minimum_required(VERSION 3.20)
project(RappFF LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Assuming vcpkg toolchain is already set via -DCMAKE_TOOLCHAIN_FILE=... or VSCode preset
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Try CONFIG first, then fallback to module-style find
find_package(Freetype CONFIG)
if(NOT Freetype_FOUND)
    find_package(Freetype REQUIRED)
endif()

# The main app
add_executable(RappFF
    src/glad.c
    src/Stbimage.c
    src/main.cpp
    src/TheGame.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/FontShader.cpp
    src/SkyboxShader.cpp
    src/Mesh.cpp
    src/Texture.cpp
    src/ObjLoader.cpp
    src/Display.cpp
    src/Viewport.cpp
    src/WindowSection.cpp
    src/Text.cpp
    src/DrawableObj.cpp
    src/GameMap.cpp
    src/TiledGameBoard.cpp
    src/Tile.cpp
    src/Character.cpp
    src/BattleManager.cpp
    src/FightMode.cpp
    src/PartyBuildMode.cpp
    src/NewOrLoadMode.cpp
    src/Observer.cpp
    src/RandUtils.cpp
    src/Toc.cpp

)

target_include_directories(RappFF PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Resolve common imported target names (vcpkg / package differences)
if(TARGET glfw::glfw)
    set(GLFW_TARGET glfw::glfw)
elseif(TARGET glfw)
    set(GLFW_TARGET glfw)
elseif(TARGET glfw3::glfw)
    set(GLFW_TARGET glfw3::glfw)
else()
    message(FATAL_ERROR "GLFW imported target not found. Expected one of: glfw::glfw, glfw, glfw3::glfw")
endif()

if(TARGET glm::glm)
    set(GLM_TARGET glm::glm)
elseif(TARGET glm)
    set(GLM_TARGET glm)
else()
    message(FATAL_ERROR "GLM imported target not found. Expected glm::glm or glm.")
endif()

if(TARGET Freetype::Freetype)
    set(FREETYPE_TARGET Freetype::Freetype)
elseif(TARGET freetype)
    set(FREETYPE_TARGET freetype)
elseif(TARGET Freetype)
    set(FREETYPE_TARGET Freetype)
else()
    # Not fatal — some find_package(Freetype) only provides include dirs/libs via variables.
    set(FREETYPE_TARGET "")
    message(WARNING "Freetype imported target not found. If linking fails, link manually or inspect the Freetype CMake export provided by your package manager.")
endif()

# Link libraries (skip empty FREETYPE_TARGET if not found)
target_link_libraries(RappFF PRIVATE
    ${GLFW_TARGET}
    ${GLM_TARGET}
    ${FREETYPE_TARGET}
)

# Optional: copy assets to build dir (very common for games)
add_custom_command(TARGET RappFF POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:RappFF>/assets
)