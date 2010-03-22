//
//	STACK.H
//
//	descr : stack template definition 
//

#pragma once


template <class Type, int max> class TStack
	{
	private:
		Type	stack[max];	// array of elements
		int		sp;			// stack pointer
	public:
				TStack	( void ){ sp = 0;	}
				~TStack ( void ){ /**/ }

		bool	IsEmpty	( void ){ return (sp==0); }
		int		Count	( void ){ return sp; }
		
		void	Push( Type &val )
				{
					if (sp>=max) FATAL("TStack<>::Push() : Stack overflow");
					stack[sp] = val;
					sp++;
				}

		Type	Pop( void )
				{
					sp--;
					if (sp<0) FATAL("TStack<>::Pop() : Stack underflow");
					return stack[sp];
				}
	};

