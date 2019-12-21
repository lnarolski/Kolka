/***************************************************************
 * Name:      GUIApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Łukasz Narolski ()
 * Created:   2015-10-24
 * Copyright: Łukasz Narolski ()
 * License:
 **************************************************************/

#include "GUIApp.h"

//(*AppHeaders
#include "GUIMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(GUIApp);

bool GUIApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	GUIFrame* Frame = new GUIFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
