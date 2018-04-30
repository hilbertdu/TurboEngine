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
}

// Destructor
//------------------------------------------------------------------------------
UIFileProperty::~UIFileProperty()
{}

/*virtual*/ void UIFileProperty::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get(), &m_Opened))
	{
	}
	ImGui::EndDock();
}

//------------------------------------------------------------------------------