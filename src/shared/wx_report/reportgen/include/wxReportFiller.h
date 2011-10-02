/////////////////////////////////////////////////////////////////////////////
// Name:         wxReportFiller.h
// Purpose:      wxReportFiller class
// Author:       Scott Fant
// Created by:   SF, 26-July-2005
// Copyright:    (c) 2005 Scott Fant <scottfant@gmail.com>
// Licence:      wxWidgets license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXREPORTFILLER_H__
#define __WXREPORTFILLER_H__

#ifdef __GNUG__
#pragma interface "wxReportFiller.cpp"
#endif

#include "wxReport.h"
#include <wx/filename.h>
#include <wx/hashmap.h>

#include "wxReportWriter.h"

// Import Python and wxPython headers
#ifdef USE_PYTHON_INTERPRETER
#ifdef _MSC_VER
  // Stop pyconfig.h from always auto-including python23_d.lib when in debug mode
  #ifdef _DEBUG
    #define _DEBUG_COPY 1
  #else
    #define _DEBUG_COPY 0
  #endif
  #undef _DEBUG
#endif
#include <Python.h>
#ifdef _MSC_VER
  #if _DEBUG_COPY==1
    #define _DEBUG
  #endif
  #undef _DEBUG_COPY
#endif
//#include <wx/wxPython/wxPython.h>
#endif // USE_PYTHON_INTERPRETER

//! print to printer (not yet implemented)
#define wxREPORT_PRINTER	wxUSER + 1
//! print to PDF (not implemented yet)
#define wxREPORT_PDF		wxUSER + 2

class wxReportFiller;

//! Data section handler responsible for
//! loading data for a given report data section
class wxReportDataSectionHandler
{
public: // functions

	wxReportDataSectionHandler( void ){};
	virtual ~wxReportDataSectionHandler( void ){};

	//! Called by the report filler to request data for the section
	virtual bool GetSectionData( wxReportDataSection* WXUNUSED(section), wxReportFiller* WXUNUSED(filler) ) { return true; };

	//! Called by the template parser routine to allow the handler to
	//! load properties specific to the section and data section handler
	virtual bool LoadSectionProperties( wxXmlNodeEx* node, wxReportDataSection* ds ) { return true; };

public: // variables

	//! Defines the type of section handler (Db, etc.)
	wxString	type;
};

WX_DEFINE_ARRAY_PTR(wxReportDataSectionHandler*, DataHandlerArray);

class wxReportFiller
{
public: // functions

	wxReportFiller( void );
	wxReportFiller( wxString reportPath );
	~wxReportFiller( void );

    wxReport* CloneReport() { if ( report ) return report->Clone(); else return NULL; };

public: // functions

	//! Opens a report template and parses the data
	//! to create a wxReport object
	bool OpenTemplate( );
	bool OpenTemplate( wxString templatePath );

	//! Creates the report and fills all data
	//! using the data section handlers
	bool FillReport();

	//! Called by the report filler to request data for a given section
	bool GetSectionData( wxReportDataSection* section );

	//! Called by the template parser to load standard section properties
	//! and properties specific to the data section and handler
	bool LoadSectionProperties( wxXmlNodeEx *node, wxReportDataSection* ds );

	//! Adds a data section handler that can be used to
	//! load data for a given section type
	void AddDataSectionHandler( wxReportDataSectionHandler* handler ) { handlers.Add( handler ); };

	//! Sets the value of a report parameter
	void SetParameter( wxString paramName, wxString value );

	//! Gets the final path of the report
	wxString GetReportPath();

private: // functions

	//! Called when opening a report template to parse the document
	bool ParseDocument( wxXmlDocument *xmldoc );

	//! Called by the template parser routine to create a new
	//! data section and load all properties for the section
	wxReportDataSection* LoadDataSection( wxXmlNodeEx *node );

	//! Called to load all object in  given data section
	bool LoadObjects( wxXmlNodeEx *node, ObjectArray *ar );

	//! Creates a wxReportObj from a wxReportObject
	wxReportObj* GetReportObject( wxReportObject *obj, wxReportDataSection* ds );

	//! Replaces variable and parameter names with the appropriate data
	void EvaluateTextField( wxReportTextField tfld, wxReportObj* obj, wxReportDataSection* ds );

#ifdef USE_PYTHON_INTERPRETER
	//! Initializes the python interpreter
	bool InitPython();
#endif // USE_PYTHON_INTERPRETER

public: // functions

	//! Called to save data for a section to the report file
	bool SaveSectionData( wxReportDataSection* section );

	//! Filters a text string for parameter values
	wxString FilterParameterString( wxString val1 );

	//! Filters a text string for variable values
	wxString FilterVariableString( wxString val1 );

	//! Filters a text string for report field values
	wxString FilterFieldString( wxReportDataSection* section, wxString val1 );

private: // variables

	//! Full path to the report template
	wxFileName			reportFile;

	//! Parent folder of the report template
	wxString			rootFolder;

	//! The main report class where template
	//! data will be stored
	wxReport*			report;

	//! The report writer object that actually
	//! reads/writes/prints the report
	wxReportWriter*		pReport;

	//! Array of wxReportDataSectionHandler objects
	//! used to input data for the report data sections
	DataHandlerArray	handlers;

public: // static XML node functions

    //static wxXmlNode* GetFirstChild( wxXmlNode* parent, wxString name = wxT("") );
    //static wxXmlNode* GetNextSibling( wxXmlNode* prev, wxString name = wxT("") );

};

#endif
