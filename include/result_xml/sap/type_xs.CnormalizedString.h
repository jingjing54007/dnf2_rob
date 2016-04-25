#ifndef _ALTOVA_INCLUDED_sap_ALTOVA_xs_ALTOVA_CnormalizedString
#define _ALTOVA_INCLUDED_sap_ALTOVA_xs_ALTOVA_CnormalizedString



namespace sap
{

namespace xs
{	

class CnormalizedString : public TypeBase
{
public:
	sap_EXPORT CnormalizedString(xercesc::DOMNode* const& init);
	sap_EXPORT CnormalizedString(CnormalizedString const& init);
	void operator=(CnormalizedString const& other) { m_node = other.m_node; }
	static altova::meta::SimpleType StaticInfo() { return altova::meta::SimpleType(types + _altova_ti_xs_altova_CnormalizedString); }
	void operator= (const string_type& value) 
	{
		altova::XmlFormatter* Formatter = static_cast<altova::XmlFormatter*>(altova::AnySimpleTypeFormatter);
		XercesTreeOperations::SetValue(GetNode(), Formatter->Format(value));
	}	
		
	operator string_type()
	{
		return CastAs<string_type >::Do(GetNode(), 0);
	}
};



} // namespace xs

}	// namespace sap

#endif // _ALTOVA_INCLUDED_sap_ALTOVA_xs_ALTOVA_CnormalizedString
