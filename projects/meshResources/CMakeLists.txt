#--------------------------------------------------------------------------
# mesh resources sim project
#--------------------------------------------------------------------------

PROJECT(meshResourcesSim)
FILE(GLOB mesh_resources_sim_headers code/*.h)
FILE(GLOB mesh_resources_sim_sources code/*.cc)

SET(files_mesh_resources_sim ${mesh_resources_sim_headers} ${mesh_resources_sim_sources})
SOURCE_GROUP("meshResourcesSim" FILES ${files_mesh_resources_sim})

ADD_EXECUTABLE(meshResourcesSim ${files_mesh_resources_sim})
TARGET_LINK_LIBRARIES(meshResourcesSim core render)
ADD_DEPENDENCIES(meshResourcesSim core render)

SET_PROPERTY(TARGET meshResourcesSim PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")