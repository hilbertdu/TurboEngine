// FileProperty.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/FileProperty.h"
#include "AutoPack/Engine/EngineCore.h"


// Constructor
//------------------------------------------------------------------------------
UIFileProperty::UIFileProperty()
{
	m_Name = "Property";
	m_Parent = "Assets";
}

// Destructor
//------------------------------------------------------------------------------
UIFileProperty::~UIFileProperty()
{}

/*virtual*/ void UIFileProperty::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get()))
	{
	}
	ImGui::EndDock();
}

//------------------------------------------------------------------------------