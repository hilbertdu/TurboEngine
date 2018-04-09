// Arribute.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_ATTRIBUTES_H
#define FOUNDATION_REFLECTION_ATTRIBUTES_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Attributes/Attribute.h"


namespace TReflection
{
	// Class AttrSerializer
	//------------------------------------------------------------------------------
	class AttrSerializer : public IAttribute
	{
	public:
		static constexpr uint16 AttrType = E_ATTR_SERIALIZER;

		explicit AttrSerializer() : m_Enable(false) {}
		explicit AttrSerializer(bool enable): m_Enable(enable) {}
		virtual ~AttrSerializer() {}

		bool IsEnable() const { return m_Enable; }

	private:
		bool m_Enable;
	};

	// Class AttrRanger
	//------------------------------------------------------------------------------
	class AttrRanger : public IAttribute
	{
	public:
		static constexpr uint16 AttrType = E_ATTR_RANGER;

		explicit AttrRanger() : m_Min(0), m_Max(1) {}
		explicit AttrRanger(int32 min, int32 max) : m_Min(min), m_Max(max) {}
		virtual ~AttrRanger() {}

	private:
		int32 m_Min;
		int32 m_Max;
	};
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_ATTRIBUTES_H