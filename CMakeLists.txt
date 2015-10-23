cmake_minimum_required(VERSION 2.8.10)

project(QtToCppXML)

find_package( SlicerExecutionModel REQUIRED )
include( ${SlicerExecutionModel_USE_FILE} )
include(${GenerateCLP_USE_FILE})

find_package(Qt5Widgets REQUIRED)

set( MySources
lgen.cpp
lgen.h
main.cpp
mgen.cpp
mgen.h
parse.h
parse.cpp
sgen.cpp
sgen.h
)

GENERATECLP(MySources QtToCppXML.xml)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QtToCppXML_BINARY_DIR}/bin)

add_executable( QtToCppXML ${MySources} )
target_link_libraries( QtToCppXML Qt5::Widgets)

# --------------------------------------------------------------------------
# Configure support files
# --------------------------------------------------------------------------  
configure_file(
  QtToCppXML.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/QtToCppXML.cmake
  @ONLY
  )
include(GenerateQtToCppXMLConfig.cmake)
