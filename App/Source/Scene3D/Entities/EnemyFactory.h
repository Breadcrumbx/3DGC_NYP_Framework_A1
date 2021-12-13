#pragma once
#include <iostream>



// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

//Include Enemy
#include "Enemy3D.h"


class EnemyFactory
{
private:
	static std::vector<CEnemy3D*> Enemies;
public:
	static CEnemy3D* SpawnEnemy(int amt = 1, float movementSpeed = 2.5f); // Spawns the enemy

};

