// Chef.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_SERVICE_CHEF_H
#define MESSIAH_AUTOPACK_SERVICE_CHEF_H

// Includes
//------------------------------------------------------------------------------
#include "../Service.h"


class Chef : public IService
{
public:
	Chef();
	~Chef();

	virtual void Initialize();
	virtual void Finalize();
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SERVICE_CHEF_H