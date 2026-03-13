
#include "config.h"
#include "render/graphics/gBuffer.h"
#include <iostream>

GBuffer::GBuffer() {}
GBuffer::~GBuffer()
{
    destroy();
}

bool GBuffer::initialize(unsigned int width, unsigned int height)
{
    destroy();
    this->width = width;
    this->height = height;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// create posision texture
    glGenTextures(1, &positionTex);
    glBindTexture(GL_TEXTURE_2D, positionTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTex, 0);

    // Normal texture
    glGenTextures(1, &normalTex);
    glBindTexture(GL_TEXTURE_2D, normalTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTex, 0);

    // Albedo texture
    glGenTextures(1, &albedoTex);
    glBindTexture(GL_TEXTURE_2D, albedoTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoTex, 0);

    // Specular texture
    glGenTextures(1, &specularTex);
    glBindTexture(GL_TEXTURE_2D, specularTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, specularTex, 0);


	GLenum attachments[4] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3
	};

	glDrawBuffers(4, attachments);

	// create and attach depth texture
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "GBuffer framebuffer is not complete!" << std::endl;
        //unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}

    //unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}


void GBuffer::bindForWriting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void GBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GBuffer::getPositionTexture() const
{ 
    return positionTex;
}

GLuint GBuffer::getNormalTexture() const
{ 
    return normalTex;
}

GLuint GBuffer::getAlbedoTexture() const 
{ 
    return albedoTex;
}

GLuint GBuffer::getSpecularTexture() const 
{ 
    return specularTex;
}

GLuint GBuffer::getDepthTexture() const 
{ 
    return depthTex;
}

GLuint GBuffer::getFramebuffer() const
{
    return fbo;
}

void GBuffer::resize(unsigned int width, unsigned int height)
{
    initialize(width, height);
}

void GBuffer::bindTexturesForReading() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, positionTex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTex);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, albedoTex);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, specularTex);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depthTex);
}

void GBuffer::destroy()
{
    if (positionTex)
    {
        glDeleteTextures(1, &positionTex);
    }
    if (normalTex)
    {
        glDeleteTextures(1, &normalTex);
    }
    if (albedoTex)
    {
        glDeleteTextures(1, &albedoTex);
    }
    if (specularTex)
    {
        glDeleteTextures(1, &specularTex);
    }
    if (depthTex)
    {
        glDeleteTextures(1, &depthTex);
    }
    if (fbo)
    {
        glDeleteFramebuffers(1, &fbo);
    }

    positionTex = 0;
    normalTex = 0;
    albedoTex = 0;
    specularTex = 0;
    depthTex = 0;
    fbo = 0;
}
