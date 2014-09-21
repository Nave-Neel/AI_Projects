// Takes an input file, parses it, to return a vector of instances (themselves a vector)

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

double string_to_double(std::string& string_to_convert){
	double value;
	try{
    	value = boost::lexical_cast<double>(string_to_convert);
	}
	catch (boost::bad_lexical_cast const&){
		std::cerr << "Unable to convert string: "<< string_to_convert<<". Assuming 0"<<std::endl;
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
	// for deleting the sub-vectors and the main vector by calling the static delete_vectors method
	std::vector<std::vector<double>* >* parse(std::string pathToFile){
		input_file.open(pathToFile.c_str());
		std::string line;
		std::vector<std::vector<double>* >* instances = new std::vector<std::vector<double>* >();
		if (input_file.is_open()){
    		while (getline (input_file, line)){
    			std::vector<std::string> instance_string;
      			boost::split(instance_string, line, boost::is_any_of(","));
      			std::vector<double>* instance = new std::vector<double>();
      			for(int i=0; i<instance_string.size(); ++i){
      				instance->push_back(string_to_double(instance_string[i]));
      			}
      			instances->push_back(instance);
      		}
    	}
		input_file.close();
		return instances;
	}

	static void delete_vectors(std::vector<std::vector<double>* >* to_delete){
		for(int i=0; i<to_delete->size(); ++i){
			delete to_delete->at(i);
		}
		delete to_delete;
	}
};

int main(){
	InstanceParser ip;
	std::vector<std::vector<double>* >* instances = ip.parse("/home/naveen/AI_Projects/IS/data/set1/test_set.txt");
	std::vector<double>* instance = instances->at(99);
	for(int i=0; i<instance->size(); ++i){
		std::cout<<i<<" "<<instance->at(i)<<std::endl;
	}
	
	InstanceParser::delete_vectors(instances);
	return 0;
}