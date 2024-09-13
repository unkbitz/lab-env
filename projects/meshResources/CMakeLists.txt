#--------------------------------------------------------------------------
# mesh resources project
#--------------------------------------------------------------------------

PROJECT(meshResources)
FILE(GLOB mesh_resources_headers code/*.h)
FILE(GLOB mesh_resources_sources code/*.cc)

SET(files_mesh_resources ${mesh_resources_headers} ${mesh_resources_sources})
SOURCE_GROUP("meshResources" FILES ${files_mesh_resources})

ADD_EXECUTABLE(meshResources ${files_mesh_resources} "code/textureResource.h" "code/camera.h" "code/stb_image.h" "code/std_image.cpp" "code/texureResource.cpp")
TARGET_LINK_LIBRARIES(meshResources core render)
ADD_DEPENDENCIES(meshResources core render)