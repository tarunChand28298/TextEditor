#pragma once

#include <wx/wx.h>

#include "..\Main\cMain.h"

class cMainApplication : public wxApp
{
public:
	bool OnInit();

private:
	cMainWindow* m_mainFrame = nullptr;
};
