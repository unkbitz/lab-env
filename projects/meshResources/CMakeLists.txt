#--------------------------------------------------------------------------
# mesh resources project
#--------------------------------------------------------------------------

PROJECT(meshResources)
FILE(GLOB mesh_resources_headers code/*.h)
FILE(GLOB mesh_resources_sources code/*.cc)

SET(files_mesh_resources ${mesh_resources_headers} ${mesh_resources_sources})
SOURCE_GROUP("meshResources" FILES ${files_mesh_resources})

ADD_EXECUTABLE(meshResources ${files_mesh_resources} "../../engine/textures/textureResource.h" "../../engine/camera/camera.h" "../../engine/textures/stb_image.h" "../../engine/textures/std_image.cpp" "../../engine/textures/texureResource.cpp")
TARGET_LINK_LIBRARIES(meshResources core render)
ADD_DEPENDENCIES(meshResources core render)