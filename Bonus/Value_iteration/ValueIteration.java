/**
 * Class that executes the Value iteration algorithm
 * @author naveen
 *
 */
public class ValueIteration {
	
	//Set Algorithm parameters
	public final static float discountFactor = (float)0.99;
	public final float accuracy = (float)0.001;
	
	//Array of possible actions
	public final static String[] action = {"up","down","left","right"};

	/*
	 * Value iteration algorithm
	 */
	public static void main(String[] args) {
		//Initialize Gridworld values
		GridWorld.initialize();
		
		//Loop till V(S) converges
		for (int iter=0;iter<10000;iter++){
			//For every state
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					//V(S) values of final states do not change -- (this should not be hardcoded here)
					if(i==3 && j==2)
						continue;
					if(i==3 && j==1)
						continue;
					//initialize Q(S) for each state
					float q=0,q_best=(float) -100.0;
					//Calculate Q(S) for each action 
					for (int a=0;a<4;a++){
						q = GridWorld.getCurrReward(i,j) + discountFactor*GridWorld.getFutureReward(i,j,action[a]);
						if(q>q_best)
							q_best = q;
					}
					//set valuefunction to Max Q(S)
					GridWorld.valueFunction[i][j]=q_best;
				}
			}
		}
		//Print out V(S) values for each state at end
		for(int i=0;i<4;i++){
			for(int j=0;j<3;j++){
				System.out.print("V("+ i+","+j+") "+" = ");
				System.out.println(GridWorld.valueFunction[i][j]);
			}
		}
	}

}
