/***************************************************************
 * Name:      GUIMain.h
 * Purpose:   Defines Application Frame
 * Author:    Łukasz Narolski ()
 * Created:   2015-10-24
 * Copyright: Łukasz Narolski ()
 * License:
 **************************************************************/

#ifndef GUIMAIN_H
#define GUIMAIN_H

//(*Headers(GUIFrame)
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)



class GUIFrame: public wxFrame
{
    public:

        GUIFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~GUIFrame();
        wxString ip;
        wxString login;
        wxString haslo;
        wxString port;
        wxString dbname;
        wxString nazwa_zapisu;
        bool polaczono_z_baza;
        bool wybrano_zapis;

    private:

        //(*Handlers(GUIFrame)
        void OnInit(wxInitDialogEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnButton1Click1(wxCommandEvent& event);
        void OnTextCtrl1Text(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnCheckBox1Click(wxCommandEvent& event);
        void OnCheckBox2Click(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        //*)

        //(*Identifiers(GUIFrame)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_CHOICE1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_CHOICE2;
        static const long ID_CHECKBOX1;
        static const long ID_CHECKBOX2;
        //*)

        //(*Declarations(GUIFrame)
        wxStaticText* StaticText2;
        wxButton* Button1;
        wxCheckBox* CheckBox2;
        wxStaticText* StaticText1;
        wxButton* Button2;
        wxCheckBox* CheckBox1;
        wxChoice* Choice1;
        wxChoice* Choice2;
        //*)

        DECLARE_EVENT_TABLE()
};



#endif // GUIMAIN_H
