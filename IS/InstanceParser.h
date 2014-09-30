/*
 * @author Naveen Neelakandan 
 *
 * This class serves to parse a data set text file into memory. Specifically,
 * it parses the entire file into a vector of vectors where the outer vector
 * represents each line in the file and the inner vector represents each comma-
 * separated column. This class will be utilised as a re-usuable component 
 * in the implementation of the all the learning algorithms. 
 */

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#define CLASS_INDEX 21

double string_to_double(std::string& string_to_convert){
	double value;
	try{
    	value = boost::lexical_cast<double>(string_to_convert);
	}
	catch (boost::bad_lexical_cast const&){
		std::cerr << "Unable to convert string: "<< string_to_convert
			<<". Assuming 0"<<std::endl;
    	value = 0;
	}
	return value;
}

class InstanceParser{
private:
	std::ifstream m_input_file;
public:
	InstanceParser();
	/*
 	 * Parses a given text file into a vector of vectors. Client code is
 	 * responsible for de-allocating the memory of the returned data structure.
     */
	std::vector<std::vector<double>* >* parse(std::string path_to_file);
	/*
 	 * Parses a given text file into a vector of vectors. Additionally, converts
 	 * the entire data set into a gaussian distribution with the specfied mean
 	 * and standard deviation. Client code is responsible for de-allocating the
 	 * memory of the returned data structure.
     */
	std::vector<std::vector<double>* >* parse(std::string& pathToFile, 
		std::vector<double>* mean, std::vector<double>* standard_dev);
	/*
	 * A convenience method that deletes a vector of vectors allocated on the heap.
	 */
	static void delete_vectors(std::vector<std::vector<double>* >* to_delete);
};

InstanceParser::InstanceParser(){}

std::vector<std::vector<double>* >* InstanceParser::parse(std::string path_to_file){
		return parse(path_to_file, NULL, NULL);
}

std::vector<std::vector<double>* >* InstanceParser::parse(std::string& pathToFile,
 std::vector<double>* mean, std::vector<double>* standard_dev){
	m_input_file.open(pathToFile.c_str());
	std::string line;
	// Allocate 2-dimensional vector to return
	std::vector<std::vector<double>* >* instances = new std::vector<std::vector<double>* >();
	bool calculate_z = false;
	if (m_input_file.is_open()){
    	while (getline (m_input_file, line)){
    		std::vector<std::string> instance_string;
    		// Split line into a vector of strings
      		boost::split(instance_string, line, boost::is_any_of(","));
      		// If mean, standard deviation is given and the dimensions of the 
      		// text input and the given mean, standard deviation match, then
      		// convert the data into a Gaussian distribution
      		if (!calculate_z 
      			&& mean != NULL 
      			&& standard_dev != NULL 
      			&& instance_string.size() == mean->size()+1 
      			&& instance_string.size() == standard_dev->size()+1){
      		calculate_z = true;}
      		// Allocate inner vector
     		std::vector<double>* instance = new std::vector<double>();
      		for(int i=0; i<instance_string.size(); ++i){
      			// Convert the string values to double
      			double value = string_to_double(instance_string[i]);
      			if (calculate_z && i!=instance_string.size()-1){
      				value = (value-mean->at(i))/standard_dev->at(i);
      			}
      			instance->push_back(value);
      		}
      		instances->push_back(instance);
      	}
    }
	m_input_file.close();
	return instances;
}

void InstanceParser::delete_vectors(std::vector<std::vector<double>* >* to_delete){
	if(to_delete == NULL){
		return;
	}
	for(int i=0; i<to_delete->size(); ++i){
		delete to_delete->at(i);
	}
	delete to_delete;
}