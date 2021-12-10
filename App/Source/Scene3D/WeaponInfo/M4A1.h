/**
 CM4A1
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CM4A1 :	public CWeaponInfo
{
public:
	// Default Constructor
	CM4A1(void);
	// Destructor
	virtual ~CM4A1(void);

	// Initialise this instance to default values
	bool Init(void);
};

