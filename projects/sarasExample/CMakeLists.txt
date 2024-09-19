#--------------------------------------------------------------------------
# sarasExample project
#--------------------------------------------------------------------------

PROJECT(sarasExample)
FILE(GLOB saras_example_headers code/*.h)
FILE(GLOB saras_example_sources code/*.cc)

SET(files_saras_example ${saras_example_headers} ${saras_example_sources})
SOURCE_GROUP("sarasExample " FILES ${files_saras_example})

ADD_EXECUTABLE(sarasExample ${files_saras_example})
TARGET_LINK_LIBRARIES(sarasExample core render)
ADD_DEPENDENCIES(sarasExample core render)

SET_PROPERTY(TARGET sarasExample PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")