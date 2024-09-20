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

SET(files_textures
	textures/textureResource.h
	textures/textureResource.cpp
	textures/stb_image.h
	textures/stb_image.cpp
	)

SOURCE_GROUP("texture" FILES ${files_textures})

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

SET(files_render
	${files_render_display}
	${files_textures}
	${files_shaders}
	${files_camera}
	${files_meshes}
	${files_graphics}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch} )
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw math)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw imgui math ${OPENGL_LIBS})