#pragma once
#include <iostream>
using namespace std;

class Attributes
{
private:
	int hp;
	int dmg;
	float movementSpeed;
public:
	int GetHP();
	int GetDMG();
	float GetMovementSpeed();


	void SetHP(int hp);
	void SetDmg(int dmg);
	void SetMovementSpeed(int speed);

};

