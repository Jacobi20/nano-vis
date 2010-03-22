/*
	The MIT License

	Copyright (c) 2010

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

#include "library.h"
#include "attr_list.h"


/*-----------------------------------------------------------------------------
	Attribute list :
-----------------------------------------------------------------------------*/

EAttributeList::EAttributeList(void) 
{
}

EAttributeList::~EAttributeList(void) 
{
}

const char *EAttributeList::GetValue(const char *key, const char *def_value) const
{
	const pair_s *pp = FindPair(key);
	if (!pp) {
		return def_value;
	} else {
		return pp->value.c_str();
	}
}


void EAttributeList::SetValue(const char *key, const char *value)
{
	pair_s *pp = const_cast<pair_s*>(FindPair(key));
	if (!pp) {
		pair_s p;
		p.key   = key;
		p.value	= value;
		pairs.push_back(p);
	} else {
		pp->value = value;
	}
}


const EAttributeList::pair_s *EAttributeList::FindPair(const char *key) const
{
	for (uint i=0; i<pairs.size(); i++) {
		if (strcmp(key, pairs[i].key.c_str())==0) {
			return &pairs[i];
		}
	}
	return NULL;
}


void EAttributeList::AppendParams(const EAttributeList *other)
{
	for (uint i=0; i<other->pairs.size(); i++) {
		this->SetValue( other->pairs[i].key.c_str(), other->pairs[i].value.c_str() );
	}
}


uint EAttributeList::GetParamNum( void ) const
{
	return pairs.size();
}


const char *EAttributeList::GetValueByID( uint index ) const
{
	if (index > GetParamNum()) {
		return NULL;
	}
	
	return pairs[index].value.c_str();
}


const char *EAttributeList::GetKeyByID( uint index ) const
{
	if (index > GetParamNum()) {
		return NULL;
	}
	
	return pairs[index].key.c_str();
}