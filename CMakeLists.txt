cmake_minimum_required(VERSION 3.10)

project(itk-js-decompression)

find_package(ITK REQUIRED COMPONENTS ITKIOImageBase ITKIOMeta)
include(${ITK_USE_FILE})

add_executable(decompression decompression.cxx)
target_link_libraries(decompression ${ITK_LIBRARIES})

enable_testing()
add_test(NAME decommpression_test
  COMMAND decompression ${CMAKE_CURRENT_SOURCE_DIR}/orientedGrid.mha
  )
