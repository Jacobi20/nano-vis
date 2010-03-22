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

#pragma once



template <typename T, typename SpatPr, typename EqPr>
class octree {
	public:
		//	
		//	Constructor :
		//
		octree	( void ) {
			root = NULL;
		}
		
		//	
		//	Destructor :
		//
		~octree	( void ) {
		}

		//
		//	insert
		//	inserts element to octree
		//	
		void insert	( const T &elem ) {
			insert_recursive(elem, &root);
		}
		
	protected:
		struct	octree_node {
			octree_node(void) {
				memset(subnodes, 0, sizeof(subnodes));
			}
			T			data;
			octree_node	*subnodes[8];
		};
		
	protected:
		void insert_recursive	( const T &elem, struct octree_node **node ) {
			if (!*node) {
				*node = new octree_node;
				(*node)->data = elem;
				return;	
			}

			//	TODO DHG : add option "replace equal elements" :
			if (EqPr((*node)->data, elem)) {
				(*node)->data = elem;
				return;
			}
			
			int r = 0x7 & SpatPr((*node)->data, elem);
			
			insert_recursive(elem, &(*node)->subnodes[r]);
		}
	
	
	protected:
		octree_node *root;
	
	};