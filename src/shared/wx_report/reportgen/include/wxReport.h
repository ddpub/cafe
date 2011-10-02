/////////////////////////////////////////////////////////////////////////////
// Name:         wxReport.h
// Purpose:      wxReport class
// Author:       Scott Fant
// Created by:   SF, 26-July-2005
// Copyright:    (c) 2005 Scott Fant <scottfant@gmail.com>
// Licence:      wxWidgets license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXREPORT_H__
#define __WXREPORT_H__

#ifdef __GNUG__
#pragma interface "wxReport.cpp"
#endif

/// Headers
#include <wx/dynarray.h>		// for dynamic arrays
#include <wx/string.h>			// wxString
#include <wx/arrstr.h>			// wxArrayString
#include <wx/hashmap.h>			// map arrays

#include "xmlex.h"
#include "wxReportWriter.h"

//! Defines the type of objects used to
//! generate a report
#define	WX_USER					1000000
//! rectangle
#define WXRPT_OBJ_RECT			WX_USER + 1
//! static line (horizontal or vertical)
#define WXRPT_OBJ_STAT_LINE		WX_USER + 2
//! static text
#define WXRPT_OBJ_STAT_TEXT		WX_USER + 3
//! text field
#define WXRPT_OBJ_TEXT_FIELD	WX_USER + 4
//! bitmap image
#define WXRPT_OBJ_BITMAP		WX_USER + 5
//! page break
#define WXRPT_OBJ_PAGE_BREAK	WX_USER + 6
//! check box
#define WXRPT_OBJ_CHECK_BOX		WX_USER + 7
//! logic
#define WXRPT_OBJ_LOGIC			WX_USER + 8

//!  Base report object 
/*!
  The base object from which all others are derived
*/
class wxReportObject
{
public: // functions

	wxReportObject( void );
	virtual ~wxReportObject( void );

	//! Sets the type of object (must be called in constructor)
	void SetType( int objectType );

	//! Retrieves the type of object
	int GetType();

    //! Clone the object
    virtual wxReportObject* Clone() = 0;
    void CopyBase( wxReportObject* obj );

    //! Xml output
    void WriteBase( wxXmlNode* node );

public: // variables

	//! Horizontal position for the object.
	int			xpos;
	//! Vertical position for the object.
	int			ypos;
	//! Width of the object.
	int			width;
	//! Height of the object.
	int			height;

	//! Vertical increment for the object.
	//! As in the original wxReportWriter,
	//! this specifies the amount of space
	//! to leave between this object and the
	//! next one below it.
	int			increment;

private: // variables

	//! Type of the object. This can be one
	//! of the types defined at the beginning
	//! of this header file and must be defined
	//! in the constructor of the object by calling
	//! SetType.
	int			type;
};

//! Page Break
/*!
 This is used to set a page break at a given location
 in the report. It is derived from wxReportObject, but
 the only parameter used is the type.
*/
class wxReportBreak : public wxReportObject
{
public: // functions

	wxReportBreak();
	~wxReportBreak();

    //! Clone the object
    virtual wxReportObject* Clone();
};

//! Check box
/*!
 This is used to create a logical marker in the form of
 a check mark or a square with an "X" in the center to 
 indicate a true value. The type is determined by the 
 type variable.
*/
class wxReportLogic : public wxReportObject
{
public: // functions

	wxReportLogic();
	~wxReportLogic();

    //! Clone the object
    virtual wxReportObject* Clone();

    //! xml output
    void Write( wxXmlNode* node );

public: // variables

	//! name of the report field
	wxString	name;
	//! pointer to the field value
	//wxString*	value;
	//! the boolean holding the value
	bool*		val;
};

//! Array of report objects
WX_DEFINE_ARRAY_PTR(wxReportObject*, ObjectArray);

//!Report parameter
/*!
 This is used to set report parameters at run-time
 such a use would be to set SQL parameters, user names, passwords, etc.
*/
class wxReportParameter
{
public: // functions

	wxReportParameter( void );
	~wxReportParameter( void );

public: // variables

	//! Name given to the parameter and used in the report parameter hash map
	//! to store and retrieve values for the parameter.
	wxString	name;
	//! Variable type for this parameter. Valid values can be one of the following:
	//! string, int, double, date. Others may be added at a later time as necessary.
	wxString	type;
	//! Value of the parameter stored as a string and retrieved using the parameter
	//! hash map.
	wxString	value;
};

//! This is the declaration of the parameter hash map used to store
//! and retrieve paramter values.
WX_DECLARE_STRING_HASH_MAP( wxReportParameter, ParameterMapType );

//! Report variable
/*!
 This is used to store report data and can be used to create running
 totals for sections or the overall report. Variables are referenced in
 scripts by enclosing the variable name in brakets, e.g. $V{}.
*/
class wxReportVariable
{
public: // functions

	wxReportVariable( void );
	~wxReportVariable( void );

public: // variables

	//! name given to the variable and used in map
	wxString	name;
	//! variable type (string, int, double, etc.)
	wxString	type;
	//! determines when/if the variable is reset to its initial value
	wxString	resetType;
	//! indicates the groups where the variable will be reset
	wxString	resetGroup;
	//! the actual text of the variable, normally a wxPython script
	wxString	calculation;

	//! initial value, used when resetting the variable
	wxString	initialValueExpression;
	//! value of the parameter stored as a string
	wxString	value;
	//! value of the field stored as a boolean
	bool		bValue;

	//! text string used to format the value using wxString::Format
	wxString	format;
	//! text string used to format the value as a mask, e.g. ###-###-#### for a telephone number
	wxString	mask;

	//! lists the groups where the variable will be analyzed
	wxArrayString	evaluateGroups;
};
//! Map for report variables
WX_DECLARE_STRING_HASH_MAP( wxReportVariable, VariableMapType );

//! Static box
/*!
 This class hasn't actually been implemented in the report and is simply
 here so that I remember to do it at some point...
*/
class wxReportStaticBox : public wxReportObject
{
public: // functions

	wxReportStaticBox( void );
	wxReportStaticBox( int x, int y, int boxwidth, int boxheight );
	~wxReportStaticBox( void );

    //! Clone the object
    virtual wxReportObject* Clone();

public: // variables

	//! Color used to fill the box
	int			fillColor;
	//! Color of the line used to draw the box
	int			lineColor;
	//! Width of the line used to draw the box
	int			lineWidth;
};

//! Horizontal or vertical line
class wxReportStaticLine : public wxReportObject
{
public: // funtions

	wxReportStaticLine( void );
	wxReportStaticLine( int x, int y, int linewidth, int lineheight );
	~wxReportStaticLine( void );

    //! Clone the object
    virtual wxReportObject* Clone();

    //! Xml output
    void Write( wxXmlNode* node );

public: // variables

	//! pen width for drawing the line
	int		penWidth;
};

//! Report data field
/*!
 This is how report data fields are defined. The fields are
 defined for each data section and are used only in that section.
 To pass values of data fields to other parts of the report,
 use a variable. The report fields are evaluated by a wxReportDataSectionHandler
 derived class used to retrieve data from a database, file, etc.
*/
class wxReportField
{
public: // functions

	wxReportField( void );
	~wxReportField( void );

public: // variables


	//! name given to the field and used in map
	wxString	name;
	//! field type (string, int, double, etc.)
	wxString	type;
	//! value of the field stored as a string
	wxString	value;
	//! value of the field stored as a boolean
	bool		bValue;
	//! text string used to format the value using wxString::Format
	wxString	format;
	//! text string used to format the value as a mask, e.g. ###-###-#### for a telephone number
	wxString	mask;
};
//! Map for report fields
WX_DECLARE_STRING_HASH_MAP( wxReportField, FieldMap );

//! Static text object
class wxReportStaticText : public wxReportObject
{
public: // functions

	wxReportStaticText( void );
	~wxReportStaticText( void );

    //! Clone the object
    virtual wxReportObject* Clone();

    //! Xml output
    void Write( wxXmlNode* node );

public: // variables

	//! font used to draw the text
	wxString	font;
	//! the actual text to be drawn
	wxString	text;

	//! alignment for drawing the text (left, right, center)
	wxString	alignment;
};

//! Report text field
/*!
 This is used to display any type of text that is not considered
 to be "static" text. This can be used to display variables,
 report fields, and special fields such as page number, date, etc.
*/
class wxReportTextField : public wxReportObject
{
public: // functions

	wxReportTextField( void );
	~wxReportTextField( void );

    //! Clone the object
    virtual wxReportObject* Clone();

    //! Xml output
    void Write( wxXmlNode* node );

public: // variables

	//! font used to draw the text
	wxString	font;
	//! text that indicates the report field to use
	wxString	text;
	//! pointer to the field value
	wxString*	value;

	//! alignment for drawing the text (left, right, center)
	wxString	alignment;
};

//! Bitmap image object
/*!
 This is a bitmap object used to display an image in the
 report. The image name is the full path to the image file.
 Images are loaded and embedded into the report so that a
 report file can be viewed on a different machine where the
 image files do not reside.
*/
class wxReportBitmap : public wxReportObject
{
public: // functions

	wxReportBitmap( void );
	~wxReportBitmap( void );

    //! Clone the object
    virtual wxReportObject* Clone();

    //! Xml output
    void Write( wxXmlNode* node );

public: // variables

	//! name of the png file
	wxString	name;
};

//! Page header section
/*!
 This is a specialized report section that only displays
 static objects and variables. The section is not evaluated
 and appears at the top of every page.
*/
class wxReportHeader
{
public: // functions

	wxReportHeader( void );
	~wxReportHeader( void );

public: // variables

	//! array of header objects
	ObjectArray	objects;
};

//! Page footer section
/*!
 This is a specialized report section that only displays
 static objects and variables. The section is not evaluated
 and appears at the bottom of every page.
*/
class wxReportFooter
{
public: // functions

	wxReportFooter( void );
	~wxReportFooter( void );

public: // variables

	//! array of footer objects
	ObjectArray	objects;
};

class wxReportDataSection;

//! Array of report data sections
WX_DEFINE_ARRAY_PTR(wxReportDataSection*, SectionArray);

//! Data section handler-specific properties
class wxReportDataSectionProperties
{
public: // functions

    wxReportDataSectionProperties( void ){};
    virtual ~wxReportDataSectionProperties( void ) {};
};


WX_DECLARE_STRING_HASH_MAP( wxVariant, SectionVariables );

//! Report data section
/*!
 Unlike the original wxReportWriter classes, the current implementation
 is based upon report data sections. These are essentially subreports
 that appear in the order they are listed in the XML file. All report
 objects are positioned with respect the beginning of the data section.
*/
class wxReportDataSection
{
public: // functions

	wxReportDataSection( void );
	~wxReportDataSection( void );

    wxReportDataSection* Clone();

    //! xml output
    void Write( wxXmlNode* node );

public: // variables

	//! name given to the data section
	wxString		name;
	//! type of section, e.g. static, Db, etc.
	wxString		type;
	//! array of report object in this section
	ObjectArray		objects;

    //! hash map for storing data section variables
    SectionVariables variables;

	//! pointer to data used by the data filler
	//wxReportDataSectionProperties*		data;
	
	//! map of the fields used in this section
	FieldMap		fields;

	//! subsections of this section
	SectionArray	sections;
};

WX_DECLARE_STRING_HASH_MAP(wxReportFont, FontMap);

//! Main report object
/*!
 This is the main report object and contains all report data
 sections, page formatting information, fonts, parameters, and
 variables.
*/
class wxReport
{
public: // functions

	wxReport( void );
	~wxReport( void );

    wxReport* Clone();
    bool Write( wxString fileName );

public: // variables

	//! name of the report
	wxString	name;
	//! total page width in mm
	double		pageWidth;
	//! total page height in mm
	double		pageHeight;
	//! total column width in mm
	double		columnWidth;
	//! width of left margin in mm
	double		leftMargin;
	//! width of right margin in mm
	double		rightMargin;
	//! height of top margin in mm
	double		topMargin;
	//! height of bottom margin in mm
	double		bottomMargin;

	//!paper type (e.g., letter, A4, legal, etc.)
	//!0  = wxPAPER_NONE,				 Use specific dimensions\n
	//!1  = wxPAPER_LETTER,				 Letter, 8 1/2 by 11 inches\n
	//!2  = wxPAPER_LEGAL,				 Legal, 8 1/2 by 14 inches\n
	//!3  = wxPAPER_A4,					 A4 Sheet, 210 by 297 millimeters\n
	//!4  = wxPAPER_CSHEET,				 C Sheet, 17 by 22 inches\n
	//!5  = wxPAPER_DSHEET,				 D Sheet, 22 by 34 inches\n
	//!6  = wxPAPER_ESHEET,				 E Sheet, 34 by 44 inches\n
	//!7  = wxPAPER_LETTERSMALL,		 Letter Small, 8 1/2 by 11 inches\n
	//!8  = wxPAPER_TABLOID,			 Tabloid, 11 by 17 inches\n
	//!9  = wxPAPER_LEDGER,				 Ledger, 17 by 11 inches\n
	//!10 = wxPAPER_STATEMENT,			 Statement, 5 1/2 by 8 1/2 inches\n
	//!11 = wxPAPER_EXECUTIVE,			 Executive, 7 1/4 by 10 1/2 inches\n
	//!12 = wxPAPER_A3,                  A3 sheet, 297 by 420 millimeters\n
	//!13 = wxPAPER_A4SMALL,             A4 small sheet, 210 by 297 millimeters\n
	//!14 = wxPAPER_A5,                  A5 sheet, 148 by 210 millimeters\n
	//!15 = wxPAPER_B4,                  B4 sheet, 250 by 354 millimeters\n
	//!16 = wxPAPER_B5,                  B5 sheet, 182-by-257-millimeter paper\n
	//!17 = wxPAPER_FOLIO,               Folio, 8-1/2-by-13-inch paper\n
	//!18 = wxPAPER_QUARTO,              Quarto, 215-by-275-millimeter paper\n
	//!19 = wxPAPER_10X14,               10-by-14-inch sheet\n
	//!20 = wxPAPER_11X17,               11-by-17-inch sheet\n
	//!21 = wxPAPER_NOTE,                Note, 8 1/2 by 11 inches\n
	//!22 = wxPAPER_ENV_9,               #9 Envelope, 3 7/8 by 8 7/8 inches\n
	//!23 = wxPAPER_ENV_10,              #10 Envelope, 4 1/8 by 9 1/2 inches\n
	//!24 = wxPAPER_ENV_11,              #11 Envelope, 4 1/2 by 10 3/8 inches\n
	//!25 = wxPAPER_ENV_12,              #12 Envelope, 4 3/4 by 11 inches\n
	//!26 = wxPAPER_ENV_14,              #14 Envelope, 5 by 11 1/2 inches\n
	//!27 = wxPAPER_ENV_DL,              DL Envelope, 110 by 220 millimeters\n
	//!28 = wxPAPER_ENV_C5,              C5 Envelope, 162 by 229 millimeters\n
	//!29 = wxPAPER_ENV_C3,              C3 Envelope, 324 by 458 millimeters\n
	//!30 = wxPAPER_ENV_C4,              C4 Envelope, 229 by 324 millimeters\n
	//!31 = wxPAPER_ENV_C6,              C6 Envelope, 114 by 162 millimeters\n
	//!32 = wxPAPER_ENV_C65,             C65 Envelope, 114 by 229 millimeters\n
	//!33 = wxPAPER_ENV_B4,              B4 Envelope, 250 by 353 millimeters\n
	//!34 = wxPAPER_ENV_B5,              B5 Envelope, 176 by 250 millimeters\n
	//!35 = wxPAPER_ENV_B6,              B6 Envelope, 176 by 125 millimeters\n
	//!36 = wxPAPER_ENV_ITALY,           Italy Envelope, 110 by 230 millimeters\n
	//!37 = wxPAPER_ENV_MONARCH,         Monarch Envelope, 3 7/8 by 7 1/2 inches\n
	//!38 = wxPAPER_ENV_PERSONAL,        6 3/4 Envelope, 3 5/8 by 6 1/2 inches\n
	//!39 = wxPAPER_FANFOLD_US,          US Std Fanfold, 14 7/8 by 11 inches\n
	//!40 = wxPAPER_FANFOLD_STD_GERMAN,  German Std Fanfold, 8 1/2 by 12 inches\n
	//!41 = wxPAPER_FANFOLD_LGL_GERMAN,  German Legal Fanfold, 8 1/2 by 13 inches
	int			paperFormat;

	//! long format for date/time
	wxString	dateLongFormat;
	//! short format for date/time
	wxString	dateShortFormat;

	//! name of report maintainer
	wxString	contactName;

	//! map of report fonts
	FontMap				fonts;
	//! map of report parameters
	ParameterMapType	parameters;
	//! map of report variables
	VariableMapType		variables;
	//! array of report variables
	//! this is needed because the hash map does
	//! not guarantee order which is needed for
	//! evaluating the variables correctly
	wxArrayString		arVariables;

	//! report header section
	wxReportHeader		header;
	//! report footer section
	wxReportFooter		footer;

	//! array of report data sections
	SectionArray		sections;
};

#endif //__WXREPORT_H__
