// Serializer.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Serializer.h"

// SaveText
//------------------------------------------------------------------------------
bool ISaveLoad::Save(IOStream* stream) const
{
	return SaveText(stream);
}

// LoadText
//------------------------------------------------------------------------------
bool ISaveLoad::Load(const IOStream* stream)
{
	bool result = LoadText(stream);
	if (result) m_Loaded = true;
	return result;
}

//------------------------------------------------------------------------------