#include <wx/xml/xml.h>

class WXDLLIMPEXP_XML wxXmlNodeEx : public wxXmlNode
{
public:

    wxXmlNodeEx& operator=( const wxXmlNodeEx& node )
    {
        wxXmlNode::operator=(node);
        return *this;
    };

    wxXmlNodeEx* GetFirstChild( wxString name )
    {
        wxXmlNodeEx* child = (wxXmlNodeEx*)GetChildren();

        if ( name == wxT("") )
            return child;

        while ( child )
        {
            if ( child->GetName().Lower() == name.Lower() )
            {
                return child;
            }
            child = (wxXmlNodeEx*)child->GetNext();
        }

        return NULL;
    };

    wxXmlNodeEx* GetNextSibling( wxXmlNodeEx* prev, wxString name = wxEmptyString )
    {
        wxXmlNodeEx* sibling = (wxXmlNodeEx*)prev->GetNext();

        if ( name == wxT("") )
            return sibling;

        while ( sibling )
        {
            if ( sibling->GetName().Lower() == name.Lower() )
                return sibling;
            else
                sibling = (wxXmlNodeEx*)sibling->GetNext();
        }

        return NULL;
    };
};
