// ReflectionDecl.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_DECL_H
#define FOUNDATION_REFLECTION_DECL_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/ReflectionMacros.h"
#include "Foundation/Objects/Object.h"

// IObject
//------------------------------------------------------------------------------
TREFLECT_BEGIN(IObject)
	TREFLECT_FIELD(m_Flag,	"Flag")
TREFLECT_END(IObject)

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_DECL_H