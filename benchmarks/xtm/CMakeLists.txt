cmake_minimum_required(VERSION 2.8.8)

set(USE_XTM_INLINE OFF CACHE BOOL "Use exclusive software transactional memory")

if(USE_XTM_INLINE)
  add_definitions(-DXTM_USE_INLINE_LOCKABLE)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

file(GLOB MM_SRC mm/*.cpp)
file(GLOB LL_SRC ll/*.cpp)
include_directories(include .)
add_library(xtm xtm.cpp ${MM_SRC} ${LL_SRC})
target_link_libraries(xtm ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS xtm
  EXPORT xtm
  RUNTIME DESTINATION bin COMPONENT bin
  LIBRARY DESTINATION lib COMPONENT shlib
  ARCHIVE DESTINATION lib COMPONENT dev)
install(DIRECTORY include/ DESTINATION "include" COMPONENT dev
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE)
install(FILES xtm-config.cmake DESTINATION "cmake/xtm")
install(EXPORT xtm DESTINATION "cmake/xtm" COMPONENT dev)
