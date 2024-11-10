cmake_minimum_required(VERSION 3.28)
project(KitNEngine VERSION 0.1 LANGUAGES C CXX)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4101 /wd4267 /wd4244 /permissive-")    # Additional compiler warnings and options for MSVC
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp" "${INCLUDE_DIR}/*.h")

include(FindDevIL)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
find_package(sdl2_image CONFIG REQUIRED)
find_package(sdl2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(DevIL REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include ${IL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${BULLET_LIBRARIES}
        GLEW::GLEW
        glm::glm 
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        imgui::imgui
        assimp::assimp
        SDL2::SDL2_gfx
        $<TARGET_NAME_IF_EXISTS:DevIL::IL>
        $<TARGET_NAME_IF_EXISTS:DevIL::ILU>
        $<TARGET_NAME_IF_EXISTS:DevIL::ILUT>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        yaml-cpp::yaml-cpp
)


# install licenses
file(GLOB LICENSE_DIRS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/*")
foreach(LICENSE_DIR ${LICENSE_DIRS})
    if (EXISTS ${LICENSE_DIR}/copyright)
        get_filename_component(LICENSE_LIB_NAME ${LICENSE_DIR} NAME)
        message(STATUS ${LICENSE_LIB_NAME})
        install(FILES ${LICENSE_DIR}/copyright DESTINATION "license" RENAME ${LICENSE_LIB_NAME})
    endif()
endforeach()