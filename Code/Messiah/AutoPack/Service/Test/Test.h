// Test.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_SERVICE_TEST_H
#define MESSIAH_AUTOPACK_SERVICE_TEST_H

// Includes
//------------------------------------------------------------------------------
#include "../Service.h"


class Test : public IService
{
public:
	Test();
	~Test();

	virtual void Initialize();
	virtual void Finalize();
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SERVICE_TEST_H