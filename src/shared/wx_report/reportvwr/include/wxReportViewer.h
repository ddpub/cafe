/////////////////////////////////////////////////////////////////////////////
// Name:         wx/repwrt.h
// Purpose:      wxReportViewer class
// Author:       Scott Fant
// Created by:   SF, 26-July-2005
// Copyright:    (c) 2005 Scott Fant <scottfant@gmail.com>
// Licence:      wxWidgets license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_REPORTVIEWER_H__
#define _WX_REPORTVIEWER_H__

#ifdef __GNUG__
#pragma interface "wxReportViewer.cpp"
#endif

#include <wx/listctrl.h>
#include <wx/print.h>
#include <wx/printdlg.h>
#include <wx/dir.h>
#include <wx/image.h>

#include "wxReportWriter.h"

class WXDLLEXPORT wxReportObjViewer : public wxReportObj
{
public: // functions

	wxReportObjViewer( void );
	wxReportObjViewer(double dXPos,		// Initial-Position X in mm
				double dYPos,			// Initial-Position Y in mm
				double dXSize = 10.0,	// Width in mm
				double dYSize =  4.2333	// Height in mm  (i.e. 1/6 inch = 4.2333mm)
				);

	~wxReportObjViewer( void );

	void		LoadBitmap( wxString strFile );

	bool		LoadBitmapData( wxFFile &fFile );

	void		Draw( wxDC & dc );

	bool		LoadSettings( wxFFile &fFile );
	bool		LoadData( wxFFile &fFile );
	wxString	LoadWxString( wxFFile &fFile );

public: // variables

    wxBitmap	*m_pBitmap;
	wxFont		*m_pFont;
};

class wxReportViewer;

class WXDLLEXPORT ReportPreviewFrame : public wxPreviewFrame
{
public:
	ReportPreviewFrame(
		wxPrintPreview* preview,
		wxFrame* parent,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxT("frame") );
	~ReportPreviewFrame( void );

	void OnCloseWindow( wxCloseEvent &event );
	void SetReportWriter( wxReportViewer *pRep ) { m_pRepVwr = pRep; };

private:
	wxReportViewer *m_pRepVwr;

	DECLARE_EVENT_TABLE()

};


class WXDLLEXPORT wxPrintSettings
{
private:
	// print data
	// (to remember settings during the session)
	wxPrintData *m_pPrintData;

	// page setup data
	wxPageSetupData* m_pPageSetupData;

public:
	wxPrintSettings( void );
	~wxPrintSettings( void );
	void Free( void );

	wxPrintData & GetPrintData( void );
	wxPageSetupData & GetPageSetupData( void );

	void SetPrintData( wxPrintData &printdata );
	void SetPageSetupData( wxPageSetupData &setupdata );
};

class WXDLLEXPORT wxReportViewer : public wxReportWriter, public wxFrame
{
public: // functions

    wxReportViewer(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size);
	~wxReportViewer();

    void     Draw(wxDC& dc);
    void     PrintPreview( void );
    void     Print( void );
    void     SetPrintout( ReportPrintout *pRef ) { m_pPrintout = pRef; };
	ReportPrintout		*GetPrintout() { return m_pPrintout; };
	long     SetupReport( wxString strFile );
	void     GetTextSize( wxDC& dc, int *iX, int *iY );

	//virtual void EndSection( void );
	void EndSection( void );
	//virtual void DeleteReportObj( wxReportObj* obj );
	void DeleteReportObj( wxReportObj* obj );

	//! Opens an existing report for printing/previewing
	bool OpenReport( );
	bool OpenReport( wxString reportPath );

	//! Previews a report
	bool Preview();

	//! Prints a report
	//bool Print();

private: // variables

	//! Full path to the report template
	wxFileName			reportFile;

	//! Parent folder of the report template
	wxString			rootFolder;

	ReportPrintout		*m_pPrintout;
};

// -------------------------------------
//  Printout Handling
// -------------------------------------
class WXDLLEXPORT ReportPrintout: public wxPrintout
{
public:
    ReportPrintout(wxReportViewer *pFrame, wxString szTitle = wxT("Report") );
    ~ReportPrintout( void );

    bool   OnPrintPage(int page);
    bool   HasPage(int page);
    bool   OnBeginDocument(int startPage, int endPage);
    void   GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);
    void   OnEndPrinting();

    void   DrawPage(wxDC *dc, int iPage);

    void   CalcScale( wxDC *dc );
    double GetMMScale( void );

private:
    wxReportViewer	*m_pReportPage;
    float			m_fOverallScale;
    float			m_fScaleMMToLogUnits;
};



// -------------------------------------
//  Report Selection Dialog
// -------------------------------------
#define ID_TX_PROMPT  10000
#define ID_LC_REPORT  10001
#define ID_PB_PREVIEW 10002
#define ID_PB_PRINT   10003
#define ID_PB_DEL     10004

#define REPSEL_NONE    0
#define REPSEL_PREVIEW 1
#define REPSEL_PRINT   2
#define REPSEL_DELETE  3

wxSizer *SelectPrintFunc( wxWindow *parent, bool call_fit = TRUE, bool set_sizer = TRUE );

class WXDLLEXPORT wxReportSelector : public wxDialog
{
private:
	wxArrayString m_aFiles;
	wxString      m_strPathOfReports;
	wxString      m_strFilename;
	int           m_iSelected;

	void CreateElements( void );
	void FillList( void );
	void SetupList( void );
	long SelectedListItem( void );


public:
	wxReportSelector( wxFrame *pParent, const wxString &strTitle, const wxString &strPath = wxT(".") );
	~wxReportSelector( void );

	int      GetSelected( void ) { return m_iSelected; };
	wxString GetFilename( void ) { return m_strFilename; };

	void OnPreview( wxCommandEvent &event );
	void OnPrint( wxCommandEvent &event );
	void OnDelete( wxCommandEvent &event );
	void OnIdle( wxIdleEvent &event );

	DECLARE_EVENT_TABLE()
};

#endif // _WX_REPORTVIEWER_H__
