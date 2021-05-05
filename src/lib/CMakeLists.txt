project(trazado_lib)

file(GLOB_RECURSE trazado_lib_files
     "*.h"
     "*.cpp"
)
message(STATUS ${trazado_lib_files})

add_library(trazado_lib STATIC ${trazado_lib_files})

target_include_directories(trazado_lib PUBLIC
                           "${CMAKE_SOURCE_DIR}/src/lib"
                           "${CMAKE_SOURCE_DIR}/include"
                           "${CMAKE_SOURCE_DIR}/include/glm"
                           "${CMAKE_SOURCE_DIR}/include/rapidxml"
                           "${CMAKE_SOURCE_DIR}/include/genericfactory"
                           )