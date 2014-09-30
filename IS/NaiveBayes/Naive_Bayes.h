/*
 * @author Naveen Neelakandan 
 *
 * This class serves to implement the Naive Bayes algorithm.
 */

#include <string>
#include <vector>
#include <map>
#include <math.h>
#include "../InstanceParser.h"

#define MEAN_INDEX 0
#define STNDRD_DEV_INDEX 1

/*
 * Calculates the probability density of some value in a Gaussian distribution given
 * the distribution's parameters.
 */
double calculate_gaussian_probability(double mean, double dev, double value){
	// We cannot be dividing by zero!
	if(dev == 0){return 0;} 
	double exponent = -0.5*(((value - mean) * (value - mean)) / (dev*dev));
	// Since we are dealing with very small numbers, scale up by 
	// multiplying by a power of 10
	double numerator = pow(10, 20)*exp (exponent);
	double denominator = sqrt(2*M_PI*dev*dev);
	return numerator/denominator;
}

double calculate_bayes_rule(double prob_x_given_a, double prob_a, double prob_x_given_c, double prob_c){
	double denominator = prob_x_given_a*prob_a + prob_x_given_c*prob_c;
	double numerator = prob_x_given_a*prob_a;
	return numerator/denominator;
}

class Naive_Bayes{
private:
	/*
	 * Stores information for the distribution encompassing all the positive examples.
	 */
	std::vector<std::vector<double> >* m_positive_distribution;
	/* 
	 * Stores information for the distribution encompassing all the negative examples.
	 */
	std::vector<std::vector<double> >* m_negative_distribution;
	std::vector<std::vector<double> >* m_total_distribution;
	std::vector<std::vector<double>* >* m_test_set;
	/* 
	 * Returns the probability that a feature takes up a certain value in a
	 * specified distribution. 
	 */
	double get_probability(std::vector<std::vector<double> >* distribution,
		int feature, double value);
	std::vector<int> perform_naive_bayes();
	/* 
	 * Calculates the accuracy against the test set.
	 */
	double calculate_accuracy(std::vector<int>&);
public:
	Naive_Bayes(std::string path_to_data_sets, int training_set_cap);
	/* 
	 * Performs the Naive bayes on the test set. Returns the accuracy of the
	 * classification.
	 */
	double classify();
};

Naive_Bayes::Naive_Bayes(std::string path_to_data_sets, int training_set_cap){
	InstanceParser instance_parser;
	std::string training_file_name = path_to_data_sets+"/training_set.txt";
	std::vector<std::vector<double>* >* m_training_set = instance_parser.parse(training_file_name);
	std::string test_file_name = path_to_data_sets+"/test_set.txt";
	m_test_set = instance_parser.parse(test_file_name);
	int number_of_features = m_training_set->at(0)->size() - 1;
	m_positive_distribution = new std::vector<std::vector<double> >(number_of_features);
	m_negative_distribution = new std::vector<std::vector<double> >(number_of_features);
	m_total_distribution = new std::vector<std::vector<double> >(number_of_features);
	for(int i=0; i<number_of_features; ++i){
		for(int j=0; j<2; ++j){
			m_positive_distribution->at(i).push_back(0);
			m_negative_distribution->at(i).push_back(0);
			m_total_distribution->at(i).push_back(0);
		}
	}

	int negative_count = 0;
	int positive_count = 0;
	
	// Get means
	for(int i=0; i<training_set_cap; ++i){
		if(i == m_training_set->size()){
			break;
		}
		std::vector<double>* training_instance = m_training_set->at(i);
		if(training_instance->at(CLASS_INDEX) == 0){
			negative_count += 1;
			for (int f=0; f<number_of_features; ++f){
				m_total_distribution->at(f)[MEAN_INDEX] += training_instance->at(f);
				m_negative_distribution->at(f)[MEAN_INDEX] += training_instance->at(f);
			}
		}
		else{
			positive_count += 1;
			for (int f=0; f<number_of_features; ++f){
				m_total_distribution->at(f)[MEAN_INDEX] += training_instance->at(f);
				m_positive_distribution->at(f)[MEAN_INDEX] += training_instance->at(f);
			}
		}
	}
	for (int f=0; f<number_of_features; ++f){
		m_total_distribution->at(f)[MEAN_INDEX] = m_total_distribution->at(f)[MEAN_INDEX]/m_training_set->size();
		m_negative_distribution->at(f)[MEAN_INDEX] = m_negative_distribution->at(f)[MEAN_INDEX]/negative_count;
		m_positive_distribution->at(f)[MEAN_INDEX] = m_positive_distribution->at(f)[MEAN_INDEX]/positive_count;
	}

	// Get Standard devs
	for(int i=0; i<training_set_cap; ++i){
		if(i == m_training_set->size()){
			break;
		}
		std::vector<double>* training_instance = m_training_set->at(i);
		if(training_instance->at(CLASS_INDEX) == 0){
			for (int f=0; f<number_of_features; ++f){
				double dev = training_instance->at(f) - m_total_distribution->at(f)[MEAN_INDEX];
				double variance = dev*dev;
				m_total_distribution->at(f)[STNDRD_DEV_INDEX] += variance;
				dev = training_instance->at(f) - m_negative_distribution->at(f)[MEAN_INDEX];
				variance = dev*dev;
				m_negative_distribution->at(f)[STNDRD_DEV_INDEX] += variance;
			}
		}
		else{
			for (int f=0; f<number_of_features; ++f){
				double dev = training_instance->at(f) - m_total_distribution->at(f)[MEAN_INDEX];
				double variance = dev*dev;
				m_total_distribution->at(f)[STNDRD_DEV_INDEX] += variance;
				dev = training_instance->at(f) - m_positive_distribution->at(f)[MEAN_INDEX];
				variance = dev*dev;
				m_positive_distribution->at(f)[STNDRD_DEV_INDEX] += variance;
			}
		}
	}
	for (int f=0; f<number_of_features; ++f){
		m_total_distribution->at(f)[STNDRD_DEV_INDEX] = sqrt(m_total_distribution->at(f)[STNDRD_DEV_INDEX]/m_training_set->size());
		m_negative_distribution->at(f)[STNDRD_DEV_INDEX] = sqrt(m_negative_distribution->at(f)[STNDRD_DEV_INDEX]/negative_count);
		m_positive_distribution->at(f)[STNDRD_DEV_INDEX] = sqrt(m_positive_distribution->at(f)[STNDRD_DEV_INDEX]/positive_count);
	}
	InstanceParser::delete_vectors(m_training_set);
}

double Naive_Bayes::classify(){
	std::vector<int> classifications(m_test_set->size());
	for(int i=0; i<m_test_set->size(); ++i){
		std::vector<double> * test_instance = m_test_set->at(i);
		double probability_given_positive = 0.0;
		double probability_given_negative = 0.0;
		for(int f=0; f<test_instance->size()-1; ++f){
			double prob = calculate_gaussian_probability(m_positive_distribution->at(f)[MEAN_INDEX], m_positive_distribution->at(f)[STNDRD_DEV_INDEX], test_instance->at(f));
			if(prob!=0){
				probability_given_positive += log(prob);
			}
		}
		for(int f=0; f<test_instance->size()-1; ++f){
			double prob = calculate_gaussian_probability(m_negative_distribution->at(f)[MEAN_INDEX], m_negative_distribution->at(f)[STNDRD_DEV_INDEX], test_instance->at(f));
			if(prob!=0){
				probability_given_negative += log(prob);
			}
		}
		double probability_instance_is_positive = calculate_bayes_rule(probability_given_positive, 0.5, probability_given_negative, 0.5);
		double probability_instance_is_negative = calculate_bayes_rule(probability_given_negative, 0.5, probability_given_positive, 0.5);
		if (probability_instance_is_positive > probability_instance_is_negative){
			classifications[i] = 1;
		}
		else{
			classifications[i] = 0;	
		}
	}
	return calculate_accuracy(classifications);	
}

double Naive_Bayes::calculate_accuracy(std::vector<int>& classifications){
	if(m_test_set->size() != classifications.size()){
		std::cerr<<"The number of instances do not match"<<std::endl;
		return -1;
	}
	int count = m_test_set->size();
	int correct_count = 0;
	for(int i=0; i<m_test_set->size(); ++i){
		if(m_test_set->at(i)->at(CLASS_INDEX) == classifications[i]){
			correct_count += 1;
		}
	}
	return ((double)correct_count/count)*100;
}