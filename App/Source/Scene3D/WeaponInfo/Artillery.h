/**
 CArtillery
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CArtillery :	public CWeaponInfo
{
public:
	// Default Constructor
	CArtillery(void);
	// Destructor
	virtual ~CArtillery(void);

	// Initialise this instance to default values
	bool Init(void);
};

