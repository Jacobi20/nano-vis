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

#include "..\library\library.h"


const uint NN_MAX_LAYERS	=	16;

class EPerceptron {
	friend class ENeuralNetwork;
	public:
						EPerceptron		( uint num_inputs );
						~EPerceptron	( void );
		float			Evaluate		( const vector<float> &input ) const;
	protected:
		vector<float>	weights;	  // last element is used for offset (or shift?)
	};

class ENetworkLayer {
	friend class ENeuralNetwork;
	public:
						ENetworkLayer	( uint num_slps, uint num_inputs );
						~ENetworkLayer	( void );
		void			Evaluate		( const vector<float> &input, vector<float> &output ) const;
						
	protected:
		uint	input_number;
		vector<EPerceptron*> slps;
	};


class ENeuralNetwork {
	public:
					ENeuralNetwork		( uint num_inputs, uint num_layers, const uint *num_perceptron);
					~ENeuralNetwork		( void );
		
		void		EvaluateNetwork		( const vector<float> &input, vector<float> &output ) const;
		float		TrainNetwork		( const vector<float> &input, const vector<float> &target_output, float train_norm );
		
	protected:
		vector<ENetworkLayer*>	layers;
	};