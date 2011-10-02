/////////////////////////////////////////////////////////////////////////////
// Name:        wxMaskController.cpp
// Purpose:     wxMaskController: masked edit control class modified to
//              work as a general purpose text mask handler
// Author:      Thomas Härtel ( later modified by Scott Fant )
// Modified by:
// Created:     2002-11-14, modified on 2005-06-14
// RCS-ID:      $Id: wxMaskController.cpp,v 1.1 2009/09/03 12:07:28 ola Exp $
// Copyright:   (c) Thomas Härtel
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
// For compilers that support precompilation, includes "wx/wx.h".
#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif

#include "wx/wxprec.h"
#include "wxMaskController.h"

IMPLEMENT_DYNAMIC_CLASS(wxFieldMaskData, wxObject)

wxFieldMaskData::wxFieldMaskData() :
	m_eType(MaskDataTypeLITERAL),
	m_chValue(chNULL)
{
}

wxFieldMaskData::~wxFieldMaskData()
{
}

void wxFieldMaskData::operator=(const wxFieldMaskData& src)
{
	m_eType = src.m_eType;
	m_chValue = src.m_chValue;
}
		
bool wxFieldMaskData::IsInputData()
{
	
	bool bIsInputData=FALSE;
	switch(m_eType)
	{
		// These are the input types. 
		case MaskDataTypeDIGIT:
		case MaskDataTypeALPHANUMERIC:
		case MaskDataTypeALPHABETIC:
		case MaskDataTypeALPHAETICUPPER:
		case MaskDataTypeALPHAETICLOWER:
		case MaskDataTypeCHARACTER:
			bIsInputData=TRUE;
			break;
	}
	return bIsInputData;
}

bool wxFieldMaskData::IsValidInput(wxChar chNewChar)
{
	bool bIsValidInput=FALSE;
	switch(m_eType)
	{
		// These are the input types. 
		case MaskDataTypeDIGIT:
			bIsValidInput=wxIsdigit(chNewChar) != 0;
			break;
		
		case MaskDataTypeALPHANUMERIC:
			bIsValidInput=wxIsalnum(chNewChar) != 0;
			break;
		
		case MaskDataTypeALPHABETIC:
		case MaskDataTypeALPHAETICUPPER:
		case MaskDataTypeALPHAETICLOWER:
			bIsValidInput=wxIsalpha(chNewChar) != 0;
			break;
		
		case MaskDataTypeCHARACTER:
			if((chNewChar >=  32) && (chNewChar <= 126))
				bIsValidInput=TRUE;
			if((chNewChar >= 128) && (chNewChar <= 255))
				bIsValidInput = TRUE;
			break;
	}
	return bIsValidInput;
}

wxChar wxFieldMaskData::PreProcessChar(wxChar chNewChar)
{
	wxChar chProcessedChar=chNewChar;
	switch(m_eType)
	{
		case MaskDataTypeALPHAETICUPPER:
			chProcessedChar=(wxChar) wxToupper(chNewChar);
			break;
		
		case MaskDataTypeALPHAETICLOWER:
			chProcessedChar=(wxChar) wxTolower(chNewChar);
			break;
	}
	return chProcessedChar;
}


wxMaskController::wxMaskController(const wxString& mask, const wxString &value)
        : m_bInsertMode(TRUE), m_chPromptSymbol(chSPACE),
			m_chIntlDecimal(chPERIOD), m_chIntlThousands(chCOMMA), m_chIntlTime(chCOLON), m_chIntlDate(chSLASH),
			m_bAutoTab(FALSE), m_bBackwardLocationRight(TRUE), m_dtMinDateTime(wxInvalidDateTime),
			m_dtDateTime(wxInvalidDateTime), m_dtMaxDateTime(wxInvalidDateTime),
			m_bNeedValidation(TRUE), m_bValidation(FALSE)
{
	SetMask(wxString(mask));
	
	SetValue( (wxString&)value );

	if(m_listData.GetCount() != 0)
		Update();
}

wxMaskController::wxMaskController()
{
}

wxMaskController::~wxMaskController()
{
	DeleteContents();
}

void wxMaskController::DeleteContents()
{
	if(m_listData.GetCount() == 0)
		return;
	
	m_listData.DeleteContents(TRUE);

	m_bNeedValidation = FALSE;
	m_bValidation = TRUE;
}

wxString wxMaskController::GetMask()
{
	wxString csMask;
	wxFieldMaskData* pobjData=NULL;
	for(unsigned long pos = 0; pos < m_listData.GetCount();pos++)
	{
		pobjData= (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		switch(pobjData->m_eType)
		{
			case MaskDataTypeDECIMALSEPARATOR:
				csMask += chMaskPlaceholderDECIMALSEPARATOR;
				break;

			case MaskDataTypeTHOUSANDSSEPARATOR:
				csMask += chMaskPlaceholderTHOUSANDSSEPARATOR;
				break;

			case MaskDataTypeTIMESEPARATOR:
				csMask += chMaskPlaceholderTIMESEPARATOR;
				break;
			case MaskDataTypeDATESEPARATOR:
				csMask += chMaskPlaceholderDATESEPARATOR;
				break;

			case MaskDataTypeALPHANUMERIC:
				csMask += chMaskPlaceholderALPHANUMERIC;
				break;

			case MaskDataTypeALPHABETIC:
				csMask += chMaskPlaceholderALPHABETIC;
				break;

			case MaskDataTypeALPHAETICUPPER:
				csMask += chMaskPlaceholderALPHABETICUPPER;
				break;

			case MaskDataTypeALPHAETICLOWER:
				csMask += chMaskPlaceholderALPHABETICLOWER;
				break;

			case MaskDataTypeCHARACTER:
				csMask += chMaskPlaceholderCHARACTER;
				break;

			case MaskDataTypeDIGIT:
				switch(pobjData->m_eSubType)
				{
					case MaskDataSubTypeDATEDAY:
						csMask += chMaskPlaceholderDATEDAY;
						break;

					case MaskDataSubTypeDATEMONTH:
						csMask += chMaskPlaceholderDATEMONTH;
						break;

					case MaskDataSubTypeDATEYEAR:
						csMask += chMaskPlaceholderDATEYEAR;
						break;

					case MaskDataSubTypeTIMEHOUR:
						csMask += chMaskPlaceholderTIMEHOUR;
						break;

					case MaskDataSubTypeTIMEMINUTE:
						csMask += chMaskPlaceholderTIMEMINUTE;
						break;

					case MaskDataSubTypeTIMESECOND:
						csMask += chMaskPlaceholderTIMESECOND;
						break;

					default:
						csMask += chMaskPlaceholderDIGIT;
						break;
				}
				break;

			case MaskDataTypeLITERALESCAPE:
				// Need to add the escape to things that were escaped. 
				csMask += chMaskPlaceholderLITERALESCAPE;
				csMask += pobjData->m_chValue;
				break;

			default:
				// Literals and everything else is kept the same. 
				csMask += pobjData->m_chValue;
				break;
		}
	}
	return csMask;
}

void wxMaskController::SetMask(wxString mask)
{
	if(mask.IsEmpty())
	{
		mask = wxT("");
	}
	DeleteContents();
	
	wxFieldMaskData* pobjData = NULL;
	for(unsigned int i = 0;i < mask.Length(); i++)
	{
		wxChar chNew = mask[i];
		pobjData = new wxFieldMaskData();
		if(pobjData)
		{
			m_listData.Append(pobjData);
			pobjData->m_eSubType = MaskDataSubTypeNONE;

			switch(chNew)
			{
				case chMaskPlaceholderDECIMALSEPARATOR:
					pobjData->m_eType = MaskDataTypeDECIMALSEPARATOR;
					pobjData->m_chValue = m_chIntlDecimal;
					break;

				case chMaskPlaceholderTHOUSANDSSEPARATOR:
					pobjData->m_eType = MaskDataTypeTHOUSANDSSEPARATOR;
					pobjData->m_chValue = m_chIntlThousands;
					break;
				
				case chMaskPlaceholderTIMESEPARATOR:
					pobjData->m_eType = MaskDataTypeTIMESEPARATOR;
					pobjData->m_chValue = m_chIntlTime;
					break;
			
				case chMaskPlaceholderDATESEPARATOR:
					pobjData->m_eType = MaskDataTypeDATESEPARATOR;
					pobjData->m_chValue = m_chIntlDate;
					break;
		
				case chMaskPlaceholderDIGIT:
					pobjData->m_eType = MaskDataTypeDIGIT;
					pobjData->m_chValue = m_chPromptSymbol;
					break;
		
				case chMaskPlaceholderALPHANUMERIC:
					pobjData->m_eType = MaskDataTypeALPHANUMERIC;
					pobjData->m_chValue = m_chPromptSymbol;
					break;
			
				case chMaskPlaceholderALPHABETIC:
					pobjData->m_eType = MaskDataTypeALPHABETIC;
					pobjData->m_chValue = m_chPromptSymbol;
					break;
			
				case chMaskPlaceholderALPHABETICUPPER:
					pobjData->m_eType = MaskDataTypeALPHAETICUPPER;
					pobjData->m_chValue = m_chPromptSymbol;
					break;
			
				case chMaskPlaceholderALPHABETICLOWER:
					pobjData->m_eType = MaskDataTypeALPHAETICLOWER;
					pobjData->m_chValue = m_chPromptSymbol;
					break;
			
				case chMaskPlaceholderCHARACTER:
					pobjData->m_eType = MaskDataTypeCHARACTER;
					pobjData->m_chValue = m_chPromptSymbol;
					break;

				default:
					if(chNew == chMaskPlaceholderLITERALESCAPE)
					{
						// It is the next character that is inserted. 
						chNew = mask[++i];
						if(chNew)
						{
							pobjData->m_eType = MaskDataTypeLITERALESCAPE;
							pobjData->m_chValue = chNew;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderDATEDAY)
					{
						// It is the next character that is inserted. 
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderDATEDAY || chBefore == chMaskPlaceholderDATEDAY) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeDATEDAY;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderDATEMONTH)
					{
						// It is the next character that is inserted. 
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderDATEMONTH || chBefore == chMaskPlaceholderDATEMONTH) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeDATEMONTH;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderDATEYEAR)
					{
						// It is the next character that is inserted. 
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if(chNext == chMaskPlaceholderDATEYEAR || chBefore == chMaskPlaceholderDATEYEAR)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeDATEYEAR;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderTIMEHOUR)
					{
						// It is the next character that is inserted. 
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderTIMEHOUR || chBefore == chMaskPlaceholderTIMEHOUR) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeTIMEHOUR;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderTIMEMINUTE)
					{
						// It is the next character that is inserted. 
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderTIMEMINUTE || chBefore == chMaskPlaceholderTIMEMINUTE) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeTIMEMINUTE;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
					else if(chNew == chMaskPlaceholderTIMESECOND)
					{
						// It is the next character that is inserted. 
						wxChar chNext = (i < (mask.Length()-1) ? mask[i+1] : wxT('\0'));
						wxChar chBefore = (i > 0 ? mask[i-1] : wxT('\0'));
						if((chNext == chMaskPlaceholderTIMESECOND || chBefore == chMaskPlaceholderTIMESECOND) && chBefore != chNext)
						{
							pobjData->m_eType = MaskDataTypeDIGIT;
							pobjData->m_eSubType = MaskDataSubTypeTIMESECOND;
							pobjData->m_chValue = m_chPromptSymbol;
							break;
						}
					}
	
					// Everything else is just a literal. 
					pobjData->m_eType = MaskDataTypeLITERAL;
					pobjData->m_chValue = chNew;
					break;
			}
		}
	}
	Update();
}

wxString wxMaskController::GetInputData()
{
	wxString csInputData;

	if(m_listData.GetCount() == 0)
	{
		csInputData = GetTextValue();
		return csInputData;
	}

	wxFieldMaskData* pobjData=NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData= (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		// Ignore everything that is not data. 
		if(pobjData->IsInputData())
			csInputData += pobjData->m_chValue;
	}
	return csInputData;
}

wxString wxMaskController::GetInputData(wxString& value)
{
	wxString csInputData = value;
	unsigned int nSymbolCount = (int)csInputData.Length();
	wxFieldMaskData* pobjData = NULL;
	wxString sToExclude;
	int nStartPos = -1;
	int nEndPos = -1;
	unsigned int nIndex = 0;
	int nRemovedCount = 0;
	
	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		if(!pobjData->IsInputData())
		{
			if(nStartPos == -1)
			{
				nStartPos = nIndex;
				sToExclude.Empty();
			}
			sToExclude+=pobjData->m_chValue;
		}
		else
		{
			if(nStartPos != -1)
			{
				nEndPos = nIndex-1;
				if(csInputData.Mid(nStartPos-nRemovedCount, nEndPos-nStartPos+1) == sToExclude)
				{
					csInputData=csInputData.Left(nStartPos-nRemovedCount) + csInputData.Mid(nEndPos-nRemovedCount+1);
					nRemovedCount+=nEndPos-nStartPos+1;
				}
				nStartPos = -1;
			}
		}

		nIndex++;
		if(nIndex >= nSymbolCount)
			break;
	}

	return csInputData;
}

bool wxMaskController::SetInputData(wxString value, int nBeginPos/*=0*/, bool bAllowPrompt/*=TRUE*/)
{
	wxString csFullInput;

	m_bNeedValidation = TRUE;
	m_bValidation = FALSE;


	// Start with existing data and append the new data. 
	csFullInput = GetInputData();
	csFullInput = csFullInput.Left(nBeginPos);
	
	if(bAllowPrompt)
		csFullInput += value;
	else
	{
		// If the prompt symbol is not valid, then 
		// add the data one-by-one ignoring any prompt symbols. 
		for(unsigned int i = 0;i < value.Length();i++)
		{
			if(value[i] != m_chPromptSymbol)
				csFullInput += value[i];
		}
	}
	
	bool bCompleteSuccess=TRUE;
	wxString pszReplaceData=csFullInput;
	wxFieldMaskData* pobjData=NULL;
	unsigned int posReplaceData=0;

	for(unsigned long pos = 0; pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();

		// Ignore everything that is not data. 
		if(pobjData->IsInputData())
		{
			// If we run out of replacement data, then use the prompt symbol. 
			// Make sure we iterate through the entire list so that the 
			// prompt symbol is applied to any empty areas. 
			if(posReplaceData < pszReplaceData.Length())
			{
				// This inner while loop is so that we can re-apply input data 
				// after an error.  This will allow us to skip over invalid 
				// input data and try the next character. 
				while(posReplaceData< pszReplaceData.Length())
				{
					wxChar chReplace = pszReplaceData[posReplaceData++];
					
					// Make sure to follow the input validation. 
					// The prompt symbol is always valid at this level. 
					// This allows the user to erase a string by overtyping a space. 
					// On error, just skip the character being inserted. 
					// This will allow the DeleteRange() function to have the remaining 
					// characters validated. 
					if((chReplace == m_chPromptSymbol) || pobjData->IsValidInput(chReplace))
					{
						pobjData->m_chValue = pobjData->PreProcessChar(chReplace);
						break;
					}
					else
						bCompleteSuccess = FALSE;
				}
			}
			else
				pobjData->m_chValue = m_chPromptSymbol;
		}
	}
	
	Update();

	return bCompleteSuccess;
}

wxChar wxMaskController::GetPromptSymbol()
{
	return m_chPromptSymbol;
}

void wxMaskController::SetPromptSymbol(wxChar chNewPromptSymbol)
{
	// The prompt symbol must be a valid edit box symbol. 

	if(wxIsprint((int) chNewPromptSymbol))
	{
		// Just for the heck of it, if the prompt symbol changes, 
		// go through and replace the existing prompts with the new prompt. 
		wxFieldMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->IsInputData())
			{
				if(pobjData->m_chValue == m_chPromptSymbol)
					pobjData->m_chValue = chNewPromptSymbol;
			}
		}
		m_chPromptSymbol = chNewPromptSymbol;
	}
	
	// Don't update the insertion point if just setting the prompt symbol. 
	Update(-1);
}

wxChar wxMaskController::GetDecimalSeperator()
{
	return m_chIntlDecimal;
}

void wxMaskController::SetDecimalSeperator(wxChar chNewDecimalSeperator)
{
	// The decimal seperator must be a valid edit box symbol. 

	if(wxIsprint((int) chNewDecimalSeperator))
	{
		// Just for the heck of it, if the decimal symbol changes, 
		// go through and replace the existing symbols with the new symbol. 
		wxFieldMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeDECIMALSEPARATOR)
				pobjData->m_chValue = chNewDecimalSeperator;
		}
		m_chIntlDecimal = chNewDecimalSeperator;
	}
	
	// Don't update the insertion point if just setting the decimal symbol. 
	Update(-1);
}

wxChar wxMaskController::GetThousandsSeperator()
{
	return m_chIntlThousands;
}

void wxMaskController::SetThousandsSeperator(wxChar chNewThousandsSeperator)
{
	// The thousands seperator must be a valid edit box symbol. 

	if(wxIsprint((int) chNewThousandsSeperator))
	{
		// Just for the heck of it, if the thousands symbol changes, 
		// go through and replace the existing symbols with the new symbol. 
		wxFieldMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeTHOUSANDSSEPARATOR)
				pobjData->m_chValue = chNewThousandsSeperator;
		}
		m_chIntlThousands = chNewThousandsSeperator;
	}
	
	// Don't update the insertion point if just setting the thousands symbol. 
	Update(-1);
}

wxChar wxMaskController::GetTimeSeperator()
{
	return m_chIntlTime;
}

void wxMaskController::SetTimeSeperator(wxChar chNewTimeSeperator)
{
	// The time seperator must be a valid edit box symbol. 

	if(wxIsprint((int) chNewTimeSeperator))
	{
		// Just for the heck of it, if the time symbol changes, 
		// go through and replace the existing symbols with the new symbol. 
		wxFieldMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeTIMESEPARATOR)
				pobjData->m_chValue = chNewTimeSeperator;
		}
		m_chIntlTime = chNewTimeSeperator;
	}
	
	// Don't update the insertion point if just setting the time symbol. 
	Update(-1);
}

wxChar wxMaskController::GetDateSeperator()
{
	return m_chIntlDate;
}

void wxMaskController::SetDateSeperator(wxChar chNewDateSeperator)
{
	// The time seperator must be a valid edit box symbol. 

	if(wxIsprint((int) chNewDateSeperator))
	{
		// Just for the heck of it, if the date symbol changes, 
		// go through and replace the existing symbols with the new symbol. 
		wxFieldMaskData* pobjData=NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->m_eType == MaskDataTypeDATESEPARATOR)
				pobjData->m_chValue = chNewDateSeperator;
		}
		m_chIntlDate = chNewDateSeperator;
	}
	
	// Don't update the insertion point if just setting the date symbol. 
	Update(-1);
}

void wxMaskController::EmptyData(bool bOnlyInput/*=FALSE*/)
{
	if(m_listData.GetCount()==0)
	{
		DeleteContents();
		return;
	}

	if(bOnlyInput)
	{
		// If emptying only the data, the iterate through the list 
		// of data and replace input data with the prompt symbol. 
		wxFieldMaskData* pobjData = NULL;
		for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
		{
			pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
			if(pobjData->IsInputData())
				pobjData->m_chValue=m_chPromptSymbol;
		}
	}
	else
		DeleteContents();
	
	Update();
}

bool wxMaskController::IsInputEmpty()
{
	if(m_listData.GetCount() == 0)
	{
		wxString csInputData;
		csInputData = GetTextValue();
		return csInputData.IsEmpty();
	}

	wxFieldMaskData* pobjData=NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		if (pobjData->IsInputData() && pobjData->m_chValue!=m_chPromptSymbol)
			return FALSE;
	}
	return TRUE;
}

bool wxMaskController::GetInsertMode()
{
	return m_bInsertMode;
}

void wxMaskController::SetInsertMode(bool bInsertMode)
{
	m_bInsertMode=bInsertMode;
}

bool wxMaskController::GetAutoTab()
{
	return m_bAutoTab;
}

void wxMaskController::SetAutoTab(bool bAutoTab)
{
	m_bAutoTab=bAutoTab;
}

void wxMaskController::SetBackwardLocationRight(bool bRight)
{
	m_bBackwardLocationRight = bRight;
}

bool wxMaskController::GetBackwardLocationRight()
{
	return m_bBackwardLocationRight;
}

wxString wxMaskController::ShowMask()
{
	wxString csShow;
	wxFieldMaskData* pobjData=NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount();pos++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		
		// There is no need to do any fancy string building because 
		// all validation is done when characters are inserted into the list. 
		// Literals and placeholders are converted properly at that time 
		// so all we have to do here is get the value. 
		if(pobjData->m_chValue != chNULL)
			csShow += pobjData->m_chValue;
	}
	return csShow;
}

bool wxMaskController::IsInputData(int nPosition)
{
	if(m_listData.GetCount() == 0)
	{
		return TRUE;
	}

	// We frequently need to know if a position refers to 
	// input data or to a literal. 
	bool bIsInputData=FALSE;
	if(nPosition >= 0 && nPosition < (int) m_listData.GetCount())
	{
		wxFieldMaskData* pobjData = (wxFieldMaskData*) (m_listData.Item(nPosition))->GetData();
		if(pobjData)
		{
			bIsInputData = pobjData->IsInputData();
		}
	}
	return bIsInputData;
}

int wxMaskController::DeleteRange(int nSelectionStart, int nSelectionEnd)
{
	// In order to delete properly, we must count the number of 
	// input characters that are selected and only delete that many. 
	// This is because the selection can include literals. 
	int nCharIndex  =0;
	int nDeleteCount=0;
	wxString csInputData;
	wxFieldMaskData* pobjData = NULL;

	m_bNeedValidation = TRUE;
	m_bValidation = FALSE;

	for(unsigned long pos = 0;pos < m_listData.GetCount(); pos++,nCharIndex++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		// Ignore everything that is not data. 
		// This is just like we do in GetInputData except that we 
		// will ignore the input data within the selection range. 
		if(pobjData->IsInputData())
		{
			if((nCharIndex < nSelectionStart) || (nCharIndex >= nSelectionEnd))
			{
				// The SetInputData() function will take care of validating 
				// the shifted characters. 
				csInputData += pobjData->m_chValue;
			}
			else
				nDeleteCount++;
		}
	}
	// Now apply the filtered data stream. 
	SetInputData(csInputData);
	// return the deleted count so that an error can be generated 
	// if none were deleted. 
	return nDeleteCount;
}

int wxMaskController::InsertAt(int nSelectionStart, wxChar chNewChar)
{
	// Although we could have some complex, yet efficient, routine 
	// that would error if inserting pushed an existing character 
	// into an invalid region.  Instead, just save the current 
	// state and restore it on error. 
	wxString csPreviousInput=GetInputData();

	int nCharIndex = 0;
	int nInsertionPoint = -1;
	wxString csInputData;
	wxFieldMaskData* pobjData = NULL;
	for(unsigned long pos = 0;pos < m_listData.GetCount(); pos++,nCharIndex++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		// Ignore everything that is not data. 
		// This is just like we do in GetInputData except that we 
		// will ignore the input data within the selection range. 
		if(pobjData->IsInputData())
		{
			// Wait until a valid insertion point and 
			// only make sure to insert once. 
			if((nInsertionPoint < 0) && (nCharIndex >= nSelectionStart))
			{
				csInputData += chNewChar;
				nInsertionPoint=nCharIndex;
			}
			csInputData += pobjData->m_chValue;
		}
	}
	// Now apply the filtered data stream and check if it was successful. 
	if(!SetInputData(csInputData))
	{
		// If not successful, then restore the previous input and return -1. 
		SetInputData(csPreviousInput);
		return -1;
	}
	return nInsertionPoint;
}

int wxMaskController::SetAt(int nSelectionStart, wxChar chNewChar)
{
	if(nSelectionStart >= 0)
	{
		m_bNeedValidation = TRUE;
		m_bValidation = FALSE;

		wxFieldMaskData* pobjData=(wxFieldMaskData*) (m_listData.Item(nSelectionStart))->GetData();
		if(pobjData)
		{
			if(pobjData->IsInputData())
			{
				if((chNewChar == m_chPromptSymbol) || pobjData->IsValidInput(chNewChar))
					pobjData->m_chValue=pobjData->PreProcessChar(chNewChar);
				else
					return -1;  // Input value is invalid or not allowed. 
			}
		}
	}
	return nSelectionStart;
}

int wxMaskController::GetNextInputLocation(int nSelectionStart)
{
	// One of the functions of this edit control is that it skips over literals. 
	// We need a function to help skip to the next position. 
	int nNextInputLocation=nSelectionStart;
	if(nNextInputLocation < 0)
		nNextInputLocation = 0;

	wxFieldMaskData* pobjData = NULL;
	while(nNextInputLocation < (int) m_listData.GetCount())
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(nNextInputLocation))->GetData();
		if(pobjData->IsInputData())
			break;

		nNextInputLocation++;
	}

	return nNextInputLocation;
}

int wxMaskController::GetPreviousInputLocation(int nSelectionStart)
{
	// One of the functions of this edit control is that it skips over literals. 
	// We need a function to help skip to the next position. 
	int nNextInputLocation=nSelectionStart;
	if(nNextInputLocation < 0)
		nNextInputLocation = 0;
	// Need to determine if we moved to a previous location. 
	// There will need to be some correction. 
	int nInitialInputLocation=nNextInputLocation;
	
	wxFieldMaskData* pobjData = NULL;
	for(wxNode* node = m_listData.Item(nNextInputLocation);node;nNextInputLocation--)
	{
		pobjData = (wxFieldMaskData *) node->GetData();
		if(pobjData->IsInputData())
		{
			if(nInitialInputLocation != nNextInputLocation)
			{
				// If we find a valid previous location, then move to the right of it. 
				// This backup, then move forward is typical when seeking backward. 
				if(m_bBackwardLocationRight)
					nNextInputLocation++;
			}
			break;
		}
		node = node->GetPrevious();
	}
	// If there is no input data to the left of the selection, 
	// then seek forward to the next location. 
	if(nNextInputLocation < 0)
		return GetNextInputLocation(nSelectionStart);
	return nNextInputLocation;
}

int wxMaskController::GetEmptyInputLocation(int nSelectionStart)
{
	int nEmptyInputLocation=nSelectionStart;
	if(nEmptyInputLocation < 0)
		nEmptyInputLocation = 0;
	
	wxFieldMaskData* pobjData=NULL;
	for(unsigned long pos = nEmptyInputLocation;;pos++,nEmptyInputLocation++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->IsInputData())
		{ 
			if(pobjData->m_chValue == m_chPromptSymbol)
				break;
		}
	}
	return nEmptyInputLocation;
}

void wxMaskController::Update(int WXUNUSED(nSelectionStart))
{
	// Update the edit control if it exists. 
	wxString sText = ShowMask();
	SetTextValue(sText);
}

void wxMaskController::ValidationError()
{
	wxBell();
}

void wxMaskController::Clear()
{
	if(m_listData.GetCount() == 0)
		SetTextValue(_(""));
	else
	{
		long nSelectionStart = 0;

		// Now we update with our standard mask. 
		Update(nSelectionStart);
	}
}

bool wxMaskController::Validate() 
{
	int day = 0, month = 0, year = 0, hour = 0, minute = 0, second = 0;
	bool bDay = FALSE, bMonth = FALSE, bYear = FALSE, bHour = FALSE, bMinute = FALSE, bSecond = FALSE;
	bool DateValid, TimeValid;
	int year_len = 0;
	unsigned long used = 0;
	long StartPosDay = -1, EndPosDay = -1, StartPosMonth = -1, EndPosMonth = -1, StartPosYear = -1, EndPosYear = -1;
	long StartPosHour = -1, EndPosHour = -1, StartPosMinute = -1, EndPosMinute = -1, StartPosSecond = -1, EndPosSecond = -1;

	if(!m_bNeedValidation)
		return m_bValidation;

	wxFieldMaskData* pobjData=NULL;
	m_dtDateTime = wxInvalidDateTime;
	for(unsigned long pos = 0; pos < m_listData.GetCount();pos++)
	{
		pobjData= (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->m_eType == MaskDataTypeDIGIT)
		{
			if(pobjData->m_chValue != m_chPromptSymbol)
				used++;

			wxFieldMaskData* pobjDataNext = NULL;

			switch(pobjData->m_eSubType)
			{
				case MaskDataSubTypeDATEDAY:
					bDay = TRUE;
					if(StartPosDay == -1)
					{
						StartPosDay = pos;
						EndPosDay = pos;
					}
					EndPosDay++;

					pobjDataNext = (wxFieldMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						day = (day * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeDATEDAY)
							{
								day = (day * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosDay++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeDATEMONTH:
					bMonth = TRUE;
					if(StartPosMonth == -1)
					{
						StartPosMonth = pos;
						EndPosMonth = pos;
					}
					EndPosMonth++;

					pobjDataNext = (wxFieldMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						month = (month * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeDATEMONTH)
							{
								month = (month * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosMonth++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeDATEYEAR:
					bYear = TRUE;
					if(StartPosYear == -1)
					{
						StartPosYear = pos;
						EndPosYear = pos;
					}
					EndPosYear++;

					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						year = (year * 10) + (pobjData->m_chValue - 48);
						year_len++;
					}
					break;

				case MaskDataSubTypeTIMEHOUR:
					bHour = TRUE;
					if(StartPosHour == -1)
					{
						StartPosHour = pos;
						EndPosHour = pos;
					}
					EndPosHour++;

					pobjDataNext = (wxFieldMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						hour = (hour * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeTIMEHOUR)
							{
								hour = (hour * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosHour++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeTIMEMINUTE:
					bMinute = TRUE;
					if(StartPosMinute == -1)
					{
						StartPosMinute = pos;
						EndPosMinute = pos;
					}
					EndPosMinute++;
					
					pobjDataNext = (wxFieldMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						
						minute = (minute * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeTIMEMINUTE)
							{
								minute = (minute * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosMinute++;
								pos++;
							}
						}
					}
					break;

				case MaskDataSubTypeTIMESECOND:
					bSecond = TRUE;
					if(StartPosSecond == -1)
					{
						StartPosSecond = pos;
						EndPosSecond = pos;
					}
					EndPosSecond++;

					pobjDataNext = (wxFieldMaskData *) (pos < (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
					if(pobjData->m_chValue != m_chPromptSymbol)
					{
						second = (second * 10) + (pobjData->m_chValue - 48);

						if(pobjDataNext != NULL)
						{
							if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeTIMESECOND)
							{
								second = (second * 10) + (pobjDataNext->m_chValue != m_chPromptSymbol ? (pobjDataNext->m_chValue - 48) : 0);
								EndPosSecond++;
								pos++;
							}
						}
					}
					break;

			}
		}
	}

	m_bNeedValidation = FALSE;

	if(used == 0)
		return (m_bValidation = TRUE);

	bool minValid = m_dtMinDateTime.IsValid();
	bool maxValid = m_dtMaxDateTime.IsValid();

	DateValid = TRUE;
	if(bDay || bMonth || bYear)
	{
		long pos;
		wxDateTime minDate = m_dtMinDateTime;
		wxDateTime maxDate = m_dtMaxDateTime;
		wxDateTime Date = wxDateTime(1, wxDateTime::Jan, (wxDateTime::Now()).GetYear(), 0, 0, 0, 0);

		if((bDay && day == 0) || (bMonth && month == 0))
			DateValid = FALSE;
		else if((bDay && day > 31) || (bMonth && month > 12))
			DateValid = FALSE;
		
		if(DateValid)
		{
			if(bYear && year_len < 3 && year < 100)
			{
				if(year_len == 0)
				{
					year = (wxDateTime::Now()).GetYear();
					year_len = 1;
				}
				else
					year += ((wxDateTime::Now()).GetYear() / 1000) * 1000;
			}

			if(bYear)
			{
				if(StartPosYear != -1)
				{
					wxString Year_Str = wxString::Format(_("%04d"), year);

					for(pos = StartPosYear; pos < EndPosYear;pos++)
						SetAt(pos, Year_Str.GetChar(Year_Str.Length() + pos - EndPosYear));
				}

				Date.SetYear(year);

				if(minValid)
				{
					minDate.SetMonth(wxDateTime::Jan);
					minDate.SetDay(1);				
				}
			
				if(maxValid)
				{
					maxDate.SetMonth(wxDateTime::Dec);
					maxDate.SetDay(31);				
				}
			}
			else
			{
				if(minValid)
					minDate.SetYear(Date.GetYear());
			
				if(maxValid)
					maxDate.SetYear(Date.GetYear());
			}

			if(bMonth)
			{
				if(StartPosMonth != -1)
				{
					wxString Month_Str = wxString::Format(_("%02d"), month);

					for(pos = StartPosMonth; pos < EndPosMonth;pos++)
						SetAt(pos, Month_Str.GetChar(pos - StartPosMonth));
				}

				Date.SetMonth(wxDateTime::Month(month-1));

				if(minValid)
					minDate.SetMonth(m_dtMinDateTime.GetMonth());
			
				if(maxValid)
				{
					maxDate.SetDay(1);
					maxDate.SetMonth(m_dtMaxDateTime.GetMonth());
					maxDate.SetDay(m_dtMaxDateTime.GetNumberOfDays(m_dtMaxDateTime.GetMonth(), m_dtMaxDateTime.GetYear()));
				}
			}
			else
			{
				if(minValid)
					minDate.SetMonth(m_dtMinDateTime.GetMonth());
			
				if(maxValid)
					maxDate.SetMonth(m_dtMaxDateTime.GetMonth());
			}

			if(bDay)
			{
				if(StartPosDay != -1)
				{
					wxString Day_Str = wxString::Format(_("%02d"), day);

					for(pos = StartPosDay; pos < EndPosDay;pos++)
						SetAt(pos, Day_Str.GetChar(pos - StartPosDay));
				}

				if(day > Date.GetNumberOfDays(Date.GetMonth(), Date.GetYear()))
					DateValid = FALSE;

				if(DateValid)
				{
					Date.SetDay(day);

					if(minValid)
						minDate.SetDay(m_dtMinDateTime.GetDay());
			
					if(maxValid)
						maxDate.SetDay(m_dtMaxDateTime.GetDay());
				}
			}
			else
			{
				if(minValid)
					minDate.SetDay(1);
			
				if(maxValid)
					maxDate.SetDay(m_dtMaxDateTime.GetNumberOfDays(m_dtMaxDateTime.GetMonth(), m_dtMaxDateTime.GetYear()));
			}
		
			if(DateValid)
			{
				if(!Date.IsValid())
					DateValid = FALSE;
						
				if(minValid && m_bValidation)
				{
					if(Date.IsEarlierThan(minDate))
						DateValid = FALSE;
				}

				if(maxValid && m_bValidation)
				{
					if(Date.IsLaterThan(maxDate))
						DateValid = FALSE;
				}

				m_dtDateTime = Date;
			}
		}
	}

	TimeValid = TRUE;
	if(bHour || bMinute || bSecond)
	{
		long pos;
		wxDateTime minDate = m_dtMinDateTime;
		wxDateTime maxDate = m_dtMaxDateTime;
		wxDateTime Date;

		if(m_dtDateTime.IsValid())
			Date = m_dtDateTime;
		else
			Date = wxDateTime(1, wxDateTime::Jan, (wxDateTime::Now()).GetYear(), 0, 0, 0, 0);

		if((bHour && hour > 23) || (bMinute && minute > 59) || (bSecond && second > 59))
			TimeValid = FALSE;
		
		if(TimeValid)
		{
			if(bHour)
			{
				if(StartPosHour != -1)
				{
					wxString Hour_Str = wxString::Format(_("%02d"), hour);

					for(pos = StartPosHour; pos < EndPosHour;pos++)
						SetAt(pos, Hour_Str.GetChar(pos - StartPosHour));
				}
	
				Date.SetHour(hour);
			}
			else
			{
				if(minValid)
					minDate.SetHour(0);
			
				if(maxValid)
					maxDate.SetHour(1);
			}

			if(bMinute)
			{
				if(StartPosMinute != -1)
				{
					wxString Minute_Str = wxString::Format(_("%02d"), minute);

					for(pos = StartPosMinute; pos < EndPosMinute;pos++)
						SetAt(pos, Minute_Str.GetChar(pos - StartPosMinute));
				}

				Date.SetMinute(minute);
			}
			else
			{
				if(minValid)
					minDate.SetMinute(0);
			
				if(maxValid)
					maxDate.SetMinute(59);
			}

			if(bSecond)
			{
				if(StartPosSecond != -1)
				{
					wxString Second_Str = wxString::Format(_("%02d"), second);

					for(pos = StartPosSecond; pos < EndPosSecond;pos++)
						SetAt(pos, Second_Str.GetChar(pos - StartPosSecond));
				}

				Date.SetSecond(second);
			}
			else
			{
				if(minValid)
					minDate.SetSecond(0);
			
				if(maxValid)
					maxDate.SetSecond(59);
			}
		
			if(!Date.IsValid())
				TimeValid = FALSE;
			
			if(TimeValid)
			{
				if(minValid)
				{
					if(Date.IsEarlierThan(minDate))
						TimeValid = FALSE;
				}

				if(maxValid)
				{
					if(Date.IsLaterThan(maxDate))
						TimeValid = FALSE;
				}

				m_dtDateTime.Set(hour, minute, second);
			}
		}
	}
	
	Update(-1);

	m_bValidation = DateValid & TimeValid;
	m_bNeedValidation = FALSE;

	return m_bValidation;
}

bool wxMaskController::IsValid(void)
{
	if(m_bNeedValidation)
		Validate();

	return m_bValidation;
}

bool wxMaskController::IsEmpty(void)
{
	return IsInputEmpty();
}

wxDateTime wxMaskController::GetMinDateTime(void)
{
	return m_dtMinDateTime;
}

void wxMaskController::SetMinDateTime(wxDateTime& minDateTime)
{
	m_dtMinDateTime = minDateTime;
}

wxDateTime wxMaskController::GetMaxDateTime(void)
{
	return m_dtMaxDateTime;
}

void wxMaskController::SetMaxDateTime(wxDateTime& maxDateTime)
{
	m_dtMaxDateTime = maxDateTime;
}

void wxMaskController::SetDateTimeRange(wxDateTime& minDateTime, wxDateTime& maxDateTime)
{
	m_dtMinDateTime = minDateTime;
	m_dtMaxDateTime = maxDateTime;
}

void wxMaskController::SetValue(wxString& value)
{
	if(m_listData.GetCount() == 0)
		SetTextValue(value);
	else
		SetInputData(GetInputData(value), 0, true);
}

void wxMaskController::SetDateTimeValue(wxDateTime& value)
{
	wxFieldMaskData *pobjData = NULL, *pobjDataNext = NULL;
	wxString str_Value;
	long length;
	int year_len;

	if(m_listData.GetCount() == 0 && value.IsValid())
		return;

	for(long pos = 0;pos < (long) m_listData.GetCount();pos++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->m_eType == MaskDataTypeDIGIT)
		{
			if(pobjData->m_eSubType == MaskDataSubTypeDATEDAY)
			{
				pobjDataNext = (wxFieldMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeDATEDAY)
						str_Value = wxString::Format(_("%02d"), value.GetDay());
					else
						str_Value = wxString::Format(_("%d"), value.GetDay() % 10);
				}
				
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeDATEMONTH)
			{
				pobjDataNext = (wxFieldMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeDATEMONTH)
						str_Value = wxString::Format(_("%02d"), value.GetMonth());
					else
						str_Value = wxString::Format(_("%d"), value.GetMonth() % 10);
				}
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeDATEYEAR)
			{
				year_len = 1;
				for(long i = pos; i < (long) m_listData.GetCount();i++)
				{
					pobjDataNext = (wxFieldMaskData *) (m_listData.Item(pos+1))->GetData();
					if(pobjDataNext->m_eType == MaskDataTypeDIGIT && pobjDataNext->m_eSubType == MaskDataSubTypeDATEYEAR)
						year_len++;
					else
						break;
				}
				str_Value = (wxString::Format(_("%04d"), value.GetYear())).Right(year_len);;
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeTIMEHOUR)
			{
				pobjDataNext = (wxFieldMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeTIMEHOUR)
						str_Value = wxString::Format(_("%02d"), value.GetHour());
					else
						str_Value = wxString::Format(_("%d"), value.GetHour() % 10);
				}
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeTIMEMINUTE)
			{
				pobjDataNext = (wxFieldMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeTIMEMINUTE)
						str_Value = wxString::Format(_("%02d"), value.GetMinute());
					else
						str_Value = wxString::Format(_("%d"), value.GetMinute() % 10);
				}
			}
			else if(pobjData->m_eSubType == MaskDataSubTypeTIMESECOND)
			{
				pobjDataNext = (wxFieldMaskData *) (pos < (long) (m_listData.GetCount()-1) ? (m_listData.Item(pos+1))->GetData() : NULL);
				if(pobjDataNext)
				{	
					if(pobjData->m_eType == MaskDataTypeDIGIT && pobjData->m_eSubType == MaskDataSubTypeTIMESECOND)
						str_Value = wxString::Format(_("%02d"), value.GetSecond());
					else
						str_Value = wxString::Format(_("%d"), value.GetSecond() % 10);
				}
			}
			
			length = (long)str_Value.Length();
			if(pos + length > (long) m_listData.GetCount())
				length = (long) str_Value.Length() - pos;

			for(long i = pos; pos < (long) (i + length);pos++)
				SetAt(pos, str_Value.GetChar(pos-i));
		}
	}
	Update();
}

wxDateTime wxMaskController::GetDateTimeValue()
{
	if(m_bNeedValidation)
		Validate();

	return m_dtDateTime;
}

int wxMaskController::RPtoLP(int nRealPos) 
{
	// All wxMaskController functions that take cursor position as argument interpret it
	// as real position within edit control (taking into account all symbols including 
	// literals). But sometimes we want to know which non-literal symbol is at  
	// particular real position. In that case this function is really useful

	if(nRealPos < 0 || nRealPos >= (int) m_listData.GetCount())
		return -1;
	
	int nLogicalPos = -1;
	wxFieldMaskData* pobjData = NULL;
	int nNextInputLocation = 0;
	for(unsigned long pos = nNextInputLocation;pos < m_listData.GetCount();pos++, nNextInputLocation++)
	{
		pobjData = (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		if(pobjData->IsInputData())
		{
			nLogicalPos++;
		}
		if(nNextInputLocation == nRealPos)
		{
			return pobjData->IsInputData() ? nLogicalPos : -1;
		}
	}

	return -1;
}
		
int wxMaskController::LPtoRP(int nLogicalPos) 
{
	// All wxMaskController functions that take cursor position as argument interpret it
	// as real position within edit control (taking into account all symbols including 
	// literals). But sometimes we want to set cursor at position before or after 
	// particular non-literal symbol. In that case this function is really useful

	if(nLogicalPos < 0 || nLogicalPos >= (int) m_listData.GetCount())
		return -1;
	
	int nRealPos = -1;
	int nNonLiterals = -1;
	wxFieldMaskData* pobjData = NULL;
	int nNextInputLocation = 0;
	for(unsigned long pos = nNextInputLocation; pos < m_listData.GetCount();pos++, nNextInputLocation++)
	{
		pobjData= (wxFieldMaskData *) (m_listData.Item(pos))->GetData();
		nRealPos++;
		if(pobjData->IsInputData())
		{
			nNonLiterals++;
			if(nNonLiterals == nLogicalPos)
			{
				return nRealPos;
			}
		}
	}

	return -1;
}
