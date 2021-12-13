#include "EnemyFactory.h"


#include "../Entities/SolidObjectManager.h"


//Weapons used by enemies
#include "../WeaponInfo/Pistol.h"

#include "Player3D.h"


using namespace std;
// @param If player does not input values for amt & movementspeed, the default value will be 1 & 2 respectively
CEnemy3D* EnemyFactory::SpawnEnemy(int amt, float movementSpeed)
{
	//Enemy
	//Randomises the enemy's position
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 prevPos = glm::vec3(0,0,0); // ensure enemy does not spawn on the same place
	cout << amt << endl;
	for (int i = 0; i < amt; i++)
	{
		
		pos.y = 0;
		srand(time(NULL));
		pos.x = rand() % 31;
		pos.z = rand() % 31;
		if (prevPos.x == pos.x || pos.x == CPlayer3D::GetInstance()->GetPosition().x)
		{
			cout << "Same Coords!!!!" << endl;
			pos.x += 5;
			if (prevPos.z == pos.z || pos.z == CPlayer3D::GetInstance()->GetPosition().z)
			{
				pos.z += 5;
			}
		}
		prevPos = pos;
		
		
		int hp = rand() % 51 + 50;

		cout << "Enemy Spawned" << endl;
		float fCheckHeight = CTerrain::GetInstance()->GetHeight(pos.x, pos.z);
		CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(pos.x, fCheckHeight, pos.z));
		cEnemy3D->SetShader("Shader3D");
		cEnemy3D->Init("model.obj");
		cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		cEnemy3D->SetMovementSpeed(movementSpeed);
		cEnemy3D->SetHP(hp);
		//Assign pistol to cEnemy3D
		CPistol* cEnemyPistol = new CPistol();
		cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
		//Initialise the instance
		cEnemyPistol->Init();
		cEnemyPistol->SetShader("Shader3D_Model");
		cEnemyPistol->SetMagRound(1); // Setting the amount of maground enemy has in his gun
		cEnemy3D->SetWeapon(0, cEnemyPistol);
		//cout << "Enemy " << i << "'s Health: " << cEnemy3D->GetHP() << endl;
		CSolidObjectManager::GetInstance()->Add(cEnemy3D);
	}
	return NULL;
}


