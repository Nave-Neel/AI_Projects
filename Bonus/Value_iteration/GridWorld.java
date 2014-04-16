/**
 * Class that represents the Grid world 
 * @author naveen
 *
 */
public class GridWorld {
	
	//Set Grid dimension
	public static final int width = 4;
	public static final int height = 3;
	
	//stores reward values
	public static float[][] world;
	//stores V(s) values
	public static float[][] valueFunction;
	//Indicates whether a particular coordinate in the grid is accessible
	public static boolean[][] accessible;
	
	/*
	 * Method to initialize the world,V(s) and accessible arrays for the grid
	 */
	public static void initialize(){
		world = new float[width][height];
		valueFunction = new float[width][height];
		accessible = new boolean[width][height];
		
		//initialize reward values for all states to -0.04 except finals states
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++)
				world[i][j]=(float) -0.04;
		}
		world[3][2]=(float)1;
		world[3][1]=(float)-1;
		
		//initialize V(s) values for all states to -0.04 except finals states
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++)
				valueFunction[i][j]=(float)-0.04;
		}
		valueFunction[3][2]=(float)1;
		valueFunction[3][1]=(float)-1;
		
		//set coordinate 1,1 as inaccessible
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++)
				accessible[i][j]=true;
		}
		accessible[1][1]=false;
		return;
	}
	
	/*
	 * Method that calculates the future reward given current coordinate and action 
	 */
	public static float getFutureReward(int x,int y,String action){
		//initialize local variables
		Coor[] moves = new Coor[3];
		Coor[] new_states;
		float q = 0;
		float[] prob = {(float)0.8,(float) 0.1,(float) 0.1};
		
		//determine possible next moves given an action -- since the effect of the action is 
		//non-deterministic
		if (action.equals("up")){
			moves[0] = new Coor(0,1);
			moves[1] = new Coor(-1,0);
			moves[2] = new Coor(1,0);
		}
		if (action.equals("down")){
			moves[0] = new Coor(0,-1);
			moves[1] = new Coor(-1,0);
			moves[2] = new Coor(1,0);
		}
		if (action.equals("left")){
			moves[0] = new Coor(-1,0);
			moves[1] = new Coor(0,1);
			moves[2] = new Coor(0,-1);
		}
		if (action.equals("right")){
			moves[0] = new Coor(1,0);
			moves[1] = new Coor(0,1);
			moves[2] = new Coor(0,-1);
		}
		
		//determine the next possible coordinates given probable moves
		new_states = calnextstates(x,y,moves);
		
		//calculate the future reward 
		for (int i=0;i<3;i++)
			q+= prob[i]*(getVfunction(new_states[i]));
		return q;
	}
	
	/*
	 * method to return reward values
	 */
	public static float getCurrReward(int x,int y){
		return world[x][y];	
	}
	
	/*
	 * method to return V(s) values
	 */
	public static float getVfunction(Coor s){
		return valueFunction[s.x][s.y];	
	}
	
	/*
	 * given current coordinate and possible moves, calculates next coordinate
	 */
	public static Coor[] calnextstates(int x,int y,Coor[] m){
		Coor[] next_states = new Coor[3];
		
		//calculate next coordinates
		for (int i=0;i<3;i++){
			int xoff = m[i].x;
			int yoff = m[i].y;
			int xnew = x+xoff;
			int ynew = y+yoff;
			next_states[i]= new Coor(xnew,ynew);
		}
		//determine if the next coordinate is accessible, if not stay in current coordinate
		for (int i=0;i<3;i++){
			if (notAccessible(next_states[i]))
				next_states[i]= new Coor(x,y);
		}
		
		return next_states;
	}
	
	/*
	 * method which indicates if a coordinate is inaccessible or out of the grid's dimensions
	 */
	public static boolean notAccessible(Coor s){
		int x = s.x;
		int y = s.y;
		//determine if coordinate is out of grid's dimensions
		if (2>=y && y>=0 && 3>=x && x>=0 ){
			//determine if there is a wall at the coordinate in the grid
			if (accessible[x][y]==false)
				return true;
			else
				return false;
		}
		else
			return true;
	}
	
}


