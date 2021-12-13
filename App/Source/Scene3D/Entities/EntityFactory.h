#pragma once
#include "Health.h"

#include <iostream>
// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class EntityFactory
{
public:
	static CHealth* SpawnHealth(glm::vec3 position);
};

