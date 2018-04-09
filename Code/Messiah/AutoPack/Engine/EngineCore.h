// EngineCore.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_ENGINECORE_H
#define MESSIAH_AUTOPACK_ENGINECORE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Pattern/Singleton.h"
#include "Foundation/Container/SmartPtr.h"
#include "Foundation/Task/TaskScheduler.h"
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"
#include "../Manager/AssetManager.h"
#include "../Manager/ServiceManager.h"
#include "../Event/EventSystem.h"
#include "../UI/UISystem.h"
#include "../Setting/Setting.h"


// Class EngineCore
//------------------------------------------------------------------------------
class EngineCore : public Singleton<EngineCore>
{
public:
	EngineCore();
	~EngineCore();

	static void Initialize();
	static void Finalize();

	void		Init();
	void		SetRootDir(const AStringView & root);
	AStringView	GetRootDir() const;
	bool		Save();
	bool		Load();
		
	inline StrongPtr<EventSystem>	 GetEventSystem()		const { return m_EventSystem; }
	inline StrongPtr<UISystem>		 GetUISystem()			const { return m_UISystem; }
	inline StrongPtr<AssetManager>	 GetAssetManager()		const { return m_AssetManager; }
	inline StrongPtr<ServiceManager> GetServiceManager()	const { return m_ServiceManager; }
	inline StrongPtr<TaskScheduler>	 GetTaskScheduler()		const { return m_TaskScheduler; }	
	inline StrongPtr<Setting>		 GetSetting()			const { return m_Setting; }
	
private:
	StrongPtr<AssetManager>		m_AssetManager;
	StrongPtr<ServiceManager>	m_ServiceManager;
	StrongPtr<EventSystem>		m_EventSystem;
	StrongPtr<TaskScheduler>	m_TaskScheduler;
	StrongPtr<UISystem>			m_UISystem;
	StrongPtr<Setting>			m_Setting;

	AString m_RootDir;
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_ENGINECORE_H
