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

#if 0

#include "neural_network.h"


/*-----------------------------------------------------------------------------
	Math functions :
-----------------------------------------------------------------------------*/
float NNRandf(void) {
	float r = (float)( rand()%10000 );
	return r / 10000.0f;
}


float NNSigmoid(float x) {
	return 1.0f / (1.0f + expf(-x));
}


float NNSigmoidDerivative(float x) {
	return ( NNSigmoid(x) * (1.0f - NNSigmoid(x)) );
}

float NNSigmoidDerivativeSigm(float sigm) {
	return ( sigm * (1.0f - sigm ) );
}


float NNSqr(float x)
{
	return x * x;
}


/*-----------------------------------------------------------------------------
	Perceptron stuff :
-----------------------------------------------------------------------------*/
EPerceptron::EPerceptron(uint num_inputs)
{
	weights.resize(num_inputs+1);
	for (uint i=0; i<weights.size(); i++) {
		weights[i] = 0.6f * NNRandf() - 0.3f;
	}
}

EPerceptron::~EPerceptron(void)
{
}

float EPerceptron::Evaluate(const vector<float> &input) const
{
	if ( input.size() != weights.size()-1 ) {
		RAISE_EXCEPTION(va("wrong number of inputs %d, should be %d", input.size(), weights.size()-1));
	}

	float net = 0;

	//	reactivate :	
	for (uint i=0; i<weights.size()-1; i++) {
		net += input[i] * weights[i];
	}

	//	compute offset :	
	net += weights[ weights.size()-1 ];
	
	return NNSigmoid(net);
}


/*-----------------------------------------------------------------------------
	Perceptron Layer stuff :
-----------------------------------------------------------------------------*/

ENetworkLayer::ENetworkLayer(uint num_perceptrons, uint num_inputs)
{
	input_number	=	num_inputs;

	slps.resize(num_perceptrons);
	
	for (uint i=0; i<num_perceptrons; i++) {
		EPerceptron *p = new EPerceptron(num_inputs);
		slps[i] = p;
	}
}

ENetworkLayer::~ENetworkLayer(void)
{
	for (uint i=0; i<slps.size(); i++) {
		delete slps[i];
	}
	slps.clear();
}


void ENetworkLayer::Evaluate(const vector<float> &input, vector<float> &output) const
{
	output.resize( slps.size() );
	
	for (uint i=0; i<slps.size(); i++) {
		output[i] = slps[i]->Evaluate(input);
	}
}


/*-----------------------------------------------------------------------------
	Neural Network :
-----------------------------------------------------------------------------*/

//
//	ENeuralNetwork::ENeuralNetwork
//
ENeuralNetwork::ENeuralNetwork(uint num_inputs, uint num_layers, const uint *num_perceptron)
{
	if (num_layers > NN_MAX_LAYERS) {
		RAISE_EXCEPTION(va("number of layers exceeded %d", NN_MAX_LAYERS));
	}

	// create layers
	for (uint i=0; i<num_layers; i++) {
		uint num_p, num_in;
		
		if (i==0) {
			num_p  = num_perceptron[0];
			num_in = num_inputs;
		} else {
			num_p  = num_perceptron[i];
			num_in = num_perceptron[i-1];
		}
		
		layers.push_back( new ENetworkLayer(num_p, num_in) );
	}
}


//
//	ENeuralNetwork::~ENeuralNetwork
//
ENeuralNetwork::~ENeuralNetwork(void)
{
	for (uint i=0; i<layers.size(); i++) {
		delete layers[i];
	}
	layers.clear();
}


//
//	ENeuralNetwork::EvaluateNetwork
//
void ENeuralNetwork::EvaluateNetwork(const vector<float> &input, vector<float> &output) const
{
	/*vector<float>	temp[2];	// temporary buffer for interlayer result
	temp[0] = input;
	for (uint i=0; i<layers.size(); i++) {
		layers[i]->Evaluate( temp[(i+0)&1], temp[(i+1)&1] );
	}
	output = temp[(i+0)&1];*/

	//
	//	Feed forward :
	//
	// temporary buffer for interlayer result
	// temp[0] contains input values
	// all following vectors contain result 
	// of forward feeding for each layers
	// temp[i+1] conatins output result of layer[i]
	//
	vector<float>	temp[NN_MAX_LAYERS+1];	

	// setup input values :
	temp[0] = input;

	for (uint i=0; i<layers.size(); i++) {
		layers[i]->Evaluate( temp[i], temp[i+1] );
	}

	//	setup output values :
	output = temp[i];
}


//
//	ENeuralNetwork::TrainNetwork
//
float ENeuralNetwork::TrainNetwork(const vector<float> &input, const vector<float> &target_output, float train_norm)
{
	vector<float>	actual_output;
	
	//
	//	Compute mean square deviation : 
	//
	EvaluateNetwork(input, actual_output);
	
	if (actual_output.size() != target_output.size()) {
		RAISE_EXCEPTION(va("wrong target output size %d, should be %d", target_output.size(), actual_output.size()));
	}

	float err = 0;	
	for (uint i=0; i<actual_output.size(); i++) {
		err += NNSqr( actual_output[i] - target_output[i] );
	}
	
	err *= 0.5;
	
	//
	//	Feed Forawrd :
	//
	// temporary buffer for interlayer result.
	// temp[0] contains input values.
	// all following vectors contain result 
	// of forward feeding for each layers.
	// temp[i+1] conatins output result of layer[i]
	//
	vector<float>	temp[NN_MAX_LAYERS+1];	
	uint layer_num = (uint)layers.size();

	// setup input values :
	temp[0] = input;

	for (uint i=0; i<layers.size(); i++) {
		layers[i]->Evaluate( temp[i], temp[i+1] );
	}
	
	
	//
	//	Back-propagation :
	//
	// Make buffer for delta :
	vector<float> delta[NN_MAX_LAYERS+1];
	for (uint i=0; i<NN_MAX_LAYERS+1; i++) {
		delta[i].resize( temp[i].size() );
	}

	// Compute error of output layer :
	// delta_j = (target_j - output_j) * Sigm`(output_j)
	for (uint j=0; j<delta[layer_num].size(); j++) {
		float oj = actual_output[j];
		float tj = target_output[j];
		delta[layer_num][j] = ( tj - oj ) * NNSigmoidDerivativeSigm( oj );
	}

	// Compute error of hidden layers :
	for (uint i = layer_num-1; i>0; i--) 
	{
		uint prev_layer = i + 1;
		uint crnt_layer = i;
		
		// for each SLP of current layer :
		for (uint j=0; j<layers[crnt_layer-1]->slps.size(); j++)
		{
			EPerceptron *slp_j = layers[crnt_layer-1]->slps[j];
			float prev_error = 0;
			
			// calc sum of errors of previous layer :
			for (uint k=0; k < layers[prev_layer-1]->slps.size(); k++) 
			{
				prev_error += delta[prev_layer][k] * layers[prev_layer-1]->slps[k]->weights[j];
			}
			
			delta[crnt_layer][j] = prev_error * NNSigmoidDerivativeSigm( temp[crnt_layer][j] );
		}
		
	}
	
	//
	//	Update weights :
	//	
	for (uint i = 1; i<layer_num+1; i++) 
	{
		for (uint j=0; j < layers[i-1]->slps.size(); j++)
		{
			EPerceptron *slp_j = layers[i-1]->slps[j];
			
			for (uint k=0; k<slp_j->weights.size()-1; k++ )
			{
				slp_j->weights[k] += train_norm * delta[i][j] * temp[i-1][k];
			}
			
			slp_j->weights[k] += train_norm * delta[i][j];
		}
	}
	
	return err;
}


#endif











