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
// grammar_char.hpp - extension to grammar.hpp for char type


#pragma once


typedef EGRuleTermT<char> EGRuleTermChar;
typedef bool (*acceptFuncChar_t) ( char );


class EGRuleTermCharFunc : public EGRuleAbstractT<char>	{
	public:
						EGRuleTermCharFunc	( acceptFuncChar_t af );
		virtual bool	Accept				( EGContextT<char> *ctxt, bool allow_cb, EGRuleT<char> *space );
	protected:
		acceptFuncChar_t	func;
	};


class EGRuleTermCharString : public EGRuleAbstractT<char> {
	public:
						EGRuleTermCharString	( const char *t );
		virtual bool	Accept					( EGContextT<char> *ctxt, bool allow_cb, EGRuleT<char> *space );
	protected:
		string	term;
	};
	
	
inline EGRuleT<char> TRM(char ch) { 
	return EGRuleT<char>( shared_ptr<EGRuleAbstractT<char> >( new EGRuleTermT<char>(ch) )); 
}	

inline EGRuleT<char> TRM(acceptFuncChar_t af) { 
	return EGRuleT<char>( shared_ptr<EGRuleAbstractT<char> >( new EGRuleTermCharFunc(af) )); 
}	

inline EGRuleT<char> TRM(const char *str) { 
	return EGRuleT<char>( shared_ptr<EGRuleAbstractT<char> >( new EGRuleTermCharString(str) )); 
}	



