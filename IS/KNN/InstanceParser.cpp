// Takes an input file, parses it, to return a vector of instances (themselves a vector)

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


double string_to_double(){
	double value;
	try{
    	value = boost::lexical_cast<double>(my_string);
	}
	catch (boost::bad_lexical_cast const&){
		cerr << "Unable to convert string. Assuming 0";
    	value = 0;
	}
	return value;
}

class InstanceParser{

private:
	std::ifstream input_file;

public:

	InstanceParser(){}

	// This code returns a n*n 2-dimensional vector (a vector of vectors) allocated on the heap. The calling code is responsible
	// for deleting the sub-vectors and the main vector
	std::vector<std::vector<double> >* parse(std::string pathToFile){
		input_file.open(pathToFile.c_str());
		std::string line;
		std::vector<std::vector<double>* >* instances = new std::vector<std::vector<double> >();
		if (input_file.is_open()){
    		while (getline (input_file, line)){
    			std::vector<std::string> instance_string;
      			boost::split(feature_string, line, boost::is_any_of(","));
      			std::vector<double>* instance = new std::vector<double>();
      			for(int i=0; i<instance_string.size(); ++i){
      				instance.push_back(toDouble(instance_string[i]));
      			}
      			instances->push_back(instance);
      		}
    	}
		input_file.close();
		return instances;
	}


};

int main(){
	InstanceParser ip;
	ip.parse("/home/naveen/Documents/IS/data/set1/test_set.txt");
	std::cout<<"seg fault forever"<<std::endl;
	return 0;
}