/////////////////////////////////////////////////////////////////////////////
// Name:         wx/repwrt.h
// Purpose:      wxReportWriter class, wxReportSelector Dialog class
// Author:       Juergen D. Geltinger
// Organisation: IFD - Ing-Buero fuer Datentechnik, Altdorf, Germany
// Modified by:  JDG, 12-Jun-2004, SF 26-July-2005
// Created by:   JDG, 21-Jun-2002
// Copyright:    (c) 2002-2004 Juergen D. Geltinger <jgelti@ifd.de>
// Licence:      wxWidgets license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_REPWRTH__
#define _WX_REPWRTH__

#ifdef __GNUG__
#pragma interface "wxReportWriter.cpp"
#endif

#include <wx/filename.h>
#include <wx/ffile.h>
#include <wx/datetime.h>
#ifdef __WXMSW__
typedef long SDWORD;
typedef short SWORD;
typedef ULONG UDWORD;
typedef USHORT UWORD;
typedef SWORD SQLSMALLINT;
typedef UWORD SQLUSMALLINT;
typedef ULONG UDWORD;
typedef UDWORD SQLUINTEGER;
typedef struct tagTIMESTAMP_STRUCT {
	SQLSMALLINT year;
	SQLUSMALLINT month;
	SQLUSMALLINT day;
	SQLUSMALLINT hour;
	SQLUSMALLINT minute;
	SQLUSMALLINT second;
	SQLUINTEGER fraction;
} TIMESTAMP_STRUCT;
//#include <sqlext.h>
#else
#include <wx/isqlext.h>		// because we need TIMESTAMP_STRUCT
#endif
#include <wx/variant.h>

#define POTYPE_NONE		0
#define POTYPE_PCHAR	1	//!< pointer to wxChar[]
#define POTYPE_PCHARDBL	2	//!< pointer to wxChar[] representing a 'double'
#define POTYPE_PCHARINT	3	//!< pointer to wxChar[] representing an 'int'
#define POTYPE_PSTR		4	//!< pointer to wxString
#define POTYPE_PSTRDBL	5	//!< pointer to wxString representing a 'double'
#define POTYPE_PSTRINT	6	//!< pointer to wxString representing an 'int'
#define POTYPE_PDBL		7	//!< pointer to a 'double' value
#define POTYPE_PINT		8	//!< pointer to an 'int' value
#define POTYPE_PLONG	9	//!< pointer to a 'long' value
#define POTYPE_PLOGIC	10	//!< pointer to a 'bool' value
#define POTYPE_PCHECK	11	//!< pointer to a 'bool' value (display checkbox)
#define POTYPE_STRING	12	//!< a 'wxString' value
#define POTYPE_DOUBLE	13	//!< a 'double' value
#define POTYPE_LONG		14	//!< a 'long' value
#define POTYPE_PGNUM	15	//!< a 'page-number'
#define POTYPE_LINE		16	//!< a 'line'
#define POTYPE_REPDATE	17	//!< the 'report-date'
#define POTYPE_PTIMEST	18	//!< pointer to a TIMESTAMP_STRUCT
#define POTYPE_BITMAP	24	//!< a 'bitmap' (PNG)
#define POTYPE_SUBSTART	30	//!< start of a sub-report
#define POTYPE_SUBEND	31	//!< end of a sub-report
#define POTYPE_PGBREAK	32	//!< start of a new page

#define VLPHEADER   wxT("VLP5")		//!< File-Header for report-files
#define VLPEXT      wxT("vlp")		//!< File-Extension for report-files
#define HLINE_INC	0				//!< a horizontal line increments the Y pos by 0.2mm

class wxReportWriter;

/// Report font
class wxReportFont
{
public: // functions

	wxReportFont( void );
	~wxReportFont( void );

	wxReportFont* Clone();

public: // variables

	//! name specified in report template for this font
	wxString	name;
	//! is this the default font for the report?
	bool		isDefault;
	//! font family
	//! wxFONTFAMILY_DEFAULT		= 70
	//! wxFONTFAMILY_DECORATIVE		= 71
	//! wxFONTFAMILY_ROMAN			= 72
	//! wxFONTFAMILY_SCRIPT			= 73
	//! wxFONTFAMILY_SWISS			= 74
	//! wxFONTFAMILY_MODERN			= 75
	//! wxFONTFAMILY_TELETYPE		= 76
	int			fontFamily;
	//! actual system font face name
	wxString	fontName;
	//! point size for this font
	int			fontSize;
	//! is this font bold?
	bool		isBold;
	//! is this font italic?
	bool		isItalic;
	//! is this font underline?
	bool		isUnderline;
	//! is this font strike through?
	bool		isStrikeThrough;
	//! color used for this font
	wxString	fontColor;
};

//! -------------------------------------
//!  Margins
//!  ('Right' margin unused)
//! -------------------------------------
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
	int			m_iPaperFormat;

	double		m_dHeaderHeight;
};

//! -------------------------------------
//!  A Report/Print object
//! -------------------------------------
class WXDLLEXPORT wxReportObj
{
protected:
	//! Pointer to the Page-Control/Margins
	wxReportWriterCtrl			*m_pMargin;

	//! Page positions:
	wxPoint			m_ptInitPos;	//!< in 1/100mm
	wxSize			m_sizSize;		//!< in 1/100mm
	wxSize			m_sizInc;		//!< in 1/100mm
	wxPoint			m_ptCurrPos;	//!< in 1/100mm

	//! Font
	wxReportFont	*m_pReportFont;

	//! Data
	void			*m_pData;		//!< pointer to data
	unsigned short	m_nType;		//!< POTYPE_xxxx
	double			m_dLocal;		//!< local 'double' value
	long			m_lLocal;		//!< local 'long' value
	wxString		m_strLocal;		//!< local 'wxString'

	//! Format (in case of 'double')
	int				m_nLen;
	int				m_nDec;

	//! Sums (not used so far)
	double			m_dSum;
	long			m_lSum;

	//! Scaling  mm --> log.Units
	double			m_dScale;

	//! Right aligned ?
	bool			m_boRightAlign;

	//! Center aligned ?
	bool			m_boCenterAlign;

	unsigned char*m_pBitmapFileData;
	long			m_sizeOfBitmapFileData;

	void			LoadBitmapFileData();
	bool			SaveBitmapData( wxFFile &fFile );

	//! Pen width (only for drawing objects, e.g. lines, boxes, etc.)
	int				m_nPenWidth;

public:
	wxReportObj( void );
	wxReportObj(double dXPos,			//!< Initial-Position X in mm
				double dYPos,			//!< Initial-Position Y in mm
				double dXSize = 10.0,	//!< Width in mm
				double dYSize =  4.2333	//!< Height in mm  (i.e. 1/6 inch = 4.2333mm)
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

	void		ResetPos( void );		//!< set to initial position
	wxPoint &	MoveForward( void );	//!< increment x/y positions and return new current position
	wxPoint &	MoveBackward( void );	//!< go back to previous x/y position and new current position

	void		SetFont( wxReportFont *pFt );
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
	void		SetCenterAlign( bool boCenter = true ) { m_boCenterAlign = boCenter; };

	void		SetBitmap( wxString strFile );
	void		SetRef(	const wxChar *cpData,
						int iType = POTYPE_PCHAR );  //!< can also be PCHARDBL or PCHARINT
	void		SetRef(	const wxString *strData,
						int iType = POTYPE_PSTR );   //!< can also be PSTRDBL or PSTRINT
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
	void		SetPenWidth( int width );
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
class WXDLLEXPORT wxReportWriter
{
protected:
	wxString            m_strFilename;
	wxString            m_strFilenameTmp;
	wxString			m_strPathOfReports;

	wxString			m_strDateFmt;
	wxString			m_strShortDateFmt;
	wxString			m_strDate;
	wxString			m_strUser;
	wxString			m_strInfo;

	wxReportObjList		m_lstHeader;	//!< list of report's header objects
	wxReportObjList		m_lstFooter;	//!< list of report's footer objects
	wxReportObjList		*m_pObjList;	//!< points to the current list of report objects
	wxReportSectionList	m_lstSections;

	wxReportWriterCtrl	m_PageCtrl;		//!< margins, page-number, # of pages etc.

	wxFFile				m_fFile;

	long				m_lDatasets;

	bool				m_boLoad;			//!< TRUE = report is loading
	bool				m_boObjectsSaved;	//!< TRUE = header/footer saved

	long				m_lCurrentFilePos;
	long				m_lNextPageInFile;

	long				m_lCurrentSection;	//!< index into wxReportSectionList
	wxArrayLong			m_aPageStart;		//!< file positions of page starts
	wxArrayLong			m_aSection;			//!< current section at each begin of page

	bool				bFinished;

public:
    wxReportWriter();
    ~wxReportWriter();

	void     DefinitionError( void );
	void     AddHeaderObj( wxReportObj *pObj );
	void     AddFooterObj( wxReportObj *pObj );
	void     AddDataObj( wxReportObj *pObj );

	long     DefineSection( void );
	long     PreviousSection( long lCurrentSection );
	virtual void     EndSection( void );
	void     BeginDataSection( void );
	void     EndDataSection( void );
	void     FinishSection( void ) { EndDataSection(); EndSection(); };
	void     NewPage( void );			//!< Saves a POTYPE_PGBREAK
	void     HorizLine( double dX = 0.0, double dLen = 170.0, double dYInc = 0.0 );

	long     LoadReportInfo( wxString strFile );
	bool     LoadFile( wxString strFile );
	wxString LoadWxString( wxFFile *pFile = 0 );
	bool     SaveObjects( void );
	bool     SaveData( void );			//!< Saves report or sub-report data array
	void     FinishReport( void );
	wxString FinalDestination( void );

	wxString GetPath( void )   { return m_strPathOfReports; };
	int      GetNumberOfPages();
	void     SetPage(unsigned int uiPage);
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
	void     SetTopMargin( double d )    { m_PageCtrl.m_dTop    = d; };
	void     SetLeftMargin( double d )   { m_PageCtrl.m_dLeft   = d; };
	void     SetBottomMargin( double d ) { m_PageCtrl.m_dBottom = d; };
	void     SetRightMargin( double d )  { m_PageCtrl.m_dRight  = d; };
	void	 SetPaperFormat( int format ) { m_PageCtrl.m_iPaperFormat = format; };
	//void     SetOrientation( int iMode );
	void	 SetPageSize( double pageWidth = 215.9, double pageHeight = 279.4 )
				{ m_PageCtrl.m_dXSizeMM = pageWidth; m_PageCtrl.m_dYSizeMM = pageHeight; };
	void	 SetPageWidth( double pageWidth = 215.9 ) { m_PageCtrl.m_dXSizeMM = pageWidth; };
	void	 SetPageHeight( double pageHeight = 279.4 ) { m_PageCtrl.m_dYSizeMM = pageHeight; };

	virtual void DeleteReportObj( wxReportObj* obj );
};

#endif
