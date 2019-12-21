#include "Wczytywanie.h"
#include <libpq-fe.h>
#include <wx/msgdlg.h>

//(*InternalHeaders(Wczytywanie)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(Wczytywanie)
const long Wczytywanie::ID_STATICTEXT1 = wxNewId();
const long Wczytywanie::ID_TEXTCTRL1 = wxNewId();
const long Wczytywanie::ID_STATICTEXT2 = wxNewId();
const long Wczytywanie::ID_STATICTEXT3 = wxNewId();
const long Wczytywanie::ID_STATICTEXT4 = wxNewId();
const long Wczytywanie::ID_TEXTCTRL2 = wxNewId();
const long Wczytywanie::ID_TEXTCTRL3 = wxNewId();
const long Wczytywanie::ID_TEXTCTRL4 = wxNewId();
const long Wczytywanie::ID_STATICTEXT5 = wxNewId();
const long Wczytywanie::ID_TEXTCTRL5 = wxNewId();
const long Wczytywanie::ID_BUTTON1 = wxNewId();
const long Wczytywanie::ID_LISTBOX1 = wxNewId();
const long Wczytywanie::ID_BUTTON2 = wxNewId();
const long Wczytywanie::ID_STATICTEXT6 = wxNewId();
const long Wczytywanie::ID_BUTTON3 = wxNewId();
const long Wczytywanie::ID_STATICTEXT7 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Wczytywanie,wxDialog)
	//(*EventTable(Wczytywanie)
	//*)
END_EVENT_TABLE()

Wczytywanie::Wczytywanie(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(Wczytywanie)
	Create(parent, id, _("Ustawienia polaczenia"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(605,399));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Adres IP bazy danych:"), wxPoint(40,32), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("127.0.0.1"), wxPoint(24,56), wxSize(152,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCtrl1->SetMaxLength(12);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Login:"), wxPoint(240,32), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Haslo:"), wxPoint(368,32), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Nazwa bazy danych:"), wxPoint(480,32), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl2 = new wxTextCtrl(this, ID_TEXTCTRL2, _("postgres"), wxPoint(200,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	TextCtrl3 = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxPoint(336,56), wxDefaultSize, wxTE_PASSWORD, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	TextCtrl4 = new wxTextCtrl(this, ID_TEXTCTRL4, _("bazatestowa"), wxPoint(480,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Port:"), wxPoint(368,88), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl5 = new wxTextCtrl(this, ID_TEXTCTRL5, _("5432"), wxPoint(336,104), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	Button1 = new wxButton(this, ID_BUTTON1, _("Polacz"), wxPoint(24,96), wxSize(264,40), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	ListBox1 = new wxListBox(this, ID_LISTBOX1, wxPoint(24,176), wxSize(552,144), 0, 0, wxLB_SINGLE, wxDefaultValidator, _T("ID_LISTBOX1"));
	ListBox1->Hide();
	Button2 = new wxButton(this, ID_BUTTON2, _("Wczytaj"), wxPoint(40,328), wxSize(248,40), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button2->Hide();
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Nie udalo sie polaczyc z baza danych"), wxPoint(192,152), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText6->Hide();
	StaticText6->SetForegroundColour(wxColour(255,0,0));
	wxFont StaticText6Font(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	StaticText6->SetFont(StaticText6Font);
	Button3 = new wxButton(this, ID_BUTTON3, _("Usun zapis"), wxPoint(312,328), wxSize(248,40), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button3->Hide();
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Udalo sie polaczyc z baza"), wxPoint(224,152), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	StaticText7->Hide();
	StaticText7->SetForegroundColour(wxColour(0,255,0));
	wxFont StaticText7Font(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	StaticText7->SetFont(StaticText7Font);

	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Wczytywanie::OnButton2Click);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Wczytywanie::OnButton1Click);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Wczytywanie::OnButton3Click);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&Wczytywanie::OnInit);
	//*)
}

/*Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Wczytywanie::OnButton2Click);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Wczytywanie::OnButton1Click);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Wczytywanie::OnButton3Click);*/

Wczytywanie::~Wczytywanie()
{
	//(*Destroy(Wczytywanie)
	//*)
	Close();
}


void Wczytywanie::OnInit(wxInitDialogEvent& event)
{
    wybrano_zapis = false;
}

void Wczytywanie::OnButton1Click(wxCommandEvent& event) {
    //PGconn *stan;
    PGresult *status_tmp;
    StaticText6 -> Hide();
    Button3 -> Hide();
    ListBox1 -> Hide();
    Button2 -> Hide();
    StaticText7 -> Hide();
    ListBox1 -> Clear();
    wxString polaczenie_z_baza;
    ip = TextCtrl1 -> GetLineText(0);
    login = TextCtrl2 -> GetLineText(0);
    haslo = TextCtrl3 -> GetLineText(0);
    port = TextCtrl5 -> GetLineText(0);
    dbname = TextCtrl4 -> GetLineText(0);
    polaczenie_z_baza = "hostaddr = " + ip + " port = " + port + " dbname = " + dbname + " user = " + login + " password = " + haslo;
    //wxMessageBox(polaczenie_z_baza, wxT("Cos"), wxOK);
    stan = PQconnectdb(polaczenie_z_baza.mb_str());
    if (PQstatus(stan) != CONNECTION_OK || haslo.CmpNoCase("") == 0) {
           StaticText6 -> Show();
    }
    else {
        polaczono_z_baza = true;
        StaticText7 -> Show();
        status_tmp = PQexec(stan, "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'");
        for (int i = 0; i < PQntuples(status_tmp); i++) {
            wxString tabela = wxString::FromAscii(PQgetvalue(status_tmp, i, 0));
            ListBox1 -> InsertItems(1, &tabela, 0);
        }
        if (PQntuples(status_tmp) > 0) {
            Button3 -> Show();
            ListBox1 -> Show();
            Button2 -> Show();
            ListBox1 -> SetSelection(0);
            //wxMessageBox(, wxT("Cos"), wxOK);
        }
    }
}

void Wczytywanie::OnButton2Click(wxCommandEvent& event) {
    nazwa_zapisu = ListBox1 -> GetString(ListBox1 -> GetSelection());
    wybrano_zapis = true;
    Close();
}

void Wczytywanie::OnButton3Click(wxCommandEvent& event) {
    PGresult *status_tmp;
    wxString polecenie = "DROP TABLE " + ListBox1 -> GetString(ListBox1 -> GetSelection());
    if (nazwa_zapisu.CmpNoCase(ListBox1 -> GetString(ListBox1 -> GetSelection())) == 0) {
        wybrano_zapis = false; }
    status_tmp = PQexec(stan, polecenie.mb_str());
    ListBox1 -> Hide();
    Button2 -> Hide();
    Button3 -> Hide();
    ListBox1 -> Clear();
    status_tmp = PQexec(stan, "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'");
    for (int i = 0; i < PQntuples(status_tmp); i++) {
            wxString tabela = wxString::FromAscii(PQgetvalue(status_tmp, i, 0));
            ListBox1 -> InsertItems(1, &tabela, 0);
    }
    if (PQntuples(status_tmp) > 0) {
            Button3 -> Show();
            ListBox1 -> Show();
            Button2 -> Show();
            ListBox1 -> SetSelection(0);
    }
}
