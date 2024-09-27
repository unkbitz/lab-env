#--------------------------------------------------------------------------
# math
#--------------------------------------------------------------------------

SET(files_math
	vec2.h
	vec3.h
	vec4.h
	mat4.h)
SOURCE_GROUP("math" FILES ${files_math})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(math STATIC ${files_math} ${files_pch})
TARGET_PCH(math ../)
ADD_DEPENDENCIES(math glew)
TARGET_LINK_LIBRARIES(math PUBLIC engine exts glew)
