import java.io.*;
import java.util.Scanner;
import java.util.regex.*;

/**
 * 
 * @author Naveen Neelakandan  CSE 4633 : Intro to Artificial Intelligence 
 *
 *This is the Parser class. It parses a input file (.tsp) to generate a distance matrix which
 *an algorithm can be run on.
 */

public class Parser {
	
	//initialize variables
	double[] coorx;
	double[] coory;
	static int dim = 0;

	/*
	 * Constructor takes in a file name and places x-ccordinates and y-coordinates into separate
	 * arrays
	 */
	public Parser(String name) {

		int count =0;
		name = name + ".tsp";
		Scanner scanner = null;
		
		//open file
		try{
			scanner = new Scanner(new File(name));
		}

		catch (FileNotFoundException f){
			System.out.println("Not found");
			System.exit(0);
		}

		int dimension;
		
		
		while(scanner.hasNextLine()){	        
			
			//get next line
			String line = scanner.nextLine();

			//find dimension of matrix
			if(line.startsWith("DIMENSION")){

				Pattern p = Pattern.compile("\\d+");
				Matcher m = p.matcher(line); 

				while (m.find()) {
					dim = Integer.parseInt(m.group());
				}

				coorx = new double[dim]; 
				coory = new double[dim];
			}

			
		
			//if a coordinate is found
			if(Character.isDigit((line.charAt(0)))){
				
				//split string into string array
				String[] words;
				words = line.split(" ");
				
				//get x coordinate
				String xx = words[1];
				double x = Double.parseDouble(xx);
				
				//get y coordinate
				String yy = words[2];
				double y = Double.parseDouble(yy);
				
				//place coordinates into array.
				coorx[count] = x;
				coory[count] = y;
				count++;

			}
		}

	}

	/*
	 * parse method takes input in x coordinate array and y coordinate array and creates
	 * a distance matrix between two cities
	 */
	public  double[][] parse() {
		
		//initialize distance matrix
		double [][] twod = new double [dim][dim];
		
		
		for(int i=0; i<dim; i++){
			for(int j=0; j<dim; j++){
				
				//get x and y coordinates of cities i and j
				double xi = coorx[i];
				double yi = coory[i];
				double xj = coorx[j];
				double yj = coory[j];
				
				//calculate distance
				double x = xi -xj;
				double y = yi -yj;
				x = java.lang.Math.pow(x,2);
				y = java.lang.Math.pow(y,2);
				double d = java.lang.Math.sqrt(x + y);
				
				//insert into distance matrix
				twod[i][j] = d;

			}
		}

		return twod;		
	}

}
