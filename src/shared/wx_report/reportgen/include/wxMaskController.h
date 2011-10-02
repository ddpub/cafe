#ifndef __wxMaskController_H__
#define __wxMaskController_H__

/////////////////////////////////////////////////////////////////////////////
// Name:        wxMaskController.cpp (originally wxMaskTextCtrl.cpp)
// Purpose:     wxMaskController: masked edit control class modified to
//              work as a general purpose text mask handler
// Author:      Thomas Härtel
// Modified by: Scott Fant
// Created:     2002-11-14, modified on 2005-06-14
// RCS-ID:      $Id: wxMaskController.h,v 1.1 2009/09/03 12:07:28 ola Exp $
// Copyright:   (c) Thomas Härtel
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
// For compilers that support precompilation, includes "wx/wx.h".
#ifdef WX_PRECOMP
#include "wx/wxprec.h"
#else
#include "wx/wx.h"
#endif

#include "wx/list.h"
#include "wx/datetime.h"

const wxChar chNULL = wxT('\0') ;
const wxChar chDECIMALNULL = wxT('0') ;
const wxChar chCR = wxT('\r') ;
const wxChar chLF = wxT('\n') ;
const wxChar chSPACE = wxT('_') ;
const wxChar chPERIOD = wxT('.') ;
const wxChar chCOMMA = wxT(',') ;
const wxChar chCOLON = wxT(':') ;
const wxChar chSLASH = wxT('/') ;

/////////////////////////////////////////////////////////////////////////////
// wxFieldMaskData object

const wxChar chMaskPlaceholderDECIMALSEPARATOR = wxT('.');
const wxChar chMaskPlaceholderTHOUSANDSSEPARATOR = wxT(',');
const wxChar chMaskPlaceholderTIMESEPARATOR = wxT(':');
const wxChar chMaskPlaceholderDATESEPARATOR = wxT('/');
const wxChar chMaskPlaceholderDIGIT = wxT('#');
const wxChar chMaskPlaceholderALPHANUMERIC = wxT('A');
const wxChar chMaskPlaceholderALPHABETIC = wxT('?');
const wxChar chMaskPlaceholderALPHABETICUPPER = wxT('>');
const wxChar chMaskPlaceholderALPHABETICLOWER = wxT('<');
const wxChar chMaskPlaceholderCHARACTER = wxT('&');
const wxChar chMaskPlaceholderLITERALESCAPE = wxT('\\');
const wxChar chMaskPlaceholderDATEDAY = wxT('d');
const wxChar chMaskPlaceholderDATEMONTH = wxT('m');
const wxChar chMaskPlaceholderDATEYEAR = wxT('y');
const wxChar chMaskPlaceholderTIMEHOUR = wxT('h');
const wxChar chMaskPlaceholderTIMEMINUTE = wxT('n');
const wxChar chMaskPlaceholderTIMESECOND = wxT('s');

enum enumMaskDataType
{
	MaskDataTypeDECIMALSEPARATOR=0,  //!< Decimal separator. 
	MaskDataTypeTHOUSANDSSEPARATOR,  //!< Thousands separator. 
	MaskDataTypeTIMESEPARATOR     ,  //!< Time separator. 
	MaskDataTypeDATESEPARATOR     ,  //!< Date separator. 
	MaskDataTypeDIGIT             ,  //!< # digit placeholder (0-9). 
	MaskDataTypeALPHANUMERIC      ,  //!< A alphanumeric character placeholder (0-9 and a-Z). 
	MaskDataTypeALPHABETIC        ,  //!< ? Alphabetic placeholder (a-Z). 
	MaskDataTypeALPHAETICUPPER    ,  //!< > Alphabetic placeholder, but forced to Uppercase characters (A-Z). 
	MaskDataTypeALPHAETICLOWER    ,  //!< < Alphabetic placeholder, but forced to Lowercase characters (a-z). 
	MaskDataTypeCHARACTER         ,  //!< # character placeholder (ANSI characters 32-126 and 128-255). 
	MaskDataTypeLITERALESCAPE     ,  //!< All other symbols are displayed as themselves. 
	MaskDataTypeLITERAL           ,  //!< All other symbols are displayed as themselves. 
	MASKDATATYPECOUNT
};

enum enumMaskDataSubType
{
	MaskDataSubTypeNONE = 0,
	MaskDataSubTypeDATEDAY,
	MaskDataSubTypeDATEMONTH,
	MaskDataSubTypeDATEYEAR,
	MaskDataSubTypeTIMEHOUR,
	MaskDataSubTypeTIMEMINUTE,
	MaskDataSubTypeTIMESECOND,
	MASKDATASUBTYPECOUNT
};

class wxFieldMaskData : public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxFieldMaskData)
	
public:
	wxFieldMaskData();
	virtual ~wxFieldMaskData();

// Attributes
public:
	enumMaskDataType m_eType;
	enumMaskDataSubType m_eSubType;
	wxChar m_chValue;
	
// Operations
public:
	void operator=(const wxFieldMaskData& src);
	bool IsInputData();
	bool IsValidInput(wxChar chNewChar);
	wxChar PreProcessChar(wxChar chNewChar);
};

class wxMaskController
{
//DECLARE_DYNAMIC_CLASS(wxMaskController)

// Member functions ---------------------------------------------------------
public:
	wxMaskController();
	wxMaskController(const wxString& mask, const wxString &value);
	virtual ~wxMaskController();

	wxString GetMask();
	void SetMask(wxString mask);
	wxChar GetPromptSymbol();
	void SetPromptSymbol(wxChar chNewPromptSymbol);
	wxChar GetDecimalSeperator();
	void SetDecimalSeperator(wxChar chNewDecimalSeperator);
	wxChar GetThousandsSeperator();
	void SetThousandsSeperator(wxChar chNewThousandsSeperator);
	wxChar GetTimeSeperator();
	void SetTimeSeperator(wxChar chNewTimeSeperator);
	wxChar GetDateSeperator();
	void SetDateSeperator(wxChar chNewDateSeperator);
	bool GetInsertMode();
	void SetInsertMode(bool bInsertMode);
	bool GetAutoTab();
	void SetAutoTab(bool bAutoTab);
	bool GetBackwardLocationRight();
	void SetBackwardLocationRight(bool bRight=TRUE);
	wxDateTime GetMinDateTime(void);
	void SetMinDateTime(wxDateTime& minDateTime);
	wxDateTime GetMaxDateTime(void);
	void SetMaxDateTime(wxDateTime& maxDateTime);
	void SetDateTimeRange(wxDateTime& minDateTime, wxDateTime& maxDateTime);

	void Clear();
	void SetValue(wxString& value);
	wxDateTime GetDateTimeValue();
	void SetDateTimeValue(wxDateTime& value);
	
	wxString GetInputData();

	wxString GetTextValue() { return m_TextValue; };
	void SetTextValue( wxString value ) { m_TextValue = value; };

// Internal Member functions ------------------------------------------------
protected:
	
	wxString ShowMask();
	wxString GetInputData(wxString& value);
	bool SetInputData(wxString value, int nBeginPos=0, bool bAllowPrompt=true);	
	void EmptyData(bool bOnlyInput=FALSE);
	bool IsInputEmpty();
	bool IsEmpty();
	virtual void ValidationError();
	bool Validate();
	bool wxMaskController::IsValid(void);
	int RPtoLP(int nRealPos);
	int LPtoRP(int nLogicalPos);

	void DeleteContents();
	bool IsInputData(int nPosition);
	int DeleteRange(int nSelectionStart, int nSelectionEnd);
	int InsertAt(int nSelectionStart, wxChar chNewChar);
	int SetAt(int nSelectionStart, wxChar chNewChar);
	int GetNextInputLocation(int nSelectionStart);
	int GetPreviousInputLocation(int nSelectionStart);
	int GetEmptyInputLocation(int nSelectionStart);

	void Update(int nSelectionStart=0);  //!< UpdateInsertionPoint is automatic. 
	//void UpdateInsertionPointForward(int nSelectionStart);
	//void UpdateInsertionPointBackward(int nSelectionStart);

// Data members -------------------------------------------------------------
	wxList m_listData;

	bool m_bInsertMode;
	wxChar m_chPromptSymbol;
	wxChar m_chIntlDecimal;
	wxChar m_chIntlThousands;
	wxChar m_chIntlTime;
	wxChar m_chIntlDate;
	bool m_bAutoTab;	
	bool m_bBackwardLocationRight;
	bool m_bNeedValidation;
	bool m_bValidation;

	wxDateTime m_dtDateTime;
	wxDateTime m_dtMinDateTime;
	wxDateTime m_dtMaxDateTime;

	wxString m_TextValue;
};

#endif // __wxMaskController_H__
