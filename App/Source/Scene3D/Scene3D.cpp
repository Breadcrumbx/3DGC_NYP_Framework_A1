/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene3D.h"

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include filesystem to read from a file
#include "System\filesystem.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"
//Weapons
#include "WeaponInfo/Pistol.h"
#include "WeaponInfo/M4A1.h"

//Camera effects
#include "CameraEffects/CameraShake.h"

//Enemy factory which spawns the enemies
#include "Entities/EnemyFactory.h"

#include "../GameStateManagement/GameStateManager.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cGUI_Scene3D(NULL)
	, cSoundController(NULL)
	, cSolidObjectManager(NULL)
	, cPlayer3D(NULL)
	, cCamera(NULL)
	, cSkyBox(NULL)
	, cTerrain(NULL)
	, cProjectileManager(NULL)
	, timer (60.f)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}




	// Destroy the cSoundController
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	//Destroy the cSkyBox
	if (cSkyBox)
	{
		cSkyBox->Destroy();
		cSkyBox = NULL;
	}

	//Destroy the cTerrain

	if (cTerrain)
	{
		cTerrain->Destroy();
		cTerrain = NULL;
	}

	//Destroy the cPlayer3D
	if (cPlayer3D)
	{
		cSolidObjectManager->Erase(cPlayer3D);
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}
	//Destroy the cSolidObjectManager
	if (cSolidObjectManager)
	{
		cSolidObjectManager->Destroy();
		cSolidObjectManager = NULL;
	}
	//Destroy the cProjectileManager
	if (cProjectileManager)
	{
		cProjectileManager->Destroy();
		cProjectileManager = NULL;
	}

	//Destroy the CGUI 3D
	if (cGUI_Scene3D)
	{
		cGUI_Scene3D->Destroy();
		cGUI_Scene3D = NULL;
	}

	
	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	
	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	
	cSettings = CSettings::GetInstance();
	float fCheckHeight;
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	
	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);
	//Load Environment Entities
	//Load the SkyBox
	cSkyBox = CSkyBox::GetInstance();
	//Set a shader to this class instance of CSkyBox
	cSkyBox->SetShader("Shader3D_SkyBox");
	cSkyBox->Init();

	//Load the Ground
	cTerrain = CTerrain::GetInstance();
	cTerrain->SetShader("Shader3D_Terrain");
	cTerrain->Init();
	//Set the size of the terrain
	cTerrain->SetRenderSize(100.0f, 5.0f, 100.0f);

	//Load the movable entities
	//Initialise the cSolidObjectManager
	cSolidObjectManager = CSolidObjectManager::GetInstance();
	cSolidObjectManager->Init();

	//Initiliase the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	cPlayer3D->SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	cPlayer3D->SetShader("Shader3D");
	cPlayer3D->Init();
	cPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	cPlayer3D->AttachCamera(cCamera);
	
	

	//Assign a cPistol to the cPlayer3D
	CPistol* cPistol = new CPistol();
	
	//Set the position, rotation and scale of this weapon
	cPistol->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cPistol->SetScale(glm::vec3(.75f, .75f, .75f));
	//Initiliase the instance
	cPistol->Init();
	cPistol->SetShader("Shader3D_Model");
	cPlayer3D->SetWeapon(0, cPistol);

	CM4A1* cM4A1 = new CM4A1();
	//Set the position, rotation and scale of this weapon
	cM4A1->SetPosition(glm::vec3(0.75f, -0.975f, -1.0f));
	cM4A1->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cM4A1->SetScale(glm::vec3(.5f, .5f, .5f));
	//Initiliase the instance
	cM4A1->Init();
	cM4A1->SetShader("Shader3D_Model");
	cPlayer3D->SetWeapon(1, cM4A1);


	//Add the cPlayer3D to the cSolidObjectManager
	cSolidObjectManager->Add(cPlayer3D);


	




	// Initiliase the cProjectileManager
	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");

	

	//Structure
	
	/*fCheckHeight = cTerrain->GetHeight(2.0f, -2.0f);
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(2.0f, fCheckHeight, -2.0f));
	cStructure3D->SetShader("Shader3D");
	cStructure3D->Init();
	cStructure3D->InitCollider("Shader3D_Line",glm::vec4(0.0f,0.0f,1.0f,1.0f));

	cSolidObjectManager->Add(cStructure3D);*/

	//Health
	

	////Enemy
	//fCheckHeight = cTerrain->GetHeight(0.0f, -10.f);
	//CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(0.0f,fCheckHeight,10.f));
	//cEnemy3D->SetShader("Shader3D");
	//cEnemy3D->Init();
	//cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//
	////Assign pistol to cEnemy3D
	//CPistol* cEnemyPistol = new CPistol();
	//cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	////Initialise the instance
	//cEnemyPistol->Init();
	//cEnemyPistol->SetShader("Shader3D_Model");
	//cEnemyPistol->SetMagRound(1); // Setting the amount of maground enemy has in his gun
	//cEnemy3D->SetWeapon(0, cEnemyPistol);

	////Add the cEnemy3D to the cSolidObjectManager
	//cSolidObjectManager->Add(cEnemy3D);

	//Spawns enemy
	EnemyFactory::SpawnEnemy(5);

	



	//Load the GUI Entities
	// Store the cGUI_Scene3D singleton instance here
	cGUI_Scene3D = CGUI_Scene3D::GetInstance();
	cGUI_Scene3D->Init();


	

	
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	//Load the minimap
	
	// Set a shader to this class of instance of minimap



	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Pistol.wav"), 4, true,false,true);

	
	spawntimer = 25.f;
	isPaused = false;

	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	//if (cPlayer3D->GetHP() <= 0)
	//{
	//	CGameStateManager::GetInstance()->SetActiveGameState("LoseState");
	//}
	//Pause
	timer -= dElapsedTime;
	spawntimer -= dElapsedTime;
	if (spawntimer <= 0.f)
	{
		EnemyFactory::SpawnEnemy(2);
		spawntimer = 25.f;
	}

	if (timer <= 0)
	{
		timer = 0;
		cGUI_Scene3D->SetWin(true);
	}

	if (cKeyboardController->IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		isPaused = !isPaused;
		if (!isPaused)
			glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
		//cSettings->bShowMousePointer = !cSettings->bShowMousePointer;
	}

	if (isPaused)
	{
		cGUI_Scene3D->updatePause(dElapsedTime);
		return true;
	}


	//Store the current position, if rollback is needed.
	cPlayer3D->StorePositionForRollback();
	//Update the projectiles
	cProjectileManager->Update(dElapsedTime);
	/*cout << boolalpha;
	cout << CCameraEffectsManager::GetInstance()->Get("CameraShake")->GetStatus() << endl;*/
	//cout << cPlayer3D->GetMovementSpeed() << endl;
	//cout << cPlayer3D->GetWeapon()->getReloadTime() << endl;
	//Call the cGUI Scene3D's update method
	//cGUI_Scene3D->Update(dElapsedTime);
	
	//Get keyboard updates for player3D
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}

	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_CONTROL))
	{
		//cout << cPlayer3D->GetMovementSpeed() << endl;
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::CROUCH, (float)dElapsedTime);
	}
	else
	{
		cCamera->vec3Offset = glm::vec3(0, 0, 0);
	}
	/*else if(!CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_CONTROL))
	{
		cCamera->vec3Offset = glm::vec3(0, 0, 0);
		cPlayer3D->SetMovementSpeed(2.5f);
	}*/
	//Sprinting code
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		//cPlayer3D->SetMovementSpeed(5.f);
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::SPRINTING, (float)dElapsedTime);
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
	{
		cPlayer3D->SetToJump();
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
		{
			cPlayer3D->AttachCamera();
		}
		else
		{
			cPlayer3D->AttachCamera(cCamera);
		}

		//Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}
	//Camera shake
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_8))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CameraShake")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(!bStatus);

		//Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_8);
	}
	//Crosshair
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CrossHair")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(!bStatus);

		//Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_9);
	}

	

	//Switching of weapons
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
	{
		cPlayer3D->SetCurrentWeapon(0);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
	{
		cPlayer3D->SetCurrentWeapon(1);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R))
	{
		cPlayer3D->GetWeapon()->Reload();
	}



	//Get mouse button updates

	if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::LMB) && nullptr != dynamic_cast<CPistol*>(cPlayer3D->GetWeapon()))
	{
		std::cout << "FIRE" << std::endl;
		cPlayer3D->DischargeWeapon();
		
	}
	else if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && nullptr != dynamic_cast<CM4A1*>(cPlayer3D->GetWeapon()))
	{
		std::cout << "M4 FIRE" << std::endl;
		cPlayer3D->DischargeWeapon();
	}
	//Scoping
	if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::RMB))
	{
		//Switch on scope mode and zoom in
		cCamera->fZoom = 1.0f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
	}
	else if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::RMB))
	{
		// switch off scope mode and zoom out
		cCamera->fZoom = 45.f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
	}


	//Get keyboard and mouse updates for camera
	if (!cPlayer3D->IsCameraAttached())
	{


		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);

		// Get mouse updates
		cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}
	else
	{
		//Get mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	//Bullet whizzing code

	



	//for (int i = 0; i < cProjectileManager->vProjectile.size(); ++i)
	//{
	//	if (cProjectileManager->vProjectile[i]->GetSource() != cPlayer3D)
	//	{
	//		if (glm::distance(cProjectileManager->vProjectile[i]->GetPosition(), cPlayer3D->GetPosition()) < 5.f)
	//		{
	//			glm::vec3 Bullet; //Bullet Distance 
	//			glm::vec3 FromBulletToPlayer;// Bullet to Player vector
	//			FromBulletToPlayer = glm::normalize(cPlayer3D->GetPosition() - cProjectileManager->vProjectile[i]->GetPosition()); // b vector
	//			
	//		}
	//	}
	//	else
	//	{
	//		continue; // skipping if the projectile is from the player else it will run the above.
	//	}
	//}


	//Update the solid objects
	cSolidObjectManager->Update(dElapsedTime);

	//Check for collisions among Entities and also with Projectiles
	cSolidObjectManager->CheckForCollision();

	

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	//Update cGUI
	cGUI_Scene3D->Update(dElapsedTime);
	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;

	cCamera->fYaw += 180.0f;
	cCamera->fPitch = -90.0f;
	cCamera->vec3Position = glm::vec3(storePlayerPosition.x, 10.f, storePlayerPosition.z);

	cCamera->ProcessMouseMovement(0, 0, false);

	//Generate the view and projection
	glm::mat4 playerView = cCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(glm::radians(45.0f),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);

	//Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true);

	CMinimap::GetInstance()->Activate();
	CMinimap::GetInstance()->PreRender();


	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	
							 // Part 2: Render the entire scene as per normal
	
	
	
	//glEnable(GL_DEPTH_TEST);

	//Render the terrain
	cTerrain->SetView(playerView);
	cTerrain->SetProjection(playerProjection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	

	//Render the solid objects
	cSolidObjectManager->SetView(playerView);
	cSolidObjectManager->SetProjection(playerProjection);
	cSolidObjectManager->Render();


	CMinimap::GetInstance()->Deactivate();

	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(glm::radians(CCamera::GetInstance()->fZoom),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Render the skybox
	cSkyBox->SetView(view);
	cSkyBox->SetProjection(projection);
	cSkyBox->PreRender();
	cSkyBox->Render();
	cSkyBox->PostRender();

	

	//Render the terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	//Render the solid objects
	cSolidObjectManager->SetView(view);
	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->Render();


	

	// Render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();

	

	

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test

	//Render player's weapon
	cPlayer3D->GetWeapon()->SetProjection(projection);
	cPlayer3D->GetWeapon()->PreRender();
	cPlayer3D->GetWeapon()->Render();
	cPlayer3D->GetWeapon()->PostRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	//Rendering cGUI
	cGUI_Scene3D->SetProjection(projection);
	//Call the cGUI_Scene3D's PreRender()
	cGUI_Scene3D->PreRender();
	cGUI_Scene3D->Render();
	cGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}

void CScene3D::setPause(bool p)
{
	isPaused = p;
}

float CScene3D::GetTimer()
{
	return timer;
}
