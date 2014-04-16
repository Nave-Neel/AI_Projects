/*This is the implementation file for the Node class.
 *@author Naveen Neelakandan(nn149)
 */

#include "Node.h"
#include <iostream>
#include <math.h>
using namespace std;

//constructor
Node::Node(){
	output=0;
	gradient=0;
	size=1;
	weights.push_back(0.5);
	inputs.push_back(0);
}

//method to add an edge in the neural network
void Node::add_edge(Node*p,float w){
	size++;
	weights.push_back(w);
	inputs.push_back(p);
}

//method to compute the ouput based on the input values
void Node::compute(){
	float total=weights[0]*(BIAS);
	for(int i=1;i<size;i++){
		total += weights[i]*(inputs[i]->output);
	}
	output = 1.0/(1.0+exp(-total));
}

//method to calculate the error gradient of the outer node
void Node::gradientcal(float err){
	gradient = output*(1.00-output)*err;
}

//method to calculate the error gradient of inner nodes
void Node::gradientcal(Node* edge,float w){
	gradient = output*(1.00-output)*(edge->gradient)*w;
}

//method to adjust weights based on the error gradient
void Node::weightcorrection(){
	weights[0] = weights[0] + ALPHA*BIAS*gradient;
	for(int i=1;i<size;i++){
		weights[i] = weights[i] + ALPHA*(inputs[i]->output)*gradient;
	}
}