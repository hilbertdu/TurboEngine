// UIDocker.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_UIDOCKER_H
#define MESSIAH_AUTOPACK_UIDOCKER_H

// Includes
//------------------------------------------------------------------------------
#include "imgui/imgui.h"
#include "imgui/imgui_dock.h"
#include "Foundation/Reflection/Objects/Object.h"

// class UIDocker
//------------------------------------------------------------------------------
struct ImGUIDocker : public IStruct
{
public:
	ImGUIDocker() {}
	ImGUIDocker(uint32 idx, const ImGui::IMDocker & docker)
		: m_Idx(idx)
		, m_Label(docker.label)
		, m_NextTab(docker.next_tab)
		, m_PrevTab(docker.prev_tab)
		, m_Parent(docker.parent)
		, m_Status(docker.status)
		, m_Location(docker.location)
		, m_Active(docker.active)
	{
		m_Children.x = (float)docker.children[0];
		m_Children.y = (float)docker.children[1];
		m_Pos.x = (float)docker.pos.x;
		m_Pos.y = (float)docker.pos.y;
		m_Size.x = (float)docker.size.x;
		m_Size.y = (float)docker.size.y;
	}

	ImGUIDocker & operator=(const ImGui::IMDocker & docker)
	{
		m_Label = docker.label;
		m_NextTab = docker.next_tab;
		m_PrevTab = docker.prev_tab;
		m_Parent = docker.parent;
		m_Status = docker.status;
		m_Location = docker.location;
		m_Active = docker.active;

		m_Children.x = (float)docker.children[0];
		m_Children.y = (float)docker.children[1];
		m_Pos.x = docker.pos.x;
		m_Pos.y = docker.pos.y;
		m_Size.x = docker.size.x;
		m_Size.y = docker.size.y;
		return *this;
	}

	void GetIMDocker(ImGui::IMDocker & docker)
	{
		docker.SetLabel(m_Label.Get());
		docker.active = m_Active;
		docker.next_tab = m_NextTab;
		docker.prev_tab = m_PrevTab;
		docker.parent = m_Parent;
		docker.status = m_Status;
		MemCopy(docker.location, m_Location.Get(), 16);

		docker.children[0] = (int)m_Children.x;
		docker.children[1] = (int)m_Children.y;
		docker.pos.x = m_Pos.x;
		docker.pos.y = m_Pos.y;
		docker.size.x = m_Size.x;
		docker.size.y = m_Size.y;
	}

	uint32	m_Idx;
	AString m_Label;
	int32	m_NextTab;
	int32	m_PrevTab;
	Vector2	m_Children;
	int32	m_Parent;	
	Vector2	m_Pos;
	Vector2	m_Size;
	uint32	m_Status;
	AString m_Location;
	bool	m_Active;

	TREFLECTION_DECLARE(ImGUIDocker, IStruct)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_UIDOCKER_H