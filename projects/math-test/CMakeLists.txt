#--------------------------------------------------------------------------
# mathTest project
#--------------------------------------------------------------------------

PROJECT(mathTest)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("mathTest" FILES ${files_example})

ADD_EXECUTABLE(mathTest ${files_example})
TARGET_LINK_LIBRARIES(mathTest core math)
ADD_DEPENDENCIES(mathTest core math)