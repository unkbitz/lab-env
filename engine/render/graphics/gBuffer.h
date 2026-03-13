#pragma once
#include <GL/glew.h>

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	bool initialize(unsigned int width, unsigned int height);
	void bindForWriting();
	void unbind();
	void resize(unsigned int width, unsigned int height);
	void bindTexturesForReading() const;

    GLuint getFramebuffer() const;
    GLuint getPositionTexture() const;
    GLuint getNormalTexture() const;
    GLuint getAlbedoTexture() const;
    GLuint getSpecularTexture() const;
    GLuint getDepthTexture() const;

private:
    void destroy();

    GLuint fbo = 0;

    GLuint positionTex = 0;
    GLuint normalTex = 0;
    GLuint albedoTex = 0;
    GLuint specularTex = 0;
    GLuint depthTex = 0;

    unsigned int width = 0;
    unsigned int height = 0;
};