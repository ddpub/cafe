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
#pragma implementation "wxReportViewer.h"
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

#if !wxUSE_PRINTING_ARCHITECTURE
#error You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h to compile this program.
#endif

// Set this to 1 if you want to test PostScript printing under MSW.
// However, you'll also need to edit src/msw/makefile.nt.
#define wxTEST_POSTSCRIPT_IN_MSW 0

#if wxTEST_POSTSCRIPT_IN_MSW
#include "wx/generic/printps.h"
#include "wx/generic/prntdlgg.h"
#endif

#include <wx/fontdlg.h>
#include <wx/fontenum.h>
#include <wx/fontmap.h>

#include "wxReportViewer.h"

// Global Printer- and Setup-Settings

#if defined(USE_OLD_PRINTSETTING)

// Global print data, to remember settings during the session
wxPrintData *g_printData = (wxPrintData*) NULL ;

// Global page setup data
wxPageSetupData* g_pageSetupData = (wxPageSetupData*) NULL;
#else
wxPrintSettings g_PrintSettings;
#endif

//static int font_count = 0;

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
	m_pRepVwr = 0;
}


ReportPreviewFrame::~ReportPreviewFrame( void )
{
}

void ReportPreviewFrame::OnCloseWindow( wxCloseEvent &event )
{
	// When the preview frame closes, we need to close
	// the corresponding report file also. This is done
	// in ...FinishReport()
	if ( m_pRepVwr )
	{
		m_pRepVwr->FinishReport();
	}

	wxPreviewFrame::OnCloseWindow( event );
}

wxReportObjViewer::wxReportObjViewer()
: wxReportObj()
{
	m_pBitmap			= 0;
	m_pFont				= 0;
	m_pBitmapFileData	= 0;
}

wxReportObjViewer::wxReportObjViewer(double dXPos, double dYPos, double dXSize, double dYSize )
: wxReportObj( dXPos, dYPos, dXSize, dYSize )
{
	
	m_pBitmap			= 0;
	m_pFont				= 0;
	m_pBitmapFileData	= 0;
}

wxReportObjViewer::~wxReportObjViewer()
{
	if ( m_pBitmap )
		wxDELETE( m_pBitmap );

	if ( m_pBitmapFileData )
		wxDELETE( m_pBitmapFileData );

	if ( m_pFont )
	{
		wxDELETE( m_pFont );
	}
}

wxString wxReportObjViewer::LoadWxString( wxFFile &fFile )
{
	wxMessageBox(_("wxReportObjViewer::LoadWxString( wxFFile &fFile )"));
	long lHelp;
	wxString strRet;

	strRet.Empty();

	if ( ! fFile.IsOpened() )
	{

		return strRet;
	}
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
	wxMessageBox(_("exit:") + strRet);
	return strRet;
}

bool wxReportObjViewer::LoadSettings( wxFFile &fFile )
{
	wxMessageBox(_("wxReportObjViewer::LoadSettings( wxFFile &fFile )"));
	long		lHelp;
	if ( ! fFile.IsOpened() )
		return false;

	fFile.Read( &m_ptInitPos, sizeof( wxPoint ) );
	fFile.Read( &m_sizSize,   sizeof( wxSize  ) );
	fFile.Read( &m_sizInc,    sizeof( wxSize  ) );

	fFile.Read( &m_nLen,      sizeof( m_nLen  ) );
	fFile.Read( &m_nDec,      sizeof( m_nDec  ) );

	fFile.Read( &m_nType,     sizeof( m_nType ) );

	fFile.Read( &m_nPenWidth, sizeof( m_nPenWidth ) );

	fFile.Read( &lHelp,       sizeof( lHelp   ) );
	m_boRightAlign = ( 0L != lHelp );

	fFile.Read( &lHelp,		  sizeof( lHelp	  ) );
	m_boCenterAlign = ( 0L != lHelp );

	m_pReportFont = new wxReportFont();
	fFile.Read( &lHelp,		  sizeof( lHelp   ) );
	m_pReportFont->fontSize = lHelp;
	fFile.Read( &lHelp,		  sizeof( lHelp   ) );
	m_pReportFont->isBold = lHelp ? true : false;
	fFile.Read( &lHelp,		  sizeof( lHelp   ) );
	m_pReportFont->isItalic = lHelp ? true : false;
	fFile.Read( &lHelp,		  sizeof( lHelp   ) );
	m_pReportFont->isStrikeThrough = lHelp ? true : false;
	fFile.Read( &lHelp,		  sizeof( lHelp   ) );
	m_pReportFont->isUnderline = lHelp ? true : false;
	fFile.Read( &lHelp,		  sizeof( lHelp   ) );
	m_pReportFont->isDefault = lHelp ? true : false;
	fFile.Read( &lHelp,		  sizeof( lHelp   ) );
	m_pReportFont->fontFamily = lHelp;

	m_pReportFont->fontColor = LoadWxString( fFile );
	m_pReportFont->fontName = LoadWxString( fFile );
	m_pReportFont->name = LoadWxString( fFile );
	if ( m_pFont )
		wxDELETE( m_pFont );
	wxMessageBox(_("load font"));
	m_pFont = new wxFont( m_pReportFont->fontSize, m_pReportFont->fontFamily, m_pReportFont->isItalic ?
			wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL, m_pReportFont->isBold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL, 
			m_pReportFont->isUnderline, m_pReportFont->fontName);
	
	//OutputDebugStr( wxString::Format( "%i fonts created...\n", ++font_count ) );
	wxMessageBox(_("exit"));
	return true;
}

bool wxReportObjViewer::LoadData( wxFFile &fFile )
{
	wxMessageBox(_("wxReportObjViewer::LoadData( wxFFile &fFile )"));
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
	if ( m_nType == POTYPE_BITMAP )
	{
		LoadBitmapData( fFile );
		if ( fFile.Eof() )
			return false;
	}
	wxMessageBox(_("exit"));
	return true;
}


void wxReportObjViewer::LoadBitmap( wxString strFile )
{
#if wxUSE_LIBPNG
    wxImage image;
	//wxLogNull logNull;
    if ( image.LoadFile( strFile ) )
        m_pBitmap = new wxBitmap( image );
	
#endif // wxUSE_LIBPNG
}

bool wxReportObjViewer::LoadBitmapData( wxFFile &fFile )
{
	fFile.Read( &m_sizeOfBitmapFileData, sizeof( m_sizeOfBitmapFileData ) );
	if ( fFile.Eof() )
		return false;

	if ( m_sizeOfBitmapFileData == 0 )
		return true;

	m_pBitmapFileData = new unsigned char[m_sizeOfBitmapFileData];
	fFile.Read( m_pBitmapFileData, m_sizeOfBitmapFileData );
	if ( fFile.Eof() )
		return false;

	// write temporary image file
	wxFFile bmpFile;
	wxString fileName = wxFileName::CreateTempFileName(_(""));

	if ( bmpFile.Open( fileName, wxT("wb") ) )
	{
		bmpFile.Write( m_pBitmapFileData, m_sizeOfBitmapFileData );
		bmpFile.Close();

		LoadBitmap( fileName );

		wxRemoveFile( fileName );
	}

	return true;
}


void wxReportObjViewer::Draw( wxDC & dc )
{
	wxMessageBox(_("wxReportObjViewer::Draw( wxDC & dc )"));
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
		//wxMessageBox(_("msg5 draw object num"));
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
		//wxMessageBox(_("msg5 draw object date"));
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
		lXP = (long)(m_dScale * 0.01 * ((double)m_ptCurrPos.x + 100.0 * m_pMargin->m_dLeft));
		lYP = (long)(m_dScale * 0.01 * ((double)m_ptCurrPos.y + 100.0 * m_pMargin->m_dTop));
		lXS = (long)(m_dScale * 0.01 * (double)m_sizSize.x);
		lYS = (long)(m_dScale * 0.01 * (double)m_sizSize.y);

		//wxLogError( wxString::Format( "Margins: left=%f, top=%f  -  Position: x=%i, y=%i  -  Scale=%f", 
		//	m_pMargin->m_dLeft, m_pMargin->m_dTop, m_ptCurrPos.x, m_ptCurrPos.y, m_dScale ) );
		//wxLogError( wxString::Format( "x=%i, y=%i, width=%i, height=%i", lXP, lYP, lXS, lYS ) );
	}
	else
	{
		// we don't know about page margins
		lXP = (long)(m_dScale * 0.01 * m_ptCurrPos.x );
		lYP = (long)(m_dScale * 0.01 * m_ptCurrPos.y);
		lXS = (long)(m_dScale * 0.01 * m_sizSize.x);
		lYS = (long)(m_dScale * 0.01 * m_sizSize.y);
	}

	//wxMessageBox(_("msg5 draw object 1"));
	switch ( m_nType )
	{
	case POTYPE_PLOGIC:
	case POTYPE_PCHECK:
		{
			//wxMessageBox(_("msg5 draw object check"));
			wxPen *pCurrPen;
			long  lOffs;

			pCurrPen = ((wxPen*)& dc.GetPen());
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
		{
		//wxMessageBox(_("msg5 draw object bmp"));
		if ( ! m_pBitmap )
			SetBitmap( m_strLocal );

		if ( m_pBitmap && m_pBitmap->Ok() )
		{
	        dc.DrawBitmap( *m_pBitmap, lXP, lYP );

			MoveForward();
	    }
		}
		break;
		

	case POTYPE_LINE:
		{
			//wxMessageBox(_("msg5 draw object line"));
			wxPen *pCurrPen;

			pCurrPen = ((wxPen*)& dc.GetPen());
			//dc.SetPen( * wxBLACK_PEN );
			//wxPen* pen = new wxPen("Black", m_nPenWidth, wxSOLID);
			dc.SetPen( wxPen(_("Black"), m_nPenWidth, wxSOLID) );
			dc.DrawLine( lXP, lYP, lXP + lXS, lYP + lYS );
			dc.SetPen( * pCurrPen );
			//if ( pen ) wxDELETE( pen );

			MoveForward();
		} break;

	case POTYPE_PGNUM:
	case POTYPE_REPDATE:
//		break;

	default:
		{
		//wxMessageBox(_("msg5 draw object default"));
		if ( m_pReportFont )
		{
			m_pFont = new wxFont( m_pReportFont->fontSize, m_pReportFont->fontFamily, m_pReportFont->isItalic ?
			wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL, m_pReportFont->isBold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL, 
			m_pReportFont->isUnderline, m_pReportFont->fontName);
			//wxMessageBox(_("msg5 draw object default setfont"));
			//if (!&dc) wxMessageBox(_("msg5 draw object default font"));
			dc.SetFont( *m_pFont );
			//wxMessageBox(_("msg5 draw object default font_ok"));
		}
		//wxMessageBox(_("msg5 draw object default 1"));
		if (strPrint == _("")) strPrint = _("12345");
		dc.GetTextExtent( strPrint, &iWid, &iHei );
		//wxMessageBox(_("msg5 draw object default 1-1-1"));
		lYP -= iHei;

		dc.SetClippingRegion( lXP, lYP, lXS, lYS );
		//wxMessageBox(_("msg5 draw object default 2"));
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
		else if ( m_boCenterAlign )
		{
			if ( m_pMargin )
			{
				lXP = (long)(m_dScale * 0.01 * (m_ptCurrPos.x + 100.0 * m_pMargin->m_dLeft));
				lXP += (long)(m_dScale * 0.01 * m_sizSize.x - iWid) / 2;
			}
			else
			{
				lXP = (long)(m_dScale * 0.01 * (m_ptCurrPos.x) );
				lXP += (long)(m_dScale * 0.01 * m_sizSize.x - iWid) / 2;
			}
		}
		
		//wxLogError( wxString::Format( "Draw Object: %s at x=%i, y=%i", strPrint.c_str(), (int)lXP, (int)lYP ) );
		//wxMessageBox(_("msg5 draw object default - text ") + strPrint);
		dc.DrawText( strPrint, lXP, lYP );
		dc.DestroyClippingRegion();
		//wxMessageBox(_("msg5 draw object default 4"));
		MoveForward();
		//wxMessageBox(_("msg5 draw object default 5"));
		}
		break;

	} // switch
	wxMessageBox(_("exit: ") + strPrint);
}



// -------------------------------------
// Class to keep settings for Printer
// -------------------------------------
wxPrintSettings::wxPrintSettings( void )
{
	wxMessageBox(_("wxPrintSettings::wxPrintSettings( void )"));
	m_pPrintData     = (wxPrintData*) NULL ;
	m_pPageSetupData = (wxPageSetupData*) NULL;
	wxMessageBox(_("exit"));
}

wxPrintSettings::~wxPrintSettings( void )
{
	Free();
}

void wxPrintSettings::Free( void )
{
	wxMessageBox(_("wxPrintSettings::Free( void )"));
	if ( m_pPrintData )
		delete m_pPrintData;
	if ( m_pPageSetupData )
		delete m_pPageSetupData;

	m_pPrintData     = 0;
	m_pPageSetupData = 0;
	wxMessageBox(_("exit"));
}

wxPrintData & wxPrintSettings::GetPrintData( void )
{
	wxMessageBox(_("wxPrintSettings::GetPrintData( void )"));
	if ( ! m_pPrintData )
		m_pPrintData = new wxPrintData();
	if ( ! m_pPageSetupData )
		m_pPageSetupData = new wxPageSetupDialogData;
	wxMessageBox(_("exit"));
	return (* m_pPrintData);
}

wxPageSetupData & wxPrintSettings::GetPageSetupData( void )
{
	wxMessageBox(_("wxPrintSettings::GetPageSetupData( void )"));
	if ( ! m_pPrintData )
		m_pPrintData = new wxPrintData;
	if ( ! m_pPageSetupData )
		m_pPageSetupData = new wxPageSetupDialogData;

	(*m_pPageSetupData) = * m_pPrintData;
	wxMessageBox(_("exit"));
	return (* m_pPageSetupData);
}

void wxPrintSettings::SetPrintData( wxPrintData &printdata )
{
	wxMessageBox(_("wxPrintSettings::SetPrintData( wxPrintData &printdata )"));
	if ( m_pPrintData )
		(*m_pPrintData) = printdata;
	wxMessageBox(_("exit"));
}

void wxPrintSettings::SetPageSetupData( wxPageSetupData &setupdata )
{
	wxMessageBox(_("wxPrintSettings::SetPageSetupData( wxPageSetupData &setupdata )"));
	if ( m_pPageSetupData )
		(*m_pPageSetupData) = setupdata;
	wxMessageBox(_("exit"));
}



wxReportViewer::wxReportViewer(wxFrame* frame, const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame(frame, -1, title, pos, size), wxReportWriter()
{
	::wxInitAllImageHandlers();

	m_pPrintout = 0;

#if defined(USE_OLD_PRINTSETTING)
    if ( ! g_printData )
	    g_printData = new wxPrintData;
	if ( ! g_pageSetupData )
    g_pageSetupData = new wxPageSetupDialogData;
#endif
}

wxReportViewer::~wxReportViewer()
{
	if ( !bFinished )
	{
		FinishReport();
		bFinished = true;
	}

	// De-allocation  of our section list
	while ( m_lstSections.GetCount() > 0 )
		EndSection();

	m_lstHeader.DeleteContents( false );
	wxwxReportObjListNode *node = m_lstHeader.GetFirst();
	while (node)
	{
		DeleteReportObj( node->GetData() );
		node = node->GetNext();
	}

	m_lstFooter.DeleteContents( false );
	node = m_lstFooter.GetFirst();
	while (node)
	{
		DeleteReportObj( node->GetData() );
		node = node->GetNext();
	}

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
}

bool wxReportViewer::OpenReport()
{
	wxMessageBox(reportFile.GetFullPath());
	SetupReport( reportFile.GetFullPath() );

	return true;
}

bool wxReportViewer::OpenReport( wxString reportPath )
{
	reportFile = reportPath;
	rootFolder = reportFile.GetVolume() + reportFile.GetVolumeSeparator() + reportFile.GetPath( true );

	return OpenReport( );
}

bool wxReportViewer::Preview()
{
	PrintPreview();

	return true;
}

long wxReportViewer::SetupReport( wxString strFile )
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
	wxReportObjViewer	*pObj;
	wxReportObjViewer	objHelp;
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

	boRet = m_fFile.Open( strFile, wxT("rb") );

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
		m_fFile.Read( &lHelp,				  sizeof( lHelp ) );
		m_PageCtrl.m_iPaperFormat = (int)lHelp;

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
			pObj = new wxReportObjViewer( 0.0, 1.1, 2.2, 3.3 );
			pObj->LoadSettings( m_fFile );
			pObj->ResetPos();
			AddHeaderObj( pObj );

			// get header height
			if ( pObj->GetInitPos().y > m_PageCtrl.m_dHeaderHeight )
				m_PageCtrl.m_dHeaderHeight = pObj->MoveForward().y * 0.01;
    	}

		// Loading footer-objects
		// ----------------------
		for ( lHelp = 0L; lHelp < nFooterObj; ++lHelp )
		{
			pObj = new wxReportObjViewer( 0.0, 1.1, 2.2, 3.3 );
			pObj->LoadSettings( m_fFile );
			pObj->ResetPos();
			AddFooterObj( pObj );
		}


		// Loading header data...
		// ----------------------
		pNode = (wxNode *)m_lstHeader.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObjViewer *)pNode->GetData();
			pObj->LoadData( m_fFile );

			pNode = pNode->GetNext();
	
            //wxLogError( wxString::Format( "Setup Header: %s at x=%i, y=%i", pObj->GetString().c_str(), pObj->GetCurrPos().x, pObj->GetCurrPos().y ) );
	    }


		// Loading footer data...
		// ----------------------

		// take care on margins 
		m_PageCtrl.m_dFooterMinY = m_PageCtrl.m_dYSizeMM - m_PageCtrl.m_dBottom;

		pNode = (wxNode *)m_lstFooter.GetFirst();
		while (pNode)
		{
			pObj = (wxReportObjViewer *)pNode->GetData();
			pObj->LoadData( m_fFile );

			// smallest Y coordinate calculation
			dY = m_PageCtrl.m_dYSizeMM - m_PageCtrl.m_dBottom
			   - 0.01 * pObj->GetInitPos().y;
			if ( dY < m_PageCtrl.m_dFooterMinY )
				m_PageCtrl.m_dFooterMinY = dY;

			pNode = pNode->GetNext();
	
            //wxLogError( wxString::Format( "Setup Footer: %s at x=%i, y=%i", pObj->GetString().c_str(), pObj->GetCurrPos().x, pObj->GetCurrPos().y ) );
		}


		// Well, this was the part with FIXED data.
		// Variable data may follow

		// The first record MUST contain a POTYPE_SUBSTART
		// record with the number of objects


		m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop + m_PageCtrl.m_dHeaderHeight;

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
						pObj = (wxReportObjViewer *)pNode->GetData();

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

					m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop + m_PageCtrl.m_dHeaderHeight;

					// object positions back to their initial pos.
					pNode = (wxNode *)m_pObjList->GetFirst();
					while (pNode)
					{
						pObj = (wxReportObjViewer *)pNode->GetData();
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
						pObj = new wxReportObjViewer( 0.0, 1.1, 2.2, 3.3 );
						pObj->LoadSettings( m_fFile );

						boEof = m_fFile.Eof();

						pObj->ResetPos();

						pt = pObj->GetCurrPos();	// without margin

						// Minimal position is object's init position
						/*if ( (m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop) > (0.01 * pt.y) )
							pObj->SetPosition( 0.01 * pt.x, 
								m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop );*/
						pObj->SetPosition( 0.01 * pt.x, 
							m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop + 0.01 * pt.y );

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
								pObj = (wxReportObjViewer *)pNode->GetData();
								pObj->ResetPos();

								pt = pObj->GetCurrPos();	// without margins

								// Minimal position is object's init position
								/*if ( (m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop) > (0.01 * pt.y) )
									pObj->SetPosition( 0.01 * pt.x, 
										m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop );*/
								pObj->SetPosition( 0.01 * pt.x, 
									m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop + 0.01 * pt.y );

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
						pObj = (wxReportObjViewer *)pNode->GetData();
						boLoad = pObj->LoadData( m_fFile );					
						if ( ! boLoad )
						{
							boEof = true;
						}

						dY  = 0.01 * pObj->MoveForward().y;	// without margin
						dY += m_PageCtrl.m_dTop;			// WITH margin
							//+ m_PageCtrl.m_dHeaderHeight;	// WITH header

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

						m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop + m_PageCtrl.m_dHeaderHeight;

						// object positions back to their initial pos.
						pNode = (wxNode *)m_pObjList->GetFirst();
						while (pNode)
						{
							pObj = (wxReportObjViewer *)pNode->GetData();
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
wxMessageBox(_("exit"));
	return lFilePos;
}



void wxReportViewer::Print( void )
{
#if defined(USE_OLD_PRINTSETTING)
    wxPrintDialogData printDialogData(* g_printData);
#else
    wxPrintData printData = g_PrintSettings.GetPrintData();
    if( m_PageCtrl.m_dYSizeMM < m_PageCtrl.m_dXSizeMM )
        printData.SetOrientation( wxLANDSCAPE );
    else
        printData.SetOrientation( wxPORTRAIT );
	printData.SetPaperId( wxPaperSize(m_PageCtrl.m_iPaperFormat) );
    wxPrintDialogData printDialogData( printData );
#endif
    wxPrinter printer(& printDialogData);

    ReportPrintout printout( this, m_strInfo );

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



void wxReportViewer::PrintPreview( void )
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
	printData.SetPaperId( wxPaperSize(m_PageCtrl.m_iPaperFormat) );
    wxPrintDialogData printDialogData( printData );
#endif

	ReportPrintout* previewPrintout = new ReportPrintout( this );
    wxPrintPreview *preview = new wxPrintPreview(
		previewPrintout,
		new ReportPrintout( this ),
		&printDialogData );
	SetPrintout( previewPrintout );

    if (!preview->Ok())
    {
        delete preview;
        wxMessageBox( wxT("There was a problem with Preview. Maybe the\n")
			             + wxString( wxT("actual printer has not been configured correctly?") ),
					  wxT("Preview"), wxOK );
        return;
    }

	// We need our subclass of wxPreviewFrame here because
	// when the preview frame closes, we must close the
	// report file also. Otherwise the file would stay
	// open and could not (for example) be deleted as long
	// as the application is active
    ReportPreviewFrame *frame = new ReportPreviewFrame( preview, this, wxT("Print Preview"),
		                                        wxPoint(70, 70), wxSize(640, 480), wxDEFAULT_FRAME_STYLE, m_strInfo );
	frame->SetReportWriter( this );


//  frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show(TRUE);
}


void wxReportViewer::Draw(wxDC& dc)
{
	long	    lPosData;
	wxString    strHeader;
	wxString    strLine;
	double      dScale;
	double		dY;
	bool		boEOP;
	bool		boLoad;
	wxNode      *pNode;
	wxReportObjViewer *pObj;
	wxReportObjViewer	objHelp;
    wxReportSection	*pSection;
	wxPoint pt;

	bool		bFirstSection = true;

	// test to fix subreports
	// ----------------------
	long		m_lmyLastSection = 0;

	if ( m_PageCtrl.m_nCurrentPage < 0 )
	{
		m_PageCtrl.m_nCurrentPage = 0;
		//wxMessageBox(_("msg1"));
		//return;
	}

	if ( m_PageCtrl.m_nCurrentPage >= (int)m_aPageStart.GetCount() )
	{
		m_PageCtrl.m_nCurrentPage = (int)m_aPageStart.GetCount();
		//wxMessageBox(_("msg2"));
		//return;
	}

	//wxMessageBox(_("msg4"));
	
	lPosData          = m_aPageStart[ m_PageCtrl.m_nCurrentPage ];
	//wxMessageBox(_("msg10-1"));
	//m_lCurrentSection = m_aSection[ m_PageCtrl.m_nCurrentPage ];
	//wxMessageBox(_("msg10-2"));

	//wxMessageBox(_("msg10"));
	
	//m_fFile.Seek( lPosData );

	dScale = 3.78;	// just a default value
	if ( m_pPrintout )
	{
		//m_pPrintout->CalcScale(&dc);
		dScale = m_pPrintout->GetMMScale();
	}
	//wxMessageBox(_("msg11"));
	//wxLogError( wxString::Format( "dScale = %f", dScale ) );

	// setting up header-objects
	// -------------------------
	pNode = (wxNode *)m_lstHeader.GetFirst();
	//wxMessageBox(_("msg12"));
	while (pNode)
	{
		pObj = (wxReportObjViewer *)pNode->GetData();

		pObj->ResetPos();
		pObj->SetScale( dScale );	// MM --> log.Units

		pNode = pNode->GetNext();
		//wxMessageBox(_("msg12-1"));

	}
	//wxMessageBox(_("msg13"));
	// setting up footer-objects
	// -------------------------
	pNode = (wxNode *)m_lstFooter.GetFirst();
	while (pNode)
	{
		pObj = (wxReportObjViewer *)pNode->GetData();

		pObj->ResetPos();
		pObj->SetScale( dScale );	// MM --> log.Units

		pNode = pNode->GetNext();

	}

	// drawing header-objects
	// ----------------------
	pNode = (wxNode *)m_lstHeader.GetFirst();
	while (pNode)
	{
		pObj = (wxReportObjViewer *)pNode->GetData();
		pObj->Draw( dc );

		pNode = pNode->GetNext();
	
        //wxLogError( wxString::Format( "Draw Header: %s at x=%i, y=%i", pObj->GetString().c_str(), pObj->GetCurrPos().x, pObj->GetCurrPos().y ) );
	}

	m_PageCtrl.m_dFooterMinY = m_PageCtrl.m_dYSizeMM - m_PageCtrl.m_dBottom;

	// drawing footer-objects
	// ----------------------
	pNode = (wxNode *)m_lstFooter.GetFirst();
	while (pNode)
	{
		wxPoint pt;

		pObj = (wxReportObjViewer *)pNode->GetData();

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
	
        //wxLogError( wxString::Format( "Draw Footer: %s at x=%i, y=%i", pObj->GetString().c_str(), pObj->GetCurrPos().x, pObj->GetCurrPos().y ) );
	}

	boEOP = false;
	m_PageCtrl.m_dDataCurrY = m_PageCtrl.m_dTop + m_PageCtrl.m_dHeaderHeight;

    // Positioning internal pointer to appropriate list
    if ( m_lCurrentSection < 0 || m_lCurrentSection >= (long)m_lstSections.GetCount() )
	{
		//wxMessageBox(_("msg15"));
        return;
	}

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
		pObj = (wxReportObjViewer *)pNode->GetData();

		pObj->ResetPos();
		pObj->SetScale( dScale );

	    pNode = pNode->GetNext();
	}

	// testing...
	// ----------------------------------
	m_lmyLastSection = m_lCurrentSection;

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

			/*case POTYPE_LINE:	// Line within data section!!!
				{
					double dMinX;
					double dMaxX;
					double dMaxY;
					double dX;
					double dY;
					wxPoint pt;

					if ( ! m_pObjList )
						break;

					dMinX = 0.0; //10000.0;
					dMaxX = 0.0; //-10000.0;
					dMaxY = 0.0; //-10000.0;
					
					pNode = (wxNode *)m_pObjList->GetFirst();
					while ( pNode )
					{
						pObj = (wxReportObjViewer *)pNode->GetData();

						// find maximum Y of PREVIOUS positions
						pt = pObj->GetCurrPos();

						dY  = 0.01 * pObj->MoveBackward().y;	// WITHOUT margin

						if ( dY > dMaxY )
							dMaxY = dY;

						//pt.y += HLINE_INC;			// 0.2mm forward in Y
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
				} break;*/

            case POTYPE_SUBSTART:
            case POTYPE_SUBEND:
                if ( objHelp.GetType() == POTYPE_SUBSTART )
				{
					long lHelp;

					// testing...
					// -------------------------------
					//pNode = (wxNode *)m_lstSections.Item( m_lCurrentSection );
					pNode = (wxNode *)m_lstSections.Item( m_lmyLastSection );
					if ( pNode )
					{
					    pSection = (wxReportSection *)pNode->GetData();

						if ( pSection )
						{

							lHelp = pSection->m_lSections;
							++lHelp;
							m_lCurrentSection = lHelp;

							// testing...
							// -----------------------------------
							m_lmyLastSection = m_lCurrentSection;

						}
					}
				}
                else
				{
	    		    m_lCurrentSection = PreviousSection( m_lCurrentSection );
					//bFirstSection = false;
				}

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
						wxReportObjViewer objDummy;
						objDummy.LoadSettings( m_fFile );	// dummy, to jump over the
															// object definitions
					}


					pObj = (wxReportObjViewer *)pNode->GetData();

					pObj->ResetPos();
					pObj->SetScale( dScale );

					pt = pObj->GetCurrPos();	// without margin

					// Minimal position is object's init position
					/*if ( (m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop) > (0.01 * pt.y) )
						pObj->SetPosition( 0.01 * pt.x, 
							m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop );*/
					if ( !bFirstSection )
					{
						pObj->SetPosition( 0.01 * pt.x,
							m_PageCtrl.m_dDataCurrY - m_PageCtrl.m_dTop + 0.01 * pt.y );
					}

				    pNode = pNode->GetNext();
				}
                break;

			default:	// Data...

				m_fFile.Seek( lPosData );	// back to the beginning of this data


				// Now analyzing new positions
				pNode = (wxNode *)m_pObjList->GetFirst();
				while (pNode && ! boEOP )
				{
					pObj = (wxReportObjViewer *)pNode->GetData();

					// add header height to first object
					if ( bFirstSection )
					{
						pObj->SetPosition( pObj->GetCurrPos().x * 0.01,
							pObj->GetCurrPos().y * 0.01 + m_PageCtrl.m_dHeaderHeight);
					}

		    		dY  = 0.01 * pObj->GetCurrPos().y;			// without margin
			    	dY += m_PageCtrl.m_dTop;					// WITH margin
					dY += 0.01 * pObj->GetInc().GetHeight();	// WITH increment
						//+ m_PageCtrl.m_dHeaderHeight;			// WITH header

    				if ( dY > m_PageCtrl.m_dDataCurrY )
	    				m_PageCtrl.m_dDataCurrY = dY;

    				if ( m_PageCtrl.m_dDataCurrY > m_PageCtrl.m_dFooterMinY )
	    				boEOP = true;
					else if ( pObj->GetType() == POTYPE_LINE && 
						(m_PageCtrl.m_dDataCurrY + pObj->GetSize().GetHeight() / 100.0) > m_PageCtrl.m_dFooterMinY )
						boEOP = true;

				    pNode = pNode->GetNext();

			    }

				// We can draw data first and the check whether at least
				// ONE of the object positions is beyond the bottom
				// margins (including footer obj.)
				pNode = (wxNode *)m_pObjList->GetFirst();
				while (pNode && ! boEOP )
				{
					pObj = (wxReportObjViewer *)pNode->GetData();

					boLoad = pObj->LoadData( m_fFile );
					if ( ! boLoad )
						boEOP = true;

					// add header height to first object
					/*if ( bFirstSection )
					{
						pObj->SetPosition( pObj->GetCurrPos().x * 0.01,
							pObj->GetCurrPos().y * 0.01 + m_PageCtrl.m_dHeaderHeight);
					}*/

		    		if ( ! boEOP )
			    		pObj->Draw( dc );

				    pNode = pNode->GetNext();

			    }
				bFirstSection = false;

				break;

        } // switch

	} // while

}

void wxReportViewer::DeleteReportObj( wxReportObj* obj )
{
	wxReportObjViewer* obj2 = (wxReportObjViewer*)obj;
	wxDELETE( obj2 );
}

void wxReportViewer::EndSection( void )
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

ReportPrintout::ReportPrintout(wxReportViewer *pFrame, wxString szTitle )
 : wxPrintout(szTitle)
{
	m_pReportPage = pFrame;

	if ( pFrame )
		pFrame->SetPrintout( this );

	m_fScaleMMToLogUnits = (float)3.78; // just a default
}


ReportPrintout::~ReportPrintout( void )
{
	if ( m_pReportPage )
		m_pReportPage->SetPrintout( 0 );
}

bool ReportPrintout::OnPrintPage(int page)
{
    wxDC *dc = GetDC();
	//wxMessageBox(_("msg3 print page"));
    if (dc)
    {
		//wxMessageBox(_("ok"));
		DrawPage( dc, page );
        return TRUE;
    }
    else
	{
        //wxMessageBox(_("bad"));
		return FALSE;
	}
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
	//wxLogError( wxString::Format( "fScale=%f", fScale ) );
    
    // Now we have to check in case our real page size is reduced
    // (e.g. because we're drawing to a print preview memory DC)
    dc->GetSize( &w, &h );
//  GetPageSizeMM( &pageWidth, &pageHeight );
    GetPageSizePixels( &pageWidth, &pageHeight );
    
    // If printer pageWidth == current DC width, then this doesn't
    // change. But w might be the preview bitmap width, so scale down.
    m_fOverallScale = fScale * (float)((float)w/(float)pageWidth);
    //wxLogError( wxString::Format( "m_fOverallScale=%f", m_fOverallScale ) );

	dc->SetUserScale( m_fOverallScale, m_fOverallScale );
    
    // Calculate conversion factor for converting millimetres into
    // logical units.
    // There are approx. 25.4 mm to the inch. There are ppi
    // device units to the inch. Therefore 1 mm corresponds to
    // ppi/25.4 device units. We also divide by the
    // screen-to-printer scaling factor, because we need to
    // unscale to pass logical units to DrawLine.
    
    m_fScaleMMToLogUnits = (float)((float)ppiPrinterX/(fScale * 25.4));
    //wxLogError( wxString::Format( "m_fScaleMMToLogUnits=%f", m_fScaleMMToLogUnits ) );

    int pageWidthMM, pageHeightMM;
    GetPageSizeMM( &pageWidthMM, &pageHeightMM );
}

void ReportPrintout::OnEndPrinting()
{
}

double ReportPrintout::GetMMScale()
{
	//wxLogError( wxString::Format( "GetMMScale: %f", m_fScaleMMToLogUnits ) );
	return m_fScaleMMToLogUnits;
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
	wxReportViewer	repPage( 0, wxT(""), wxPoint(0,0), wxSize(10,10) );

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

		iIdx    = (int)aSortedByDate.GetCount() - 1 - iItem;
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

void wxReportSelector::OnPreview( wxCommandEvent& WXUNUSED(event) )
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


void wxReportSelector::OnPrint( wxCommandEvent& WXUNUSED(event) )
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


void wxReportSelector::OnDelete( wxCommandEvent& WXUNUSED(event) )
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

void wxReportSelector::OnIdle( wxIdleEvent& WXUNUSED(event) )
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

