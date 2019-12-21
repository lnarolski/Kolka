/***************************************************************
 * Name:      GUIMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Łukasz Narolski ()
 * Created:   2015-10-24
 * Copyright: Łukasz Narolski ()
 * License:
 **************************************************************/

#include "GUIMain.h"
#include <wx/msgdlg.h>
#include <windows.h>
#include <process.h>
#include <string.h>
#include "Wczytywanie.h"
#include "GUIApp.h"

//(*InternalHeaders(GUIFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

Wczytywanie *Ustawienia;
bool ustawienia_utworzone = false;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(GUIFrame)
const long GUIFrame::ID_BUTTON1 = wxNewId();
const long GUIFrame::ID_BUTTON2 = wxNewId();
const long GUIFrame::ID_CHOICE1 = wxNewId();
const long GUIFrame::ID_STATICTEXT1 = wxNewId();
const long GUIFrame::ID_STATICTEXT2 = wxNewId();
const long GUIFrame::ID_CHOICE2 = wxNewId();
const long GUIFrame::ID_CHECKBOX1 = wxNewId();
const long GUIFrame::ID_CHECKBOX2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(GUIFrame,wxFrame)
    //(*EventTable(GUIFrame)
    //*)
END_EVENT_TABLE()

GUIFrame::GUIFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(GUIFrame)

    Create(parent, wxID_ANY, _("Start gry"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX), _T("wxID_ANY"));
    SetClientSize(wxSize(423,228));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("Osmos.ico"))));
    	SetIcon(FrameIcon);
    }
    Button1 = new wxButton(this, ID_BUTTON1, _("Ustawienia bazy danych"), wxPoint(248,120), wxSize(136,88), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button1->Hide();
    Button2 = new wxButton(this, ID_BUTTON2, _("GRAJ!!!"), wxPoint(40,120), wxSize(136,88), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    wxFont Button2Font(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    Button2->SetFont(Button2Font);
    Choice1 = new wxChoice(this, ID_CHOICE1, wxPoint(40,64), wxSize(136,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice1->Append(_("0"));
    Choice1->SetSelection( Choice1->Append(_("1")) );
    Choice1->Append(_("2"));
    Choice1->Append(_("3"));
    Choice1->Append(_("4"));
    Choice1->Append(_("5"));
    Choice1->Append(_("6"));
    Choice1->Append(_("10"));
    Choice1->Append(_("20"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Ilosc przeciwnikow:"), wxPoint(56,40), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Ilosc pozostalych obiektow:"), wxPoint(248,40), wxSize(160,24), 0, _T("ID_STATICTEXT2"));
    Choice2 = new wxChoice(this, ID_CHOICE2, wxPoint(248,64), wxSize(136,21), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    Choice2->SetSelection( Choice2->Append(_("10")) );
    Choice2->Append(_("20"));
    Choice2->Append(_("30"));
    Choice2->Append(_("40"));
    Choice2->Append(_("50"));
    Choice2->Append(_("60"));
    Choice2->Append(_("70"));
    Choice2->Append(_("80"));
    Choice2->Append(_("90"));
    Choice2->Append(_("100"));
    Choice2->Append(_("150"));
    Choice2->Append(_("200"));
    CheckBox1 = new wxCheckBox(this, ID_CHECKBOX1, _("Uzyj bazy danych"), wxPoint(264,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(false);
    CheckBox2 = new wxCheckBox(this, ID_CHECKBOX2, _("Wczytaj gre"), wxPoint(72,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBox2->SetValue(false);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GUIFrame::OnButton1Click1);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GUIFrame::OnButton2Click);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&GUIFrame::OnCheckBox1Click);
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&GUIFrame::OnCheckBox2Click);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&GUIFrame::OnClose);
    //*)
}

GUIFrame::~GUIFrame()
{
    //(*Destroy(GUIFrame)
    //*)
}

void GUIFrame::OnQuit(wxCommandEvent& event)
{
    Close();
    //Destroy();
}

void GUIFrame::OnInit(wxInitDialogEvent& event) {
}

void GUIFrame::OnButton2Click(wxCommandEvent& event) {
    char parametry[4000];
    wxString biali = Choice2 -> GetString(Choice2 -> GetSelection());
    wxString niebiescy = Choice1 -> GetString(Choice1 -> GetSelection());
    wxString polecenie = biali + " " + niebiescy + " ";
    if (CheckBox2 -> GetValue() == true) {
        polecenie += "1 ";
    }
    else {
        polecenie += "0 ";
    }
    if (CheckBox1 -> GetValue() == true) {
        polecenie += "1 ";
    }
    else {
        polecenie += "0 ";
    }
    if(ustawienia_utworzone) {
        if (Ustawienia -> polaczono_z_baza == true && CheckBox1 -> GetValue() == true) {
            polecenie += Ustawienia -> ip + " " + Ustawienia -> login + " " + Ustawienia -> haslo + " " + Ustawienia -> dbname + " " + Ustawienia -> port;
            if (Ustawienia -> wybrano_zapis == true)
                polecenie += " " + Ustawienia -> nazwa_zapisu; }}
    strcpy(parametry, polecenie.mb_str());
    //printf("%s", parametry);
    /*if (ustawienia_utworzone)
        wxMessageBox(wxT("true"), wxT("ustawienia utworzone"), wxOK);
    else
        wxMessageBox(wxT("false"), wxT("ustawienia utworzone"), wxOK);
    wxMessageBox(polecenie, wxT("Cos"), wxOK);*/
    ShellExecuteA(NULL, "open", "game.exe", parametry, NULL, SW_NORMAL);
}

void GUIFrame::OnButton1Click1(wxCommandEvent& event) {
    if (!ustawienia_utworzone) {
        Ustawienia = new Wczytywanie(0);
        ustawienia_utworzone = true; }
    Ustawienia -> ShowModal();
}

void GUIFrame::OnCheckBox1Click(wxCommandEvent& event) {
    if (CheckBox1 -> GetValue() == true) {
        Button1 -> Show();
    }
    else
        Button1 -> Hide();
}

void GUIFrame::OnCheckBox2Click(wxCommandEvent& event) {
    if (CheckBox2 -> GetValue() == true) {
        StaticText1 -> Hide();
        StaticText2 -> Hide();
        Choice1 -> Hide();
        Choice2 -> Hide();
    }
    else {
        StaticText1 -> Show();
        StaticText2 -> Show();
        Choice1 -> Show();
        Choice2 -> Show();
    }
}

void GUIFrame::OnClose(wxCloseEvent& event)
{
    if (ustawienia_utworzone) {
        Ustawienia -> Destroy();
    }
    Destroy();
}
