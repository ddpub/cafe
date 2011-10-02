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
#pragma implementation "wx_report/repwrt.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#if !wxUSE_PRINTING_ARCHITECTURE
#error You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h to compile this program.
#endif

// Set this to 1 if you want to test PostScript printing under MSW.
// However, you'll also need to edit src/msw/makefile.nt.
#define wxTEST_POSTSCRIPT_IN_MSW 0

#include <ctype.h>

#include <wx/filename.h>
#include <wx/ffile.h>
#include <wx/metafile.h>
#include <wx/print.h>
#include <wx/printdlg.h>
#include <wx/image.h>

#include <wx/accel.h>
#include <wx/dynarray.h>

#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
#include <wx/choicdlg.h>
#include <wx/fontdlg.h>
#include <wx/fontenum.h>
#include <wx/fontmap.h>
#include <wx/encconv.h>
#include <wx/textfile.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include "wx/datetime.h"



#if wxTEST_POSTSCRIPT_IN_MSW
#include "wx/generic/printps.h"
#include "wx/generic/prntdlgg.h"
#endif

#include <wx_report/repwrt.h>

#include <wx/listimpl.cpp>
WX_DEFINE_EXPORTED_LIST(wxReportObjList);
WX_DEFINE_EXPORTED_LIST(wxReportSectionList);

#define VLPHEADER   wxT("VLP5")		// File-Header for report-files
#define VLPEXT      wxT("vlp")		// File-Extension for report-files
#define HLINE_INC	20				// a horizontal line increments the Y pos
									//    by 0.2mm

// Global Printer- and Setup-Settings

#if defined(USE_OLD_PRINTSETTING)

// Global print data, to remember settings during the session
wxPrintData *g_printData = (wxPrintData*) NULL ;

// Global page setup data
wxPageSetupData* g_pageSetupData = (wxPageSetupData*) NULL;

#else

wxPrintSettings g_PrintSettings;

#endif


// -----------------------------------------------------
// Class (for preview) which is aware of wxReportWriter
// -----------------------------------------------------
BEGIN_EVENT_TABLE(ReportPreviewFrame, wxPreviewFrame)
    EVT_CLOSE(ReportPreviewFrame::OnCloseWindow)
END_EVENT_TABLE()

ReportPreviewFrame::ReportPreviewFrame(
		wxPrintPreview* preview,
		wxFrame* parent,
		const wxString& title,
		const wxPoint& pos,
		const wxSize& size,
		long  style,
		const wxString& name)
: wxPreviewFrame( preview, parent, title, pos, size, style, name )
{
	m_pRepWrt = 0;
}


ReportPreviewFrame::~ReportPreviewFrame( void )
{
}

void ReportPreviewFrame::OnCloseWindow( wxCloseEvent &event )
{
	// When the preview frame closes, we need to close
	// the corresponding report file also. This is done
	// in ...FinishReport()
	if ( m_pRepWrt )
	{
		m_pRepWrt->FinishReport();
	}

	wxPreviewFrame::OnCloseWindow( event );
}



// -------------------------------------
// Class to keep settings for Printer
// -------------------------------------
wxPrintSettings::wxPrintSettings( void )
{
	m_pPrintData     = (wxPrintData*) NULL ;
	m_pPageSetupData = (wxPageSetupData*) NULL;
}

wxPrintSettings::~wxPrintSettings( void )
{
	Free();
}

void wxPrintSettings::Free( void )
{
	if ( m_pPrintData )
		delete m_pPrintData;
	if ( m_pPageSetupData )
		delete m_pPageSetupData;

	m_pPrintData     = 0;
	m_pPageSetupData = 0;
}

wxPrintData & wxPrintSettings::GetPrintData( void )
{
	if ( ! m_pPrintData )
		m_pPrintData = new wxPrintData();
	if ( ! m_pPageSetupData )
		m_pPageSetupData = new wxPageSetupDialogData;

	return (* m_pPrintData);
}

wxPageSetupData & wxPrintSettings::GetPageSetupData( void )
{
	if ( ! m_pPrintData )
		m_pPrintData = new wxPrintData;
	if ( ! m_pPageSetupData )
		m_pPageSetupData = new wxPageSetupDialogData;

	(*m_pPageSetupData) = * m_pPrintData;

	return (* m_pPageSetupData);
}

void wxPrintSettings::SetPrintData( wxPrintData &printdata )
{
	if ( m_pPrintData )
		(*m_pPrintData) = printdata;
}

void wxPrintSettings::SetPageSetupData( wxPageSetupData &setupdata )
{
	if ( m_pPageSetupData )
		(*m_pPageSetupData) = setupdata;
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
	m_pBitmap      = 0;
	m_pMargin      = 0;
	m_dLocal       = 0.0;
	m_lLocal       = 0;
	m_dSum         = 0.0;
	m_lSum         = 0;
	m_nLen         = 0;
	m_nDec         = 0;
	m_nType        = POTYPE_NONE;
	m_pFont        = 0;
	m_boRightAlign = false;
	m_dScale       = 3.78;		// just a default so far

	SetInitPosition( dXPos, dYPos );
	m_sizSize.x    = (int)(100.0 * dXSize);
	m_sizSize.y    = (int)(100.0 * dYSize);

	m_ptCurrPos    = m_ptInitPos;
	m_sizInc.x     = 0;
	m_sizInc.y     = m_sizSize.y;

}

wxReportObj::~wxReportObj( void )
{
	if ( m_pFont )
		delete m_pFont;

	m_pFont = 0;

	if ( m_pBitmap )
		delete m_pBitmap;

	m_pBitmap = 0;
}

void wxReportObj::SetFont( const wxFont *pFt )
{
	if ( m_pFont )
		delete m_pFont;

	m_pFont = new wxFont( 
		pFt->GetPointSize(),
		pFt->GetFamily(),
		pFt->GetStyle(),
		pFt->GetWeight(),
		pFt->GetUnderlined(),
		pFt->GetFaceName(),
		pFt->GetEncoding() );
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
#if wxUSE_LIBPNG
    wxImage image;
	wxLogNull logNull;

    if ( image.LoadFile( strFile ) )
	{
        m_pBitmap = new wxBitmap( image );

		m_nType = POTYPE_BITMAP;
		m_strLocal = strFile;
	}
#endif // wxUSE_LIBPNG
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
void wxReportObj::Draw( wxDC & dc )
{
	wxString strPrint;
	double   dVal;
	int      iWid, iHei;
	long     lVal;
	long     lXP, lYP, lXS, lYS;

	// In all cases the output data is available as 
	// a string in m_strLocal

	dVal = 0.0;
	lVal = 0L;

/*
	if ( m_nType == POTYPE_PTIMEST )
	{
		if ( m_pMargin && m_pMargin->m_pShortDateFmt )
		{
			wxString			strSub;
			wxDateTime			dt;
			TIMESTAMP_STRUCT	TS;

			strSub = m_strLocal.Mid( 0, 4 );
			TS.year = wxAtoi( strSub );

			strSub = m_strLocal.Mid( 4, 2 );
			TS.month = wxAtoi( strSub );
			
			strSub = m_strLocal.Mid( 6, 2 );
			TS.day = wxAtoi( strSub );

			strSub = m_strLocal.Mid( 8, 2 );
			TS.hour = wxAtoi( strSub );

			strSub = m_strLocal.Mid( 10, 2 );
			TS.minute = wxAtoi( strSub );

			strSub = m_strLocal.Mid( 12, 2 );
			TS.second = wxAtoi( strSub );

			TS.fraction = 0;

			wxVariant v( &TS );

			dt = v.GetDateTime();
			strPrint = dt.Format( *m_pMargin->m_pShortDateFmt );
		}
		else
			strPrint = m_strLocal;
	}
	else
*/
	{
		// string representation always in m_strLocal
		strPrint = m_strLocal;
	}

	// page numbers must be handled seperately
	if ( m_nType == POTYPE_PGNUM )
	{
		if ( m_pMargin )
		{
			wxString strPgNum;
			strPgNum.Printf( wxT("%ld/%ld"),
				1 + m_pMargin->m_nCurrentPage, m_pMargin->m_nPages );
			strPrint += wxT(" ");
			strPrint += strPgNum;
		}
		else
			strPrint = wxT("PGNUM?");
	}

	// date must be handled seperately also
	if ( m_nType == POTYPE_REPDATE )
	{
		if ( m_pMargin && m_pMargin->m_pDateStr )
			strPrint = *m_pMargin->m_pDateStr;
		else
			strPrint = wxT("REPDATE?");
	}

	// calculating position and size
	//   internal datas are in  1/100 Millimeters;
	//   external datas are in  Millimeters
	//
	//   For getting logical coordinates, we use the scale
	//   which was calculated by CalcScale()

	if ( m_pMargin )
	{
		// we have a pointer to the page margins
		lXP = (long)(m_dScale * 0.01 * (m_ptCurrPos.x + 100.0 * m_pMargin->m_dLeft));
		lYP = (long)(m_dScale * 0.01 * (m_ptCurrPos.y + 100.0 * m_pMargin->m_dTop));
		lXS = (long)(m_dScale * 0.01 * m_sizSize.x);
		lYS = (long)(m_dScale * 0.01 * m_sizSize.y);
	}
	else
	{
		// we don't know about page margins
		lXP = (long)(m_dScale * 0.01 * m_ptCurrPos.x );
		lYP = (long)(m_dScale * 0.01 * m_ptCurrPos.y);
		lXS = (long)(m_dScale * 0.01 * m_sizSize.x);
		lYS = (long)(m_dScale * 0.01 * m_sizSize.y);
	}


	switch ( m_nType )
	{
	case POTYPE_PLOGIC:
	case POTYPE_PCHECK:
		{
			const wxPen *pCurrPen;
			long  lOffs;

			pCurrPen = & dc.GetPen();
			dc.SetPen( * wxBLACK_PEN );
			lOffs = lYS / 3;

			if ( m_nLen > 0 )
			{
				// Draw a box
				dc.DrawLine( lXP +   0, lYP - lOffs,       lXP + lXS, lYP - lOffs );
				dc.DrawLine( lXP + lXS, lYP - lOffs,       lXP + lXS, lYP - lOffs - lYS );
				dc.DrawLine( lXP + lXS, lYP - lOffs - lYS, lXP +   0, lYP - lOffs - lYS );
				dc.DrawLine( lXP +   0, lYP - lOffs - lYS, lXP +   0, lYP - lOffs );
			}

			if ( m_lLocal != 0 )
			{
				if ( m_nType == POTYPE_PLOGIC )
				{
					long lAy, lBx;

					lAy = (lYP + lYP - lYS) / 2;
					lBx = (3 * lXP + lXS) / 3;

					dc.DrawLine( lXP + 1, lAy - lOffs + 1, lBx, lYP - lOffs );
					dc.DrawLine( lBx,     lYP - lOffs,     lXP + lXS - 1, lYP - lOffs - lYS + 1 );
				}
				else
				{
					dc.DrawLine( lXP, lYP - lOffs,       lXP + lXS, lYP - lOffs - lYS );
					dc.DrawLine( lXP, lYP - lOffs - lYS, lXP + lXS, lYP - lOffs );
				}
			}

			dc.SetPen( * pCurrPen );

			MoveForward();
		}
		break;

	case POTYPE_BITMAP:
		// Object is a bitmap
		// ------------------
		if ( ! m_pBitmap )
			SetBitmap( m_strLocal );

		if ( m_pBitmap && m_pBitmap->Ok() )
		{
	        dc.DrawBitmap( *m_pBitmap, lXP, lYP );

			MoveForward();
	    }
		break;

	case POTYPE_LINE:
		{
			const wxPen *pCurrPen;

			pCurrPen = & dc.GetPen();
			dc.SetPen( * wxBLACK_PEN );
			dc.DrawLine( lXP, lYP, lXP + lXS, lYP + lYS );
			dc.SetPen( * pCurrPen );

			MoveForward();
		} break;

	case POTYPE_PGNUM:
	case POTYPE_REPDATE:
//		break;

	default:
		if ( m_pFont )
			dc.SetFont( *m_pFont );

		dc.GetTextExtent( strPrint, &iWid, &iHei );
		lYP -= iHei;

		dc.SetClippingRegion( lXP, lYP, lXS, lYS );

		// drawing position is likely to change
		// with right-aligned text
		if ( m_boRightAlign )
		{
			if ( m_pMargin )
			{
				lXP = (long)(m_dScale * 0.01 * (m_ptCurrPos.x + m_sizSize.x + 100.0 * m_pMargin->m_dLeft));
				lXP -= (iWid + 2);
			}
			else
			{
				lXP = (long)(m_dScale * 0.01 * (m_ptCurrPos.x + m_sizSize.x) );
				lXP -= (iWid + 2);
			}
		}

		dc.DrawText( strPrint, lXP, lYP );
		dc.DestroyClippingRegion();

		MoveForward();

		break;

	} // switch
}


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
	long		lHelp;
	wxString	strFace;
	const wxFont		*pFt;

	if ( ! fFile.IsOpened() )
		return false;

	fFile.Write( &m_ptInitPos, sizeof( wxPoint ) );
	fFile.Write( &m_sizSize,   sizeof( wxSize ) );
	fFile.Write( &m_sizInc,    sizeof( wxSize ) );

	fFile.Write( &m_nLen,  sizeof( m_nLen ) );
	fFile.Write( &m_nDec,  sizeof( m_nDec ) );

	fFile.Write( &m_nType, sizeof( m_nType ) );

	lHelp = m_boRightAlign;
	fFile.Write( &lHelp, sizeof( lHelp ) );

	if ( m_pFont )
		pFt = m_pFont;
	else
		pFt = wxSWISS_FONT;

	lHelp = pFt->GetPointSize();
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = pFt->GetFamily();
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = pFt->GetStyle();
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = pFt->GetWeight();
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = pFt->GetUnderlined();
	fFile.Write( &lHelp, sizeof( lHelp ) );
	lHelp = (long)pFt->GetDefaultEncoding();
	fFile.Write( &lHelp, sizeof( lHelp ) );

	strFace = pFt->GetFaceName();
	SaveWxString( fFile, strFace );

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

	return true;
}


wxString wxReportObj::LoadWxString( wxFFile &fFile )
{
	long lHelp;
	wxString strRet;

	strRet.Empty();

	if ( ! fFile.IsOpened() )
		return strRet;

	if ( sizeof( lHelp ) == fFile.Read( &lHelp, sizeof( lHelp ) ) )
	{
		if ( lHelp > 0L )
		{
			wxChar *cp;

			cp = new wxChar[ lHelp ];

			if ( cp )
			{
				fFile.Read( cp, lHelp );
				*(cp + lHelp - 1) = 0x00;
				strRet = cp;

				delete [] cp;
			}
		}
	}

	return strRet;
}

bool wxReportObj::LoadSettings( wxFFile &fFile )
{
	long		lHelp;
	wxString	strFace;
	long		lPoint, lFam, lStyle, lWeight;
	bool		boUl;
    wxFontEncoding eEnc;

	if ( ! fFile.IsOpened() )
		return false;

	fFile.Read( &m_ptInitPos, sizeof( wxPoint ) );
	fFile.Read( &m_sizSize,   sizeof( wxSize  ) );
	fFile.Read( &m_sizInc,    sizeof( wxSize  ) );

	fFile.Read( &m_nLen,      sizeof( m_nLen  ) );
	fFile.Read( &m_nDec,      sizeof( m_nDec  ) );

	fFile.Read( &m_nType,     sizeof( m_nType ) );

	fFile.Read( &lHelp,       sizeof( lHelp   ) );
	m_boRightAlign = ( 0L != lHelp );

	fFile.Read( &lPoint,      sizeof( lPoint  ) );
	fFile.Read( &lFam,        sizeof( lFam    ) );
	fFile.Read( &lStyle,      sizeof( lStyle  ) );
	fFile.Read( &lWeight,     sizeof( lWeight ) );
	fFile.Read( &lHelp,       sizeof( lHelp   ) );
	boUl = ( 0L != lHelp );
	fFile.Read( &lHelp,       sizeof( lHelp   ) );
	eEnc = (wxFontEncoding)lHelp;

	strFace = LoadWxString( fFile );
 
	if ( m_pFont )
		delete m_pFont;

	m_pFont = new wxFont( lPoint, lFam, lStyle, lWeight, boUl, strFace, eEnc );

	return true;
}

bool wxReportObj::LoadData( wxFFile &fFile )
{
	wxString	strSave;

	if ( ! fFile.IsOpened() )
		return false;

	fFile.Read( &m_nType, sizeof( m_nType ) );
	if ( fFile.Eof() )
		return false;

	fFile.Read( &m_dLocal, sizeof( m_dLocal ) );
	if ( fFile.Eof() )
		return false;

	fFile.Read( &m_lLocal, sizeof( m_lLocal ) );
	if ( fFile.Eof() )
		return false;

	m_strLocal = LoadWxString( fFile );
	if ( fFile.Eof() )
		return false;

	return true;
}



// -------------------------------------
//  A report page
//
//  Handles setup of a complete report
//  as well as drawing of a selected
//  page
// -------------------------------------
wxReportWriter::wxReportWriter(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size):
wxFrame(frame, -1, title, pos, size)
{
#if defined(USE_OLD_PRINTSETTING)
    if ( ! g_printData )
	    g_printData = new wxPrintData;
	if ( ! g_pageSetupData )
    g_pageSetupData = new wxPageSetupDialogData;
#endif
	m_pPrintout = 0;

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
	m_PageCtrl.m_dYSizeMM		= 297.0;	
	m_PageCtrl.m_dXSizeMM		= 210.0;
	m_PageCtrl.m_pDateStr		= &m_strDate;
	m_PageCtrl.m_pDateFmt		= &m_strDateFmt;
	m_PageCtrl.m_pShortDateFmt	= &m_strShortDateFmt;

	m_strDateFmt          = wxT("%m/%d/%Y  %H:%M:%S");
	m_strShortDateFmt     = wxT("%m/%d/%Y");

	m_lDatasets           = 0L;
	m_lCurrentSection     = -1L;

	m_boLoad              = false;
	m_boObjectsSaved      = false;

	// we do not have any objects so far
	m_pObjList           = 0;
}

wxReportWriter::~wxReportWriter()
{
#if defined(USE_OLD_PRINTSETTING)
    if ( g_printData )
	    delete g_printData;
	if ( g_pageSetupData )
		delete g_pageSetupData;

    g_printData     = 0;
    g_pageSetupData = 0;
#else
	g_PrintSettings.Free();
#endif

	FinishReport();

	// De-allocation  of our section list
	while ( m_lstSections.GetCount() > 0 )
		EndSection();
}


void wxReportWriter::DefinitionError( void )
{
	wxMessageBox( wxT(	"Wrong definition sequence.\n\n" )
						+ wxString(wxT( "Header, Footer and Misc Information not allowed\n" ))
						+ wxT( "during or after report data!" ),
		          wxT("Error"), wxOK | wxICON_HAND | wxCENTRE );
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

	boRet = m_fFile.Open( m_strFilenameTmp, _("wb") );
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

		lHelp = m_lstHeader.GetCount();	// Anzahl Header-Objekte
		m_fFile.Write( &lHelp, sizeof( lHelp ) );

		lHelp = m_lstFooter.GetCount();	// Anzahl Footer-Objekte
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
	pSection->m_lSections        = m_lstSections.GetCount();

	if ( pSection->m_pList )
	{
		m_lstSections.Append( pSection );
		m_lCurrentSection = m_lstSections.GetCount() - 1;
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
			pSection->m_pList->DeleteContents( true );
			pSection->m_pList->Clear();
		}

		// positioning to previous list...
		pNode = pNode->GetPrevious();
		if ( pNode )
			m_pObjList = ((wxReportSection *)pNode->GetData())->m_pList;
		else
			m_pObjList = 0;
		
		m_lstSections.DeleteNode( m_lstSections.GetLast() );

		m_lCurrentSection = m_lstSections.GetCount() - 1;

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
	obj.SetSubReport( m_pObjList->GetCount() );
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
	wxReportObj	obj( 0.0, 0.0, 0.0, 0.0 );

	// Saving a POTYPE_LINE object
	obj.SetLine( 0.0, 0.0 );
//	obj.SetIncrements( 0.0, dYInc );
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

	boRet = f.Open( strFile, _("rb") );

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


long wxReportWriter::SetupReport( wxString strFile )
{
	bool		boRet;
	bool		boLoad;
	long		lHelp;
	long		lFilePos;
	wxChar		szHelp[256];
	wxString	strData;
	long		nHeaderObj;
	long		nFooterObj;
	long		lSection;
	double		dY;
	wxNode		*pNode;
	wxReportObj	*pObj;
	wxReportObj	objHelp;
	bool		boEof;
	wxPoint		pt;

	if ( strFile.IsEmpty() )
	{
		wxFileDialog dlg( this, wxT("Loading a report..."),
			              m_strPathOfReports, wxT(""),
						  wxT("Report (*.vlp)|*.vlp|All Files (*.*)|*.*"),
						  wxOPEN );

		if ( wxID_OK != dlg.ShowModal() )
			return -1L;

		strFile = dlg.GetPath();
	}

	if ( m_fFile.IsOpened() )
		m_fFile.Close();

	boRet = m_fFile.Open( strFile, _("rb") );

	m_boLoad         = true;
	m_boObjectsSaved = false;

	if ( boRet )
	{
		// No data so far (page-start positions, section information)
		m_aPageStart.Clear();
		m_aSection.Clear();

		// No datasets so far
		m_lDatasets = 0L;

		// Is this a report file?
		m_fFile.Read( szHelp, wxStrlen( VLPHEADER ) );
		szHelp[ wxStrlen( VLPHEADER ) ] = 0x00;
		strData = szHelp;

		if ( strData != VLPHEADER )
		{
			return -2L;	// wrong file header. Not a report file!
		}

		// Number of saved datasets (informational only)
		m_fFile.Read( &m_lDatasets, sizeof( lHelp ) );

		m_strDate = LoadWxString();
		m_strUser = LoadWxString();
		m_strInfo = LoadWxString();

		m_fFile.Read( &m_PageCtrl.m_dTop,     sizeof( m_PageCtrl.m_dTop     ) );
		m_fFile.Read( &m_PageCtrl.m_dLeft,    sizeof( m_PageCtrl.m_dLeft    ) );
		m_fFile.Read( &m_PageCtrl.m_dBottom,  sizeof( m_PageCtrl.m_dBottom  ) );
		m_fFile.Read( &m_PageCtrl.m_dRight,   sizeof( m_PageCtrl.m_dRight   ) );
		m_fFile.Read( &m_PageCtrl.m_dYSizeMM, sizeof( m_PageCtrl.m_dYSizeMM ) );
		m_fFile.Read( &m_PageCtrl.m_dXSizeMM, sizeof( m_PageCtrl.m_dXSizeMM ) );

		m_lstHeader.DeleteContents( true );
		m_lstFooter.DeleteContents( true );
		m_lstSections.DeleteContents( true );

		m_lstHeader.Clear();
		m_lstFooter.Clear();
		m_lstSections.Clear();

		m_fFile.Read( &nHeaderObj, sizeof( nHeaderObj ) );
		m_fFile.Read( &nFooterObj, sizeof( nFooterObj ) );

		// Loading header-objects
		// ----------------------
		for ( lHelp = 0L; lHelp < nHeaderObj; ++lHelp )
		{
			pObj = new wxReportObj( 0.0, 1.1, 2.2, 3.3 );
			pObj->LoadSettings( m_fFile );
			pObj->ResetPos();
			AddHeaderObj( pObj );
		}

		// Loading footer-objects
		// ----------------------
		for ( lHelp = 0L; lHelp < nFooterObj; ++lHelp )
		{
			pObj = new wxReportObj( 0.0, 1.1, 2.2, 3.3 );
			pObj->LoadSettings( m_fFile );
			pObj->ResetPos();
			AddFooterObj( pObj );

		}


		// Loading header data...
		// ----------------------
		pNode = (wxNode *)m_lstHeader.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObj *)pNode->GetData();
			pObj->LoadData( m_fFile );

			pNode = pNode->GetNext();
		}


		// Loading footer data...
		// ----------------------

		// take care on margins 
		m_PageCtrl.m_dFooterMinY = m_PageCtrl.m_dYSizeMM - m_PageCtrl.m_dBottom;

		pNode = (wxNode *)m_lstFooter.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObj *)pNode->GetData();
			pObj->LoadData( m_fFile );

			// smallest Y coordinate calculation
			dY = m_PageCtrl.m_dYSizeMM - m_PageCtrl.m_dBottom
			   - 0.01 * pObj->GetInitPos().y;
			if ( dY < m_PageCtrl.m_dFooterMinY )
				m_PageCtrl.m_dFooterMinY = dY;

			pNode = pNode->GetNext();
		}


		// Well, this was the part with FIXED data.
		// Variable data may follow

		// The first record MUST contain a POTYPE_SUBSTART
		// record with the number of objects


		m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop;

		boEof    = false;
		lSection = -1L;

		while ( ! boEof )
		{
			// File should have a section start as the first record!

			lFilePos = m_fFile.Tell();

			boLoad = objHelp.LoadData( m_fFile );
			if ( ! boLoad )
			{
				boEof = true;
				continue;
			}

			// Checking that report starts with a POTYPE_SUBSTART
			if ( lSection == -1L )
			{
				if ( objHelp.GetType() != POTYPE_SUBSTART )
				{
					boEof = true;
					wxMessageBox( wxT("Report File corrupt") );

					continue;
				}
			}

			switch ( objHelp.GetType() )
            {
				case POTYPE_LINE:
					lFilePos = m_fFile.Tell();

					// advance object's Y positions by 0.2mm
					pNode = (wxNode *)m_pObjList->GetFirst();
					while (pNode)
					{
						pObj = (wxReportObj *)pNode->GetData();

						pt = pObj->GetCurrPos();
						pt.y += HLINE_INC;
						pObj->SetCurrPos( pt );

						pNode = pNode->GetNext();
					}
					break;

				case POTYPE_PGBREAK:
					lFilePos = m_fFile.Tell();

					m_aPageStart.Add( lFilePos );
					m_aSection.Add( lSection );

					m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop;

					// object positions back to their initial pos.
					pNode = (wxNode *)m_pObjList->GetFirst();
					while (pNode)
					{
						pObj = (wxReportObj *)pNode->GetData();
						pObj->ResetPos();

						pNode = pNode->GetNext();
					}
					break;

                case POTYPE_SUBSTART:
    				lSection = DefineSection();

					// Loading data-objects
					// --------------------
					for ( lHelp = 0L; (lHelp < objHelp.GetLocalLong()) && ! boEof; ++lHelp )
					{
						pObj = new wxReportObj( 0.0, 1.1, 2.2, 3.3 );
						pObj->LoadSettings( m_fFile );

						boEof = m_fFile.Eof();

						pObj->ResetPos();

						pt = pObj->GetCurrPos();	// without margin

						// Minimal position is object's init position
						if ( (m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop) > (0.01 * pt.y) )
							pObj->SetPosition( 0.01 * pt.x, 
								m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop );

						AddDataObj( pObj );
					}
                    break;

                case POTYPE_SUBEND:
	    			// we keep definition of section; thus no EndSection() here
					// EndSection();

					// going back to the section where we came from
					lSection = PreviousSection( lSection );

					// if that was the very first one, report ends
			    	if ( lSection < 0 )
				    	boEof = true;
				    else
				    {
						// Selecting the corresponding obj-list
					    wxReportSection	*pSection;

					    pNode = (wxNode *)m_lstSections.Item( lSection );
					    if (pNode)
					    {
						    pSection = (wxReportSection *)pNode->GetData();

						    if ( pSection )
							{
							    m_pObjList = pSection->m_pList;
								m_lCurrentSection = lSection;
							}
						    else
							    boEof = true;
					    }
					    else
						    boEof = true;

						// Re-positioning objects
					    if ( m_pObjList )
						{
							pNode = (wxNode *)m_pObjList->GetFirst();
							while (pNode )
							{
								pObj = (wxReportObj *)pNode->GetData();
								pObj->ResetPos();

								pt = pObj->GetCurrPos();	// without margins

								// Minimal position is object's init position
								if ( (m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop) > (0.01 * pt.y) )
									pObj->SetPosition( 0.01 * pt.x, 
										m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop );

								pNode = pNode->GetNext();
							}
						} // valid obj-list
				    }
                    break;

				default:	// Data...

					m_fFile.Seek( lFilePos );	// back to the beginning of this data

					// Reading all data here. We analyze where the bottom
					// margin (including footer objects) is going to be reached

					// FIRST page starts HERE (at this file pos)
					if ( m_aPageStart.GetCount() == 0 )
					{
						m_aPageStart.Add( m_fFile.Tell() );	// file position of FIRST page
						m_aSection.Add( lSection );			// we are in this section
					}

					lFilePos = m_fFile.Tell();	// Does the next set of objects
												// still fit on the page...?

					// Calculating new max. Y positions after (simulated) data output
					pNode = (wxNode *)m_pObjList->GetFirst();
					while (pNode && ! boEof )
					{
						pObj = (wxReportObj *)pNode->GetData();

						boLoad = pObj->LoadData( m_fFile );
						if ( ! boLoad )
							boEof = true;

						dY  = 0.01 * pObj->MoveForward().y;	// without margin
						dY += m_PageCtrl.m_dTop;			// WITH margin

						if ( dY > m_PageCtrl.m_dDataCurrY )
							m_PageCtrl.m_dDataCurrY = dY;	// new max. pos. WITH margin

						pNode = pNode->GetNext();
					}

					if ( boEof )
						continue;	// This is the end

					lFilePos = m_fFile.Tell();	// Now we're here

					// If at least ONE of the new Y positions of the current objects is
					// beyond the bootom margin/footer, we'll start a new page
					if ( m_PageCtrl.m_dDataCurrY > m_PageCtrl.m_dFooterMinY )
					{
						m_aPageStart.Add( lFilePos );
						m_aSection.Add( lSection );

						m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop;

						// object positions back to their initial pos.
						pNode = (wxNode *)m_pObjList->GetFirst();
						while (pNode)
						{
							pObj = (wxReportObj *)pNode->GetData();
							pObj->ResetPos();
							pNode = pNode->GetNext();
						}

					}

					break;

            } // switch

		} // while ! Eof (section)


//		m_fFile.Close();	// keeping file open

	} // File opened

	m_boObjectsSaved = false;

	lFilePos = m_fFile.Tell();

	return lFilePos;
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

	if ( strFile.IsEmpty() )
	{
		wxFileDialog dlg( this, wxT("Choose a Text"),
			              wxT("."), wxT(""),
						  wxT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*"),
						  wxOPEN );

		if ( wxID_OK != dlg.ShowModal() )
			return false;

		strFile = dlg.GetFilename();
	}

	m_aPageStart.Clear();

	if ( m_fFile.IsOpened() )
		m_fFile.Close();

	boRet = m_fFile.Open( strFile, _("rb") );

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
			lRead = m_fFile.Read( szDummy, sizeof( szDummy ) - 1 );
			szDummy[ lRead ] = 0x00;

			while ( lRead )
			{
				cpFF = strchr( szDummy, '\014' );
				if ( NULL != cpFF )
				{
					m_lNextPageInFile += (cpFF - szDummy) + 1;
					lRead -= (cpFF - szDummy) + 1;

					// If get a FF at the end, and after that only
					// CR and/or LF, we don't add a "new" page
					if ( m_lNextPageInFile < (lSizeOfFile - 1) )
					{
						m_aPageStart.Add( m_lNextPageInFile );
						strcpy( szDummy, cpFF + 1 );
					}
					else
					{
						// That's it....!
						lRead = 0;
					}
				}
				else
				{
					m_lNextPageInFile += lRead;
					lRead = 0;
				}

			} // while lRead

		} while ( ! m_fFile.Eof() );

		for ( unsigned int i = 0; i < m_aPageStart.GetCount(); ++i )
			m_lCurrentFilePos = m_aPageStart[ i ];

		// If the VERY LAST Char in the File was a FormFeed (FF), we have
		// calculate 1 page too much

/*		Now we already handle this while reading...
		===========================================
		m_fFile.Seek( -1L, wxFromEnd );
		if ( 1 == m_fFile.Read( szDummy, 1 ) )
			if ( *szDummy == '\014' )
			{
				i = m_aPageStart.GetCount();
				if ( i > 1 )
					m_aPageStart.RemoveAt( i - 1 );
			}
*/

		m_lNextPageInFile = 0;
		m_lCurrentFilePos = 0;
		m_PageCtrl.m_nCurrentPage = 0;

		m_fFile.Seek( m_lCurrentFilePos );

	} // file open()

	return boRet;
}


int  wxReportWriter::GetNumberOfPages()
{
	return m_aPageStart.GetCount();
}


void wxReportWriter::SetPage( unsigned int uiPage )
{
	if ( uiPage > 0 && uiPage <= m_aPageStart.GetCount() )
		m_PageCtrl.m_nCurrentPage = uiPage - 1;
	m_PageCtrl.m_nPages = m_aPageStart.GetCount();
}


void wxReportWriter::Print( void )
{
#if defined(USE_OLD_PRINTSETTING)
    wxPrintDialogData printDialogData(* g_printData);
#else
    wxPrintData printData = g_PrintSettings.GetPrintData();
    if( m_PageCtrl.m_dYSizeMM < m_PageCtrl.m_dXSizeMM )
        printData.SetOrientation( wxLANDSCAPE );
    else
        printData.SetOrientation( wxPORTRAIT );
    wxPrintDialogData printDialogData( printData );
#endif
    wxPrinter printer(& printDialogData);

    ReportPrintout printout( this );

    if (! printer.Print( this, &printout, TRUE ) )
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
	        wxMessageBox( wxT("There was a problem with Preview. Maybe the\n")
				            + wxString(wxT( "actual printer has not been configured correctly?" )),
						  wxT("Print"), wxOK );
        else
            wxMessageBox( wxT("Printing was canceled."), wxT("Print"), wxOK );
    }
    else
    {
#if defined(USE_OLD_PRINTSETTING)
        (*g_printData) = printer.GetPrintDialogData().GetPrintData();
#else
		g_PrintSettings.SetPrintData( printer.GetPrintDialogData().GetPrintData() );
#endif
    }
}



void wxReportWriter::PrintPreview( void )
{
    // Pass two printout objects: for preview, and possible printing.
#if defined(USE_OLD_PRINTSETTING)
    wxPrintDialogData printDialogData(* g_printData);
#else
    wxPrintData printData = g_PrintSettings.GetPrintData();
    if( m_PageCtrl.m_dYSizeMM < m_PageCtrl.m_dXSizeMM )
        printData.SetOrientation( wxLANDSCAPE );
    else
        printData.SetOrientation( wxPORTRAIT );
    wxPrintDialogData printDialogData( printData );
#endif
    wxPrintPreview *preview = new wxPrintPreview(
		new ReportPrintout( this ),
		new ReportPrintout( this ),
		&printDialogData );

    if (!preview->Ok())
    {
        delete preview;
        wxMessageBox( wxT("There was a problem with Preview. Maybe the\n")
			             + wxString( wxT("actual printer has not been configured correctly?") ),
					  wxT("Preview"), wxOK );
        return;
    }

/*  // This is the OLD version:  
    wxPreviewFrame *frame = new wxPreviewFrame( preview, this, wxT("Print Preview"),
		                                        wxPoint(70, 70), wxSize(640, 480) );
*/

	// We need our subclass of wxPreviewFrame here because
	// when the preview frame closes, we must close the
	// report file also. Otherwise the file would stay
	// open and could not (for example) be deleted as long
	// as the application is active
    ReportPreviewFrame *frame = new ReportPreviewFrame( preview, this, wxT("Print Preview"),
		                                        wxPoint(70, 70), wxSize(640, 480) );
	frame->SetReportWriter( this );


//  frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show(TRUE);
}


void wxReportWriter::Draw(wxDC& dc)
{
	long	    lPosData;
	wxString    strHeader;
	wxString    strLine;
	double      dScale;
	double		dY;
	bool		boEOP;
	bool		boLoad;
	wxNode      *pNode;
	wxReportObj *pObj;
	wxReportObj	objHelp;
    wxReportSection	*pSection;
	wxPoint pt;

	if ( m_PageCtrl.m_nCurrentPage < 0 )
		return;

	if ( m_PageCtrl.m_nCurrentPage >= (int)m_aPageStart.GetCount() )
		return;

	lPosData          = m_aPageStart[ m_PageCtrl.m_nCurrentPage ];
	m_lCurrentSection = m_aSection[ m_PageCtrl.m_nCurrentPage ];

	m_fFile.Seek( lPosData );

	dScale = 3.78;	// just a default value
	if ( m_pPrintout )
		dScale = m_pPrintout->GetMMScale();

	// setting up header-objects
	// -------------------------
	pNode = (wxNode *)m_lstHeader.GetFirst();
	while (pNode)
	{
		pObj = (wxReportObj *)pNode->GetData();

		pObj->ResetPos();
		pObj->SetScale( dScale );	// MM --> log.Units

		pNode = pNode->GetNext();

	}

	// setting up footer-objects
	// -------------------------
	pNode = (wxNode *)m_lstFooter.GetFirst();
	while (pNode)
	{
		pObj = (wxReportObj *)pNode->GetData();

		pObj->ResetPos();
		pObj->SetScale( dScale );	// MM --> log.Units

		pNode = pNode->GetNext();

	}

	// drawing header-objects
	// ----------------------
	pNode = (wxNode *)m_lstHeader.GetFirst();
	while (pNode)
	{
		pObj = (wxReportObj *)pNode->GetData();
		pObj->Draw( dc );

		pNode = pNode->GetNext();
	}

	m_PageCtrl.m_dFooterMinY = m_PageCtrl.m_dYSizeMM - m_PageCtrl.m_dBottom;

	// drawing footer-objects
	// ----------------------
	pNode = (wxNode *)m_lstFooter.GetFirst();
	while (pNode)
	{
		wxPoint pt;

		pObj = (wxReportObj *)pNode->GetData();

		dY = m_PageCtrl.m_dYSizeMM - m_PageCtrl.m_dBottom
		   - 0.01 * pObj->GetInitPos().y;

		if ( dY < m_PageCtrl.m_dFooterMinY )
			m_PageCtrl.m_dFooterMinY = dY;

		// The object's Draw() function takes Top & Left Margins into account
		// Therefore we must subtract the top margin here
		dY -= m_PageCtrl.m_dTop;

		pt = pObj->GetInitPos();
		pObj->SetPosition( 0.01 * pt.x, dY );

		pObj->Draw( dc );

		pNode = pNode->GetNext();
	}

	boEOP = false;
	m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop;

    // Positioning internal pointer to appropriate list
    if ( m_lCurrentSection < 0 || m_lCurrentSection >= (long)m_lstSections.GetCount() )
        return;

    pNode = (wxNode *)m_lstSections.Item( m_lCurrentSection );
    if (pNode)
    {
	    pSection = (wxReportSection *)pNode->GetData();

	    if ( pSection )
		    m_pObjList = pSection->m_pList;
	    else
		    boEOP = true;
    }
    else
	    boEOP = true;

	// Object positions to their initial pos.
	// --------------------------------------
    pNode = (wxNode *)m_pObjList->GetFirst();
	while ( pNode && ! boEOP )
	{
		pObj = (wxReportObj *)pNode->GetData();

		pObj->ResetPos();

	    pNode = pNode->GetNext();
	}

	// Drawing report-objects
	// ----------------------
	while ( ! boEOP )
	{
		lPosData = m_fFile.Tell();

		boLoad = objHelp.LoadData( m_fFile );
		if ( ! boLoad )
		{
			boEOP = true;
			continue;
		}

		switch ( objHelp.GetType() )
        {
			case POTYPE_PGBREAK:
				boEOP = true;
				break;

			case POTYPE_REPDATE:
			case POTYPE_PGNUM:
				break;			// not possible in data section!

			case POTYPE_LINE:	// Line within data section!!!
				{
					double dMinX;
					double dMaxX;
					double dMaxY;
					double dX;
					double dY;
					wxPoint pt;

					if ( ! m_pObjList )
						break;

					dMinX =  10000.0;
					dMaxX = -10000.0;
					dMaxY = -10000.0;

					
					pNode = (wxNode *)m_pObjList->GetFirst();
					while ( pNode )
					{
						pObj = (wxReportObj *)pNode->GetData();

						// find maximum Y of PREVIOUS positions
						pt = pObj->GetCurrPos();

						dY  = 0.01 * pObj->MoveBackward().y;	// WITHOUT margin

						if ( dY > dMaxY )
							dMaxY = dY;

						pt.y += HLINE_INC;			// 0.2mm forward in Y
						pObj->SetCurrPos( pt );


		    			pt  = pObj->GetCurrPos();	// without margin
						dX  = 0.01 * pt.x;

						if ( dX > dMaxX )
							dMaxX = dX;

						if ( dX < dMinX )
							dMinX = dX;

						dX  = 0.01 * (pt.x + pObj->GetSize().x);

						if ( dX > dMaxX )
							dMaxX = dX;


					    pNode = pNode->GetNext();
					}

					if ( dMaxX > dMinX )
					{
						objHelp.SetPosition( dMinX, dMaxY );
						objHelp.SetIncrements( 0.0, 0.0 );
						objHelp.SetHeight( 0.0 );
						objHelp.SetWidth( dMaxX - dMinX );
						objHelp.SetMargins( &m_PageCtrl );

						objHelp.Draw( dc );
					}
				} break;

            case POTYPE_SUBSTART:
            case POTYPE_SUBEND:
                if ( objHelp.GetType() == POTYPE_SUBSTART )
				{
					long lHelp;

					pNode = (wxNode *)m_lstSections.Item( m_lCurrentSection );
					if ( pNode )
					{
					    pSection = (wxReportSection *)pNode->GetData();

						if ( pSection )
						{

							lHelp = pSection->m_lSections;
							++lHelp;
							m_lCurrentSection = lHelp;
						}
					}
				}
                else
	    		    m_lCurrentSection = PreviousSection( m_lCurrentSection );

                if ( m_lCurrentSection < 0 || m_lCurrentSection >= (long)m_lstSections.GetCount() )
                    break;

				// Selecting corresponding data-objects
                pNode = (wxNode *)m_lstSections.Item( m_lCurrentSection );
                if (pNode)
                {
                    pSection = (wxReportSection *)pNode->GetData();

                    if ( pSection )
                        m_pObjList = pSection->m_pList;
                    else
                        boEOP = true;
                }
                else
                    boEOP = true;

				// Re-positioning data-objects
				// ---------------------------
                pNode = (wxNode *)m_pObjList->GetFirst();
				while ( pNode && ! boEOP )
				{
					// object settings are in file.
					// must jump over them here
	                if ( objHelp.GetType() == POTYPE_SUBSTART )
					{
						wxReportObj objDummy;
						objDummy.LoadSettings( m_fFile );	// dummy, to jump over the
															// object definitions
					}


					pObj = (wxReportObj *)pNode->GetData();

					pObj->ResetPos();

					pt = pObj->GetCurrPos();	// without margin

					// Minimal position is object's init position
					if ( (m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop) > (0.01 * pt.y) )
						pObj->SetPosition( 0.01 * pt.x, 
							m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop );

				    pNode = pNode->GetNext();
				}
                break;

			default:	// Data...

				m_fFile.Seek( lPosData );	// back to the beginning of this data


				// We can draw data first and the check whether at least
				// ONE of the object positions is beyond the bottom
				// margins (including footer obj.)
				pNode = (wxNode *)m_pObjList->GetFirst();
				while (pNode && ! boEOP )
				{
					pObj = (wxReportObj *)pNode->GetData();

					boLoad = pObj->LoadData( m_fFile );
					if ( ! boLoad )
						boEOP = true;

		    		if ( ! boEOP )
			    		pObj->Draw( dc );

				    pNode = pNode->GetNext();

			    }

				// Now analyzing new positions

				pNode = (wxNode *)m_pObjList->GetFirst();
				while (pNode && ! boEOP )
				{
					pObj = (wxReportObj *)pNode->GetData();

		    		dY  = 0.01 * pObj->GetCurrPos().y;	// without margin
			    	dY += m_PageCtrl.m_dTop;			// WITH margin

    				if ( dY > m_PageCtrl.m_dDataCurrY )
	    				m_PageCtrl.m_dDataCurrY = dY;

    				if ( m_PageCtrl.m_dDataCurrY > m_PageCtrl.m_dFooterMinY )
	    				boEOP = true;

				    pNode = pNode->GetNext();

			    }

				break;

        } // switch

	} // while

}

void wxReportWriter::SetOrientation( int iMode )
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
}
 

// -------------------------------------
//  Report Printout Handling
// -------------------------------------
ReportPrintout::ReportPrintout(wxReportWriter *pFrame, wxChar *szTitle )
 : wxPrintout(szTitle)
{
	m_pReportPage = pFrame;

	if ( pFrame )
		pFrame->SetPrintout( this );
}


ReportPrintout::~ReportPrintout( void )
{
	if ( m_pReportPage )
		m_pReportPage->SetPrintout( 0 );
}

bool ReportPrintout::OnPrintPage(int page)
{
    wxDC *dc = GetDC();

    if (dc)
    {
		DrawPage( dc, page );
        return TRUE;
    }
    else
        return FALSE;
}

bool ReportPrintout::OnBeginDocument(int startPage, int endPage)
{
    if (!wxPrintout::OnBeginDocument(startPage, endPage))
        return FALSE;
    
    return TRUE;
}


void ReportPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
    *minPage     = 1;
    *maxPage     = m_pReportPage->GetNumberOfPages();
    *selPageFrom = 1;
    *selPageTo   = m_pReportPage->GetNumberOfPages();
}


bool ReportPrintout::HasPage(int pageNum)
{
    return (pageNum <= m_pReportPage->GetNumberOfPages() );
}


void ReportPrintout::DrawPage( wxDC *dc, int iPage )
{
	CalcScale( dc );	
	
	m_pReportPage->SetPage( iPage );

    m_pReportPage->Draw(*dc);
//	dc->SetUserScale( m_fOverallScale, m_fOverallScale );
	dc->SetUserScale( 1.0, 1.0 );
}


void ReportPrintout::CalcScale( wxDC *dc )
{
    // Get the logical pixels per inch of screen and printer
    int ppiScreenX,  ppiScreenY;
    int ppiPrinterX, ppiPrinterY;
    int pageWidth,   pageHeight;
    int w, h;
	double dTopM, dLeftM;

	dTopM = dLeftM = 0.0;

    GetPPIScreen(  &ppiScreenX,  &ppiScreenY );
    GetPPIPrinter( &ppiPrinterX, &ppiPrinterY );
    
    // This scales the DC so that the printout roughly represents the
    // the screen scaling. The text point size _should_ be the right size
    // but in fact is too small for some reason. This is a detail that will
    // need to be addressed at some point but can be fudged for the
    // moment.
    float fScale = (float)((float)ppiPrinterX/(float)ppiScreenX);
    
    // Now we have to check in case our real page size is reduced
    // (e.g. because we're drawing to a print preview memory DC)
    dc->GetSize( &w, &h );
//  GetPageSizeMM( &pageWidth, &pageHeight );
    GetPageSizePixels( &pageWidth, &pageHeight );
    
    // If printer pageWidth == current DC width, then this doesn't
    // change. But w might be the preview bitmap width, so scale down.
    m_fOverallScale = fScale * (float)(w/(float)pageWidth);
    
	dc->SetUserScale( m_fOverallScale, m_fOverallScale );
    
    // Calculate conversion factor for converting millimetres into
    // logical units.
    // There are approx. 25.4 mm to the inch. There are ppi
    // device units to the inch. Therefore 1 mm corresponds to
    // ppi/25.4 device units. We also divide by the
    // screen-to-printer scaling factor, because we need to
    // unscale to pass logical units to DrawLine.
    
    m_fScaleMMToLogUnits = (float)(ppiPrinterX/(fScale * 25.4));
    
    int pageWidthMM, pageHeightMM;
    GetPageSizeMM( &pageWidthMM, &pageHeightMM );
}

void ReportPrintout::OnEndPrinting()
{
}


// -----------------------------------------------------------
//  DIALOG
//
//  Selecting a report for preview or printing
// -----------------------------------------------------------

BEGIN_EVENT_TABLE(wxReportSelector, wxDialog)
    EVT_BUTTON(ID_PB_PREVIEW,  wxReportSelector::OnPreview)
    EVT_BUTTON(ID_PB_PRINT,    wxReportSelector::OnPrint)
    EVT_BUTTON(ID_PB_DEL,      wxReportSelector::OnDelete)
	EVT_IDLE(wxReportSelector::OnIdle)
END_EVENT_TABLE()


wxReportSelector::wxReportSelector( wxFrame *pParent, const wxString &strTitle, const wxString &strPath )
: wxDialog( pParent, (wxWindowID)-1, strTitle, wxPoint(50,50) )
{
	m_strPathOfReports = strPath;
	m_iSelected        = REPSEL_NONE;
	m_strFilename.Empty();

	CreateElements();
	SetupList();
	FillList();
};

wxReportSelector::~wxReportSelector( void )
{
	m_aFiles.Clear();
};

void wxReportSelector::CreateElements( void )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxStaticText *item1 = new wxStaticText( this, ID_TX_PROMPT, _("Select a report:"), wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxListCtrl *item2 = new wxListCtrl( this, ID_LC_REPORT, wxDefaultPosition, wxSize(450,200), wxLC_REPORT|wxSUNKEN_BORDER );
    item0->Add( item2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer *item3 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *item4 = new wxButton( this, ID_PB_PREVIEW, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item4, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *item5 = new wxButton( this, ID_PB_PRINT, _("Print"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item5, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *item6 = new wxButton( this, ID_PB_DEL, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item6, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *item7 = new wxButton( this, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->SetDefault();
    item3->Add( item7, 0, wxALIGN_CENTRE|wxALL, 5 );

    item0->Add( item3, 0, wxALIGN_CENTRE|wxALL, 5 );

    SetAutoLayout( TRUE );
    SetSizer( item0 );
    item0->Fit( this );
    item0->SetSizeHints( this );
};

void wxReportSelector::FillList( void )
{
	wxListCtrl		*pLst;
	wxString		strData;
	wxString		strDir;
	wxString		strFiles;
	wxArrayString	aRepFiles;
	int				iItem;
	wxReportWriter	repPage( 0, wxT(""), wxPoint(0,0), wxSize(10,10) );

	repPage.SetLoading();

	pLst = (wxListCtrl *)FindWindow( ID_LC_REPORT );
	pLst->DeleteAllItems();

	aRepFiles.Clear();
	m_aFiles.Clear();

	strDir    = m_strPathOfReports;

	strFiles  = wxT("*.");
	strFiles += VLPEXT;

	wxDir::GetAllFiles( strDir, &aRepFiles, strFiles, wxDIR_FILES );

	// Now sorting files by date...
	wxSortedArrayString	aSortedByDate;
	wxFileName			fn;
	wxDateTime			dt;
	for ( iItem = 0; iItem < (int)aRepFiles.GetCount(); ++iItem )
	{
		fn.Assign( aRepFiles[ iItem ] );
		fn.Normalize();
		if ( fn.GetTimes( 0, 0, &dt ) )
		{
			strData.Printf( wxT("%s%s=%s"),
				dt.FormatISODate().c_str(),
				dt.FormatISOTime().c_str(),
				aRepFiles[ iItem ].c_str() );
			aSortedByDate.Add( strData );
		}
	}

	// Pushing the sorted list into our local array,
	// beginning with the lastest file on top
	for ( iItem = 0; iItem < (int)aSortedByDate.GetCount(); ++iItem )
	{
		int iIdx;

		iIdx    = aSortedByDate.GetCount() - 1 - iItem;
		strData = aSortedByDate[ iIdx ].AfterFirst( wxT('=') );
		m_aFiles.Add( strData );
	}

    // Now filling the list
	for ( iItem = 0; iItem < (int)m_aFiles.GetCount(); ++iItem )
	{
		repPage.LoadReportInfo( m_aFiles[ iItem ] );

		pLst->InsertItem( iItem, repPage.GetDate() );
		pLst->SetItemData( iItem, iItem );

		pLst->SetItem( iItem, 1, repPage.GetUser() );

		pLst->SetItem( iItem, 2, repPage.GetInfo() );

	}

	aSortedByDate.Clear();

}


void wxReportSelector::SetupList( void )
{
	wxListCtrl *pLst;

	pLst = (wxListCtrl *)FindWindow( ID_LC_REPORT );
	pLst->ClearAll();
	pLst->InsertColumn( 0, wxT("Date"), wxLIST_FORMAT_LEFT, 120 );
	pLst->InsertColumn( 1, wxT("User"), wxLIST_FORMAT_LEFT,  80 );
	pLst->InsertColumn( 2, wxT("Info"), wxLIST_FORMAT_LEFT, 200 );

}

void wxReportSelector::OnPreview( wxCommandEvent &event )
{
	long         lItem;

	lItem = SelectedListItem();
	if ( lItem >= 0 )
	{
		m_strFilename = m_aFiles[ lItem ];
		m_iSelected   = REPSEL_PREVIEW;

	}

	EndModal( wxID_OK );
}


void wxReportSelector::OnPrint( wxCommandEvent &event )
{
	long lItem;

	lItem = SelectedListItem();
	if ( lItem >= 0 )
	{
		m_strFilename = m_aFiles[ lItem ];
		m_iSelected   = REPSEL_PRINT;
	}
	EndModal( wxID_OK );
}


void wxReportSelector::OnDelete( wxCommandEvent &event )
{
	wxString    strFile;
	wxListCtrl *pLst;
	long        lItem;
	long        lLine;

	pLst = (wxListCtrl *)FindWindow( ID_LC_REPORT );

	lLine = -1L;

	for (;;)
	{
		lLine = pLst->GetNextItem( lLine, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	
		if ( lLine < 0L )
			break;

		lItem = pLst->GetItemData( lLine );

		if ( lItem >= 0 )
		{
			strFile = m_aFiles[ lItem ];

			wxRemoveFile( strFile );

		}
	}

	FillList();

	// EndModal( wxID_OK );
}

long wxReportSelector::SelectedListItem( void )
{
	wxListCtrl *pLst;
	long lRet;

	pLst = (wxListCtrl *)FindWindow( ID_LC_REPORT );

	lRet = -1L;
	lRet = pLst->GetNextItem( lRet, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	
	if ( lRet != -1L )
		return pLst->GetItemData( lRet );

	return lRet;
}

void wxReportSelector::OnIdle( wxIdleEvent &event )
{
	wxButton	*pBtnPrint;
	wxButton	*pBtnPreview;
	wxButton	*pBtnDelete;
	long		lItem;

	pBtnPrint   = (wxButton *)FindWindow( ID_PB_PRINT );
	pBtnPreview = (wxButton *)FindWindow( ID_PB_PREVIEW );
	pBtnDelete  = (wxButton *)FindWindow( ID_PB_DEL );

	lItem = SelectedListItem();
	
	pBtnPrint->Enable(   lItem != -1 );
	pBtnPreview->Enable( lItem != -1 );
	pBtnDelete->Enable(  lItem != -1 );
}

