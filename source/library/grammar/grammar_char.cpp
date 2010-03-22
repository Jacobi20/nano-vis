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
// grammar_char.cpp - 

#include "../library.h"
#include "grammar.hpp"
#include "grammar_char.hpp"


/*-----------------------------------------------------------------------------
	Char func term :
-----------------------------------------------------------------------------*/

EGRuleTermCharFunc::EGRuleTermCharFunc(acceptFuncChar_t af)
{
	func = af;
}


bool EGRuleTermCharFunc::Accept(EGContextT<char> *ctxt, bool allow_cb, EGRuleT<char> *space)
{
	if (func(ctxt->GetSymbol())) {
		ctxt->Next();
		return true;
	}
	return false;
}



/*-----------------------------------------------------------------------------
	String term func
-----------------------------------------------------------------------------*/

EGRuleTermCharString::EGRuleTermCharString(const char *t)
{
	term = t;
}


bool EGRuleTermCharString::Accept(EGContextT<char> *ctxt, bool allow_cb, EGRuleT<char> *space)
{
	for (uint i=0; i<term.length(); i++) {
		if (ctxt->GetSymbol()==term[i]) {
			ctxt->Next();
		} else {
			return false;
		}
	}
	return true;
}










