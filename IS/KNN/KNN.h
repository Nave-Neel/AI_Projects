/*
 * @author Naveen Neelakandan 
 *
 * This class serves to implement the KNN (K- Nearest Neighbors) algorithm.
 */

#include <string>
#include <vector>
#include <map>
#include <math.h>
#include "../InstanceParser.h"

/*
 * Given an array and the value of the element with the max value in the array
 * this function serves to return the count of the most occurring element in 
 * the array.
 */
int get_most_occuring(std::vector<int>& array, int max_val_in_array){
	if(array.size()==0){
		std::cerr<<"Empty array passed"<<std::endl;
		return 0;
	}
	std::vector<int> count_array(max_val_in_array+1);
	for(int i=0; i<count_array.size(); ++i){
		count_array[i] = 0;
	}
	for(int i=0; i<array.size(); ++i){
		count_array[array[i]] = count_array[array[i]] + 1;
	}
	int most_occuring = 0;
	int most_occuring_index = 0;
	for(int i=0; i<count_array.size(); ++i){
		if(count_array[i] > most_occuring){
			most_occuring = count_array[i];
			most_occuring_index = i;
		}
	}
	return most_occuring_index;
}

class KNN{
private:
	std::vector<std::vector<double>* >* m_training_set;
	std::vector<std::vector<double>* >* m_test_set;
	/*
	 * Returns the euclidean distance between two points (instances). A boolean 
	 * array can also be passed in to specify which features to leave out of the
	 * calculation.
	 */
	double get_euclidean_distance(std::vector<double>* instance, 
		std::vector<double>* other_instance, std::vector<bool>*);
	/*
	 * Carries out the actual KNN algorithm on an input set using the specified K
	 * value. A boolean array can also be passed in to specify which features to 
	 * leave out of the calculation of the distance.
	 */
	std::vector<int> perform_KNN(std::vector<std::vector<double>* >* input_set,
	 int k, std::vector<bool>*);
	/*
	 * Given the original correct classifications and the output from the KNN, 
	 * returns the accuracy of the KNN output.
	 */
	double calculate_accuracy(std::vector<std::vector<double> * >*  input_set,
	 std::vector<int>&);
	/*
	 * For a single point, returns its k nearest neigbours.
	 */ 
	std::vector<int> get_K_nearest_neighbors(std::vector<double>* instance, 
		int k, std::vector<bool>*);
	/*
	 * Given a vector of the training data indices, returns their classifications.
	 */
	std::vector<int> get_classifications(std::vector<int>);
public:
	KNN(std::string path_to_distributions, std::string path_to_data_sets);
	/* 
	 * Performs KNN on training set using leave-one out testing and with different k
	 * values to find the best value of k for that particular data set.
	 */
	int get_best_k();
	/*
	 * Performs the actual run of the KNN algorithm on the test set. Returns
	 * the accuracy of the run.
	 */
	double classify(int k);
	/*
	 * Returns a list of the REMOVED features
	 */ 
	std::vector<bool> feature_select(int k);
};

KNN::KNN(std::string path_to_distributions, std::string path_to_data_sets){
		// Create an InstanceParser on stack.
		InstanceParser instance_parser;
		std::string distribution_file_name = path_to_distributions + 
			"/total_distribution.txt";
		// Initializes the mean and standard dev array.
		std::vector<std::vector<double>* >* distribution = 
			instance_parser.parse(distribution_file_name);
		// Initializes the training and test sets to z values.
		std::string training_file_name = path_to_data_sets+"/training_set.txt";
		m_training_set = instance_parser.parse(training_file_name, 
			distribution->at(0), distribution->at(1));
		std::string test_file_name = path_to_data_sets+"/test_set.txt";
		m_test_set = instance_parser.parse(test_file_name, 
			distribution->at(0), distribution->at(1));
		// We don't need the distribution anymore after calculating the z values. 
		// Get rid of it.
		InstanceParser::delete_vectors(distribution);
}

double KNN::calculate_accuracy(
		std::vector<std::vector<double> * >* input_set, 
		std::vector<int>& classifications){
	if(input_set->size() != classifications.size()){
		std::cerr<<"The number of instances do not match"<<std::endl;
		return -1;
	}
	int count = input_set->size();
	int correct_count = 0;
	// Go through the input and compare the classifications
	for(int i=0; i<input_set->size(); ++i){
		if(input_set->at(i)->at(CLASS_INDEX) == classifications[i]){
			correct_count += 1;
		}
	}
	return ((double)correct_count/count) * 100;
}

int KNN::get_best_k(){
	int k_values[] = {1, 3, 5, 15, 25, 51};
	int best_k_index = 0;
	double best_accuracy = 0;
	// Perform KNN with different values of K. Update the best value of K
	for (int i=0; i<sizeof(k_values)/sizeof(int); ++i){
		std::vector<int> classifications = perform_KNN(m_training_set, 
			k_values[i], NULL);
		double cur_accuracy = calculate_accuracy(m_training_set, classifications);
		std::cout<<" K:"<<k_values[i]<<" Accuracy:"<<cur_accuracy<<std::endl;
		if(cur_accuracy > best_accuracy){
			best_accuracy = cur_accuracy;
			best_k_index = i;
		}
	}
	return k_values[best_k_index];
}

double KNN::classify(int k){
	std::vector<int> classifications = perform_KNN(m_test_set, k, NULL);
	return calculate_accuracy(m_test_set, classifications);
}

std::vector<int> KNN::get_classifications(std::vector<int> input){
	std::vector<int> classes;
	for(int i=0; i<input.size(); ++i){
		classes.push_back(m_training_set->at(input[i])->at(CLASS_INDEX));
	}
	return classes;
}

std::vector<int> KNN::perform_KNN(
		std::vector<std::vector<double>* >* input_set, 
		int k, 
		std::vector<bool>* features_to_exclude){
	std::vector<int> classifications(input_set->size());
	for(int i=0; i<input_set->size(); ++i){
		std::vector<int> k_neighbors = get_K_nearest_neighbors(input_set->at(i), 
			k, features_to_exclude);
		// Get the classifications for each of the neighbors
		std::vector<int> k_classifications = get_classifications(k_neighbors);
		// Get the majority class
		int majority_class = get_most_occuring(k_classifications, 1);
		classifications[i] = majority_class;
	}
	return classifications;
}

std::vector<int> KNN::get_K_nearest_neighbors(
		std::vector<double>* instance, 
		int k, 
		std::vector<bool>* features_to_exclude){
	// Create a map of k indices and the nearest distance
	std::map<int, double> neighbors;
	// Go through training set and find the k nearest beigbors
	std::map<int, double>::iterator iterator;
	for (int i=0; i<m_training_set->size(); ++i){
		double distance = get_euclidean_distance(instance, 
			m_training_set->at(i), features_to_exclude);
		// Looking at same instance?
		if (distance == 0){continue;}
		if(neighbors.size() < k){
			neighbors.insert(std::pair<char,int>(i, distance));
		}
		else{
			// Check if the new candidate neighbor is nearer than the already found
			double max_distance = distance;
			int max_index = i;
			for(iterator = neighbors.begin(); 
				iterator != neighbors.end(); 
				iterator++) {
    			if(iterator->second > max_distance){
    				max_index = iterator->first;
    				max_distance = iterator->second;
    			}
			}
			if (max_index == i){continue;}
			else{
				neighbors.erase(max_index);
				neighbors.insert(std::pair<int,int>(i, distance));
			}
		}
	}
	std::vector<int> nearest_neighbors(k);
	int i = 0;
	for(iterator = neighbors.begin(); iterator != neighbors.end(); iterator++) {
    	nearest_neighbors[i] = iterator->first;
    	i++;
	}
	return nearest_neighbors;
}

double KNN::get_euclidean_distance(
		std::vector<double>* instance, 
		std::vector<double>* other_instance, 
		std::vector<bool>* features_to_exclude){
	if(instance->size() != other_instance->size()){
		std::cerr<<"Cannot compare points of different dimensions : " 
		<< instance->size() << "/t" << other_instance->size()<<std::endl;
		return -1;
	}
	double distance = 0;
	for(int i=0; i<instance->size()-1; ++i){
		// If boolean is set to true for this feature then exclude it from calculation
		if(features_to_exclude != NULL && features_to_exclude->at(i)){
			continue;
		}
		double difference = instance->at(i) - other_instance->at(i);
		distance += difference*difference;
	}
	return sqrt(distance);
}

std::vector<bool> KNN::feature_select(int k){
	int dimensions_size = m_training_set->at(0)->size();
	std::vector<bool> removed_features(dimensions_size);
	double accuracy_difference = 0;
	double last_accuracy = 0;
	while(true){
		double cur_best_accuracy = 0;
		int cur_best_feature_to_remove = 0;
		for(int i=0; i<removed_features.size(); ++i){
			// Check if feature already removed
			if(removed_features[i]){
				continue;
			}
			removed_features[i] = true;
			std::vector<int> classifications = perform_KNN(
				m_test_set, 
				k, 
				&removed_features);
			// Get accuracy upon leaving out this feature.
			double cur_accuracy = calculate_accuracy(m_test_set, classifications);
			if(cur_accuracy > cur_best_accuracy){
				cur_best_accuracy = cur_accuracy;
				cur_best_feature_to_remove = i;
			}
			removed_features[i] = false;
		}
		std::cout.precision(15);
		printf("%s\t%.5f\n", "Current Best Accuracy:", cur_best_accuracy);
		accuracy_difference = cur_best_accuracy - last_accuracy;
		last_accuracy = cur_best_accuracy;
		if(accuracy_difference >= 0){
			std::cout<<"Removed: "<<cur_best_feature_to_remove<<std::endl;
			removed_features[cur_best_feature_to_remove] = true;
		}
		else{
			break;
		}
	}
	return removed_features;
}