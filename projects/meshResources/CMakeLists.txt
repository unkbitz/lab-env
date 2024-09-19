#--------------------------------------------------------------------------
# mesh resources project
#--------------------------------------------------------------------------

PROJECT(meshResources)
FILE(GLOB mesh_resources_headers code/*.h)
FILE(GLOB mesh_resources_sources code/*.cc)

SET(files_mesh_resources ${mesh_resources_headers} ${mesh_resources_sources})
SOURCE_GROUP("meshResources" FILES ${files_mesh_resources})

ADD_EXECUTABLE(meshResources ${files_mesh_resources})
TARGET_LINK_LIBRARIES(meshResources core render)
ADD_DEPENDENCIES(meshResources core render)

SET_PROPERTY(TARGET meshResources PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")