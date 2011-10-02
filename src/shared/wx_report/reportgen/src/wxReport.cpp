#ifdef __GNUG__
#pragma implementation "wxReport.h"
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

#include "wxReport.h"

wxReportObject::wxReportObject()
{
	increment = 0;
};

wxReportObject::~wxReportObject()
{
}

void wxReportObject::CopyBase( wxReportObject* obj )
{
    obj->xpos = xpos;
    obj->ypos = ypos;
    obj->width = width;
    obj->height = height;
    obj->increment = increment;
}

void wxReportObject::SetType( int objectType )
{
	type = objectType;
}

int wxReportObject::GetType()
{
	return type;
}

void wxReportObject::WriteBase( wxXmlNode* node )
{
    node->AddProperty( new wxXmlProperty( _("x"), wxString::Format(_("%i"), xpos), NULL ) );
    node->AddProperty( new wxXmlProperty( _("y"), wxString::Format(_("%i"), ypos), NULL ) );
    node->AddProperty( new wxXmlProperty( _("width"), wxString::Format(_("%i"), width), NULL ) );
    node->AddProperty( new wxXmlProperty( _("height"), wxString::Format(_("%i"), height), NULL ) );
    node->AddProperty( new wxXmlProperty( _("increment"), wxString::Format(_("%i"), increment), NULL ) );
}

// -----------------------------------------------------------
// wxReportBreak
// -----------------------------------------------------------
wxReportBreak::wxReportBreak()
{
	SetType( WXRPT_OBJ_PAGE_BREAK );
}

wxReportBreak::~wxReportBreak()
{
}

wxReportObject* wxReportBreak::Clone()
{
    wxReportBreak* rb = new wxReportBreak();
    CopyBase( rb );

    return (wxReportObject*)rb;
}

// -----------------------------------------------------------
// wxReportLogic
// -----------------------------------------------------------
wxReportLogic::wxReportLogic()
{
    SetType( WXRPT_OBJ_LOGIC );
}

wxReportLogic::~wxReportLogic()
{
}

wxReportObject* wxReportLogic::Clone()
{
    wxReportLogic* rl = new wxReportLogic();
    CopyBase( rl );

    rl->name = name;
    rl->SetType( GetType() );

    return (wxReportObject*)rl;
}

void wxReportLogic::Write( wxXmlNode* node )
{
    wxXmlNode* stnode;
    if ( GetType() == WXRPT_OBJ_LOGIC )
        stnode = new wxXmlNode( wxXML_ELEMENT_NODE, _("logic") );
    else
        stnode = new wxXmlNode( wxXML_ELEMENT_NODE, _("checkBox") );

    WriteBase( stnode );

    wxXmlNode* node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("name") );
    wxXmlNode* node3 = new wxXmlNode( wxXML_CDATA_SECTION_NODE, _(""), name );
    node2->AddChild( node3 );
    stnode->AddChild( node2 );

    node->AddChild( stnode );
}

// -----------------------------------------------------------
// wxReportParameter
// -----------------------------------------------------------
wxReportParameter::wxReportParameter()
{
	name	= wxT("");
	type	= wxT("");
	value	= wxT("");
}

wxReportParameter::~wxReportParameter()
{
}

// -----------------------------------------------------------
// wxReportField
// -----------------------------------------------------------
wxReportField::wxReportField()
{
	name	= wxT("");
	type	= wxT("");
	value	= wxT("");
	format	= wxT("");
	mask	= wxT("");
}

wxReportField::~wxReportField()
{
}

// -----------------------------------------------------------
// wxReportVariable
// -----------------------------------------------------------
wxReportVariable::wxReportVariable()
{
	name					= wxT("");
	type					= wxT("");
	resetType				= wxT("");
	resetGroup				= wxT("");
	calculation				= wxT("");
	initialValueExpression	= wxT("");
	value					= wxT("");
	format					= wxT("");
	mask					= wxT("");
}

wxReportVariable::~wxReportVariable()
{
}

// -----------------------------------------------------------
// wxReportStaticText
// -----------------------------------------------------------
wxReportStaticText::wxReportStaticText() : wxReportObject()
{
	xpos		= 0;
	ypos		= 0;
	height		= 20;
	width		= 100;
	font		= wxT("Default");
	text		= wxT("");

	alignment	= wxT("left");

	SetType( WXRPT_OBJ_STAT_TEXT );
}

wxReportStaticText::~wxReportStaticText()
{
    font.clear();
    text.clear();
}

wxReportObject* wxReportStaticText::Clone()
{
    wxReportStaticText* rst = new wxReportStaticText();
    rst->font = font;
    rst->text = text;
    rst->alignment = alignment;

    CopyBase( rst );

    return (wxReportObject*)rst;
}

void wxReportStaticText::Write( wxXmlNode* node )
{
    wxXmlNode* stnode = new wxXmlNode( wxXML_ELEMENT_NODE, _("staticText") );

    WriteBase( stnode );

    stnode->AddProperty( new wxXmlProperty( _("align"), alignment, NULL ) );
    
    wxXmlNode* node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("font") );
    node2->AddProperty( new wxXmlProperty( _("reportFont"), font, NULL ) );
    stnode->AddChild( node2 );
    node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("text") );
    wxXmlNode* node3 = new wxXmlNode( wxXML_CDATA_SECTION_NODE, _(""), text );
    node2->AddChild( node3 );
    stnode->AddChild( node2 );

    node->AddChild( stnode );
}

// -----------------------------------------------------------
// wxReportStaticBox
// -----------------------------------------------------------
wxReportStaticBox::wxReportStaticBox() : wxReportObject()
{
    wxReportStaticBox( 0, 0, 20, 100 );
}

wxReportStaticBox::wxReportStaticBox( int x, int y, int boxwidth, int boxheight )
{
	xpos		= x;
	ypos		= y;
	height		= boxheight;
	width		= boxwidth;

    fillColor   = 0;
    lineColor   = 0;
    lineWidth  = 0;

	SetType( WXRPT_OBJ_STAT_TEXT );
}

wxReportStaticBox::~wxReportStaticBox()
{
}

wxReportObject* wxReportStaticBox::Clone()
{
    wxReportStaticBox* rsb = new wxReportStaticBox();
    rsb->fillColor = fillColor;
    rsb->lineColor = lineColor;
    rsb->lineWidth = lineWidth;

    CopyBase( rsb );

    return (wxReportObject*)rsb;
}

// -----------------------------------------------------------
// wxReportTextField
// -----------------------------------------------------------
wxReportTextField::wxReportTextField() : wxReportObject()
{
	xpos		= 0;
	ypos		= 0;
	height		= 20;
	width		= 100;
	font		= wxT("Default");
	text		= wxT("");

	value		= NULL;

	alignment	= wxT("left");

	SetType( WXRPT_OBJ_TEXT_FIELD );
}

wxReportTextField::~wxReportTextField()
{
    font.clear();
    text.clear();
}

wxReportObject* wxReportTextField::Clone()
{
    wxReportTextField* rtf = new wxReportTextField();
    rtf->font = font;
    rtf->text = text;
    rtf->value = value;
    rtf->alignment = alignment;

    CopyBase( rtf );

    return (wxReportObject*)rtf;
}

void wxReportTextField::Write( wxXmlNode* node )
{
    wxXmlNode* stnode = new wxXmlNode( wxXML_ELEMENT_NODE, _("textField") );

    WriteBase( stnode );

    stnode->AddProperty( new wxXmlProperty( _("align"), alignment, NULL ) );
    
    wxXmlNode* node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("font") );
    node2->AddProperty( new wxXmlProperty( _("reportFont"), font, NULL ) );
    stnode->AddChild( node2 );
    node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("text") );
    wxXmlNode* node3 = new wxXmlNode( wxXML_CDATA_SECTION_NODE, _(""), text );
    node2->AddChild( node3 );
    stnode->AddChild( node2 );

    node->AddChild( stnode );
}

// -----------------------------------------------------------
// wxReportBitmap
// -----------------------------------------------------------
wxReportBitmap::wxReportBitmap() : wxReportObject()
{
	xpos		= 0;
	ypos		= 0;
	height		= 20;
	width		= 100;
	name		= wxT("");

	SetType( WXRPT_OBJ_BITMAP );
}

wxReportBitmap::~wxReportBitmap()
{
}

wxReportObject* wxReportBitmap::Clone()
{
    wxReportBitmap* rb = new wxReportBitmap();
    rb->name = name;

    CopyBase( rb );

    return (wxReportObject*)rb;
}

void wxReportBitmap::Write( wxXmlNode* node )
{
    wxXmlNode* stnode = new wxXmlNode( wxXML_ELEMENT_NODE, _("bitmapImage") );

    WriteBase( stnode );

    wxXmlNode* node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("name") );
    wxXmlNode* node3 = new wxXmlNode( wxXML_CDATA_SECTION_NODE, _(""), name );
    node2->AddChild( node3 );
    stnode->AddChild( node2 );
    
    node->AddChild( stnode );
}

// -----------------------------------------------------------
// wxReportStaticLine
// -----------------------------------------------------------
wxReportStaticLine::wxReportStaticLine()
{
	xpos		= 0;
	ypos		= 0;
	height		= 0;
	width		= 170;
	increment	= 0;
	penWidth	= 1;

	SetType( WXRPT_OBJ_STAT_LINE );
}

wxReportStaticLine::wxReportStaticLine( int x, int y, int linewidth, int lineheight )
{
	xpos		= x;
	ypos		= y;
	height		= lineheight;
	width		= linewidth;
	increment	= 0;
	penWidth	= 1;

	SetType( WXRPT_OBJ_STAT_LINE );
}

wxReportStaticLine::~wxReportStaticLine()
{
}

wxReportObject* wxReportStaticLine::Clone()
{
    wxReportStaticLine* rsl = new wxReportStaticLine();
    rsl->penWidth = penWidth;

    CopyBase( rsl );

    return (wxReportObject*)rsl;
}

void wxReportStaticLine::Write( wxXmlNode* node )
{
    wxXmlNode* stnode = new wxXmlNode( wxXML_ELEMENT_NODE, _("staticLine") );

    WriteBase( stnode );

    stnode->AddProperty( new wxXmlProperty( _("penWidth"), wxString::Format(_("%i"), penWidth), NULL ) );
    
    node->AddChild( stnode );
}

// -----------------------------------------------------------
// wxReportHeader
// -----------------------------------------------------------
wxReportHeader::wxReportHeader()
{
}

wxReportHeader::~wxReportHeader()
{
	for (int i=0; i<(int)objects.GetCount(); i++)
		wxDELETE( objects[i] );
}

// -----------------------------------------------------------
// wxReportFooter
// -----------------------------------------------------------
wxReportFooter::wxReportFooter()
{
}

wxReportFooter::~wxReportFooter()
{
	for (int i=0; i<(int)objects.GetCount(); i++)
		wxDELETE( objects[i] );
}

// -----------------------------------------------------------
// wxReportDataSection
// -----------------------------------------------------------
wxReportDataSection::wxReportDataSection()
{
	name		= wxT("");
	type		= wxT("static");
	//data		= NULL;
}

wxReportDataSection::~wxReportDataSection()
{
	for (int i=0; i<(int)objects.GetCount(); i++)
		wxDELETE( objects[i] );

	//if ( data ) wxDELETE( data );

	for (int i=0; i<(int)sections.GetCount(); i++)
		wxDELETE( sections[i] );
}

wxReportDataSection* wxReportDataSection::Clone()
{
    wxReportDataSection* s = new wxReportDataSection();

    s->name = name;
    s->type = type;

    for (int x=0; x<(int)objects.Count(); x++)
        s->objects.Add( objects[x]->Clone() );

    SectionVariables::iterator itv;
    for (itv = variables.begin(); itv != variables.end(); itv++)
        s->variables[ itv->first ] = itv->second;

    FieldMap::iterator itf;
    for (itf = fields.begin(); itf != fields.end(); itf++)
        s->fields[ itf->first ] = itf->second;

    int num = (int)sections.Count();
    for ( int i=0; i<num; i++ )
        s->sections.Add( sections[i]->Clone() );

    return s;
}

void wxReportDataSection::Write( wxXmlNode* node )
{
    wxXmlNode* secnode = new wxXmlNode( wxXML_ELEMENT_NODE, _("dataSection") );

    secnode->AddProperty( new wxXmlProperty( _("name"), name, NULL ) );
    secnode->AddProperty( new wxXmlProperty( _("type"), type, NULL ) );

    FieldMap::iterator itf;
    for (itf = fields.begin(); itf != fields.end(); itf++)
    {
        wxXmlNode* node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("field") );
		node2->AddProperty( new wxXmlProperty( _("name"), itf->second.name, NULL ) );
		node2->AddProperty( new wxXmlProperty( _("class"), itf->second.type, NULL ) );
		node2->AddProperty( new wxXmlProperty( _("format"), itf->second.format, NULL ) );
		node2->AddProperty( new wxXmlProperty( _("mask"), itf->second.mask, NULL ) );
		secnode->AddChild( node2 );
	}

    for ( int i=0; i<(int)objects.Count(); i++ )
    {
        switch ( objects[i]->GetType() )
        {
        case WXRPT_OBJ_STAT_TEXT:
            ((wxReportStaticText*)objects[i])->Write( secnode );
            break;
        case WXRPT_OBJ_TEXT_FIELD:
            ((wxReportTextField*)objects[i])->Write( secnode );
            break;
        case WXRPT_OBJ_STAT_LINE:
            ((wxReportStaticLine*)objects[i])->Write( secnode );
            break;
        case WXRPT_OBJ_BITMAP:
            ((wxReportBitmap*)objects[i])->Write( secnode );
            break;
        case WXRPT_OBJ_PAGE_BREAK:
            {
                wxXmlNode* node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("pageBreak") );
                secnode->AddChild( node2 );
            }
            break;
        case WXRPT_OBJ_CHECK_BOX:
        case WXRPT_OBJ_LOGIC:
            ((wxReportLogic*)objects[i])->Write( secnode );
            break;
        }
    }

    for ( int i=0; i<(int)sections.Count(); i++ )
        sections[i]->Write( secnode );

    node->AddChild( secnode );
}

// -----------------------------------------------------------
// wxReport
// -----------------------------------------------------------
wxReport::wxReport()
{
	name			= wxT("");
	pageWidth		= 0;
	pageHeight		= 0;
	columnWidth		= 0;
	leftMargin		= 0;
	rightMargin		= 0;
	topMargin		= 0;
	bottomMargin	= 0;

	paperFormat		= wxPAPER_LETTER;

	dateLongFormat	= wxT("%d.%m.%Y  %H:%M:%S");
	dateShortFormat	= wxT("%d/%m/%Y");

	contactName		= wxT("");
}

wxReport::~wxReport()
{
	for (int i=0; i<(int)sections.GetCount(); i++)
		wxDELETE( sections[i] );
}

wxReport* wxReport::Clone()
{
    wxReport* rpt = new wxReport();
    rpt->name = name;
    rpt->pageWidth = pageWidth;
    rpt->pageHeight = pageHeight;
    rpt->columnWidth = columnWidth;
    rpt->leftMargin = leftMargin;
    rpt->rightMargin = rightMargin;
    rpt->topMargin = topMargin;
    rpt->bottomMargin = bottomMargin;
    
    rpt->paperFormat = paperFormat;
    
    rpt->dateLongFormat = dateLongFormat;
    rpt->dateShortFormat = dateShortFormat;
    
    rpt->contactName = contactName;
    
    for ( int i=0; i<(int)header.objects.Count(); i++ )
        rpt->header.objects.Add( header.objects[i]->Clone() );

    for ( int i=0; i<(int)footer.objects.Count(); i++ )
        rpt->footer.objects.Add( footer.objects[i]->Clone() );
    
    for ( int i=0; i<(int)sections.Count(); i++ )
        rpt->sections.Add( sections[i]->Clone() );
    
    VariableMapType::iterator it;
    for (it = variables.begin(); it != variables.end(); it++)
        rpt->variables[ it->first ] = it->second;

    FontMap::iterator itf;
    for (itf = fonts.begin(); itf != fonts.end(); itf++)
        rpt->fonts[ itf->first ] = itf->second;

    ParameterMapType::iterator itp;
    for (itp = parameters.begin(); itp != parameters.end(); itp++)
        rpt->parameters[ itp->first ] = itp->second;
    
    return rpt;
}

bool wxReport::Write( wxString fileName )
{
    bool bGood = false;

	// try to do everything
    wxXmlDocument *doc = new wxXmlDocument();

    try
    {
        // root node
        wxXmlNode* root = new wxXmlNode( wxXML_ELEMENT_NODE, _("wxReport") );
        root->AddProperty( new wxXmlProperty( _("name"), name, NULL ) );
        root->AddProperty( new wxXmlProperty( _("pageWidth"), wxString::Format(_("%g"), pageWidth), NULL ) );
        root->AddProperty( new wxXmlProperty( _("pageHeight"), wxString::Format(_("%g"), pageHeight), NULL ) );
        root->AddProperty( new wxXmlProperty( _("leftMargin"), wxString::Format(_("%g"), leftMargin), NULL ) );
        root->AddProperty( new wxXmlProperty( _("rightMargin"), wxString::Format(_("%g"), rightMargin), NULL ) );
        root->AddProperty( new wxXmlProperty( _("topMargin"), wxString::Format(_("%g"), topMargin), NULL ) );
        root->AddProperty( new wxXmlProperty( _("bottomMargin"), wxString::Format(_("%g"), bottomMargin), NULL ) );
        wxXmlNode* node = new wxXmlNode( wxXML_ELEMENT_NODE, _("paperFormat") );
        node->AddProperty( new wxXmlProperty( _("style"), wxString::Format(_("%i"), paperFormat), NULL ) );
        root->AddChild( node );
        node = new wxXmlNode( wxXML_ELEMENT_NODE, _("reportContact") );
        node->AddProperty( new wxXmlProperty( _("name"), contactName, NULL ) );
        root->AddChild( node );
        node = new wxXmlNode( wxXML_ELEMENT_NODE, _("dateFormat") );
        node->AddProperty( new wxXmlProperty( _("long"), dateLongFormat, NULL ) );
        node->AddProperty( new wxXmlProperty( _("short"), dateShortFormat, NULL ) );
        root->AddChild( node );

        // fonts
        FontMap::iterator itf;
        for (itf = fonts.begin(); itf != fonts.end(); itf++)
        {
            node = new wxXmlNode( wxXML_ELEMENT_NODE, _("reportFont") );
            node->AddProperty( new wxXmlProperty( _("name"), itf->second.name, NULL ) );
            node->AddProperty( new wxXmlProperty( _("isDefault"), itf->second.isDefault ? _("true") : _("false"), NULL ) );
            node->AddProperty( new wxXmlProperty( _("fontName"), itf->second.fontName, NULL ) );
            node->AddProperty( new wxXmlProperty( _("fontFamily"), wxString::Format(_("%i"), itf->second.fontFamily), NULL ) );
            node->AddProperty( new wxXmlProperty( _("size"), wxString::Format(_("%i"), itf->second.fontSize), NULL ) );
            node->AddProperty( new wxXmlProperty( _("fontColor"), itf->second.fontColor, NULL ) );
            node->AddProperty( new wxXmlProperty( _("isBold"), itf->second.isBold ? _("true") : _("false"), NULL ) );
            node->AddProperty( new wxXmlProperty( _("isItalic"), itf->second.isItalic ? _("true") : _("false"), NULL ) );
            node->AddProperty( new wxXmlProperty( _("isUnderline"), itf->second.isUnderline ? _("true") : _("false"), NULL ) );
            node->AddProperty( new wxXmlProperty( _("isStrikeThrough"), itf->second.isStrikeThrough ? _("true") : _("false"), NULL ) );
            root->AddChild( node );
        }

        // variables
        VariableMapType::iterator it;
        for (it = variables.begin(); it != variables.end(); it++)
        {
            node = new wxXmlNode( wxXML_ELEMENT_NODE, _("variable") );
            node->AddProperty( new wxXmlProperty( _("name"), it->second.name, NULL ) );
            node->AddProperty( new wxXmlProperty( _("type"), it->second.type, NULL ) );
            node->AddProperty( new wxXmlProperty( _("resetType"), it->second.resetType, NULL ) );
            node->AddProperty( new wxXmlProperty( _("resetGroup"), it->second.resetGroup, NULL ) );
            node->AddProperty( new wxXmlProperty( _("format"), it->second.format, NULL ) );
            node->AddProperty( new wxXmlProperty( _("mask"), it->second.mask, NULL ) );

            wxXmlNode* node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("calculation") );
            wxXmlNode* node3 = new wxXmlNode( wxXML_CDATA_SECTION_NODE, _(""), it->second.calculation );
            node2->AddChild( node3 );
            node->AddChild( node2 );

            node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("initialValueExpression") );
            node3 = new wxXmlNode( wxXML_CDATA_SECTION_NODE, _(""), it->second.initialValueExpression );
            node2->AddChild( node3 );
            node->AddChild( node2 );

            wxString temp;
            for ( int i=0; i<(int)it->second.evaluateGroups.Count(); i++ )
            {
                if ( !temp.IsEmpty() ) temp << _(",");
                temp << it->second.evaluateGroups[i];
            }

            node2 = new wxXmlNode( wxXML_ELEMENT_NODE, _("evaluateOnGroups") );
            node3 = new wxXmlNode( wxXML_CDATA_SECTION_NODE, _(""), temp );
            node2->AddChild( node3 );
            node->AddChild( node2 );

            root->AddChild( node );
        }

        // parameters
        ParameterMapType::iterator itp;
        for (itp = parameters.begin(); itp != parameters.end(); itp++)
        {
            node = new wxXmlNode( wxXML_ELEMENT_NODE, _("parameter") );
            node->AddProperty( new wxXmlProperty( _("name"), it->second.name, NULL ) );
            node->AddProperty( new wxXmlProperty( _("type"), it->second.type, NULL ) );

            root->AddChild( node );
        }
        
        // header
        wxXmlNode* head = new wxXmlNode( wxXML_ELEMENT_NODE, _("pageHeader") );
        for ( int i=0; i<(int)header.objects.Count(); i++ )
        {
            switch ( header.objects[i]->GetType() )
            {
            case WXRPT_OBJ_STAT_TEXT:
                ((wxReportStaticText*)header.objects[i])->Write( head );
                break;
            case WXRPT_OBJ_TEXT_FIELD:
                ((wxReportTextField*)header.objects[i])->Write( head );
                break;
            case WXRPT_OBJ_STAT_LINE:
                ((wxReportStaticLine*)header.objects[i])->Write( head );
                break;
            case WXRPT_OBJ_BITMAP:
                ((wxReportBitmap*)header.objects[i])->Write( head );
                break;
            case WXRPT_OBJ_CHECK_BOX:
            case WXRPT_OBJ_LOGIC:
                ((wxReportLogic*)header.objects[i])->Write( head );
                break;
            }
        }
        root->AddChild( head );

        // data sections
        for ( int i=0; i<(int)sections.Count(); i++ )
        {
            sections[i]->Write( root );
        }

        // footer
        wxXmlNode* foot = new wxXmlNode( wxXML_ELEMENT_NODE, _("pageFooter") );
        for ( int i=0; i<(int)footer.objects.Count(); i++ )
        {
            switch ( footer.objects[i]->GetType() )
            {
            case WXRPT_OBJ_STAT_TEXT:
                ((wxReportStaticText*)footer.objects[i])->Write( foot );
                break;
            case WXRPT_OBJ_TEXT_FIELD:
                ((wxReportTextField*)footer.objects[i])->Write( foot );
                break;
            case WXRPT_OBJ_STAT_LINE:
                ((wxReportStaticLine*)footer.objects[i])->Write( foot );
                break;
            case WXRPT_OBJ_BITMAP:
                ((wxReportBitmap*)footer.objects[i])->Write( foot );
                break;
            case WXRPT_OBJ_CHECK_BOX:
            case WXRPT_OBJ_LOGIC:
                ((wxReportLogic*)footer.objects[i])->Write( foot );
                break;
            }
        }
        root->AddChild( foot );

        // save document
        doc->SetRoot( root);
        bGood = doc->Save( fileName );

        if ( doc ) wxDELETE( doc );
    }
    catch (...)
    {
        if ( doc ) wxDELETE( doc );
        return false;
    }

    return bGood;
}
