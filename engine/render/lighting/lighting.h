#pragma once
#include "config.h"
#include "math/vec3.h"
#include "render/shaders/shaderResource.h"

class Lighting {
public:
	Lighting();
	~Lighting();

	vec3 getPointLightPos() const;
	vec3 getPointLightColor() const;
	float getPointLightIntensity() const;
	vec3 getDirectionalLightDir() const;
	vec3 getDirectionalLightColor() const;
	float getDirectionalLightIntensity() const;

	void setPosition(const vec3& pos);
	void setColor(const vec3& col);
	void setIntensity(const float& intens);
	void setDirektionalLightDir(const vec3& dir);
	void setDirektionalLightColor(const vec3& col);
	void setDirektionalLightIntensity(const float& intens);
private:
	vec3 pointLightPos;
	vec3 pointLightColor;
	float pointLightIntensity;

	vec3 directionalLightDir;
	vec3 directionalLightColor;
	float directionalLightIntensity;
};