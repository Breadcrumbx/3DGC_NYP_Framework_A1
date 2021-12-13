#include "Attributes.h"

int Attributes::GetHP()
{
    return this->hp;
}

int Attributes::GetDMG()
{
    return this->dmg;
}

float Attributes::GetMovementSpeed()
{
    return movementSpeed;
}



void Attributes::SetHP(int hp)
{
    this->hp = hp;
}

void Attributes::SetDmg(int dmg)
{
    this->dmg = dmg;
}

void Attributes::SetMovementSpeed(int speed)
{
    movementSpeed = speed;
}
