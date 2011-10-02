#ifdef __GNUG__
#pragma implementation "wxReportFiller.h"
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
#include <wx/tokenzr.h>

#include "wxReportFiller.h"
#include "wxMaskController.h"

wxReportFiller::wxReportFiller()
{
	report	= NULL;
	pReport	= NULL;

#ifdef USE_PYTHON_INTERPRETER
	if ( !InitPython() )
		return; //false;
#endif // USE_PYTHON_INTERPRETER
}

wxReportFiller::wxReportFiller( wxString reportPath )
{
	wxReportFiller();

	reportFile = reportPath;
	rootFolder = reportFile.GetPath( true );
}

wxReportFiller::~wxReportFiller()
{
	if ( report ) wxDELETE( report );
	if ( pReport ) wxDELETE( pReport );

	for (int i=0; i<(int)handlers.GetCount(); i++)
		wxDELETE( handlers[i] );

#ifdef USE_PYTHON_INTERPRETER
    // Restore the thread state and tell Python to cleanup after itself.
    // wxPython will do its own cleanup as part of that process.  This is done
    // in OnExit instead of ~MyApp because OnExit is only called if OnInit is
    // successful.
    //wxPyEndAllowThreads(m_mainTState);
    Py_Finalize();
#endif // USE_PYTHON_INTERPRETER
}

bool wxReportFiller::OpenTemplate( )
{
	// try to do everything
    wxXmlDocument *doc = new wxXmlDocument();
    //TiXmlDocument *doc = new TiXmlDocument();
	wxString err;

	// parse the file
	if ( !doc->Load( reportFile.GetFullPath() ) )
	{
		//wxLogError(doc->ErrorDesc());
        wxLogError( _("The XML template was not loaded successfully.") );
		return false;
	}

    if ( ParseDocument( doc ) )
	{
		if ( pReport ) wxDELETE( pReport );
		pReport = new wxReportWriter( /*parent, report->name, wxPoint(10,10), wxSize( 100, 100 )*/ );
        if ( doc ) wxDELETE( doc );
		return true;
	}

    if ( doc ) wxDELETE( doc );
	return false;
}

bool wxReportFiller::OpenTemplate( wxString templatePath )
{
	reportFile = templatePath;
	rootFolder = reportFile.GetVolume() + reportFile.GetVolumeSeparator() + reportFile.GetPath( true );

	return OpenTemplate( );
}

bool wxReportFiller::ParseDocument( wxXmlDocument *xmldoc )
{
	try
	{
		// Get root document element
        wxXmlNodeEx* root = (wxXmlNodeEx*)xmldoc->GetRoot();
		if (!root)
		{
			return false;
		}

		// create the report object
		if ( report ) wxDELETE ( report );
		report = new wxReport();

		// get the report properties
        if ( root->HasProp( wxT("name") ) )
            report->name = root->GetPropVal( wxT("name"), wxT("") );
		if ( root->HasProp( wxT("pageWidth") ) )
		    report->pageWidth = wxAtof( root->GetPropVal( wxT("pageWidth"), wxT("") ) );
		if ( root->HasProp( wxT("pageHeight") ) )
		    report->pageHeight = wxAtof( root->GetPropVal( wxT("pageHeight"), wxT("") ) );
		if ( root->HasProp( wxT("columnWidth") ) )
		    report->columnWidth = wxAtof( root->GetPropVal( wxT("columnWidth"), wxT("") ) );
		if ( root->HasProp( wxT("leftMargin") ) )
		    report->leftMargin = wxAtof( root->GetPropVal( wxT("leftMargin"), wxT("") ) );
		if ( root->HasProp( wxT("rightMargin") ) )
		    report->rightMargin = wxAtof( root->GetPropVal( wxT("rightMargin"), wxT("") ) );
		if ( root->HasProp( wxT("topMargin") ) )
		    report->topMargin = wxAtof( root->GetPropVal( wxT("topMargin"), wxT("") ) );
		if ( root->HasProp( wxT("bottomMargin") ) )
		    report->bottomMargin = wxAtof( root->GetPropVal( wxT("bottomMargin"), wxT("") ) );

		// get page format
        wxXmlNodeEx *paperNode = root->GetFirstChild( wxT("paperFormat") );
		if ( paperNode )
		{
            if ( paperNode->HasProp( wxT("style") ) )
                report->paperFormat = wxAtoi( paperNode->GetPropVal( wxT("style"), wxT("") ) );
		}

		// get date formats
		wxXmlNodeEx *dateNode = root->GetFirstChild( wxT("dateFormat") );
		if ( dateNode )
		{
            if ( dateNode->HasProp( wxT("long") ) )
                report->dateLongFormat = dateNode->GetPropVal( wxT("long"), wxT("") );
			else
                report->dateLongFormat = wxT("%d.%m.%Y  %H:%M:%S");
            if ( dateNode->HasProp( wxT("short") ) )
                report->dateShortFormat = dateNode->GetPropVal( wxT("short"), wxT("") );
            else
                report->dateShortFormat = wxT("%d/%m/%Y");
		}

		// get report contact info
        wxXmlNodeEx *contactNode = root->GetFirstChild( wxT("reportContact") );
		if ( contactNode )
		{
            if ( contactNode->HasProp( wxT("name") ) )
			    report->contactName = contactNode->GetPropVal( wxT("name"), wxT("") );
		}

		// get report fonts
        wxXmlNodeEx *fontNode = root->GetFirstChild( wxT("reportFont") );
		for ( int i=1; fontNode != NULL ; i++ )
		{
			wxReportFont font;
			// get the font properties
            if ( fontNode->HasProp( wxT("name") ) )
			    font.name = fontNode->GetPropVal( wxT("name"), wxT("") );
            if ( fontNode->HasProp( wxT("fontFamily") ) )
			    font.fontFamily = wxAtoi( fontNode->GetPropVal( wxT("fontFamily"), wxT("") ) );
            if ( fontNode->HasProp( wxT("fontName") ) )
			    font.fontName = fontNode->GetPropVal( wxT("fontName"), wxT("") );
            if ( fontNode->HasProp( wxT("isDefault") ) )
                font.isDefault = wxAtoi( fontNode->GetPropVal( wxT("isDefault"), wxT("") ) ) == 1 ? true : false;
            if ( fontNode->HasProp( wxT("size") ) )
			    font.fontSize = wxAtoi( fontNode->GetPropVal( wxT("size"), wxT("") ) );
            if ( fontNode->HasProp( wxT("isBold") ) )
			    font.isBold = wxAtoi( fontNode->GetPropVal( wxT("isBold"), wxT("") ) ) == 1 ? true : false;
            if ( fontNode->HasProp( wxT("isItalic") ) )
			    font.isItalic = wxAtoi( fontNode->GetPropVal( wxT("isItalic"), wxT("") ) ) == 1 ? true : false;
            if ( fontNode->HasProp( wxT("isUnderline") ) )
			    font.isUnderline = wxAtoi( fontNode->GetPropVal( wxT("isUnderline"), wxT("") ) ) == 1 ? true : false;
            if ( fontNode->HasProp( wxT("isStrikeThrough") ) )
			    font.isStrikeThrough = wxAtoi( fontNode->GetPropVal( wxT("isStrikeThrough"), wxT("") ) ) == 1 ? true : false;
            if ( fontNode->HasProp( wxT("fontColor") ) )
			    font.fontColor = fontNode->GetPropVal( wxT("fontColor"), wxT("") );

			// add font
			report->fonts[ font.name ] = font;

			// add the default font
			if ( font.isDefault )
				report->fonts[ wxT("Default") ] = font;

			// get the next font
            fontNode = root->GetNextSibling( fontNode, wxT("reportFont") );
		}

		// get report parameters
        wxXmlNodeEx *paramNode = root->GetFirstChild( wxT("parameter") );
		for ( int i=1; paramNode != NULL; i++ )
		{
			wxReportParameter param;
			// get the parameter properties
            if ( paramNode->HasProp( wxT("name") ) )
                param.name = paramNode->GetPropVal( wxT("name"), wxT("") );
            if ( paramNode->HasProp( wxT("type") ) )
                param.type = paramNode->GetPropVal( wxT("type"), wxT("") );

			// add font
			report->parameters[ param.name.c_str() ] = param;

			// get the next font
            paramNode = root->GetNextSibling(paramNode, wxT("parameter") );
		}

		// get report variables
        wxXmlNodeEx *varNode = root->GetFirstChild( wxT("variable") );
		for ( int i=1; varNode != NULL; i++ )
		{
			wxReportVariable var;
			// get the variable properties
            if ( varNode->HasProp( wxT("name") ) )
                var.name = varNode->GetPropVal( wxT("name"), wxT("") );
            if ( varNode->HasProp( wxT("type") ) )
                var.type = varNode->GetPropVal( wxT("type"), wxT("") );
            if ( varNode->HasProp( wxT("resetType") ) )
                var.resetType = varNode->GetPropVal( wxT("resetType"), wxT("") );
            if ( varNode->HasProp( wxT("resetGroup") ) )
                var.resetGroup = varNode->GetPropVal( wxT("resetGroup"), wxT("") );
            if ( varNode->HasProp( wxT("format") ) )
                var.format = varNode->GetPropVal( wxT("format"), wxT("") );
            if ( varNode->HasProp( wxT("mask") ) )
                var.mask = varNode->GetPropVal( wxT("mask"), wxT("") );

			// get the calculation to be performed
            wxXmlNode *calcNode = varNode->GetFirstChild( wxT("calculation") );
			if ( calcNode != NULL )
			{
				// get cdata node
                var.calculation = calcNode->GetChildren()->GetContent();
			}

			// get the initial value node
			wxXmlNode *iniNode = varNode->GetFirstChild( wxT("initialValueExpression") );
			if ( iniNode != NULL )
			{
				// get cdata node
                var.initialValueExpression = iniNode->GetChildren()->GetContent();
			}

			// get the initial value node
            wxXmlNode *evalNode = varNode->GetFirstChild( wxT("evaluateOnGroups") );
			if ( evalNode != NULL )
			{
				// get cdata node
                wxStringTokenizer tok( evalNode->GetChildren()->GetContent(), wxT(",") );
				while ( tok.HasMoreTokens() )
					var.evaluateGroups.Add( tok.GetNextToken() );
			}

			// set initial value
			var.value = var.initialValueExpression;

			// add variable
			report->variables[ var.name.c_str() ] = var;
			report->arVariables.Add( var.name );

			// get the next variable
            varNode = root->GetNextSibling( varNode, wxT("variable") );
		}

		// get page header
        wxXmlNodeEx *phNode = root->GetFirstChild( wxT("pageHeader") );
		if ( phNode != NULL )
		{
			if ( !LoadObjects( phNode, &report->header.objects ) )
				return false;
		}

		// get page footer
        wxXmlNodeEx *pfNode = root->GetFirstChild( wxT("pageFooter") );
		if ( pfNode != NULL )
		{
			if ( !LoadObjects( pfNode, &report->footer.objects ) )
				return false;
		}

		// get data sections
        wxXmlNodeEx *dsNode = root->GetFirstChild( wxT("dataSection") );
		for ( int i=1; dsNode != NULL; i++ )
		{
			wxReportDataSection* ds = LoadDataSection( dsNode );
			
			if ( ds ) report->sections.Add( ds );

            dsNode = root->GetNextSibling( dsNode, wxT("dataSection") );
		}

		return true;
	}
	catch (...)
	{
		wxLogStatus( wxT("An error occurred while parsing the xml documet.") );
		return false;
	}
}

wxReportDataSection* wxReportFiller::LoadDataSection( wxXmlNodeEx *node )
{
	wxReportDataSection *ds = new wxReportDataSection();

    if ( node->HasProp( wxT("name") ) )
        ds->name = node->GetPropVal( wxT("name"), wxT("") );
    if ( node->HasProp( wxT("type") ) )
        ds->type = node->GetPropVal( wxT("type"), wxT("") );

	// load section properties
	if ( !LoadSectionProperties( node, ds ) )
		return NULL;
		
	// use default loader
	if ( !LoadObjects( node, &(ds->objects) ) )
		return NULL;

	// get data sections
    wxXmlNodeEx *dsNode = node->GetFirstChild( wxT("dataSection") );
	for ( int i=1; dsNode != NULL; i++ )
	{
		wxReportDataSection* ds2 = LoadDataSection( dsNode );
		
		if ( ds2 ) ds->sections.Add( ds2 );

        dsNode = node->GetNextSibling( dsNode, wxT("dataSection") );
	}

	return ds;
}

bool wxReportFiller::LoadSectionProperties( wxXmlNodeEx *node, wxReportDataSection* ds )
{
	try
	{
		// get report fields
        wxXmlNodeEx *fieldNode = node->GetFirstChild( wxT("field") );
		for ( int i=1; fieldNode != NULL; i++ )
		{
			wxReportField field;
			// get the font properties
            if ( fieldNode->HasProp( wxT("name") ) )
                field.name = fieldNode->GetPropVal( wxT("name"), wxT("") );
            if ( fieldNode->HasProp( wxT("class") ) )
                field.type = fieldNode->GetPropVal( wxT("class"), wxT("") );
			// get format
            if ( fieldNode->HasProp( wxT("format") ) )
                field.format = fieldNode->GetPropVal( wxT("format"), wxT("") );
			// get mask
            if ( fieldNode->HasProp( wxT("mask") ) )
                field.mask = fieldNode->GetPropVal( wxT("mask"), wxT("") );

			// add font
			ds->fields[field.name.c_str()] = field;

			// get the next font
            fieldNode = node->GetNextSibling( fieldNode, wxT("field") );
		}

		// get hanlder-specific properties
		for (int i=0; i<(int)handlers.GetCount(); i++)
		{
			if ( ds->type.CmpNoCase( handlers[i]->type ) == 0 )
			{
				handlers[i]->LoadSectionProperties( node, ds );
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool wxReportFiller::LoadObjects( wxXmlNodeEx *node, ObjectArray *ar )
{
	try
	{
		// loop through child nodes
        wxXmlNodeEx *childNode = (wxXmlNodeEx*)node->GetChildren();
		while ( childNode != NULL )
		{
            if ( wxString(childNode->GetName()).CmpNoCase( _("staticText") ) == 0 )
			{
				// create a new static text object
				wxReportStaticText *st = new wxReportStaticText();
				
				// get the text properties
                if ( childNode->HasProp( wxT("x") ) )
                    st->xpos = wxAtoi( childNode->GetPropVal( wxT("x"), wxT("") ) );
                if ( childNode->HasProp( wxT("y") ) )
                    st->ypos = wxAtoi( childNode->GetPropVal( wxT("y"), wxT("") ) );
                if ( childNode->HasProp( wxT("width") ) )
                    st->width = wxAtoi( childNode->GetPropVal( wxT("width"), wxT("") ) );
                if ( childNode->HasProp( wxT("height") ) )
                    st->height = wxAtoi( childNode->GetPropVal( wxT("height"), wxT("") ) );
                if ( childNode->HasProp( wxT("align") ) )
                    st->alignment = childNode->GetPropVal( wxT("align"), wxT("") );
                if ( childNode->HasProp( wxT("increment") ) )
                    st->increment = wxAtoi( childNode->GetPropVal( wxT("increment"), wxT("") ) );

				// get the font name
                wxXmlNode *stFontNode = childNode->GetFirstChild( wxT("font") );
				if ( stFontNode != NULL )
                {
                    if ( stFontNode->HasProp( wxT("reportFont") ) )
                        st->font = stFontNode->GetPropVal( wxT("reportFont"), wxT("") );
                }

				// get the text to print
                wxXmlNode* stTextNode = childNode->GetFirstChild( wxT("text") );
				if ( stTextNode != NULL )
				{
					// get cdata node
                    st->text = stTextNode->GetChildren()->GetContent();
				}
				// add static text to section
				ar->Add( st );
			}
			else if ( wxString(childNode->GetName()).CmpNoCase( wxT("textField") ) == 0 )
			{
				// create a new static text object
				wxReportTextField *tf = new wxReportTextField();
				
				// get the text properties
                if ( childNode->HasProp( wxT("x") )) 
                    tf->xpos = wxAtoi(childNode->GetPropVal( wxT("x"), wxT("") ));
				if ( childNode->HasProp( wxT("y") )) 
                    tf->ypos = wxAtoi(childNode->GetPropVal( wxT("y"), wxT("") ));
				if ( childNode->HasProp( wxT("width") )) 
                    tf->width = wxAtoi(childNode->GetPropVal( wxT("width"), wxT("") ));
				if ( childNode->HasProp( wxT("height") )) 
                    tf->height = wxAtoi(childNode->GetPropVal( wxT("height"), wxT("") ));
				if ( childNode->HasProp( wxT("align") )) 
                    tf->alignment = childNode->GetPropVal( wxT("align"), wxT("") );
				if ( childNode->HasProp( wxT("increment") )) 
                    tf->increment = wxAtoi(childNode->GetPropVal( wxT("increment"), wxT("") ));

				// get the font name
                wxXmlNodeEx* tfFontNode = childNode->GetFirstChild( wxT("font") );
				if ( tfFontNode != NULL )
                {
					if ( tfFontNode->HasProp( wxT("reportFont") ) ) 
                        tf->font = tfFontNode->GetPropVal( wxT("reportFont"), wxT("") );
                }

				// get the text to print
                wxXmlNodeEx* tfTextNode = childNode->GetFirstChild( wxT("text") );
				if ( tfTextNode != NULL )
				{
					// get cdata node
                    tf->text = tfTextNode->GetChildren()->GetContent();
				}

				// add static text to section
				ar->Add( tf );
			}
			else if ( wxString(childNode->GetName()).CmpNoCase( wxT("bitmapImage") ) == 0 )
			{
				// create a bitmap object
				wxReportBitmap *bmp = new wxReportBitmap();
				
				// get the text properties
				if ( childNode->HasProp( wxT("x") ) ) 
                    bmp->xpos = atoi((childNode->GetPropVal( wxT("x"), wxT("") ) ).mb_str(wxConvUTF8));
				if ( childNode->HasProp( wxT("y") ) ) 
                    bmp->ypos = atoi((childNode->GetPropVal( wxT("y"), wxT("") ) ).mb_str(wxConvUTF8));
				if ( childNode->HasProp( wxT("width") ) ) 
                    bmp->width = wxAtoi( childNode->GetPropVal( wxT("width"), wxT("") ) );
				if ( childNode->HasProp( wxT("height") ) ) 
                    bmp->height = atoi((childNode->GetPropVal( wxT("height"), wxT("") ) ).mb_str(wxConvUTF8));

				// get the text to print
                wxXmlNode* bmpNameNode = childNode->GetFirstChild( wxT("name") );
				if ( bmpNameNode != NULL )
				{
					// get cdata node
                    bmp->name = bmpNameNode->GetChildren()->GetContent();
				}

				// add bitmap to section
				ar->Add( bmp );
			}
			else if ( wxString(childNode->GetName()).CmpNoCase( wxT("staticLine") ) == 0 )
			{
				// create a bitmap object
				wxReportStaticLine *line = new wxReportStaticLine();
				
				// get the text properties
				if ( childNode->HasProp( wxT("x") ) ) 
                    line->xpos = wxAtoi( childNode->GetPropVal( wxT("x"), wxT("") ) );
				if ( childNode->HasProp( wxT("y") ) ) 
                    line->ypos = wxAtoi( childNode->GetPropVal( wxT("y"), wxT("") ) );
				if ( childNode->HasProp( wxT("width") ) ) 
                    line->width = wxAtoi( childNode->GetPropVal( wxT("width"), wxT("") ) );
				if ( childNode->HasProp( wxT("height") ) ) 
                    line->height = wxAtoi( childNode->GetPropVal( wxT("height"), wxT("") ) );
				if ( childNode->HasProp( wxT("increment") ) ) 
                    line->increment = wxAtoi( childNode->GetPropVal( wxT("increment"), wxT("") ) );
				if ( childNode->HasProp( wxT("penWidth") ) ) 
                    line->penWidth = wxAtoi( childNode->GetPropVal( wxT("penWidth"), wxT("") ) );

				// add line to section
				ar->Add( line );
			}
			else if ( wxString(childNode->GetName()).CmpNoCase( wxT("pageBreak") ) == 0 )
			{
				// create a page break object
				wxReportBreak *brk = new wxReportBreak();

				// add break to section
				ar->Add( brk );
			}
			else if ( wxString(childNode->GetName()).CmpNoCase( wxT("checkBox") ) == 0 )
			{
				// create a logic object
				wxReportLogic *logic = new wxReportLogic();

				// set checkbox type
				logic->SetType( WXRPT_OBJ_CHECK_BOX );

				// get the check box properties
				if ( childNode->HasProp( wxT("x") ) ) 
                    logic->xpos = wxAtoi( childNode->GetPropVal( wxT("x"), wxT("") ) );
				if ( childNode->HasProp( wxT("y") ) ) 
                    logic->ypos = wxAtoi( childNode->GetPropVal( wxT("y"), wxT("") ) );
				if ( childNode->HasProp( wxT("width") ) ) 
                    logic->width = wxAtoi( childNode->GetPropVal( wxT("width"), wxT("") ) );
				if ( childNode->HasProp( wxT("height") ) ) 
                    logic->height = wxAtoi( childNode->GetPropVal( wxT("height"), wxT("") ) );
				if ( childNode->HasProp( wxT("increment") ) ) 
                    logic->increment = wxAtoi( childNode->GetPropVal( wxT("increment"), wxT("") ) );

				// get the name of the variable or field
                wxXmlNode* lgBoolNode = childNode->GetFirstChild( wxT("name") );
				if ( lgBoolNode != NULL )
				{
					// get cdata node
                    logic->name = lgBoolNode->GetChildren()->GetContent();
				}

				// add check box to section
				ar->Add( logic );
			}
			else if ( wxString(childNode->GetName()).CmpNoCase( wxT("logic") ) == 0 )
			{
				// create a logic object
				wxReportLogic *logic = new wxReportLogic();

				// set logic type
				logic->SetType( WXRPT_OBJ_LOGIC );

				// get the logic properties
				if ( childNode->HasProp( wxT("x") ) ) 
                    logic->xpos = wxAtoi( childNode->GetPropVal( wxT("x"), wxT("") ) );
				if ( childNode->HasProp( wxT("y") ) ) 
                    logic->ypos = wxAtoi( childNode->GetPropVal( wxT("y"), wxT("") ) );
				if ( childNode->HasProp( wxT("width") ) ) 
                    logic->width = wxAtoi( childNode->GetPropVal( wxT("width"), wxT("") ) );
				if ( childNode->HasProp( wxT("height") ) ) 
                    logic->height = wxAtoi( childNode->GetPropVal( wxT("height"), wxT("") ) );
				if ( childNode->HasProp( wxT("increment") ) ) 
                    logic->increment = wxAtoi( childNode->GetPropVal( wxT("increment"), wxT("") ) );

				// get the name of the variable or field
                wxXmlNode* lgBoolNode = childNode->GetFirstChild( wxT("name") );
				if ( lgBoolNode != NULL )
				{
					// get cdata node
                    logic->name = lgBoolNode->GetChildren()->GetContent();
				}

				// add logic to section
				ar->Add( logic );
			}

			// get next child
            childNode = node->GetNextSibling( childNode );
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool wxReportFiller::FillReport()
{
	wxReportObj		*pObj;

	wxString		strName;

	//wxLogNull		logNull;

	// Set the output path
	pReport->SetPath( wxT(".") );

	// -------------------------
	//  Set report margins
	// -------------------------
	pReport->SetTopMargin( (double)report->topMargin );
	pReport->SetBottomMargin( (double)report->bottomMargin );
	pReport->SetLeftMargin( (double)report->leftMargin );
	pReport->SetRightMargin( (double)report->rightMargin );
	pReport->SetPaperFormat( report->paperFormat );
	pReport->SetPageSize( report->pageWidth, report->pageHeight );
	//pReport->SetOrientation( report->pageHeight > report->pageWidth ? wxPORTRAIT : wxLANDSCAPE );

	// -----------------------
	//  Extra data
	// -----------------------
	pReport->SetDateFormat( report->dateLongFormat );
	pReport->SetShortDateFormat( report->dateShortFormat );
	pReport->SetDate( wxDateTime::Now() );
	pReport->SetUser( report->contactName );
	pReport->SetInfo( report->name );

	// -----------------------
	//  Page headers
	// -----------------------
	for (int i=0; i<(int)report->header.objects.GetCount(); i++)
	{
		wxReportObject *obj = report->header.objects[i];
		pObj = GetReportObject( obj, NULL );
		if ( pObj ) pReport->AddHeaderObj( pObj );
	}

	// -----------------------
	//  Page footers
	// -----------------------
	for (int i=0; i<(int)report->footer.objects.GetCount(); i++)
	{
		wxReportObject *obj = report->footer.objects[i];
		pObj = GetReportObject( obj, NULL );
		if ( pObj ) pReport->AddFooterObj( pObj );
	}

	// -----------------------
	//  DATA...
	// -----------------------
	wxString dummy = wxT("");
	pReport->DefineSection();

	pObj = new wxReportObj( 0, 0, 5, 5 );
	pObj->SetRef( &dummy );
	pObj->SetIncrements( 0.0, 2.5 );
	pObj->SetFont( &report->fonts[wxT("Default")] );
	pReport->AddDataObj( pObj );

	pReport->BeginDataSection();

	// -----------------------
	//  Data sections
	// -----------------------
	for (int i=0; i<(int)report->sections.GetCount(); i++)
	{
		wxReportDataSection *ds = report->sections[i];

		// start the section
		pReport->DefineSection();

		// loop through objects
		for (int z=0; z<(int)ds->objects.GetCount(); z++)
		{
			wxReportObject *obj = ds->objects[z];
			pObj = GetReportObject( obj, ds );
			if ( pObj ) pReport->AddDataObj( pObj );
		}

		// add dummy section if needed
		if ( (int)ds->objects.GetCount() < 1 )
		{
			pObj = new wxReportObj( 0, 0, 5, 5 );
			pObj->SetRef( &dummy );
			pObj->SetIncrements( 0.0, 2.5 );
			pObj->SetFont( &report->fonts[wxT("Default")] );
			pReport->AddDataObj( pObj );
		}

		// add data
		pReport->BeginDataSection();
		GetSectionData( ds );

		// close the section
		pReport->FinishSection();
	}

	pReport->SaveData();

	pReport->FinishSection();

	pReport->FinishReport();

	//pReport->SetupReport( pReport->FinalDestination() );

	return true;
}

bool wxReportFiller::GetSectionData( wxReportDataSection* section )
{
	try
	{
		for (int i=0; i<(int)handlers.GetCount(); i++)
		{
			if ( section->type.CmpNoCase( handlers[i]->type ) == 0 )
			{
				return handlers[i]->GetSectionData( section, this );
			}
		}

		return SaveSectionData( section );
	}
	catch (...)
	{
		return false;
	}
}

wxReportObj* wxReportFiller::GetReportObject( wxReportObject *obj, wxReportDataSection* ds )
{
	wxReportObj *pObj = NULL;
	if ( obj->GetType() == WXRPT_OBJ_STAT_TEXT )
	{
		wxReportStaticText *st = (wxReportStaticText*)(obj);
		pObj = new wxReportObj( st->xpos, st->ypos, st->width, st->height );
		pObj->SetRef( st->text );
		pObj->SetIncrements( 0.0, st->increment );
		pObj->SetFont( &report->fonts[st->font.c_str()] );
		if ( st->alignment.CmpNoCase( wxT("right") ) == 0 )
			pObj->SetRightAlign();
		else if ( st->alignment.CmpNoCase( wxT("center") ) == 0 )
			pObj->SetCenterAlign();
	}
	else if ( obj->GetType() == WXRPT_OBJ_BITMAP )
	{
		wxReportBitmap *bmp = (wxReportBitmap*)(obj);
		pObj = new wxReportObj( bmp->xpos, bmp->ypos, bmp->width, bmp->height );
		pObj->SetBitmap( bmp->name );
	}
	else if ( obj->GetType() == WXRPT_OBJ_TEXT_FIELD )
	{
		wxReportTextField *tfld = (wxReportTextField*)(obj);
		pObj = new wxReportObj( tfld->xpos, tfld->ypos, tfld->width, tfld->height );
		EvaluateTextField( *tfld, pObj, ds );
		pObj->SetIncrements( 0.0, tfld->increment );
		pObj->SetFont( &report->fonts[tfld->font.c_str()] );
		if ( tfld->alignment.CmpNoCase( wxT("right") ) == 0 )
			pObj->SetRightAlign();
		else if ( tfld->alignment.CmpNoCase( wxT("center") ) == 0 )
			pObj->SetCenterAlign();
	}
	else if ( obj->GetType() == WXRPT_OBJ_STAT_LINE )
	{
		wxReportStaticLine *line = (wxReportStaticLine*)(obj);
		pObj = new wxReportObj( line->xpos, line->ypos, line->width, line->height );
		pObj->SetPenWidth( line->penWidth );
		pObj->SetIncrements( 0, line->increment );
		pObj->SetLine();
	}
	else if ( obj->GetType() == WXRPT_OBJ_PAGE_BREAK )
	{
		pObj = new wxReportObj();
		pObj->SetPageBreak();
	}
	else if ( obj->GetType() == WXRPT_OBJ_LOGIC )
	{
		wxReportLogic *logic = (wxReportLogic*)obj;
		pObj = new wxReportObj( logic->xpos, logic->ypos, logic->width, logic->height );
		pObj->SetIncrements( 0.0, logic->increment );
		//logic->val = &ds->fields[logic->name.c_str()].bValue;
		// variables
		if ( logic->name.Find(wxT("$V{")) > -1 )
		{
			wxString varName = logic->name.Mid(3, logic->name.Length() - 4);
			logic->val = &report->variables[ varName.c_str() ].bValue;
			pObj->SetLogic( logic->val, -1 );
		}
		else if ( logic->name.Find( wxT("$F{") ) > -1 )
		{
			wxString fieldName = logic->name.Mid(3, logic->name.Length() - 4);
			logic->val = &ds->fields[fieldName.c_str()].bValue;
			pObj->SetLogic( logic->val, -1 );
		}
	}
	else if ( obj->GetType() == WXRPT_OBJ_CHECK_BOX )
	{
		wxReportLogic *logic = (wxReportLogic*)obj;
		pObj = new wxReportObj( logic->xpos, logic->ypos, logic->width, logic->height );
		pObj->SetIncrements( 0.0, logic->increment );
		//logic->val = &ds->fields[logic->name.c_str()].bValue;
		// variables
		if ( logic->name.Find(wxT("$V{")) > -1 )
		{
			wxString varName = logic->name.Mid(3, logic->name.Length() - 4);
			logic->val = &report->variables[ varName.c_str() ].bValue;
			pObj->SetCheckbox( logic->val, -1 );
		}
		else if ( logic->name.Find( wxT("$F{") ) > -1 )
		{
			wxString fieldName = logic->name.Mid(3, logic->name.Length() - 4);
			logic->val = &ds->fields[fieldName.c_str()].bValue;
			pObj->SetCheckbox( logic->val );
		}
	}

	return pObj;
}

void wxReportFiller::EvaluateTextField( wxReportTextField tfld, wxReportObj* obj, wxReportDataSection* ds )
{
	// variables
	if ( tfld.text.Find(wxT("$V{")) > -1 )
	{
		// check for page number
		if ( tfld.text.Find( wxT("$V{PageNumber}") ) > -1 )
		{
			wxString str(tfld.text);
			str.Replace(wxT("$V{PageNumber}"), wxT(""));
			obj->SetPgNum( str );
		}
		// check for report date
		else if ( tfld.text.CompareTo( wxT("$V{ReportDate}") ) == 0 )
			obj->SetRepDate();
		// check for report name
		else if ( tfld.text.CmpNoCase( wxT("$V{ReportName}") ) == 0 )
		{
			tfld.text.Replace( wxT("$V{ReportName}"), pReport->GetInfo() );
			obj->SetData( tfld.text );
		}
		// check for user name
		else if ( tfld.text.Find( wxT("$V{ContactName}") ) > -1 )
		{
			tfld.text.Replace( wxT("$V{ContactName}"), pReport->GetUser() );
			obj->SetData( tfld.text );
		}
		// user defined variable
		else
		{
			wxString varName = tfld.text.Mid(3, tfld.text.Length() - 4);
			obj->SetRef( &report->variables[ varName.c_str() ].value );
		}
	}
	// parameters
	else if ( tfld.text.Find( wxT("$P{") ) > -1 )
	{
		obj->SetData( FilterParameterString( tfld.text ) );
	}
	// data fields
	else if ( tfld.text.Find( wxT("$F{") ) > -1 )
	{
		wxString fieldName = tfld.text.Mid(3, tfld.text.Length() - 4);
		tfld.value = &ds->fields[fieldName.c_str()].value;
		obj->SetRef( tfld.value );
	}
	// static text
	else
		obj->SetData( tfld.text );
}

bool wxReportFiller::SaveSectionData( wxReportDataSection* section )
{
	// updated variables
	for (int x=0; x<(int)report->arVariables.GetCount(); x++)
	{
		VariableMapType::iterator it;
		for (it = report->variables.begin(); it != report->variables.end(); it++)
		{
			if ( it->second.name.CmpNoCase( report->arVariables[x] ) == 0 )
			{
#ifdef USE_PYTHON_INTERPRETER
				// check for evaluating variables
				bool bFound = false;
				for (int i=0; i<(int)it->second.evaluateGroups.GetCount(); i++)
				{
					if ( section->name.CmpNoCase( it->second.evaluateGroups[i] ) == 0 )
					{
						bFound = true;
						break;
					}
				}

				if ( bFound )
				{
					wxString name	= it->second.name;
					wxString calc	= it->second.calculation;
					wxString type	= it->second.type;
					wxString format	= it->second.format;
					wxString mask	= it->second.mask;

					// prepare calculation
					calc = FilterFieldString( section, 
							FilterVariableString( 
								FilterParameterString( calc )
							)
						);

                    // filter extra line breaks
                    calc.Trim(true);

					/* make a new namespace */
					PyObject *pdict, *pval;
					pdict = PyDict_New(  ); 
					PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins(  ));
				 
					// execute the calculation
					PyRun_String( calc, Py_file_input, pdict, pdict );

					// get the returned value
					pval = PyDict_GetItemString(pdict, wxT("X"));

					if ( pval )
					{
					// get the return value
						if ( type.CmpNoCase( "int" ) == 0 )
						{
							int value;
							PyArg_Parse(pval, wxT("i"), &value);                         /* convert to C */
							if ( format == wxEmptyString ) format = wxT("%i");
							it->second.value = wxString::Format( format, value );
						}
						else if ( type.CmpNoCase( wxT("long") ) == 0 )
						{
							long value;
							PyArg_Parse(pval, wxT("l"), &value);                         /* convert to C */
							if ( format == wxEmptyString ) format = wxT("%l");
							it->second.value = wxString::Format( format, value );
						}
						else if ( type.CmpNoCase( wxT("double") ) == 0 )
						{
							double value;
							PyArg_Parse(pval, wxT("d"), &value);                         /* convert to C */
							if ( format == wxEmptyString ) format = wxT("%f");
							it->second.value = wxString::Format( format, value );
						}
						else if ( type.CmpNoCase( wxT("bool") ) == 0 )
						{
							bool value;
							PyArg_Parse(pval, wxT("b"), &value);
							it->second.bValue = value;
						}
						else
						{
							char* value;
							PyArg_Parse(pval, wxT("s"), &value);                         /* convert to C */
							if ( format == wxEmptyString ) format = wxT("%s");
							it->second.value = value;
						}

						// check for a mask
						wxString val = it->second.value;

						if ( mask != wxEmptyString )
						{
							wxMaskController mctrl( mask, val.c_str() );
							it->second.value = mctrl.GetTextValue();
						}
					}
					else
					{
						PyObject *type, *value, *traceback;
						PyErr_Fetch(&type, &value, &traceback);
						PyErr_Clear();
		 
						wxString message = wxT("Python error: ");
						if (type) {
							type = PyObject_Str(type);
							message += PyString_AsString(type);
						}
						if (value) {
							value = PyObject_Str(value);
							message += wxT(": ");
							message += PyString_AsString(value);
						}
                        if (traceback) {
                            traceback = PyObject_Str(traceback);
                            message += wxT(": ");
                            message += PyString_AsString(traceback);
                        }
						Py_XDECREF(type);
						Py_XDECREF(value);
						Py_XDECREF(traceback);

						wxLogError( message.c_str() );
					}

				}
#endif // USE_PYTHON_INTERPRETER
            }
		}
	}

	// formatting and masking
	FieldMap::iterator it2;
	for (it2 = section->fields.begin(); it2 != section->fields.end(); it2++)
	{
		wxString name	= it2->second.name;
		wxString val	= it2->second.value;
		wxString type   = it2->second.type;
		wxString format = it2->second.format;

		// format the data
		if ( type.CmpNoCase( wxT("string") ) == 0 )
			it2->second.value = val.c_str();
		else if ( type.CmpNoCase( wxT("date") ) == 0 && strcmp(val.mb_str(wxConvUTF8), ("")) != 0 )
			it2->second.value = wxVariant(val.c_str()).GetDateTime().Format( format );
		else if ( type.CmpNoCase( wxT("integer") ) == 0 )
			it2->second.value = wxString::Format( format, (int)wxVariant(val.c_str()).GetLong());
		else if ( type.CmpNoCase( wxT("long") ) == 0 )
			it2->second.value = wxString::Format( format, wxVariant(val.c_str()).GetLong());
		else if ( type.CmpNoCase( wxT("double") ) == 0 )
			it2->second.value = wxString::Format( format, wxVariant(val.c_str()).GetDouble());
		else if ( type.CmpNoCase( wxT("bool") ) == 0 )
            it2->second.bValue = val == wxEmptyString ? false : wxVariant(val.c_str()).GetBool();
		else
			it2->second.value = val.c_str();

		// check for a mask
		val				= it2->second.value;
		wxString mask	= it2->second.mask;

		if ( mask != wxEmptyString )
		{
			wxMaskController mctrl( mask, val.c_str() );
			it2->second.value = mctrl.GetTextValue();
		}
	}

	if ( !pReport->SaveData() )
		return false;

	// -----------------------
	//  Data sections
	// -----------------------
	for (int i=0; i<(int)section->sections.GetCount(); i++)
	{
		wxReportDataSection *ds = section->sections[i];

		// start the section
		pReport->DefineSection();

		// loop through objects
		for (int z=0; z<(int)ds->objects.GetCount(); z++)
		{
			wxReportObject *obj = ds->objects[z];
			wxReportObj	*pObj = GetReportObject( obj, ds );
			if ( pObj ) pReport->AddDataObj( pObj );
		}

		// add dummy section if needed
		wxString dummy = wxT("");
		if ( (int)ds->objects.GetCount() < 1 )
		{
			wxReportObj	*pObj = new wxReportObj( 0, 0, 5, 5 );
			pObj->SetRef( &dummy );
			pObj->SetIncrements( 0.0, 2.5 );
			pObj->SetFont( &report->fonts[wxT("Default")] );
			pReport->AddDataObj( pObj );
		}

		// add data
		pReport->BeginDataSection();
		GetSectionData( ds );

		// close the section
		pReport->FinishSection();
	}

	// reset variables
	for (int x=0; x<(int)report->arVariables.GetCount(); x++)
	{
		VariableMapType::iterator it;
		for (it = report->variables.begin(); it != report->variables.end(); it++)
		{
			if ( it->second.name.CmpNoCase( report->arVariables[x] ) == 0 )
			{
				// check for resetting variables
				if ( section->name.CmpNoCase( it->second.resetGroup ) == 0
					|| section->name.CmpNoCase( wxT("all") ) == 0 )
				{
					it->second.value = it->second.initialValueExpression;
				}
			}
		}
	}

	return true;
}

void wxReportFiller::SetParameter( wxString paramName, wxString value )
{
	try
	{
		report->parameters[ paramName.c_str() ].value = value.c_str();
	}
	catch (...)
	{
	}
}

wxString wxReportFiller::FilterFieldString( wxReportDataSection* section, wxString val1 )
{
	if ( val1.Find( _("$F{") ) > -1 )
	{
		FieldMap::iterator it;
		for (it = section->fields.begin(); it != section->fields.end(); it++)
		{
			wxString name = it->second.name;
			wxString val  = it->second.value;

			if ( val1.Find( wxString::Format( wxT("$F{%s}"), name.c_str() ) ) > -1 )
				val1.Replace( wxString::Format( wxT("$F{%s}"), name.c_str() ), val.c_str() );
		}
		return val1;
	}
	else
		return val1;
}

wxString wxReportFiller::FilterParameterString( wxString val1 )
{
	if ( val1.Find( wxT("$P{") ) > -1 )
	{
		ParameterMapType::iterator it;
		for (it = report->parameters.begin(); it != report->parameters.end(); it++)
		{
			wxString name = it->second.name;
			wxString val  = it->second.value;

			if ( val1.Find( wxString::Format( wxT("$P{%s}"), name.c_str() ) ) > -1 )
				val1.Replace( wxString::Format( wxT("$P{%s}"), name.c_str() ), val.c_str() );
		}
		return val1;
	}
	else
		return val1;
}

wxString wxReportFiller::FilterVariableString( wxString val1 )
{
	if ( val1.Find( wxT("$V{") ) > -1 )
	{
		VariableMapType::iterator it;
		for (it = report->variables.begin(); it != report->variables.end(); it++)
		{
			wxString name = it->second.name;
			wxString val  = it->second.value;

			if ( val1.Find( wxString::Format( wxT("$V{%s}"), name.c_str() ) ) > -1 )
				val1.Replace( wxString::Format( wxT("$V{%s}"), name.c_str() ), val.c_str() );
		}
		return val1;
	}
	else
		return val1;
}

#ifdef USE_PYTHON_INTERPRETER
bool wxReportFiller::InitPython()
{
	// Initialize Python
	Py_Initialize();
	PyEval_InitThreads();

    return true;
}
#endif // USE_PYTHON_INTERPRETER

wxString wxReportFiller::GetReportPath()
{
	if ( pReport )
		return pReport->FinalDestination();
	else
		return wxEmptyString;
}
