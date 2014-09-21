class KNN{

	

	KNN(trainingset);

	// does the kNN on training set using leave-one out
	// Retrun error rate + classification of each
	// repeatedly calls performKNN with different k
	computeTrainingError()

	// does the actual kNN on a training set
	classify(testset, int k)

	getEuclideanDistance();

	getShortestNeighbours()?

	// iterates over input set, for each example in the input set, cdoes the knn
	// get called by both classify and computerTainingError as subrotuine
	performKNN(set, int k)

	// for a single point, get its k nearett neigbours
	getKNearestNeighbors(int k)

	// for the neighbors give, get thier classification..any tie braking..biasing.. goes here
	getClassification(KNeighbors)

	getClass();
}