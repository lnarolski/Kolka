#ifndef WCZYTYWANIE_H
#define WCZYTYWANIE_H
#include <libpq-fe.h>
#include "GUIMain.h"

//(*Headers(Wczytywanie)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class Wczytywanie: public wxDialog
{
	public:

		Wczytywanie(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~Wczytywanie();
		PGconn *stan;
		wxString ip;
        wxString login;
        wxString haslo;
        wxString port;
        wxString dbname;
        wxString nazwa_zapisu;
        bool polaczono_z_baza = false;
        bool wybrano_zapis;

		//(*Declarations(Wczytywanie)
		wxTextCtrl* TextCtrl4;
		wxStaticText* StaticText2;
		wxButton* Button1;
		wxStaticText* StaticText6;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* Button2;
		wxButton* Button3;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxTextCtrl* TextCtrl2;
		wxTextCtrl* TextCtrl1;
		wxTextCtrl* TextCtrl5;
		wxStaticText* StaticText4;
		wxTextCtrl* TextCtrl3;
		wxListBox* ListBox1;
		//*)

	protected:

		//(*Identifiers(Wczytywanie)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL2;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_BUTTON1;
		static const long ID_LISTBOX1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT6;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT7;
		//*)

	private:

		//(*Handlers(Wczytywanie)
		void OnInit(wxInitDialogEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
