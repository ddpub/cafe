/////////////////////////////////////////////////////////////////////////////
// Name:         wx/repwrt.h
// Purpose:      wxReportWriter class, wxReportSelector Dialog class
// Author:       Juergen D. Geltinger
// Organisation: IFD - Ing-Buero fuer Datentechnik, Altdorf, Germany
// Modified by:  JDG, 12-Jun-2004
// Created by:   JDG, 21-Jun-2002
// Copyright:    (c) 2002-2004 Juergen D. Geltinger <jgelti@ifd.de>
// Licence:      wxWidgets license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_REPWRTH__
#define _WX_REPWRTH__

#ifdef __GNUG__
#pragma interface "repwrt.h"
#endif


#include <wx/dir.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/filename.h>
#include <wx/ffile.h>
#include <wx/metafile.h>
#include <wx/datetime.h>
#ifdef __WXMSW__
#include <sqlext.h>
#else
#include <wx/isqlext.h>		// because we need TIMESTAMP_STRUCT
#endif
#include <wx/variant.h>
#include <wx/print.h>
#include <wx/printdlg.h>
#include <wx/dynarray.h>

#define POTYPE_NONE		0
#define POTYPE_PCHAR	1	// pointer to wxChar[]
#define POTYPE_PCHARDBL	2	// pointer to wxChar[] representing a 'double'
#define POTYPE_PCHARINT	3	// pointer to wxChar[] representing an 'int'
#define POTYPE_PSTR		4	// pointer to wxString
#define POTYPE_PSTRDBL	5	// pointer to wxString representing a 'double'
#define POTYPE_PSTRINT	6	// pointer to wxString representing an 'int'
#define POTYPE_PDBL		7	// pointer to a 'double' value
#define POTYPE_PINT		8	// pointer to an 'int' value
#define POTYPE_PLONG	9	// pointer to a 'long' value
#define POTYPE_PLOGIC	10	// pointer to a 'bool' value
#define POTYPE_PCHECK	11	// pointer to a 'bool' value (display checkbox)
#define POTYPE_STRING	12	// a 'wxString' value
#define POTYPE_DOUBLE	13	// a 'double' value
#define POTYPE_LONG		14	// a 'long' value
#define POTYPE_PGNUM	15	// a 'page-number'
#define POTYPE_LINE		16	// a 'line'
#define POTYPE_REPDATE	17	// the 'report-date'
#define POTYPE_PTIMEST	18	// pointer to a TIMESTAMP_STRUCT
#define POTYPE_BITMAP	24	// a 'bitmap' (PNG)
#define POTYPE_SUBSTART	30	// start of a sub-report
#define POTYPE_SUBEND	31	// end of a sub-report
#define POTYPE_PGBREAK	32	// start of a new page


class wxReportWriter;

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
	void SetReportWriter( wxReportWriter *pRep ) { m_pRepWrt = pRep; };

private:
	wxReportWriter *m_pRepWrt;

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



// -------------------------------------
//  Margins
//  ('Right' margin unused)
// -------------------------------------
class WXDLLEXPORT wxReportWriterCtrl
{
public:
	double		m_dTop;
	double		m_dLeft;
	double		m_dBottom;
	double		m_dRight;
	double		m_dYSizeMM;
	double		m_dXSizeMM;
	long		m_nPages;
	long		m_nCurrentPage;
	double		m_dDataCurrY;
	double		m_dFooterMinY;
	wxString	*m_pDateStr;
	wxString	*m_pDateFmt;
	wxString	*m_pShortDateFmt;
};

// -------------------------------------
//  A Report/Print object
// -------------------------------------
class WXDLLEXPORT wxReportObj
{
private:
	// Pointer to the Page-Control/Margins
	wxReportWriterCtrl			*m_pMargin;

	// Page positions:
	wxPoint			m_ptInitPos;	// in 1/100mm
	wxSize			m_sizSize;		// in 1/100mm
	wxSize			m_sizInc;		// in 1/100mm
	wxPoint			m_ptCurrPos;	// in 1/100mm

	// Font
	wxFont			*m_pFont;

	// Data
	void			*m_pData;		// pointer to data
	unsigned short	m_nType;		// POTYPE_xxxx
	double			m_dLocal;		// local 'double' value
	long			m_lLocal;		// local 'long' value
	wxString		m_strLocal;		// local 'wxString'

	// Format (in case of 'double')
	int				m_nLen;
	int				m_nDec;

	// Sums (not used so far)
	double			m_dSum;
	long			m_lSum;

	// Scaling  mm --> log.Units
	double			m_dScale;

	// Right aligned ?
	bool			m_boRightAlign;

    wxBitmap		*m_pBitmap;

public:
	wxReportObj( void );
	wxReportObj(double dXPos,			// Initial-Position X in mm
				double dYPos,			// Initial-Position Y in mm
				double dXSize = 10.0,	// Width in mm
				double dYSize =  4.2333	// Height in mm  (i.e. 1/6 inch = 4.2333mm)
				);
	~wxReportObj( void );
	void        Init(double dXPos, double dYPos, double dXSize, double dYSize );

	wxPoint &   GetInitPos( void )   { return m_ptInitPos; };
	wxSize &    GetSize( void )      { return m_sizSize; };
	wxSize &    GetInc( void )       { return m_sizInc; };
	wxPoint &   GetCurrPos( void )   { return m_ptCurrPos; };
	long        GetLocalLong( void ) { return m_lLocal; };
	int         GetType( void )      { return m_nType; };
	wxString &  GetString( void )    { return m_strLocal; };

	bool		SaveSettings( wxFFile &fFile );
	bool		SaveData( wxFFile &fFile );
	bool		SaveWxString( wxFFile &fFile, wxString &str );
	bool		LoadSettings( wxFFile &fFile );
	bool		LoadData( wxFFile &fFile );
	wxString	LoadWxString( wxFFile &fFile );

	void		ResetPos( void );		// set to initial position
	wxPoint &	MoveForward( void );	// increment x/y positions and return
										//   new current position
	wxPoint &	MoveBackward( void );	// go back to previous x/y position and
										//   new current position

	void		SetFont( const wxFont *pFt );
	void		SetMargins( wxReportWriterCtrl *pMargin );
	void		SetCurrPos( wxPoint pt )   { m_ptCurrPos = pt; };
	void		SetPosition( double dXPos, double dYPos );
	void		SetInitPosition( double dXPos, double dYPos );
	void		SetIncrements( double dXInc, double dYInc );
	void		SetWidth( double dWid );
	void		SetHeight( double dHei );
	void		SetStepH( double dStep );
	void		SetStepV( double dStep );
	void		SetScale( double dScale ) { m_dScale = dScale; };
	void		SetRightAlign( bool boRight = true ) { m_boRightAlign = boRight; };

	void		SetBitmap( wxString strFile );
	void		SetRef(	const wxChar *cpData,
						int iType = POTYPE_PCHAR );  // can also be PCHARDBL or PCHARINT
	void		SetRef(	const wxString *strData,
						int iType = POTYPE_PSTR );   // can also be PSTRDBL or PSTRINT
	void		SetRef( const double *pdData, unsigned short nLen = 10, unsigned short nDec = 2 );
	void		SetRef( const int *piData, unsigned short nLen = 8 );
	void		SetRef( const long *plData, unsigned short nLen = 8 );
	void		SetRef( const TIMESTAMP_STRUCT *pData );
	void		SetCheckbox( const bool *pboData, int nSize = 3 );
	void		SetLogic( const bool *pboData, int nSize = 3 );

	void		SetData( double dData, unsigned short nLen = 10, unsigned short nDec = 2 );
	void		SetData( long lData, unsigned short nLen = 8 );
	void		SetData( const wxString & strData );
	void		SetData( const wxChar *cpData );
	void		SetPgNum( const wxString & strPage );
	void		SetPgNum( const wxChar *cpPage );
	void		SetRepDate( void );
	void		SetLine( double dX = -1.0, double dY = -1.0 );
	void		SetSubReport( long nCount );
	void		SetSubReportEnd( void );
	void		SetPageBreak( void );

	void		Draw( wxDC & dc );
};


WX_DECLARE_EXPORTED_LIST(wxReportObj, wxReportObjList);

// -------------------------------------
//  
// -------------------------------------
class WXDLLEXPORT wxReportSection
{
public:
	wxReportObjList *m_pList;
	long            m_lPreviousSection;
	long            m_lSections;
	bool			m_boSaved;

	wxReportSection::wxReportSection( void ) { m_boSaved = false; m_pList = 0; m_lPreviousSection = -1L; };
	wxReportSection::~wxReportSection( void ) { if ( m_pList ) delete m_pList; };
};

WX_DECLARE_EXPORTED_LIST(wxReportSection, wxReportSectionList);



class WXDLLEXPORT ReportPrintout;


// -------------------------------------
//  THE REPORT (page)
// -------------------------------------
class WXDLLEXPORT wxReportWriter: public wxFrame
{
private:
	wxString            m_strFilename;
	wxString            m_strFilenameTmp;
	wxString			m_strPathOfReports;

	wxString			m_strDateFmt;
	wxString			m_strShortDateFmt;
	wxString			m_strDate;
	wxString			m_strUser;
	wxString			m_strInfo;

	wxReportObjList		m_lstHeader;	// list of report's header objects
	wxReportObjList		m_lstFooter;	// list of report's footer objects
	wxReportObjList		*m_pObjList;	// points to the current list of report objects
	wxReportSectionList	m_lstSections;

	wxReportWriterCtrl	m_PageCtrl;		// margins, page-number, # of pages etc.

	ReportPrintout		*m_pPrintout;

	wxFFile				m_fFile;

	long				m_lDatasets;

	bool				m_boLoad;			// TRUE = report is loading
	bool				m_boObjectsSaved;	// TRUE = header/footer saved

	long				m_lCurrentFilePos;
	long				m_lNextPageInFile;

	long				m_lCurrentSection;	// index into wxReportSectionList
	wxArrayLong			m_aPageStart;		// file positions of page starts
	wxArrayLong			m_aSection;			// current section at each begin of page

public:
    wxReportWriter(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size);
    ~wxReportWriter();

	void     DefinitionError( void );
	void     AddHeaderObj( wxReportObj *pObj );
	void     AddFooterObj( wxReportObj *pObj );
	void     AddDataObj( wxReportObj *pObj );

	long     DefineSection( void );
	long     PreviousSection( long lCurrentSection );
	void     EndSection( void );
	void     BeginDataSection( void );
	void     EndDataSection( void );
	void     FinishSection( void ) { EndDataSection(); EndSection(); };
	void     NewPage( void );			// Saves a POTYPE_PGBREAK
	void     HorizLine( double dX = 0.0, double dLen = 170.0, double dYInc = 0.0 );



	long     LoadReportInfo( wxString strFile );
	bool     LoadFile( wxString strFile );
	wxString LoadWxString( wxFFile *pFile = 0 );
	bool     SaveObjects( void );
	bool     SaveData( void );			// Saves report or sub-report data array
	long     SetupReport( wxString strFile );
	void     FinishReport( void );
	wxString FinalDestination( void );

	wxString GetPath( void )   { return m_strPathOfReports; };
	int      GetNumberOfPages();
	void     SetPage(unsigned int uiPage);
	void     GetTextSize( wxDC& dc, int *iX, int *iY );
	double   GetTopMargin()    { return m_PageCtrl.m_dTop; };
	double   GetLeftMargin()   { return m_PageCtrl.m_dLeft; };
	double   GetBottomMargin() { return m_PageCtrl.m_dBottom; };
	double   GetRightMargin()  { return m_PageCtrl.m_dRight; };
	wxString GetDate()         { return m_strDate; };
	wxString GetUser()         { return m_strUser; };
	wxString GetInfo()         { return m_strInfo; };

	void     SetLoading( void ) { m_boLoad = true; };

	void     SetPath( const wxString &strPath );
	void     SetDateFormat( const wxString &strFmt );
	void     SetShortDateFormat( const wxString &strFmt );
    void     SetDate( wxString strDate ) { m_strDate = strDate; };
    void     SetDate( const wxDateTime &dt );
	void     SetUser( wxString strUser ) { m_strUser = strUser; };
	void     SetInfo( wxString strInfo ) { m_strInfo = strInfo; };
    void     SetPrintout( ReportPrintout *pRef ) { m_pPrintout = pRef; };
	void     SetTopMargin( double d )    { m_PageCtrl.m_dTop    = d; };
	void     SetLeftMargin( double d )   { m_PageCtrl.m_dLeft   = d; };
	void     SetBottomMargin( double d ) { m_PageCtrl.m_dBottom = d; };
	void     SetRightMargin( double d )  { m_PageCtrl.m_dRight  = d; };
	void     SetOrientation( int iMode );

    void     Draw(wxDC& dc);
    void     PrintPreview( void );
    void     Print( void );
};


// -------------------------------------
//  Printout Handling
// -------------------------------------
class WXDLLEXPORT ReportPrintout: public wxPrintout
{
public:
    ReportPrintout(wxReportWriter *pFrame, wxChar *szTitle = wxT("Polix Druck") );
    ~ReportPrintout( void );

    bool   OnPrintPage(int page);
    bool   HasPage(int page);
    bool   OnBeginDocument(int startPage, int endPage);
    void   GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);
    void   OnEndPrinting();

    void   DrawPage(wxDC *dc, int iPage);

    void   CalcScale( wxDC *dc );
    double GetMMScale( void ) { return (double)m_fScaleMMToLogUnits; }

private:
    wxReportWriter	*m_pReportPage;
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


#endif  // _WX_REPWRTH__




