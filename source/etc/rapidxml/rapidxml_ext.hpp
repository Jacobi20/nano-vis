/*
	The MIT License

	Copyright (c) 2008 IFMO/GameDev Studio

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#ifndef RAPIDXML_EXT_INCLUDED
#define RAPIDXML_EXT_INCLUDED


#include "rapidxml.hpp"

namespace rapidxml
{

	//
	//	xml_add_attr()
	//	adds pair to xml_node. xml_node must be settled on xml_document.
	//
	template <typename Ch>
	void	xml_add_attr( xml_node<Ch> *xnode, const Ch *key, const Ch *value ) 
	{
		xml_document<Ch>	*xdoc = xnode->document();
		
		Ch *xkey	= xdoc->allocate_string( key );
		Ch *xvalue	= xdoc->allocate_string( value );
		
		xml_attribute<Ch>	*xattr = xdoc->allocate_attribute( xkey, xvalue );
		
		xnode->append_attribute( xattr );
	}


	//
	//	xml_get_attr()
	//	return value of specified attribute, if last one exist,
	//	or def_value if it doesn't.
	//
	template <typename Ch>
	const Ch *xml_get_attr( const xml_node<Ch> *xnode, const Ch *key, const Ch *def_value )
	{
		if (!xnode) {
			return def_value;
		}
	
		xml_attribute<Ch>	*attr = xnode->first_attribute(key);
		
		if (!attr) {
			return def_value;
		}
		
		return attr->value();
	}


}

#endif
