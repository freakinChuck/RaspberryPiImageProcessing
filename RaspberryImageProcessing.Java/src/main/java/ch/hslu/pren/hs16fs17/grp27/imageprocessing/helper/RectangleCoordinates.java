package ch.hslu.pren.hs16fs17.grp27.imageprocessing.helper;

/**
 *
 * @author Tenga
 * @version 1.0
 */
public class RectangleCoordinates {
    private int xPos, yPos;

    public RectangleCoordinates(){
        this.xPos = 0;
        this.yPos = 0;
    }

    public RectangleCoordinates(int xPos, int yPos){
        this.xPos = xPos;
        this.yPos = yPos;
    }

    public int getxPos() {
        return xPos;
    }

    public int getyPos() {
        return yPos;
    }

    public void setxPos(int xPos) {
        this.xPos = xPos;
    }

    public void setyPos(int yPos) {
        this.yPos = yPos;
    }


}