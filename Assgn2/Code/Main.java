import java.util.Random;
import java.util.Scanner;

/**
 * @author Naveen Neelakandan CSE 4633 : Intro to Artificial Intelligence 
 * 
 * This is the Main class containing the main method.An input file is read in and according to
 * the method set, the respective algorithm is run to find the shortest tour of the cities(points)
 * given in the input file
 */

public class Main {

	//The two_opt local search alogrithm
	public static double two_opt(double[][] d){
		
		//create initial tour
		int[] tour = new int[Parser.dim+1];
		for(int i=0;i<Parser.dim;i++){
			tour[i] = i;

		}
		tour[Parser.dim]=0;
		
		//calculate initial length of tour
		double length=0;
		for(int i=0; i<Parser.dim;i++){
			length += d[tour[i]][tour[i+1]];
		}

		int iteration=0;
		int unchanged=0;
		boolean exit = false;
		System.out.println("Solution so far: " + length);
		while(exit!=true){
			iteration++;
			if (iteration==1000){
				System.out.println("Solution so far: " + length);
				iteration=0;
			}
			//create copy of current tour
			int[] tour_copy = new int[Parser.dim+1];
			for(int i=0;i<Parser.dim+1;i++){
				tour_copy[i] = tour[i] ;
			}
			
			//generate two random edges to remove
			Random generator = new Random();
			int s1 = generator.nextInt(Parser.dim);   //not Parser.dim + 1 as we do not want to change starting/ending point
			int s2 = generator.nextInt(Parser.dim);	  //not Parser.dim + 1 as we do not want to change starting/ending point

			if(s1 == s2)
				continue;

			int d1 = s1+1;
			int d2 = s2+1;
			
			//if city s1 is further on tour
			if(s1>s2){

				if(d2 == s1)
					continue;
				
				//swap cities between s1 and d2
				while(s1-d2>0){

					int temp = tour_copy[d2];
					tour_copy[d2] = tour_copy[s1];
					tour_copy[s1] = temp;
					d2++;
					s1--;

				}
			}

			else if(s1<s2){
				
				//if city s1 is further on tour
				if(d1 == s2)
					continue;

				while(s2-d1>0){
					//swap cities between s2 and d1
					int temp = tour_copy[d1];
					tour_copy[d1] = tour_copy[s2];
					tour_copy[s2] = temp;
					d1++;
					s2--;

				}
			}

			double new_len=0;
			
			//calculate new tour length
			for(int i=0; i<Parser.dim;i++){
				new_len += d[tour_copy[i]][tour_copy[i+1]];
			}
			
			//if new length < original length, modify tour
			if (new_len < length){
				unchanged=0;
				length = new_len;
				for(int i=0;i<Parser.dim+1;i++){
					tour[i] = tour_copy[i] ;
				}
			}
			
			//if new length > original length, leave tour unchanged 
			else{
				unchanged++;
				if(unchanged > 10000){
					exit = true;
				}
			}
			
	
			
		}		

		return length;
	}


	/*public static double two_opt_sim(double[][] d){


		int[] tour = new int[Parser.dim+1];
		for(int i=0;i<Parser.dim;i++){
			tour[i] = i;

		}
		tour[Parser.dim]=0;


		double length=0;
		for(int i=0; i<Parser.dim;i++){
			length += d[tour[i]][tour[i+1]];
		}


		double temperature = 20000;

		while(temperature > 0.0001){

			int[] tour_copy = new int[Parser.dim+1];
			for(int i=0;i<Parser.dim+1;i++){
				tour_copy[i] = tour[i] ;
			}

			Random generator = new Random();
			int s1 = generator.nextInt(Parser.dim);   //not Parser.dim + 1 as we do not want to change starting/ending point
			int s2 = generator.nextInt(Parser.dim);	  //not Parser.dim + 1 as we do not want to change starting/ending point


			if(s1 == s2)
				continue;

			int d1 = s1+1;
			int d2 = s2+1;


			if(s1>s2){

				if(d2 == s1)
					continue;

				while(s1-d2>0){

					int temp = tour_copy[d2];
					tour_copy[d2] = tour_copy[s1];
					tour_copy[s1] = temp;
					d2++;
					s1--;

				}
			}


			else if(s1<s2){

				if(d1 == s2)
					continue;

				while(s2-d1>0){

					int temp = tour_copy[d1];
					tour_copy[d1] = tour_copy[s2];
					tour_copy[s2] = temp;
					d1++;
					s2--;

				}
			}


			double new_len=0;

			for(int i=0; i<Parser.dim;i++){
				new_len += d[tour_copy[i]][tour_copy[i+1]];
			}

			if (new_len < length){
				length = new_len;
				for(int i=0;i<Parser.dim+1;i++){
					tour[i] = tour_copy[i] ;
				}
			}

			else{

				double len_diff = length - new_len;
				len_diff = len_diff/temperature;
				double prob = java.lang.Math.exp(len_diff);
				double r = generator.nextInt(1000);
				r = r / 1000;
				if(r<=prob){
					length = new_len;
					for(int i=0;i<Parser.dim+1;i++){
						tour[i] = tour_copy[i] ;
					}
				}


			}

			temperature = 0.999*temperature;

		}		

		for(int i=0;i<Parser.dim+1;i++){
			System.out.print(tour[i]) ;
			System.out.print(" ") ;
		}
		return length;
	}*/
	
	/*
	 * The two opt algorithm supplemented with random restarts. 10 runs of the two opt algorithm
	 * are run with a random starting tour each time. The best solution is then returned.
	 */
	public static double two_opt_random(double[][] d){
		
		//initialize variables  
		double best_length = 100000000000.0;
		int RESTARTS = 10;

		//no of random restarts
		for(int n=0; n<RESTARTS; n++){

			//initialize tour 
			int[] tour = new int[Parser.dim+1];
			for(int i=0;i<Parser.dim;i++){
				tour[i] = i;
			}
			tour[Parser.dim]=0;
			
			//get random number
			Random generator = new Random();

			//create random tour
			for(int i=1;i<Parser.dim*100;i++){
				int ran1 = generator.nextInt(Parser.dim);
				int ran2 = generator.nextInt(Parser.dim);
				if(ran1 != 0 && ran1 != Parser.dim && ran2 != 0 && ran2 != Parser.dim){
					int temp = tour[ran1];
					tour[ran1] = tour[ran2]; 
					tour[ran2] = temp;
				}
			}

			//calculate initial length
			double length=0;
			for(int i=0; i<Parser.dim;i++){
				length += d[tour[i]][tour[i+1]];
			}


			int unchanged=0;
			boolean exit = false;
			
			
			while(exit!=true){
				
				//make a copy of the current tour
				int[] tour_copy = new int[Parser.dim+1];
				for(int i=0;i<Parser.dim+1;i++){
					tour_copy[i] = tour[i] ;
				}

				//select two random edges to remove
				int s1 = generator.nextInt(Parser.dim);   //not Parser.dim + 1 as we do not want to change starting/ending point
				int s2 = generator.nextInt(Parser.dim);	  //not Parser.dim + 1 as we do not want to change starting/ending point


				if(s1 == s2)
					continue;
				
				//determine next city on edges
				int d1 = s1+1;
				int d2 = s2+1;

				//if city s1 is further in tour
				if(s1>s2){

					if(d2 == s1)
						continue;
					
					//swap cities between s1 and d2
					while(s1-d2>0){

						int temp = tour_copy[d2];
						tour_copy[d2] = tour_copy[s1];
						tour_copy[s1] = temp;
						d2++;
						s1--;

					}
				}

				//if s1 is further in tour
				else if(s1<s2){

					if(d1 == s2)
						continue;
					
					//swap cities between s2 and d1
					while(s2-d1>0){

						int temp = tour_copy[d1];
						tour_copy[d1] = tour_copy[s2];
						tour_copy[s2] = temp;
						d1++;
						s2--;

					}
				}

				double new_len=0;
				
				//calculate new length of tour
				for(int i=0; i<Parser.dim;i++){
					new_len += d[tour_copy[i]][tour_copy[i+1]];
				}
				
				//if new length < original length, modify tour
				if (new_len < length){
					unchanged=0;
					length = new_len;
					for(int i=0;i<Parser.dim+1;i++){
						tour[i] = tour_copy[i] ;
					}
				}
				
				//if new length > original, leave tour unchanged
				else{
					unchanged++;
					if(unchanged > 10000){
						exit = true;
					}
				}

			}		
			
			//on finding a better solution, print to screen
			if (length<best_length){
				best_length = length;
				System.out.println("Solution Updated..");
			}
			
			//print progress
			System.out.print((n+1)*(100/RESTARTS));
			System.out.println(" % Complete");

		}

		return best_length;

	}


	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		//get input from file
		System.out.println("Input file name (without .tsp): ");
		Scanner input = new Scanner(System.in);
		String fname = input.next();
		input.close();
		
		//start clock
		long start = System.currentTimeMillis();
		
		//parse input
		Parser p = new Parser(fname);
		double[][] dist;
		dist = p.parse();

		//call alorgithm -- Change this to select algorithm
		double r = two_opt(dist);

		//print tour length and time taken information
		System.out.println(" ");
		System.out.print("The length of the tour is: ");
		System.out.println(r);

		double elapsedTime = System.currentTimeMillis()-start;

		System.out.print("Time Taken: ");
		System.out.print(elapsedTime/1000);
		System.out.print(" s");
	}

}
