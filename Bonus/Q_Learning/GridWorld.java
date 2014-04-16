import java.util.Random;
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
	//stores Q(s,a) values
	public static float[][] stateAction;
	//stores Optimal Policy at each state
	public static String[][] policy;
	//Indicates whether a particular coordinate in the grid is accessible
	public static boolean[][] accessible;
	
	/*
	 * Method to initialize the world,V(s) and accessible arrays for the grid
	 */
	public static void initialize(){
		world = new float[width][height];
		stateAction = new float[width*height][Q_Learning.action.length];
		accessible = new boolean[width][height];
		policy = new String[width][height];
		//initialize reward values for all states to -0.04 except finals states
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++)
				world[i][j]=(float) -0.04;
		}
		world[3][2]=(float)1;
		world[3][1]=(float)-1;
		
		//initialize Q(s,a) values for all states to 0
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++)
				stateAction[i][j]=(float)0;
		}
		
		//set coordinate 1,1 as inaccessible
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++)
				accessible[i][j]=true;
		}
		accessible[1][1]=false;
		
		
		return;
	}
	
	/*
	 * Method that calculates the maximum learned value over each action given coordinate 
	 */
	public static float getLearnedValue(Coor c){
		//calculate immediate reward
		float reward = world[c.x][c.y];
		//calculate max future value over all actions
		float bestFutureReward = (float) -100.0;
		int index = c.toInt();
		for(int i=0;i<4;i++){
			if(stateAction[index][i] > bestFutureReward)
				bestFutureReward = stateAction[index][i];
		}
		//return maximum learned value
		return reward+ Q_Learning.discountFactor*bestFutureReward;
			
	}
	
	/*
	 * given current coordinate and possible moves, calculates next coordinates
	 */
	public static Coor[] calNextStates(Coor c,Coor[] m){
		Coor[] next_states = new Coor[3];
		
		//calculate next coordinates
		for (int i=0;i<3;i++){
			next_states[i]= new Coor(c.x+m[i].x,c.y+m[i].y);
		}
		//determine if the next coordinate is accessible, if not stay in current coordinate
		for (int i=0;i<3;i++){
			if (notAccessible(next_states[i]))
				next_states[i]= new Coor(c.x,c.y);
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
	
	/*
	 * given current coordinate and action returns next coordinate based on the probability distribution
	 */
	public static Coor getNextState(Coor c, String s) {
		Coor[] moves = new Coor[3];
		if (s.equals("up")){
			moves[0] = new Coor(0,1);
			moves[1] = new Coor(-1,0);
			moves[2] = new Coor(1,0);
		}
		if (s.equals("down")){
			moves[0] = new Coor(0,-1);
			moves[1] = new Coor(-1,0);
			moves[2] = new Coor(1,0);
		}
		if (s.equals("left")){
			moves[0] = new Coor(-1,0);
			moves[1] = new Coor(0,1);
			moves[2] = new Coor(0,-1);
		}
		if (s.equals("right")){
			moves[0] = new Coor(1,0);
			moves[1] = new Coor(0,1);
			moves[2] = new Coor(0,-1);
		}
		//obtain possible next coordinates
		moves = calNextStates(c,moves);
		//based on the probability distribution return a next coordinate
		Random r = new Random();
		int chance = r.nextInt(10);
		if (chance<8)
			return moves[0];
		else{
			if (chance == 8)
				return moves[1];
			else
				return moves[2];
		}
		
	}
	
	//Evaluates Q(s,a) to determine best action for each state
	public static void setPolicy() {
		//for each coordinate
		for(int i=0;i<width*height;i++){
			String bestAction="";float bestVal=(float) -100.0;
			//for each action
			for(int j=0;j<Q_Learning.action.length;j++)
				//determine Max Q(s,a)
				if (stateAction[i][j]>bestVal){
					bestVal = stateAction[i][j];
					bestAction = Q_Learning.action[j];
				}
			//set policy
			Coor temp = Coor.toCoor(i);
			policy[temp.x][temp.y] = bestAction;
		}
		//set policy of inaccessible states and final states to Null
		policy[1][1] = "Null";
		policy[3][1] = "Null";
		policy[3][2] = "Null";
	}
	
}


