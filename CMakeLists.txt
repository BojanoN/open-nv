cmake_minimum_required(VERSION 3.10)
project(OpenNV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(LibraryDirectory "${CMAKE_CURRENT_SOURCE_DIR}/lib") 
set(SourceDirectory "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TestsDirectory "${CMAKE_CURRENT_SOURCE_DIR}/tests")

set(LibraryIncludeDirectory "${CMAKE_CURRENT_SOURCE_DIR}/lib/include")

list(APPEND IncludeDirectories
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${LibraryDirectory}/src/glm"
  "${LibraryDirectory}/src/glew/include"
  "${LibraryDirectory}/src/sdl2/include"
  "${LibraryDirectory}/src/zlib"
  "${LibraryDirectory}/src/libav"
  )


add_subdirectory(${SourceDirectory})
