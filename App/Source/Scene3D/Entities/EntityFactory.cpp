#include "EntityFactory.h"
#include "../Entities/SolidObjectManager.h"
#include "../Terrain/Terrain.h"

CHealth* EntityFactory::SpawnHealth(glm::vec3 position)
{
	float fCheckHeight = CTerrain::GetInstance()->GetHeight(position.x, position.z);
	CHealth* cHealth3D = new CHealth(glm::vec3(position.x, fCheckHeight, position.z));
	cHealth3D->SetShader("Shader3D");
	cHealth3D->Init();
	cHealth3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	CSolidObjectManager::GetInstance()->Add(cHealth3D);
	return cHealth3D;
}
