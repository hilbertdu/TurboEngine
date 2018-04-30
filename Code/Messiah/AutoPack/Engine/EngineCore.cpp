// EngineCore.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "EngineCore.h"
#include "../Event/Events.h"
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Reflection/Objects/ObjectPool.h"
#include "Foundation/FileIO/FileStream.h"

// Extern
//------------------------------------------------------------------------------
extern void BindReflectionAll();

// Constructor
//------------------------------------------------------------------------------
EngineCore::EngineCore()
{
	ObjectPool::Initialize();
	TReflection::Initialization();
	BindReflectionAll();	
}

// Destructor
//------------------------------------------------------------------------------
EngineCore::~EngineCore()
{
	Save();
}

// Init
//------------------------------------------------------------------------------
void EngineCore::Init()
{
	m_AssetManager		= TNEW(AssetManager);
	m_ServiceManager	= TNEW(ServiceManager);
	m_EventSystem		= TNEW(EventSystem);
	m_TaskScheduler		= TNEW(TaskScheduler);
	m_UISystem			= TNEW(UISystem);
	m_Setting			= TNEW(Setting);

	RegisterAllEvents();

	m_AssetManager->SetRoot("I:\\Projects\\H42");
	m_AssetManager->RefreshRoot();
	m_TaskScheduler->InitWorker(1);
	m_UISystem->InitGUI();

	Load();
}

// Initialize
//------------------------------------------------------------------------------
/*static*/ void EngineCore::Initialize()
{
	Singleton<EngineCore>::Initialize();
	EngineCore::Instance().Init();
}

// Finalize
//------------------------------------------------------------------------------
/*static*/ void EngineCore::Finalize()
{
	Singleton<EngineCore>::Finalize();
}

// SetRootDir
//------------------------------------------------------------------------------
void EngineCore::SetRootDir(const AStringView & root)
{
	m_RootDir = root.Get();
}

// GetRootDir
//------------------------------------------------------------------------------
AStringView	EngineCore::GetRootDir() const
{
	return m_RootDir;
}

// Save
//------------------------------------------------------------------------------
bool EngineCore::Save()
{
	// save all ui widgets
	MemWStream uiStream;
	m_UISystem->Save(&uiStream);
	LINFO(__FUNCTION__, "UISystem: \n%s\n", uiStream.GetData());
	FileStream stream;
	stream.Open("layout.tcfg", FileStream::WRITE_ONLY);
	stream.WriteBuffer(uiStream.GetData(), uiStream.GetSize());
	stream.Close();

	// save all services
	return true;
}

// Load
//------------------------------------------------------------------------------
bool EngineCore::Load()
{
	// load all ui widgets
	FileStream stream;
	if (stream.Open("layout.tcfg", FileStream::READ_ONLY))
	{
		SIZET size = stream.GetSize();
		char * buffer = new char[size];
		stream.ReadBuffer(buffer, size);
		stream.Close();
		MemRStream uiStream(buffer, size);
		m_UISystem->Load(&uiStream);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------