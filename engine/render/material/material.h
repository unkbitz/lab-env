#pragma once
#include "config.h"

class Material {
public:
	virtual ~Material() {}

	virtual void Apply() const = 0;
};
