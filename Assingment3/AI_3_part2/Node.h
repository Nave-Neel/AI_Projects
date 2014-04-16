// when using stl components , have to use namespace std as the components are defined in this namespace
/*This is the header file for the Node class.The node is an individual component of a neural network 
 *@author Naveen Neelakandan(nn149)
 */
#ifndef _NODE_H
#define _NODE_H
#include <vector>
using namespace std; 


#define BIAS -1
#define ALPHA 0.2

class Node{

public:
		vector<float>weights;
		vector<Node*>inputs;
		float output;
		float gradient;
		int size;

		Node();
		void compute();
		void gradientcal(float);
		void gradientcal(Node*,float);
		void weightcorrection();
		void add_edge(Node*,float);
		void add_edge(float,float);
};

#endif