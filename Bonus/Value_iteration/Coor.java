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
	
	public Coor add(int ox,int oy){
		x+=ox;
		y+=oy;
		return this;
	}
}
