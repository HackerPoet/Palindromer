#include <deque>
#include <thread>
#include <future>
#include <set>
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/listctrl.h>

class VisualUI : public wxApp {
    public:
        bool OnInit() override;
};

class Frame : public wxFrame {
    public:
        Frame(wxWindow* parent,
              wxWindowID id = wxID_ANY,
              const wxString& title = _("Palindromer"),
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxSize(500,300),
              long style = wxCAPTION|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLOSE_BOX);
        virtual ~Frame();

        void OnDictionaryLoaded(bool success);
        void SetPalindromes(std::set<std::string> palindromes);
        void OnSearchDone(wxCommandEvent& event);
    
    protected:
        wxTextCtrl* forTextCtrl;
        wxTextCtrl* bacTextCtrl;
        wxTextCtrl* palindromeTextCtrl;
        wxButton* backButton;
        wxButton* fwdButton;
        wxButton* searchButton;
        wxListCtrl* listCtrl;
        wxButton* prevPageButton;
        wxButton* nextPageButton;

        std::future<void> searchFuture;
        std::future<void> pollFuture;
        bool searching = false;
    
    private:
        void OnSearch(wxCommandEvent&);
        void OnItemSelected(wxListEvent& event);
        void OnBack(wxCommandEvent& event);
        void OnForward(wxCommandEvent& event);
        void PrevPage(wxCommandEvent& event);
        void NextPage(wxCommandEvent& event);
        void Search();
        void DisplayList();

        std::deque<std::pair<std::string, std::string>> backStack;
        std::deque<std::pair<std::string, std::string>> fwdStack;
        std::string startFor;
        std::string startBac;
        std::set<std::string> palindromes;
        std::vector<std::string> resultList;
        int pageNo;
};