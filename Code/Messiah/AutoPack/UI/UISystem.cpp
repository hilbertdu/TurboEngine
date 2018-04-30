// UISYSTEM.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "UISystem.h"
#include "../UI/Basic/UIWidget.h"
#include "../UI/Compat/UIDocker.h"
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Reflection/Serialization/SerializerText.h"
#include "Foundation/FileIO/MemWStream.h"
#include "Foundation/FileIO/MemRStream.h"


UIWidget * UISystem::CreateWidget(AStringView name)
{
	if (HasWidget(name)) return GetWidget(name);
	IObject * wNew = ObjectPool::Instance().CreateObject(name.Get());
	m_Widgets[name.Get()] = static_cast<UIWidget*>(wNew);
	return static_cast<UIWidget*>(wNew);
}

void UISystem::DeleteWidget(AStringView name)
{
	ASSERT(m_Widgets.Find(name.Get()) != m_Widgets.End());
	TDELETE_SAFE(m_Widgets[name.Get()]);
	m_Widgets.Erase(name.Get());
}

UISystem::UISystem()	
{
	MetaTypeDB::Instance().CreateMetaType<WidgetMap>();
	MetaTypeDB::Instance().CreateMetaType<StackArray<ImGUIDocker, 64>>();
}

UISystem::~UISystem()
{
	for (WidgetMap::Iter iter = m_Widgets.Begin(); iter != m_Widgets.End(); ++iter)
	{
		DeleteWidget(iter->First());
	}
}

void UISystem::InitGUI()
{
	// Setup style
	//ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesChinese());

	CreateWidget("UIMainMenu");
	CreateWidget("UIMainToolBar");
	CreateWidget("UIAssetBrowser");
	CreateWidget("UIFileProperty");
	CreateWidget("UICommands");
	CreateWidget("UIOutputPanel");
}

void UISystem::OnFrameStart()
{
}

void UISystem::OnFrameEnd()
{
	for (WidgetMap::Iter iter = m_Widgets.Begin(); iter != m_Widgets.End(); ++iter)
	{
		iter->Second()->OnFrameUpdate();
	}
	ImGui::Render();
}

UIWidget * UISystem::GetWidget(AStringView name)
{
	ASSERT(m_Widgets.Find(name.Get()) != m_Widgets.End());
	return m_Widgets[name.Get()];
}

bool UISystem::HasWidget(AStringView name)
{
	return m_Widgets.Find(name.Get()) != m_Widgets.End();
}

bool UISystem::Undo()
{
	return false;
}

bool UISystem::Redo()
{
	return false;
}

/*virtual*/ bool UISystem::SaveText(IOStream* stream) const
{
	TReflection::TextSerializer writer;
	// save widgets count (for docker create)
	SaveDockers(stream, &writer);

	// save all widgets
	writer.Save(stream, &m_Widgets, MetaTypeDB::Instance().GetMetaType<WidgetMap>());
	return true;
}

/*virtual*/ bool UISystem::LoadText(const IOStream* stream)
{
	TReflection::TextSerializer reader;
	// load widgets count (for docker create)
	LoadDockers(stream, &reader);

	// load all widgets
	reader.Load(stream, &m_Widgets, MetaTypeDB::Instance().GetMetaType<WidgetMap>());
	return true;
}

bool UISystem::SaveDockers(IOStream* stream, ISerializer * writer) const
{
	uint32 size = ImGui::GetDockSize();
	StackArray<ImGUIDocker, 64> dockers(size);
	for (uint32 idx = 0; idx < size; ++idx)
	{
		dockers[idx].m_Idx = idx;
		ImGui::IMDocker docker;
		ImGui::FindDock(idx, docker);
		dockers[idx] = docker;
	}	
	writer->Save(stream, &dockers, MetaTypeDB::Instance().GetMetaType<StackArray<ImGUIDocker, 64>>());
	return true;
}

bool UISystem::LoadDockers(const IOStream* stream, ISerializer * reader)
{
	StackArray<ImGUIDocker, 64> dockers;
	reader->Load(stream, &dockers, MetaTypeDB::Instance().GetMetaType<StackArray<ImGUIDocker, 64>>());

	ImGui::LoadDock((int)dockers.GetSize());
	ImGui::IMDocker docker;
	for (uint32 idx = 0; idx < dockers.GetSize(); ++idx)
	{
		dockers[idx].GetIMDocker(docker);
		ImGui::LoadDock(idx, docker);
	}
	return true;
}

//------------------------------------------------------------------------------