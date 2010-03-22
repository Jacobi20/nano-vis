/*
	The MIT License

	Copyright (c) 2010 IFMO/GameDev Studio

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
// tixml_helper.h - some helpful functions for TinyXML

#pragma once


inline void XML_LoadAttribute(const TiXmlElement *xml_element, const char *attr_name, float *dst)
{
	if (xml_element && xml_element->Attribute(attr_name)) {
		*dst = (float)atof( xml_element->Attribute(attr_name) );
	}
}


inline void XML_LoadAttribute(const TiXmlElement *xml_element, const char *attr_name, int *dst)
{
	if (xml_element && xml_element->Attribute(attr_name)) {
		*dst = atoi( xml_element->Attribute(attr_name) );
	}
}


inline void XML_LoadAttribute(const TiXmlElement *xml_element, const char *attr_name, uint *dst)
{
	if (xml_element && xml_element->Attribute(attr_name)) {
		*dst = atoi( xml_element->Attribute(attr_name) );
	}
}


inline void XML_LoadAttribute(const TiXmlElement *xml_element, const char *attr_name, string *dst)
{
	if (xml_element && xml_element->Attribute(attr_name)) {
		*dst = xml_element->Attribute(attr_name);
	}
}

