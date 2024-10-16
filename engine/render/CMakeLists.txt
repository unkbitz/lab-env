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
	nlohmann/adl_serializer.hpp
	nlohmann/byte_container_with_subtype.hpp
	nlohmann/json.hpp
	nlohmann/json_fwd.hpp
	nlohmann/ordered_map.hpp

	nlohmann/detail/abi_macros.hpp
	nlohmann/detail/exceptions.hpp
	nlohmann/detail/hash.hpp
	nlohmann/detail/json_custom_base_class.hpp
	nlohmann/detail/json_pointer.hpp
	nlohmann/detail/json_ref.hpp
	nlohmann/detail/macro_scope.hpp
	nlohmann/detail/macro_unscope.hpp
	nlohmann/detail/string_concat.hpp
	nlohmann/detail/string_escape.hpp
	nlohmann/detail/value_t.hpp

	nlohmann/detail/conversions/from_json.hpp
	nlohmann/detail/conversions/to_chars.hpp
	nlohmann/detail/conversions/to_json.hpp

	nlohmann/detail/input/binary_reader.hpp
	nlohmann/detail/input/input_adapters.hpp
	nlohmann/detail/input/json_sax.hpp
	nlohmann/detail/input/lexer.hpp
	nlohmann/detail/input/parser.hpp
	nlohmann/detail/input/position_t.hpp

	nlohmann/detail/iterators/internal_iterator.hpp
	nlohmann/detail/iterators/iter_impl.hpp
	nlohmann/detail/iterators/iteration_proxy.hpp
	nlohmann/detail/iterators/iterator_traits.hpp
	nlohmann/detail/iterators/json_reverse_iterator.hpp
	nlohmann/detail/iterators/primitive_iterator.hpp

	nlohmann/detail/meta/cpp_future.hpp
	nlohmann/detail/meta/detected.hpp
	nlohmann/detail/meta/identity_tag.hpp
	nlohmann/detail/meta/is_sax.hpp
	nlohmann/detail/meta/std_fs.hpp
	nlohmann/detail/meta/type_traits.hpp
	nlohmann/detail/meta/void_t.hpp
	nlohmann/detail/meta/call_std/begin.hpp
	nlohmann/detail/meta/call_std/end.hpp

	nlohmann/detail/output/binary_writer.hpp
	nlohmann/detail/output/output_adapters.hpp
	nlohmann/detail/output/serializer.hpp

	nlohmann/thirdparty/hedley/hedley.hpp
	nlohmann/thirdparty/hedley/hedley_undef.hpp)
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