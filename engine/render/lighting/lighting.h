#pragma once;
#include "config.h"
#include "math/vec3.h"
#include "render/shaders/shaderResource.h"

class Lighting {
private:
	vec3 pointLightPos;
	vec3 color;
	float intensity;
public:
	Lighting();
	~Lighting();

	vec3 getPosition() const;
	vec3 getColor() const;
	float getIntesity() const;

	void setPosition(const vec3& pos);
	void setColor(const vec3& col);
	void setIntensity(const float& intens);
};