#ifndef _ALTOVA_INCLUDED_sap_ALTOVA_xs_ALTOVA_CpositiveInteger
#define _ALTOVA_INCLUDED_sap_ALTOVA_xs_ALTOVA_CpositiveInteger



namespace sap
{

namespace xs
{	

class CpositiveInteger : public TypeBase
{
public:
	sap_EXPORT CpositiveInteger(xercesc::DOMNode* const& init);
	sap_EXPORT CpositiveInteger(CpositiveInteger const& init);
	void operator=(CpositiveInteger const& other) { m_node = other.m_node; }
	static altova::meta::SimpleType StaticInfo() { return altova::meta::SimpleType(types + _altova_ti_xs_altova_CpositiveInteger); }
	void operator= (const unsigned __int64& value) 
	{
		altova::XmlFormatter* Formatter = static_cast<altova::XmlFormatter*>(altova::IntegerFormatter);
		XercesTreeOperations::SetValue(GetNode(), Formatter->Format(value));
	}	
		
	operator unsigned __int64()
	{
		return CastAs<unsigned __int64 >::Do(GetNode(), 0);
	}
};



} // namespace xs

}	// namespace sap

#endif // _ALTOVA_INCLUDED_sap_ALTOVA_xs_ALTOVA_CpositiveInteger
