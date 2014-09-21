import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * A k-means clustering algorithm implementation.
 * @author Naveen Neelakandan
 */

public class KMeans {

	double[][] mCentroids;
	double[][] mInstances;
	int[] mClusterAssignment;
	List<Double> mDistortions = new ArrayList<Double>();
	final static double KveryLargeNumber = Double.MAX_VALUE;
	// The number of features (dimensions)
	int mDimensions = -1;
	// The number of cluster
	int mK = 0;
	// The number of instances
	int mN = 0;

	/***
	 * Partitions n observations into k clusters in which each observation 
	 * belongs to the cluster with the nearest mean.
	 *
	 * @param centroids - A two-dimensional array of the initial position of the cluster centers. 
	 * Each row corresponds to a cluster and each column to a feature dimension
	 *
	 * @param instances - The data set to be clustered. Each row corresponds to an instance, 
	 * and this array will have the same dimensions as the centroids array
	 *
	 * @param threshold - The threshold that determined when to stop iterations
	 *
	 * @return kMeansResult - a variable with the type KMeansResult with information about the
	 * final positions of the clusters, the assignments and the distortions at each iteration
	 */
	public KMeansResult cluster(double[][] centroids, double[][] instances, double threshold) {
		// Assuming threshold will be a 
		mK = centroids.length;
		mN = instances.length;
		mCentroids = centroids;
		mInstances = instances;
		mDimensions = centroids[0].length;
		mClusterAssignment = new int[mN];
		// Records the change in the distortion
		double distortionDelta = Double.MAX_VALUE;
		// What should this be initialized to? - a very small number?
		double lastDistortion = 1;	
		//randomizeClusterAssignment();

		while(distortionDelta >= threshold){
			boolean reallocate = true;
			while (reallocate){
				// For each instance
				for(int i=0; i<mN; ++i){
					double minDistance = KveryLargeNumber;
					// Reallocate to nearest cluster
					for(int c=0; c<mK; ++c){
						double distanceToCluster = getDistanceToCentroid(c, i);
						if (distanceToCluster < minDistance){
							minDistance = distanceToCluster;
							mClusterAssignment[i] = c;
						}
					}
				}
				List<Integer> orphanedCentroids = getOrphanedCentroids();
				if (orphanedCentroids.size() == 0){
					reallocate = false;
				}
				else{
					// If there are 2 ore more orphaned centroids, do we re-assign both
					// to the farthest instance, or re-assign one, do the re-allocation
					// for all and then check again?
					/*for(int i=0; i<orphanedCentroids.size(); ++i)
						reAssignOrphanedCentroid(orphanedCentroids.get(i));
					*/
					// Only re-assigning the first one.
					reAssignOrphanedCentroid(orphanedCentroids.get(0));
				}
			}
			// For each cluster
			for(int c=0; c<mK; ++c){
				// Get new cluster coordinates (mean of instance's coordinates)
				double[] averageCoordinates = getClusterMean(c);
				centroids[c] = averageCoordinates;
			}

			// Calculate the change in the distortion
			double currentDistortion = getDistortion();
			distortionDelta = Math.abs((currentDistortion - lastDistortion)/lastDistortion);
			//System.out.println("DistortionDelta: " + distortionDelta);
			mDistortions.add(currentDistortion);
			lastDistortion = currentDistortion;
		}

		return getResult();
	}

	private void reAssignOrphanedCentroid(Integer centroidIndex) {
		double maxDistance = 0;
		int furthestInstance = -1;
		for (int i=0; i<mN; ++i){
			double distance = getDistanceToCentroid(mClusterAssignment[i], i);
			if (distance > maxDistance){
				maxDistance = distance;
				furthestInstance = i;
			}
		}
		mCentroids[centroidIndex] = mInstances[furthestInstance];
	}

	private List<Integer> getOrphanedCentroids() {
		List<Integer> orphans = new ArrayList<Integer>();
		boolean[] clusterHasInstance = new boolean[mK];
		for(int i=0; i<mN; ++i){
			clusterHasInstance[mClusterAssignment[i]] = true;
		}
		for (int i=0; i<mK; ++i){
			if(clusterHasInstance[i] == false){
				orphans.add(i);
			}
		}
		return orphans;
	}

	private void randomizeClusterAssignment() {
		Random rand = new Random();
		for(int i=0; i<mClusterAssignment.length; ++i){
			int j = rand.nextInt(mK);
			mClusterAssignment[i] = j;
		}
	}

	private double[] getClusterMean(int clusterIndex) {
		double[] mean = new double[mDimensions];
		int noOfInstances = 0;
		for(int i=0; i<mN ; ++i){
			if(mClusterAssignment[i] == clusterIndex){
				noOfInstances ++;
				for(int d=0; d<mDimensions; ++d){
					mean[d] += mInstances[i][d]; 
				}
			}
		}
		for(int d=0; d<mDimensions; ++d){
			mean[d] /= noOfInstances; 
		}
		return mean;
	}

	// Returns the sum of the squared euclidean distance between an all instances
	// and their respective centroids.
	private double getDistortion() {
		double distortion = 0;
		// For each instance
		for (int i=0; i<mN; ++i){
			// Calculate distance to the centroid that instance belongs to 
			int centroid = mClusterAssignment[i];
			distortion += getDistanceToCentroid(centroid, i);
		}
		return distortion;
	}

	// Returns the squared euclidean distance between an instance and a centroid
	private double getDistanceToCentroid(int centroidIndex, int instanceIndex) {
		double distance = 0;
		for (int i=0; i<mDimensions; ++i){
			distance += Math.pow(mCentroids[centroidIndex][i] - mInstances[instanceIndex][i], 2);
		}
		return distance;
	}

	private KMeansResult getResult(){
		KMeansResult kMeansResult = new KMeansResult();
		kMeansResult.centroids = mCentroids;
		kMeansResult.clusterAssignment = mClusterAssignment;
		double[] target = new double[mDistortions.size()];
		for (int i = 0; i < target.length; i++) {
			target[i] = mDistortions.get(i);                
		}
		kMeansResult.distortionIterations = target;
		return kMeansResult;
	}

}
