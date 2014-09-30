#include "KNN.h"
#include <iostream>

using namespace std;

void print_arrayw(std::vector<int> array){
	for(int i=0; i<array.size(); ++i){
		cout<<array[i]<<endl;
	}
}

int main(){
	KNN kNN("/home/naveen/Dropbox/IS/data", "/home/naveen/Dropbox/IS/data/set1");
	/*
	int testArray[] = {1,1,1,3};
	std::vector<double> test_vector(testArray, testArray + sizeof(testArray) / sizeof(testArray[0]) );
	int testArray2[] = {1,7,1,1};
	std::vector<double> test_vector2(testArray2, testArray2 + sizeof(testArray2) / sizeof(testArray2[0]) );
	double testArray3[] = {17,34,0,1};
	std::vector<double> test_vector3(testArray3, testArray3 + sizeof(testArray3) / sizeof(testArray3[0]) );
	std::vector<std::vector<double>* > super_vector;
	super_vector.push_back(&test_vector2);
	super_vector.push_back(&test_vector3);
	std::vector<int> classifications =  kNN.perform_KNN(&super_vector, 2);
	print_arrayw(classifications);*/
	//cout<<"Best K: "<<kNN.get_best_k()<<endl;
	kNN.feature_select(1);
}