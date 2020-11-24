#include "cApp.h"
wxIMPLEMENT_APP(cMainApplication);

bool cMainApplication::OnInit()
{
    m_mainFrame = new cMainWindow();
    m_mainFrame->Show();

    return true;
}
