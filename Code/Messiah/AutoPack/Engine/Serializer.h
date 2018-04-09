// Serializer.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_ENGINE_ISERIALIZER_H
#define MESSIAH_AUTOPACK_ENGINE_ISERIALIZER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"

// Class ISerializer
//------------------------------------------------------------------------------
class ISaveLoad
{
public:
	bool Save(IOStream* stream) const;
	bool Load(const IOStream* stream);	

	inline bool IsLoaded() const { return m_Loaded; }
	inline void SetLoaded() { m_Loaded = true; }

private:
	virtual bool SaveText(IOStream* stream) const = 0;
	virtual bool LoadText(const IOStream* stream) = 0;

	bool m_Loaded = false;
};

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_ENGINE_ISERIALIZER_H
