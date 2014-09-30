'''
@author Naveen Neelakandan
This script is intended to run on a large data set in order to find the mean and
standard deviation of the entire data set. Additionally, it also segregates the data
set into 10 different training and test folds located in different folders. 
'''

'''
Prints a list to the specified file 
'''
def print_to_file(example, output_file):
    for i in range(len(example)-1):
        output_file.write(str(example[i])+",")
    output_file.write(str(example[len(example)-1]))
    output_file.write("\n")

'''
Given mean and standard deviation, formats them and prints to specified file
'''
def print_distribution_to_file(file_name, mean, standard_dev=None, separator=","):
    distribution_file = open(file_name, "w")
    for i in range(len(mean)-1):
        distribution_file.write(str(mean[i])+separator)
    distribution_file.write(str(mean[len(mean)-1])+"\n")
    if standard_dev is not None:
        for i in range(len(standard_dev)-1):
            distribution_file.write(str(standard_dev[i])+separator)
        distribution_file.write(str(mean[len(standard_dev)-1])+"\n")
    distribution_file.close()
    
import sys
import math
import os

file_handle = None
input_file_path = "/home/naveen/AI_Projects/IS/data/jm1_full.txt"
try:
    file_handle = open(input_file_path, "r")
except:
    print "Error in opening file handle"
    sys.exit(0)

KThreshold = 500

negative_examples = []
positive_examples = []
total = []
max = []
count = 0
for i in range (21):
    total.append(0)
    max.append(0)

# Find total value, count
for line in file_handle:
    count += 1
    split = line.rstrip().split(',')
    for i in range (21):
        split[i] = float(split[i])
        total[i] += split[i]
        if max[i] < split[i]:
            max[i] = split[i]
    # Group data sets into positive and negative examples
    if split[21] == 'N':
        if len(negative_examples) < KThreshold:
            split[21] = 0
            negative_examples.append(split)
    else:
        if len(positive_examples) < KThreshold:
            split[21] = 1
            positive_examples.append(split)

# Calculate mean
mean = []
for i in range (len(total)):
    mean.append(total[i]/count)

# Calculate total variance
standard_dev = []
for i in range (21):
    standard_dev.append(0)
file_handle.seek(0)
for line in file_handle:
    split = line.rstrip().split(',')
    for i in range (21):
        dev = float(split[i]) - mean[i]
        standard_dev[i] += dev * dev
        
# We have the total variance, now find the standard deviation
for i in range(len(standard_dev)):
    standard_dev[i] = (math.sqrt(standard_dev[i]/count))

file_handle.close()

# Segregate the input data into 10 different training and test sets
base_output_directory = "/home/naveen/AI_Projects/IS/newdata"
directory_name_prefix = "set"
training_name_prefix = "training_set"
test_name_prefix = "test_set"
test_size = KThreshold/10
test_sample_base = 0
for i in range (10):
    output_directory = base_output_directory + "/" + directory_name_prefix + str(i+1)
    os.makedirs(output_directory)
    training_file_name = output_directory + "/" + training_name_prefix + ".txt"
    test_file_name = output_directory + "/" + test_name_prefix + ".txt"
    training_output_file = open(training_file_name, "w")
    test_output_file =  open(test_file_name, "w")
    test_sample_ceiling = test_sample_base + test_size
    for i in range (KThreshold):
        if i >= test_sample_base and i < test_sample_ceiling:
            print_to_file(negative_examples[i], test_output_file)
            print_to_file(positive_examples[i], test_output_file)
        else:
            print_to_file(negative_examples[i], training_output_file)
            print_to_file(positive_examples[i], training_output_file)
    training_output_file.close()
    test_output_file.close()
    test_sample_base += test_size
    
print_distribution_to_file(base_output_directory + "/" + "total_distribution" + ".txt", mean, standard_dev)
print_distribution_to_file(base_output_directory + "/" + "max_distribution" + ".txt", max, None, "\n")