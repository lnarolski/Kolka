/***************************************************************
 * Name:      GUIApp.h
 * Purpose:   Defines Application Class
 * Author:    Łukasz Narolski ()
 * Created:   2015-10-24
 * Copyright: Łukasz Narolski ()
 * License:
 **************************************************************/

#ifndef GUIAPP_H
#define GUIAPP_H

#include <wx/app.h>

class GUIApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // GUIAPP_H
