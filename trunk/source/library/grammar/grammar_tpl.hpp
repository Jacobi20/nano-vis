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
// grammar_tpl.hpp - 


#pragma once

#include "..\shared_ptr\shared_ptr.hpp"
#include "..\library.h"

/*-----------------------------------------------------------------------------
	Grammar context - pointer to sumbol buffer
-----------------------------------------------------------------------------*/

template <class Type>
class EGContextT {
	public:
					EGContextT	( const Type *buf_ptr, uint size );
					~EGContextT	( void );
		void		Next		( void );
		const Type	&GetSymbol	( void ) const;
	protected:
		const Type *buf_ptr;
		uint		cursor;
		uint		size;
	};


template <class Type>
EGContextT<Type>::EGContextT(const Type *buf_ptr, uint size)
{
	this->buf_ptr	=	bfr_ptr;
	this->size		=	size;
	cursor			=	0;
}


template <class Type>
void EGContextT<Type>::Next(void)
{
	cursor++;
	if (cursor>size) {
		RAISE_EXCEPTION("Range check violation");
	}
}


template <class Type>
const Type  &EGContextT<Type>::GetSymbol(void)   const
{
	return buf_ptr[cursor];
}

/*-----------------------------------------------------------------------------
	Base grammar rule :
-----------------------------------------------------------------------------*/

template<class Type>
class EGRuleAbstractT {
	public:
						EGRuleAbstractT		( void ) {};
		virtual 		~EGRuleAbstractT	( void ) {};
		virtual	bool	Accept				( EGContextT<Type> *ctxt, bool allow_cb ) = 0;
	};



/*-----------------------------------------------------------------------------
	Rule holder :
-----------------------------------------------------------------------------*/	

template <class Type>	
class EGRuleT {
	public:
						EGRuleT		( void );
						EGRuleT		( shared_ptr<EGRuleAbstractT<Type> > rule );
						~EGRuleT	( void );
		EGRuleT<Type>	&operator = ( const EGRuleT &r );
		void			Set			( const EGRuleT &r );
		bool			Accept		( EGContextT<Type> *ctxt, bool allow_cbs );
		
		template <typename Action>
		EGRuleT	operator [] (Action action) ;
	protected:		
		shared_ptr<shared_ptr<EGRuleAbstractT<Type> > > rule_ptr;
	};

	
template <class Type>	
EGRuleT<Type>::EGRuleT(void)
{
	rule_ptr.reset( new shared_ptr<EGRuleAbstractT<Type> > );
}


template <class Type>	
EGRuleT<Type>::EGRuleT(shared_ptr<EGRuleAbstractT<Type> > rule)
{
	rule_ptr.reset( new shared_ptr<EGRuleAbstractT<Type> > );
	*rule_ptr = rule; 
}


template <class Type>	
EGRuleT<Type>::~EGRuleT(void)
{
}


template <class Type>	
EGRuleT<Type> &EGRuleT<Type>::operator =( const EGRuleT &r )
{
	*rule_ptr=*r.rule_ptr;
	return *this;
}


template <class Type>	
bool EGRuleT<Type>::Accept(EGContextT<Type> *ctxt, bool allow_cbs)
{
	EGContext<Type> start = *ctxt;
	ASSERT(rule_ptr->get());

	bool result = rule_ptr->get()->Accept(ctxt, allow_cbs); 

	if (!result) {
		*ctxt = start;
	}
	
	return result;
}


template <class Type>	
void EGRuleT<Type>::Set(const EGRuleT<Type> &r)
{
	rule_ptr = r.rule_ptr;
}
	

/*-----------------------------------------------------------------------------
	Term rule :	
-----------------------------------------------------------------------------*/	

template <class Type>
class EGRuleTermT : public EGRuleAbstractT<Type> {
	public:
						EGRuleTermT		( Type _term );
		virtual bool	Accept			( EGContextT<Type> *ctxt, bool allow_cbs );
	protected:
		Type term;
	};


template<class Type>
EGRuleTermT<Type>::EGRuleTermT(Type _term) {
	term = _term;
}


template<class Type>
bool EGRuleTermT<Type>::Accept(EGContextT<Type> *ctxt, bool allow_cbs)
{
	if (term == ctxt->GetSymbol()) {
		ctxt->Next();
		return true;
	}
	return false;
}


/*-----------------------------------------------------------------------------
	Sequence rule :
-----------------------------------------------------------------------------*/

template <class Type>
class EGRuleSequenceT : public EGRuleAbstractT<Type> {
	public:				
						EGRuleSequenceT	( EGRuleT<Type> ra, EGRuleT<Type> rb ) ;
		virtual bool	Accept			( EGContextT<Type> *ctxt, bool allow_cbs );
	protected:
		EGRuleT<Type> ra, rb;
	};
	

template<class Type>
EGRuleSequenceT<Type>::EGRuleSequenceT(EGRuleT<Type> ra, EGRuleT<Type> rb)
{
	this->ra.Set(ra);
	this->rb.Set(rb);
}
	
	
template<class Type>
bool EGRuleSequenceT<Type>::Accept(EGContextT<Type> *ctxt, bool allow_cbs)
{
	if (ra.Accept(ctxt, allow_cbs)) {
	} else {
		return false;
	}
	
	if (rb.Accept(ctxt, allow_cbs)) {
	} else {
		return false;
	}
	
	return true;
}

	
/*-----------------------------------------------------------------------------
	Alternative rule :
-----------------------------------------------------------------------------*/

template <class Type>
class EGRuleAlternativeT : public EGRuleAbstractT<Type> {
	public:
						EGRuleAlternativeT	( EGRuleT<Type> ra, EGRuleT<Type> rb ) ;
		virtual bool	Accept				( EGContextT<Type> *ctxt, bool allow_cbs );
	protected:
		EGRuleT<Type> ra, rb;
	};	


template<class Type>
EGRuleAlternativeT<Type>::EGRuleAlternativeT(EGRuleT<Type> ra, EGRuleT<Type> rb)
{
	this->ra.Set(ra);
	this->rb.Set(rb);
}


template<class Type>
bool EGRuleAlternativeT<Type>::Accept(EGContextT<Type> *ctxt, bool allow_cbs)
{
	EGContextT<Type> ctxt_a = *ctxt;
	EGContextT<Type> ctxt_b = *ctxt;
	
	uint result = 0;

	// Try both rules :
	if (ra.Accept(&ctxt_a, false)) {
		result |= 0x01;
	}
	if (rb.Accept(&ctxt_b, false)) {
		result |= 0x02;
	}
	
	// only first rule was passed :
	if (result==0x01) {	
		VERIFY( ra.Accept(ctxt, allow_cbs) );
		return true;
	}
	// only second rule was passed :
	if (result==0x02) {	
		VERIFY( rb.Accept(ctxt, allow_cbs) );
		return true;
	}
	
	// both rules were passed :
	if (result==0x03) { 
		if (ctxt_a.Cursor() > ctxt_b.Cursor()) {
			VERIFY( ra.Accept(ctxt, allow_cbs) );
		} else {
			VERIFY( rb.Accept(ctxt, allow_cbs) );
		}
		return true;
	}

	return false;
}

/*-----------------------------------------------------------------------------
	Repetition rule :
-----------------------------------------------------------------------------*/

enum grammarRepeat2_t {
		REPEAT2_ZERO_ONCE,
		REPEAT2_ZERO_MORE,
		REPEAT2_ONCE_MORE,
	};

template <class Type>
class EGRuleRepeatitionT : public EGRuleAbstractT<Type> {
	public:
						EGRuleRepeatitionT	( EGRuleT<Type> r, grammarRepeat2_t repeat );
		virtual bool	Accept				( EGContextT<Type> *ctxt, bool allow_cbs );
	protected:
		EGRuleT<Type>	rule;
		grammarRepeat2_t repeat;
	};


template <class Type>
EGRuleRepeatitionT<Type>::EGRuleRepeatitionT(EGRuleT<Type> r, grammarRepeat2_t repeat)
{
	this->repeat = repeat;
	this->rule.Set(r);
}


template <class Type>
bool EGRuleRepeatitionT<Type>::Accept(EGContextT<Type> *ctxt, bool allow_cbs)
{
	if (repeat==REPEAT_ZERO_ONCE) {
		if (rule.Accept(ctxt, allow_cbs)) {
			return true;
		} else {
			return true;
		}
	}

	if (repeat==REPEAT_ZERO_MORE) {
		while (rule.Accept(ctxt, allow_cbs)) {
			// empty loop
		};
		return true;
	}

	if (repeat==REPEAT_ONCE_MORE) {
		
		if (!rule.Accept(ctxt, allow_cbs)) {
			return false;
		}
		
		while (rule.Accept(ctxt, allow_cbs)) {
			// empty loop
		};
		return true;
	}

	return false;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/

inline EGRuleT<char> TRMCh(char ch) {
	return EGRuleT<char>( shared_ptr<EGRuleAbstractT<char> >( new EGRuleTermT<char>(ch) ) );
}
	
	
	
	
	
	
	
	
	
	
	
	