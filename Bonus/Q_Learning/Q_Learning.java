import java.util.Random;

/**
 * Class that executes the Value iteration algorithm
 * @author naveen
 *
 */
public class Q_Learning {
	
	//Set Algorithm parameters
	public final static float discountFactor = (float)0.99;
	public final static float alp = (float)0.8;
	
	//Array of possible actions
	public final static String[] action = {"up","down","left","right"};

	/*
	 * Q-learning algorithm
	 */
	public static void main(String[] args) {
		//Initialize Gridworld values
		GridWorld.initialize();
		int cal=0;
		//Loop for 10000000 iterations
		for (int iter=0;iter<1000000;iter++){
				//Choose random starting coordinate
				Random r = new Random();
				Coor startC = new Coor(r.nextInt(4),r.nextInt(3));
				//While the final states are not reached
				while(startC.notEquals(new Coor(3,2)) && startC.notEquals(new Coor(3,1))){
					//if the next state is invalid, break
					if (GridWorld.notAccessible(startC))
						break;
					//choose randomly an action from list of actions. 
					int a = r.nextInt(4);
					//calculate a next state based on the tranisition probabilities
					Coor nextC = GridWorld.getNextState(startC,action[a]);
					//compute the learned value
					float learnedValue = GridWorld.getLearnedValue(nextC);
					//compute old value
					float oldValue = GridWorld.stateAction[startC.toInt()][a]; 
					//value iteration update
					GridWorld.stateAction[startC.toInt()][a] = oldValue + alp*(learnedValue-oldValue);
					//s <- s'
					startC = nextC;
				}
				//progress message
				if (iter%50000==0){
					//System.out.println(GridWorld.stateAction[10][3]);
					System.out.println("Calculating "+cal+"%");
					cal+=10;
				}
		}
		//set the optimal policy for the problem
		GridWorld.setPolicy();
		//print to screen the optimal policy at each state
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++){
				System.out.println("Q("+i+","+j+")"+"="+GridWorld.policy[i][j]);
			}
		}
	}

}
