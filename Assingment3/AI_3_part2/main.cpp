#include <iostream>
#include <String>
#include "Node.h"
#include "math.h"
using namespace std;

int main(){

	//input nodes
	Node in1;
	Node in2;
	
	//define xor function
	int input[4][4] = {{1,1},{0,1},{1,0},{0,0}};
	float output[] = {0.00,1.00,1.00,0.00};

	//neural network nodes
	Node n1;
	Node n2;
	Node n3;

	//define connection between nodes
	n1.add_edge(&in1,0.5);
	n1.add_edge(&in2,0.4);
	n1.weights[0] = 0.8;

	n2.add_edge(&in1,0.9);
	n2.add_edge(&in2,1.0);
	n2.weights[0] = -0.1;

	n3.add_edge(&n1,-1.2);
	n3.add_edge(&n2,1.1);
	n3.weights[0] = 0.3;
	
	Node* network[3] = {&n1, &n2, &n3};	//cant use node directly as in network[3]={n1,n2} when creating an array that way, the array contains copies of the nodes and so the pointers do not point to the original
										//notice an array is a continuous block of memory but n1 and n2 may not be continuous in memory, thus the array contains copies of n1,n2 and 

	bool flag=true;
	int count=0;

	while (flag==true){
		float error=0.0;

		//one epoch of training
		for(int j=0;j<4;j++){

			float outerror=0;

			//set input
			in1.output=input[j][0];
			in2.output=input[j][1];

			//forward pass of algorithm
			for(int k=0;k<3;k++)
				network[k]->compute();

			//calculate output node's error
			outerror = float(output[j]-n3.output);
			
			//string represntation of output node's
			string outputstr;
			if(n3.output>=0.8)
				outputstr = "1";
			else{
				if(n3.output <= 0.2)
					outputstr = "0";
				else
					outputstr = "Undetermined";
			}

			//print training information
			cout<<"Inputs: "<<in1.output<<" "<<in2.output<<"   "<<"Expected Output: "<<output[j]<<" Actual Output: "<<outputstr<<endl;

			//calculate mean square error
			error += pow(outerror,2);

			//calculate error gradient for each node
			n3.gradientcal(outerror);
			n2.gradientcal(&n3,n3.weights[2]);
			n1.gradientcal(&n3,n3.weights[1]);

			//adjust weights
			n3.weightcorrection();
			n2.weightcorrection();
			n1.weightcorrection();


		}

		//print error infromation
		error = error/4;
		cout<<"Mean Square Error: "<<error<<endl;
		cout<<endl;

		count++;

		//ask user if trianing is to be continued
		if (count>100){
			cout<<"Weights.."<<endl;
			cout<<"Input to inner nodes: "<<n1.weights[1]<<" "<<n1.weights[2]<<" "<<n2.weights[1]<<" "<<n2.weights[2]<<" "<<endl;
			cout<<"Inner nodes to outer node: "<<" "<<n3.weights[1]<<" "<<n3.weights[2]<<endl;
			cout<<"Biasing Inputs: "<<n1.weights[0]<<" "<<n2.weights[0]<<" "<<n3.weights[0]<<endl;
			count=0;
			cout<<"Continue training?"<<endl;
			char f;
			cin>>f;
			if (f=='n'){
				flag = false;

				}
		}
	}

system("pause");
return 0;}