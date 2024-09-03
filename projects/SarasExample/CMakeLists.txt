#--------------------------------------------------------------------------
# saras example project
#--------------------------------------------------------------------------

PROJECT(SarasExample)
FILE(GLOB saras_example_headers code/*.h)
FILE(GLOB saras_example_sources code/*.cc)

SET(files_example ${saras_example_headers} ${saras_example_sources})
SOURCE_GROUP("SarasExample" FILES ${files_saras_example})

ADD_EXECUTABLE(SarasExample ${files_example})
TARGET_LINK_LIBRARIES(SarasExample core render)
ADD_DEPENDENCIES(SarasExample core render)