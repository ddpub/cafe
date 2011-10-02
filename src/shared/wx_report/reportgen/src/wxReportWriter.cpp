/////////////////////////////////////////////////////////////////////////////
// Name:         src/common/repwrt.cpp
// Purpose:      wxReportWriter implementation
// Author:       Juergen D. Geltinger
// Organisation: IFD - Ing-Buero fuer Datentechnik, Altdorf, Germany
// Modified by:  JDG, 12-Jun-2004
// Created:      JDG, 21-Jun-2002
// Copyright:    (c) 2002-2004 Juergen D. Geltinger <jgelti@ifd.de>
// Licence:      wxWidgets license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "wxReportWriter.h"
#endif

#pragma warning(disable: 4006)

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//#if !wxUSE_PRINTING_ARCHITECTURE
//#error You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h to compile this program.
//#endif

#include <ctype.h>

#include <wx/filename.h>
#include <wx/ffile.h>
#include <wx/metafile.h>

#include <wx/accel.h>
#include <wx/dynarray.h>

#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
#include <wx/choicdlg.h>
#include <wx/encconv.h>
#include <wx/textfile.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include "wx/datetime.h"

#include "wxReportWriter.h"

#include <wx/listimpl.cpp>
WX_DEFINE_EXPORTED_LIST(wxReportObjList);
WX_DEFINE_EXPORTED_LIST(wxReportSectionList);

// -----------------------------------------------------------
// wxReportFont
// -----------------------------------------------------------
wxReportFont::wxReportFont()
{
	name			= wxT("Normal_12");
	isDefault		= false;
	fontFamily		= wxFONTFAMILY_DEFAULT;
	fontName		= wxT("");
	fontSize		= 12;
	isBold			= false;
	isItalic		= false;
	isUnderline		= false;
	isStrikeThrough	= false;
	fontColor		= wxT("Black");
}

wxReportFont::~wxReportFont()
{
}

wxReportFont* wxReportFont::Clone()
{
	wxReportFont* rft = new wxReportFont();

	rft->fontColor			= fontColor;
	rft->fontFamily			= fontFamily;
	rft->fontName			= fontName;
	rft->fontSize			= fontSize;
	rft->isBold				= isBold;
	rft->isDefault			= isDefault;
	rft->isItalic			= isItalic;
	rft->isStrikeThrough	= isStrikeThrough;
	rft->isUnderline		= isUnderline;
	rft->name				= name;

	return rft;
}

// -------------------------------------
// A report object
// -------------------------------------
wxReportObj::wxReportObj( void )
{
	Init( 0.0, 0.0, 1.0, 2.0 );
}

wxReportObj::wxReportObj(
	double dXPos,		// Initial-Position X in mm
	double dYPos,		// Initial-Position Y in mm
	double dXSize,		// Width in mm
	double dYSize		// Height in mm
)
{
	Init( dXPos, dYPos, dXSize, dYSize );
}

void wxReportObj::Init( double dXPos, double dYPos, double dXSize, double dYSize )
{
	m_strLocal.Empty();
	m_pMargin      = 0;
	m_dLocal       = 0.0;
	m_lLocal       = 0;
	m_dSum         = 0.0;
	m_lSum         = 0;
	m_nLen         = 0;
	m_nDec         = 0;
	m_nType        = POTYPE_NONE;
	m_pReportFont  = NULL;
	m_boRightAlign = false;
	m_boCenterAlign= false;
	m_dScale       = 3.78;		// just a default so far

	SetInitPosition( dXPos, dYPos );
	m_sizSize.x    = (int)(100.0 * dXSize);
	m_sizSize.y    = (int)(100.0 * dYSize);

	m_ptCurrPos    = m_ptInitPos;
	m_sizInc.x     = 0;
	m_sizInc.y     = m_sizSize.y;

	m_nPenWidth	   = 1;

    m_pBitmapFileData = NULL;
}

wxReportObj::~wxReportObj( void )
{
	if ( m_pReportFont )
		wxDELETE( m_pReportFont );

    if ( m_pBitmapFileData )
        wxDELETE( m_pBitmapFileData );
}

void wxReportObj::SetFont( wxReportFont *pFt )
{
	if ( m_pReportFont )
		wxDELETE( m_pReportFont );

	m_pReportFont = pFt->Clone();
}

// Store pointer to (global) page margins
void wxReportObj::SetMargins( wxReportWriterCtrl *pMargin )
{
	m_pMargin = pMargin;
}

// Set width of object (in mm)
// (internal representation is in 1/100mm units)
void wxReportObj::SetWidth( double dWid )
{
	m_sizSize.x = (int)(100.0 * dWid);
}

// Set height of object (in mm)
// (internal representation is in 1/100mm units)
void wxReportObj::SetHeight( double dHei )
{
	m_sizSize.y = (int)(100.0 * dHei);
}

// Set horizontal increment of object (in mm)
// (internal representation is in 1/100mm units)
void wxReportObj::SetStepH( double dStep )
{
	m_sizInc.x = (int)(100.0 * dStep);
}

// Set vertical increment of object (in mm)
// (internal representation is in 1/100mm units)
void wxReportObj::SetStepV( double dStep )
{
	m_sizInc.y = (int)(100.0 * dStep);
}

// Set CURRENT position of object (in mm)
// (internal representation is in 1/100mm units)
void wxReportObj::SetPosition( double dXPos, double dYPos )
{
	m_ptCurrPos.x = (int)(100.0 * dXPos);
	m_ptCurrPos.y = (int)(100.0 * dYPos);
}

// Set INITIAL position (i.e. when a new page begins) of object (in mm)
// (internal representation is in 1/100mm units)
void wxReportObj::SetInitPosition( double dXPos, double dYPos )
{
	m_ptInitPos.x = (int)(100.0 * dXPos);
	m_ptInitPos.y = (int)(100.0 * dYPos);
}

// Set horizontal and vertical increment of object (in mm)
// (internal representation is in 1/100mm units)
void wxReportObj::SetIncrements( double dXInc, double dYInc )
{
	m_sizInc.x = (int)(100.0 * dXInc);
	m_sizInc.y = (int)(100.0 * dYInc);
}


// Set object's position to its initial position
void wxReportObj::ResetPos( void )
{
	m_ptCurrPos   = m_ptInitPos;
}

// Store a pointer to data of type wxChar[].
// This may be
//   a) text,
//   b) text representing a double value
//   c) text representing an integer
void wxReportObj::SetRef(	const wxChar *cpData, int iType )
{
	if ( iType == POTYPE_PCHAR
	  || iType == POTYPE_PCHARDBL
	  || iType == POTYPE_PCHARINT )
	{
		m_pData = (void *)cpData;
		m_nType = iType;
	}
}

// Store a pointer to data of type wxString
// This may be
//   a) string,
//   b) string representing a double value
//   c) string representing an integer
void wxReportObj::SetRef(	const wxString *strData, int iType )
{
	if ( iType == POTYPE_PSTR
	  || iType == POTYPE_PSTRDBL
	  || iType == POTYPE_PSTRINT )
	{
		m_pData = (void *)strData;
		m_nType = iType;
	}
}

// Store a pointer to data of type 'double'
void wxReportObj::SetRef( const double *pdData, unsigned short nLen, unsigned short nDec )
{
	m_pData = (void *)pdData;
	m_nType = POTYPE_PDBL;
	m_nLen  = nLen;
	m_nDec  = nDec;
}

// Store a pointer to data of type 'long'
void wxReportObj::SetRef( const long *plData, unsigned short nLen )
{
	m_pData = (void *)plData;
	m_nType = POTYPE_PLONG;
	m_nLen  = nLen;
}

// Store a pointer to data of type 'int'
void wxReportObj::SetRef( const int *piData, unsigned short nLen )
{
	m_pData = (void *)piData;
	m_nType = POTYPE_PINT;
	m_nLen  = nLen;
}

// Store a pointer to data of type 'bool'
//   for presentation as a checkbox or a checkmark
void wxReportObj::SetCheckbox( const bool *pboData, int nLen )
{
	m_pData = (void *)pboData;
	m_nType = POTYPE_PCHECK;
	m_nLen  = nLen;
}

// Store a pointer to data of type 'bool'
void wxReportObj::SetLogic( const bool *pboData, int nLen )
{
	m_pData = (void *)pboData;
	m_nType = POTYPE_PLOGIC;
	m_nLen  = nLen;
}

// Store a pointer to data of type 'TIMESTAMP'
void wxReportObj::SetRef( const TIMESTAMP_STRUCT *pDate )
{
	m_pData = (void *)pDate;
	m_nType = POTYPE_PTIMEST;
}


// Store the name of a file containing a PNG bitmap
// (for use in header)
void wxReportObj::SetBitmap( wxString strFile )
{
	m_strLocal = strFile;
	m_nType = POTYPE_BITMAP;

	LoadBitmapFileData();
}

void wxReportObj::LoadBitmapFileData()
{
	wxFFile bmpFile;

	if ( bmpFile.Open( m_strLocal, wxT("rb") ) )
	{
		unsigned char buff;
		m_sizeOfBitmapFileData = bmpFile.Length() + 1;
		m_pBitmapFileData = new unsigned char[m_sizeOfBitmapFileData];
		
		unsigned char* pos = m_pBitmapFileData;
		while ( !bmpFile.Eof() )
		{
			bmpFile.Read( &buff, 1 );
			pos[0] = buff;
			pos++;
		}
		
		bmpFile.Close();
	}
}

// Store a single (fixed) data object of type 'double'
void wxReportObj::SetData( double dData, unsigned short nLen, unsigned short nDec )
{
	wxString strFmt;

	m_dLocal = dData;
	m_nLen   = nLen;
	m_nDec   = nDec;
	m_nType  = POTYPE_DOUBLE;

	strFmt.Printf( wxT("%%%d.%df"), m_nLen, m_nDec );
	m_strLocal.Printf( strFmt, m_dLocal );
}

// Store a single (fixed) data object of type 'long'
void wxReportObj::SetData( long lData, unsigned short nLen )
{
	wxString strFmt;

	m_lLocal = lData;
	m_nLen   = nLen;
	m_nType  = POTYPE_LONG;

	strFmt.Printf( wxT("%%%dld"), m_nLen );
	m_strLocal.Printf( strFmt, m_lLocal );
}

// Store a single (fixed) data object of type 'wxString'
void wxReportObj::SetData( const wxString & strData )
{
	m_strLocal = strData;
	m_nType  = POTYPE_STRING;
}

// Store a single (fixed) data object of type 'wxChar[]'
void wxReportObj::SetData( const wxChar *cpData )
{
	wxString strHelp;

	strHelp = cpData;
	SetData( strHelp );
}

// Set object to type 'line'
void wxReportObj::SetLine( double dX, double dY )
{
	m_nType = POTYPE_LINE;
	if ( dX > 0.0 )
		m_sizInc.x = (int)(100.0 * dX);
	if ( dY > 0.0 )
		m_sizInc.y = (int)(100.0 * dY);
}

// Set object to type 'page-number' (with auxiliary prompt)
// Later seen as
//    Page ####
//    Seite ####
//    Pagina ####
//    etc.
void wxReportObj::SetPgNum( const wxString & strData )
{
	m_strLocal = strData;
	m_nType  = POTYPE_PGNUM;
}

// Set object to type 'date'
void wxReportObj::SetRepDate( void )
{
	m_strLocal = wxT("->");
	m_nType  = POTYPE_REPDATE;
}

// Set object to type 'page-number' (with auxiliary prompt)
void wxReportObj::SetPgNum( const wxChar *cpData )
{
	wxString strHelp;

	strHelp = cpData;
	SetPgNum( strHelp );
}

// Start a SUB-REPORT (at current position)
void wxReportObj::SetSubReport( long nCount )
{
	m_nType  = POTYPE_SUBSTART;
	m_lLocal = nCount;
}

// Ends a SUB-REPORT
void wxReportObj::SetSubReportEnd( void )
{
	m_nType = POTYPE_SUBEND;
	m_lLocal = 0;
}

// Move the object to its new position for the next use
wxPoint & wxReportObj::MoveForward( void )
{
	m_ptCurrPos.x += m_sizInc.x;
	m_ptCurrPos.y += m_sizInc.y;

	return m_ptCurrPos;
}

// Move the object to its previous position
// (where it was before it was used)
wxPoint & wxReportObj::MoveBackward( void )
{
	m_ptCurrPos.x -= m_sizInc.x;
	m_ptCurrPos.y -= m_sizInc.y;

	return m_ptCurrPos;
}

// Start a new page
void wxReportObj::SetPageBreak( void )
{
	m_nType  = POTYPE_PGBREAK;
	m_lLocal = 0;
}

// -----------------------------------------
// Drawing the object in a DC
// -----------------------------------------
bool wxReportObj::SaveWxString( wxFFile &fFile, wxString &str )
{
	long lHelp;

	if ( ! fFile.IsOpened() )
		return false;

	lHelp = (long)str.Len() + 1L;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	fFile.Write( str.c_str(), lHelp );

	return true;
}

bool wxReportObj::SaveSettings( wxFFile &fFile )
{
	long			lHelp;
	wxReportFont	*prFt;

	if ( ! fFile.IsOpened() )
		return false;

	fFile.Write( &m_ptInitPos, sizeof( wxPoint ) );
	fFile.Write( &m_sizSize,   sizeof( wxSize ) );
	fFile.Write( &m_sizInc,    sizeof( wxSize ) );

	fFile.Write( &m_nLen,  sizeof( m_nLen ) );
	fFile.Write( &m_nDec,  sizeof( m_nDec ) );

	fFile.Write( &m_nType, sizeof( m_nType ) );

	fFile.Write( &m_nPenWidth, sizeof( m_nPenWidth ) );

	lHelp = m_boRightAlign;
	fFile.Write( &lHelp, sizeof( lHelp ) );

	lHelp = m_boCenterAlign;
	fFile.Write( &lHelp, sizeof( lHelp ) );

	if ( m_pReportFont )
		prFt = m_pReportFont;
	else
		prFt = new wxReportFont();

	lHelp = prFt->fontSize;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = prFt->isBold;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = prFt->isItalic;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = prFt->isStrikeThrough;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = prFt->isUnderline;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = prFt->isDefault;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = prFt->fontFamily;
	fFile.Write( &lHelp, sizeof( lHelp ) );
	SaveWxString( fFile, prFt->fontColor );
	SaveWxString( fFile, prFt->fontName );
	SaveWxString( fFile, prFt->name );

	if ( prFt != m_pReportFont )
		wxDELETE( prFt );

	return true;
}

bool wxReportObj::SaveData( wxFFile &fFile )
{
	wxString strSave;
	wxString strFmt;
	double   dVal;
	long     lVal;

	if ( ! fFile.IsOpened() )
		return false;

	strSave.Empty();

	switch ( m_nType )
	{
		case POTYPE_PCHAR:
			strSave = (const wxChar *)m_pData;
			break;

		case POTYPE_PCHARDBL:
			strSave = (const wxChar *)m_pData;
			if ( strSave.ToDouble( &dVal ) )
				m_dLocal = dVal;
			break;

		case POTYPE_PCHARINT:
			strSave = (const wxChar *)m_pData;
			if ( strSave.ToLong( &lVal ) )
				m_lLocal = lVal;
			break;

		case POTYPE_PSTR:
			strSave = *(wxString *)m_pData;
			break;

		case POTYPE_PSTRDBL:
			strSave = *(wxString *)m_pData;
			if ( strSave.ToDouble( &dVal ) )
				m_dLocal = dVal;
			break;

		case POTYPE_PSTRINT:
			strSave = *(wxString *)m_pData;
			if ( strSave.ToLong( &lVal ) )
				m_lLocal = lVal;
			break;

		case POTYPE_STRING:
			strSave = m_strLocal;
			break;

		case POTYPE_PTIMEST:
			if ( m_pMargin && m_pMargin->m_pShortDateFmt )
			{
				wxDateTime	dt;
				wxVariant	v( (TIMESTAMP_STRUCT *)m_pData );

				dt = v.GetDateTime();
				strSave = dt.Format( *m_pMargin->m_pShortDateFmt );
			}
			break;

		case POTYPE_PDBL:
			m_dLocal = *(double *)m_pData;
			// fall through here!
		case POTYPE_DOUBLE:
			strFmt.Printf( wxT("%%%d.%df"), m_nLen, m_nDec );
			m_strLocal.Printf( strFmt, m_dLocal );
			strSave = m_strLocal;
			break;

		case POTYPE_PINT:
		case POTYPE_PLONG:
			if ( m_nType == POTYPE_PINT )
				m_lLocal = *(int *)m_pData;
			else
				m_lLocal = *(long *)m_pData;
			// fall through here!
		case POTYPE_LONG:
			strFmt.Printf( wxT("%%%dld"), m_nLen );
			m_strLocal.Printf( strFmt, m_lLocal );
			strSave = m_strLocal;
			break;

		case POTYPE_PCHECK:
		case POTYPE_PLOGIC:
			m_lLocal = *(bool *)m_pData;
			m_strLocal.Printf( wxT("%lu"), m_lLocal );
			strSave = m_strLocal;
			break;

		case POTYPE_LINE:
			break;

		case POTYPE_REPDATE:
			strSave = m_strLocal;
			break;

		case POTYPE_PGNUM:
			strSave = m_strLocal;
			break;

		case POTYPE_BITMAP:
			strSave = m_strLocal;
			break;

		default:
			break;

	}

	fFile.Write( &m_nType,  sizeof( m_nType ) );
	fFile.Write( &m_dLocal, sizeof( m_dLocal ) );
	fFile.Write( &m_lLocal, sizeof( m_lLocal ) );

	SaveWxString( fFile, strSave );

	if ( m_nType == POTYPE_BITMAP )
		SaveBitmapData( fFile );

	return true;
}


bool wxReportObj::SaveBitmapData( wxFFile &fFile )
{
	if ( !fFile.IsOpened() )
		return false;

	fFile.Write( &m_sizeOfBitmapFileData, sizeof( m_sizeOfBitmapFileData ) );

	if ( m_sizeOfBitmapFileData == 0 || m_pBitmapFileData == 0 )
		return true;

	fFile.Write( m_pBitmapFileData, m_sizeOfBitmapFileData );

	return true;
}

void wxReportObj::SetPenWidth( int width )
{
	m_nPenWidth = width;
}

// -------------------------------------
//  A report page
//
//  Handles setup of a complete report
//  as well as drawing of a selected
//  page
// -------------------------------------
wxReportWriter::wxReportWriter()
{
	m_strFilename.Empty();
	m_strFilenameTmp.Empty();
	SetPath( wxT("." ) );
	m_strPathOfReports = wxT(".");

	m_lstHeader.DeleteContents( true );
	m_lstFooter.DeleteContents( true );
	m_lstSections.DeleteContents( true );

	m_lstHeader.Clear();
	m_lstFooter.Clear();
	m_lstSections.Clear();

	// Some default values for page-size and margins
	m_PageCtrl.m_dTop			=  10.0;
//	m_PageCtrl.m_dLeft			=  20.0;
	m_PageCtrl.m_dLeft			=  10.0;
	m_PageCtrl.m_dBottom		=  15.0;
	m_PageCtrl.m_dRight			=   0.0;
	m_PageCtrl.m_dYSizeMM		= 297.0; //pageHeight; //297.0;	
	m_PageCtrl.m_dXSizeMM		= 210.0; //pageWidth;  //210.0;
	m_PageCtrl.m_pDateStr		= &m_strDate;
	m_PageCtrl.m_pDateFmt		= &m_strDateFmt;
	m_PageCtrl.m_pShortDateFmt	= &m_strShortDateFmt;

	m_PageCtrl.m_dHeaderHeight	=   0.0;

	m_strDateFmt          = wxT("%m/%d/%Y  %H:%M:%S");
	m_strShortDateFmt     = wxT("%m/%d/%Y");

	m_lDatasets           = 0L;
	m_lCurrentSection     = -1L;

	m_boLoad              = false;
	m_boObjectsSaved      = false;

	// we do not have any objects so far
	m_pObjList            = 0;

	bFinished			  = false;
}

wxReportWriter::~wxReportWriter()
{
	if ( !bFinished )
		FinishReport();

	// De-allocation  of our section list
	while ( m_lstSections.GetCount() > 0 )
		EndSection();
}


void wxReportWriter::DefinitionError( void )
{
    wxLogError( wxT("Wrong definition sequence.\n\n" )
        + wxString(wxT( "Header, Footer and Misc Information not allowed\n" ))
    	+ wxT( "during or after report data!" ) );
}


void wxReportWriter::SetDateFormat( const wxString &strFmt )
{
	m_strDateFmt = strFmt;
}

void wxReportWriter::SetShortDateFormat( const wxString &strFmt )
{
	m_strShortDateFmt = strFmt;
}


void wxReportWriter::SetDate( const wxDateTime &dt )
{
	wxString strDate;

	if ( m_boObjectsSaved )
	{
		DefinitionError();
		return;
	}

	strDate.Printf( wxT("%s"), dt.Format( m_strDateFmt ).c_str() );

	SetDate( strDate );
}



void wxReportWriter::AddHeaderObj( wxReportObj *pObj )
{
	if ( m_boObjectsSaved )
	{
		DefinitionError();
		return;
	}

	pObj->SetMargins( &m_PageCtrl );
	m_lstHeader.Append( pObj );
}

void wxReportWriter::AddFooterObj( wxReportObj *pObj )
{
	if ( m_boObjectsSaved )
	{
		DefinitionError();
		return;
	}

	pObj->SetMargins( &m_PageCtrl );
	m_lstFooter.Append( pObj );
}

void wxReportWriter::AddDataObj( wxReportObj *pObj )
{
	// giving the report object the information about page margins
	pObj->SetMargins( &m_PageCtrl );

	// appending the report object to the current object-list
	if ( m_pObjList )
		m_pObjList->Append( pObj );
}


void wxReportWriter::SetPath( const wxString &strPath )
{
	wxFileName fName( strPath, wxT("") );
	fName.Normalize();

	if ( ! fName.DirExists() )
	{
		if ( ! fName.Mkdir( 0777, true ) )
			m_strPathOfReports = wxT("." );
		else
			m_strPathOfReports = fName.GetLongPath();
	}
	else
		m_strPathOfReports = fName.GetLongPath();
}

bool wxReportWriter::SaveObjects( void )
{
	bool		boRet;
	long		lHelp;
	double		dHelp;
	wxNode		*pNode;
	wxReportObj	*pObj;

	if ( m_strFilename.IsEmpty() )
	{
		wxFileName  fName(m_strPathOfReports, wxT("report") );
		fName.Normalize();
		fName = wxFileName::CreateTempFileName( fName.GetLongPath() );
		m_strFilenameTmp = fName.GetLongPath();
		fName.SetExt( VLPEXT );

		m_strFilename = fName.GetLongPath();

	}

	if ( m_fFile.IsOpened() )
		m_fFile.Close();

	boRet = m_fFile.Open( m_strFilenameTmp, wxT("wb") );
	m_boLoad = false;

	if ( boRet )
	{
		m_lDatasets = 0L;

		m_fFile.Write( VLPHEADER, wxStrlen( VLPHEADER ) );		// File-Header

		lHelp = 0L;		// placeholder for number of data-objects
						//   (becomes set when closing the file by writing
						//    m_lDatasets into this location. This is not
						//    important for functionality; just for information)

		m_fFile.Write( &lHelp, sizeof( lHelp ) );

		lHelp = (long)m_strDate.Len() + 1L;
		m_fFile.Write( &lHelp, sizeof( lHelp ) );
		m_fFile.Write( m_strDate.c_str(), lHelp );

		lHelp = (long)m_strUser.Len() + 1L;
		m_fFile.Write( &lHelp, sizeof( lHelp ) );
		m_fFile.Write( m_strUser.c_str(), lHelp );

		lHelp = (long)m_strInfo.Len() + 1L;
		m_fFile.Write( &lHelp, sizeof( lHelp ) );
		m_fFile.Write( m_strInfo.c_str(), lHelp );

		dHelp = m_PageCtrl.m_dTop;		// Top-Margin
		m_fFile.Write( &dHelp, sizeof( dHelp ) );

		dHelp = m_PageCtrl.m_dLeft;		// Left-Margin
		m_fFile.Write( &dHelp, sizeof( dHelp ) );

		dHelp = m_PageCtrl.m_dBottom;		// Bottom-Margin
		m_fFile.Write( &dHelp, sizeof( dHelp ) );

		dHelp = m_PageCtrl.m_dRight;		// Right-Margin
		m_fFile.Write( &dHelp, sizeof( dHelp ) );

		dHelp = m_PageCtrl.m_dYSizeMM;	// Blattgröße Y
		m_fFile.Write( &dHelp, sizeof( dHelp ) );

		dHelp = m_PageCtrl.m_dXSizeMM;	// Blattgröße X
		m_fFile.Write( &dHelp, sizeof( dHelp ) );

		lHelp = (long)m_PageCtrl.m_iPaperFormat;  // paper format (letter, A4, legal, etc.)
		m_fFile.Write( &lHelp, sizeof( lHelp ) );

		lHelp = (long)m_lstHeader.GetCount();	// Anzahl Header-Objekte
		m_fFile.Write( &lHelp, sizeof( lHelp ) );

		lHelp = (long)m_lstFooter.GetCount();	// Anzahl Footer-Objekte
		m_fFile.Write( &lHelp, sizeof( lHelp ) );


		// Header-Objekte speichern
		// ------------------------
		pNode = (wxNode *)m_lstHeader.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObj *)pNode->GetData();

			pObj->SaveSettings( m_fFile );

			pNode = pNode->GetNext();

		}

		// Footer-Objekte speichern
		// ------------------------
		pNode = (wxNode *)m_lstFooter.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObj *)pNode->GetData();

			pObj->SaveSettings( m_fFile );

			pNode = pNode->GetNext();

		}

		// Header-Daten speichern
		// ----------------------
		pNode = (wxNode *)m_lstHeader.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObj *)pNode->GetData();
			pObj->SaveData( m_fFile );

			pNode = pNode->GetNext();
		}

		// Footer-Daten speichern
		// ----------------------
		pNode = (wxNode *)m_lstFooter.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObj *)pNode->GetData();
			pObj->SaveData( m_fFile );

			pNode = pNode->GetNext();
		}

		
		m_boObjectsSaved = true;

		// File stays open!!!
	}

	return boRet;
}


bool wxReportWriter::SaveData( void )
{
	wxNode		*pNode;
	wxReportObj	*pObj;

	if ( ! m_boObjectsSaved )
		SaveObjects();

	if ( ! m_fFile.IsOpened() )
		return false;

	pNode = (wxNode *)m_pObjList->GetFirst();
	while (pNode)
	{
		pObj = (wxReportObj *)pNode->GetData();
		pObj->SaveData( m_fFile );

		pNode = pNode->GetNext();
	}

	++m_lDatasets;

	return true;
}


long wxReportWriter::DefineSection( void )
{
	// adding an (empty) list to the section-list
	wxReportSection	*pSection;

	pSection = new wxReportSection;

	if ( ! pSection )
		return m_lCurrentSection;

	pSection->m_pList            = new wxReportObjList;
	pSection->m_lPreviousSection = m_lCurrentSection;
	pSection->m_lSections        = (long)m_lstSections.GetCount();

	if ( pSection->m_pList )
	{
		m_lstSections.Append( pSection );
		m_lCurrentSection = (long)m_lstSections.GetCount() - 1;
		m_pObjList = pSection->m_pList;

	}

	return m_lCurrentSection;	// this is the new section
}


long wxReportWriter::PreviousSection( long lCurrentSection )
{
	if ( lCurrentSection < 0L )
		return lCurrentSection;

	if ( lCurrentSection >= (long)m_lstSections.GetCount() )
		return -1L;

	wxReportSection	*pSection;
	wxNode			*pNode;

	pNode = (wxNode *)m_lstSections.Item( lCurrentSection );
	if (pNode)
	{
		pSection = (wxReportSection *)pNode->GetData();
		return pSection->m_lPreviousSection;
	}

	return -1;
}


void wxReportWriter::EndSection( void )
{
	// removing the LAST object-list from the list of sections
	wxReportSection	*pSection;
	wxNode			*pNode;

	pNode = (wxNode *)m_lstSections.GetLast();
	if (pNode)
	{
		pSection = (wxReportSection *)pNode->GetData();

		if ( pSection )
		{
			//pSection->m_pList->DeleteContents( true );
			wxwxReportObjListNode *node = pSection->m_pList->GetFirst();
			while (node)
			{
				DeleteReportObj( node->GetData() );
				node = node->GetNext();
			}
			pSection->m_pList->Clear();
		}

		// positioning to previous list...
		pNode = pNode->GetPrevious();
		if ( pNode )
			m_pObjList = ((wxReportSection *)pNode->GetData())->m_pList;
		else
			m_pObjList = 0;
		
		m_lstSections.DeleteNode( m_lstSections.GetLast() );

		m_lCurrentSection = (long)m_lstSections.GetCount() - 1;

	}
}


// Saves a POTYPE_SUBSTART and n settings
void wxReportWriter::BeginDataSection( void )
{
	wxNode		*pNode;
	wxReportObj	*pObj;
	wxReportObj	obj;

	if ( ! m_pObjList )
		return;	// No sections defined yet

	// (fixed) objects (header/footer) must have been saved before!!!
	if ( ! m_boObjectsSaved )
		SaveObjects();

	// Saving a POTYPE_SUBSTART object which contains the
	// number of following object settings
	obj.SetSubReport( (long)m_pObjList->GetCount() );
	obj.SaveData( m_fFile );

	// The object settings...
	pNode = (wxNode *)m_pObjList->GetFirst();
	while (pNode)
	{
		pObj = (wxReportObj *)pNode->GetData();
		pObj->SaveSettings( m_fFile );

		pNode = pNode->GetNext();
	}

}

// Saves a POTYPE_SUBEND
void wxReportWriter::EndDataSection( void )
{
	wxReportObj	obj;

	// Saving a POTYPE_SUBEND object
	obj.SetSubReportEnd();
	obj.SaveData( m_fFile );
}


// Saves a POTYPE_PGBREAK
void wxReportWriter::NewPage( void )
{
	wxReportObj	obj;

	// Saving a POTYPE_PGBREAK object
	obj.SetPageBreak();
	obj.SaveData( m_fFile );
}


// Saves a POTYPE_LINE
void wxReportWriter::HorizLine( double dX, double dLen, double dYInc )
{
	wxReportObj	obj( dX, 0.0, dLen, 0.0 );

	// Saving a POTYPE_LINE object
	obj.SetLine( dX, 0.0 );
	obj.SetIncrements( 0.0, dYInc );
	obj.SaveData( m_fFile );
}



wxString wxReportWriter::LoadWxString( wxFFile *pFile )
{
	long lHelp;
	wxString strRet;
	wxFFile *pLoadFile;

	if ( pFile )
		pLoadFile = pFile;
	else
		pLoadFile = &m_fFile;

	strRet.Empty();

	if ( ! pLoadFile->IsOpened() )
		return strRet;

	if ( sizeof( lHelp ) == pLoadFile->Read( &lHelp, sizeof( lHelp ) ) )
	{
		if ( lHelp > 0 )
		{
			wxChar *cp;

			cp = new wxChar[ lHelp ];

			if ( cp )
			{
				pLoadFile->Read( cp, lHelp );
				*(cp + lHelp - 1) = 0x00;
				strRet = cp;

				delete [] cp;
			}
		}
	}

	return strRet;
}

long wxReportWriter::LoadReportInfo( wxString strFile )
{
	bool		boRet;
	long		lHelp;
	wxChar		szHelp[ 20 ];
	wxString	strData;
	wxFFile		f;

	if ( strFile.IsEmpty() )
		return -1;

	boRet = f.Open( strFile, wxT("rb") );

	if ( boRet )
	{
		// Is this a report file?
		f.Read( szHelp, wxStrlen( VLPHEADER ) );
		szHelp[ wxStrlen( VLPHEADER ) ] = 0x00;
		strData = szHelp;

		if ( strData != VLPHEADER )
		{
			f.Close();
			return -3L;	// wrong file header. Not a report file!
		}

		// Number of saved datasets (informational only)
		f.Read( &lHelp, sizeof( lHelp ) );

		m_strDate = LoadWxString( &f );
		m_strUser = LoadWxString( &f );
		m_strInfo = LoadWxString( &f );

		f.Close();

		return 0;
	}

	return -2L;
}


void wxReportWriter::FinishReport( void )
{
	// If Report ONLY has header/footer objects,
	// .SaveData has never been called. Thus the
	// header/footer objects have never been saved!
	if ( ! m_boLoad )
		if ( ! m_boObjectsSaved )
			SaveObjects();

	if ( m_fFile.IsOpened() )
	{
		if ( ! m_boLoad )
			if ( m_fFile.Seek( wxStrlen( VLPHEADER ) ) )
				m_fFile.Write( &m_lDatasets, sizeof( m_lDatasets ) );

		m_fFile.Close();

		if ( ! m_boLoad )
		{
			wxRenameFile( m_strFilenameTmp, m_strFilename );
		}
	}
}

wxString wxReportWriter::FinalDestination( void )
{
	// If Report is finished, we can supply its
	// final filename
	//
	// (A Report is finished when the output file is
	//  closed AND we're not loading it!)
	if ( ! m_fFile.IsOpened() && ! m_boLoad )
		return m_strFilename;

	return wxEmptyString;
}


bool wxReportWriter::LoadFile( wxString strFile )
{
	bool boRet;

	/*if ( strFile.IsEmpty() )
	{
		wxFileDialog dlg( this, wxT("Choose a Text"),
			              wxT("."), wxT(""),
						  wxT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*"),
						  wxOPEN );

		if ( wxID_OK != dlg.ShowModal() )
			return false;

		strFile = dlg.GetFilename();
	}*/
wxMessageBox("loadfile msg 1");
	m_aPageStart.Clear();

	if ( m_fFile.IsOpened() )
		m_fFile.Close();

	boRet = m_fFile.Open( strFile, wxT("rb") );

	if ( boRet )
	{
		long lRead;
		long lSizeOfFile;
		char szDummy[12];
		char *cpFF;

		lSizeOfFile = m_fFile.Length();

		m_aPageStart.Add( 0L );
		m_lNextPageInFile = 0;

		do
		{
			lRead = (long)m_fFile.Read( szDummy, sizeof( szDummy ) - 1 );
			szDummy[ lRead ] = 0x00;
			wxMessageBox("loadfile msg 2");
			while ( lRead )
			{
				wxMessageBox("loadfile msg 3");
				cpFF = strchr( szDummy, '\014' );
				if ( NULL != cpFF )
				{
					m_lNextPageInFile += (cpFF - szDummy) + 1;
					lRead -= (cpFF - szDummy) + 1;
			
					// If get a FF at the end, and after that only
					// CR and/or LF, we don't add a "new" page
					if ( m_lNextPageInFile < (lSizeOfFile - 1) )
					{
						wxMessageBox("loadfile msg 4");
						m_aPageStart.Add( m_lNextPageInFile );
						strcpy( szDummy, cpFF + 1 );
					}
					else
					{
						// That's it....!
						wxMessageBox("loadfile msg 5");
						lRead = 0;
					}
				}
				else
				{
					wxMessageBox("loadfile msg 6");
					m_lNextPageInFile += lRead;
					lRead = 0;
				}

			} // while lRead

		} while ( ! m_fFile.Eof() );

		for ( unsigned int i = 0; i < m_aPageStart.GetCount(); ++i )
			m_lCurrentFilePos = m_aPageStart[ i ];

		m_lNextPageInFile = 0;
		m_lCurrentFilePos = 0;
		m_PageCtrl.m_nCurrentPage = 0;

		m_fFile.Seek( m_lCurrentFilePos );

	} // file open()

	return boRet;
}


int  wxReportWriter::GetNumberOfPages()
{
	return (int)m_aPageStart.GetCount();
}


void wxReportWriter::SetPage( unsigned int uiPage )
{
	if ( uiPage > 0 && uiPage <= m_aPageStart.GetCount() )
		m_PageCtrl.m_nCurrentPage = uiPage - 1;
	m_PageCtrl.m_nPages = (long)m_aPageStart.GetCount();
}

void wxReportWriter::DeleteReportObj( wxReportObj* obj )
{
	wxDELETE( obj );
}

 /*void wxReportWriter::SetOrientation( int iMode )
{
   if ( iMode == wxLANDSCAPE )
	{
        m_PageCtrl.m_dYSizeMM = 210.0;
        m_PageCtrl.m_dXSizeMM = 297.0;
    }
    else
	{
        m_PageCtrl.m_dYSizeMM = 297.0;
        m_PageCtrl.m_dXSizeMM = 210.0;
    }
}*/
 

// -------------------------------------
//  Report Printout Handling
// -------------------------------------
