/**
 * Custom data class to represent a coordinate -- better than using strings!
 * @author naveen
 *
 */
public class Coor {
	public int x;
	public int y;
	
	public Coor(int ix,int iy){
		x=ix;
		y=iy;
	}
	
	public int toInt(){
		int ypos = y*(GridWorld.height+1);
		return x+ypos;
		
	}
	
	public static Coor toCoor(int i){
		int x = i%GridWorld.width;
		int y = (i-x)/GridWorld.height;
		return new Coor(x,y);
	}

	public boolean notEquals(Coor c) {
		if (x==c.x && y==c.y)
			return false;
		else
			return true;
	}
	

}
