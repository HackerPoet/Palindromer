#include "visualUI.h"
#include "palindrome.cpp"

#define MAX_DISPLAY 200

bool VisualUI::OnInit() {
    wxXmlResource::Get()->InitAllHandlers();
    wxXmlResource::Get()->Load("ui.xrc");

    Frame *frame = new Frame(nullptr);
    frame->Show();
    bool success = LoadDictionary("dictionary.txt");
    frame->OnDictionaryLoaded(success);
    return true;
}

Frame::Frame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style) {
    
    wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer1);
    
    wxFlexGridSizer* flexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    flexGridSizer1->SetFlexibleDirection( wxBOTH );
    flexGridSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer1->AddGrowableCol(0);
    flexGridSizer1->AddGrowableCol(1);
    
    boxSizer1->Add(flexGridSizer1, 0, wxALL|wxEXPAND, FromDIP(5));
    
    forTextCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    forTextCtrl->SetHint(_("start text"));
    #endif
    
    flexGridSizer1->Add(forTextCtrl, 1, wxALL|wxEXPAND, FromDIP(5));
    
    bacTextCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    bacTextCtrl->SetHint(_("end text"));
    #endif
    
    flexGridSizer1->Add(bacTextCtrl, 1, wxALL|wxEXPAND, FromDIP(5));
    
    searchButton = new wxButton(this, wxID_ANY, _("Search"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer1->Add(searchButton, 0, wxALL, FromDIP(5));
    
    wxFlexGridSizer* flexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    flexGridSizer2->SetFlexibleDirection( wxBOTH );
    flexGridSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer1->Add(flexGridSizer2, 0, wxALL|wxEXPAND, FromDIP(5));
    
    backButton = new wxButton(this, wxID_ANY, _("<- Back"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    backButton->Disable();
    
    flexGridSizer2->Add(backButton, 0, wxALL, FromDIP(5));
    
    fwdButton = new wxButton(this, wxID_ANY, _("Fwd ->"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    fwdButton->Disable();
    
    flexGridSizer2->Add(fwdButton, 0, wxALL, FromDIP(5));
    
    wxBoxSizer* boxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer1->Add(boxSizer3, 0, wxALL|wxEXPAND, FromDIP(5));
    
    palindromeTextCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTE_WORDWRAP|wxTE_READONLY|wxTE_MULTILINE|wxBORDER_NONE);
    palindromeTextCtrl->Hide();
    
    boxSizer3->Add(palindromeTextCtrl, 1, wxALL, FromDIP(5));
    
    wxBoxSizer* boxSizer2 = new wxBoxSizer(wxVERTICAL);
    
    boxSizer1->Add(boxSizer2, 1, wxALL|wxEXPAND, FromDIP(5));
    
    listCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxLC_NO_HEADER|wxLC_ALIGN_LEFT|wxLC_ALIGN_TOP|wxLC_REPORT);
    
    boxSizer2->Add(listCtrl, 1, wxALL|wxEXPAND, FromDIP(5));
    
    listCtrl->InsertColumn(listCtrl->GetColumnCount(), _("Beginning"), wxLIST_FORMAT_LEFT, -1);
    listCtrl->InsertColumn(listCtrl->GetColumnCount(), _("End"), wxLIST_FORMAT_LEFT, -1);

    wxBoxSizer* boxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer2->Add(boxSizer4, 0, wxALL|wxALIGN_RIGHT, FromDIP(5));
    
    prevPageButton = new wxButton(this, wxID_ANY, _("<"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    prevPageButton->Hide();
    prevPageButton->Disable();
    
    boxSizer4->Add(prevPageButton, 0, wxALL, FromDIP(5));
    
    nextPageButton = new wxButton(this, wxID_ANY, _(">"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    nextPageButton->Hide();
    nextPageButton->Disable();
    
    boxSizer4->Add(nextPageButton, 0, wxALL, FromDIP(5));
    
    SetName(wxT("Frame"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
    // Connect events
    backButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::OnBack, this);
    fwdButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::OnForward, this);
    searchButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::OnSearch, this);
    listCtrl->Bind(wxEVT_COMMAND_LIST_ITEM_SELECTED, &Frame::OnItemSelected, this);
    prevPageButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::PrevPage, this);
    nextPageButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::NextPage, this);

    CreateStatusBar();
    SetStatusText("Loading dictionary...");
    
}

Frame::~Frame() {
    backButton->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::OnBack, this);
    fwdButton->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &Frame::OnForward, this);
    
}

void Frame::OnDictionaryLoaded(bool success) {
    if (success) {
        SetStatusText("Dictionary loaded");
    } else {
        SetStatusText("Failed to load dictionary.txt");
    }
}

void Frame::OnItemSelected(wxListEvent& e) {
    wxListItem selected = e.GetItem();
    wxString str1 = selected.GetText();
    selected.SetColumn(1);
    listCtrl->GetItem(selected);
    wxString str2 = selected.GetText();

    forTextCtrl->ChangeValue(str1);
    bacTextCtrl->ChangeValue(str2);

    startFor = str1.ToStdString();
    startBac = str2.ToStdString();

    backStack.push_back(std::make_pair(startFor, startBac));
    backButton->Enable();
    fwdStack.clear();
    fwdButton->Disable();

    Search();
}

void Frame::OnBack(wxCommandEvent&) {
    fwdStack.push_back(std::make_pair(forTextCtrl->GetValue().ToStdString(),
                    bacTextCtrl->GetValue().ToStdString()));
    fwdButton->Enable();

    backStack.pop_back();
    startFor = backStack.back().first;
    startBac = backStack.back().second;
    forTextCtrl->ChangeValue(startFor);
    bacTextCtrl->ChangeValue(startBac);
    Search();

    if (backStack.size() == 0) {
        backButton->Disable();
    }
}

void Frame::OnForward(wxCommandEvent&) {
    backStack.push_back(std::make_pair(forTextCtrl->GetValue().ToStdString(),
                    bacTextCtrl->GetValue().ToStdString()));

    startFor = fwdStack.back().first;
    startBac = fwdStack.back().second;
    forTextCtrl->ChangeValue(startFor);
    bacTextCtrl->ChangeValue(startBac);
    Search();

    fwdStack.pop_back();
    if (fwdStack.size() == 0) {
        fwdButton->Disable();
    }
}

void Frame::OnSearch(wxCommandEvent&) {
    startFor = forTextCtrl->GetValue().ToStdString();
    startBac = bacTextCtrl->GetValue().ToStdString();
    backStack.push_back(std::make_pair(startFor, startBac));
    backButton->Enable();
    fwdStack.clear();
    fwdButton->Disable();

    Search();
}

void Frame::Search() {
    std::set<std::string> palindromes;
    palindromeTextCtrl->ChangeValue(startFor + " " + startBac);
    palindromeTextCtrl->Show();
    SendSizeEvent();
    SetStatusText("Generating palindromes...");

    BruteSearch(palindromes, startFor, startBac);

    
    listCtrl->DeleteAllItems();
    double show_pct = 1.0;
    std::uniform_real_distribution<float> rand(0.0f, 1.0f);

    if (palindromes.size() > MAX_DISPLAY) {
        prevPageButton->Show();
        nextPageButton->Show();
        SendSizeEvent();
        nextPageButton->Enable();
        prevPageButton->Disable();
    } else {
        prevPageButton->Hide();
        nextPageButton->Hide();
    }
    resultList.assign(palindromes.begin(), palindromes.end());
    pageNo = 0;
    SetStatusText(wxString::Format("Found %zu possibilities", palindromes.size()));
    DisplayList();
}

void Frame::DisplayList() {
    for (int i = pageNo * MAX_DISPLAY; i < resultList.size() && i < (pageNo + 1) * MAX_DISPLAY; i++) {
        std::string palindrome = resultList[i];
        int delim = palindrome.find("|", 0);
        std::string str1 = startFor + palindrome.substr(0, delim);
        std::string str2 = palindrome.substr(delim + 1, palindrome.length() - delim - 1) + startBac;

        long index = listCtrl->InsertItem(0, str1);
        listCtrl->SetItem(index, 1, str2);
    }

    listCtrl->SetColumnWidth(0, wxLIST_AUTOSIZE);
    listCtrl->SetColumnWidth(1, wxLIST_AUTOSIZE);
}

void Frame::PrevPage(wxCommandEvent& event) {
    pageNo--;
    nextPageButton->Enable();
    if (pageNo == 0) {
        prevPageButton->Disable();
    }
    DisplayList();
}

void Frame::NextPage(wxCommandEvent& event) {
    pageNo++;
    prevPageButton->Enable();
    if ((pageNo + 1) * MAX_DISPLAY > resultList.size()) {
        nextPageButton->Disable();
    }
    DisplayList();
}
wxIMPLEMENT_APP(VisualUI);