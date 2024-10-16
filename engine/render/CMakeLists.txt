#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_display
	window.h
	window.cc
	grid.h
	grid.cc
	)
SOURCE_GROUP("display" FILES ${files_render_display})

SET(files_gltf
	gltf.h
	)
SOURCE_GROUP("gltf" FILES ${files_gltf})

SET(files_nlohmann
	json.hpp
	)
SOURCE_GROUP("nlohmann" FILES ${files_nlohmann})

SET(files_textures
	textures/textureResource.h
	textures/textureResource.cpp
	textures/stb_image.h
	textures/stb_image.cpp
	)
SOURCE_GROUP("texture" FILES ${files_textures})

SET(files_material
	material/blinnPhongMaterial.cpp
	material/blinnPhongMaterial.h
	material/material.h
	)
SOURCE_GROUP("material" FILES ${files_material})

SET(files_shaders
	shaders/shaderResource.h
	shaders/shaderResource.cpp
	)
SOURCE_GROUP("shaders" FILES ${files_shaders})

SET(files_camera
	camera/camera.cpp
	camera/camera.h
	)
SOURCE_GROUP("camera" FILES ${files_camera})

SET(files_meshes
	meshes/meshResource.h
	meshes/meshResource.cpp
	meshes/meshTransform.h
	meshes/meshTransform.cpp
	)
SOURCE_GROUP("meshes" FILES ${files_meshes})

SET(files_graphics
	graphics/graphics.cpp
	graphics/graphics.h
	)
SOURCE_GROUP("graphics" FILES ${files_graphics})

SET(files_lighting
	lighting/lighting.h
	lighting/lighting.cpp
	)
SOURCE_GROUP("lighting" FILES ${files_lighting})

SET(files_render
	${files_render_display}
	${files_textures}
	${files_gltf}
	${files_nlohmann}
	${files_shaders}
	${files_camera}
	${files_lighting}
	${files_meshes}
	${files_graphics}
	${files_material}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch} )
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw math)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw imgui math ${OPENGL_LIBS})