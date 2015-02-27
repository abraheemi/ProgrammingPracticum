import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


/* ============================================================== */
/* ######################### MAIN CLASS ######################### */
public class GameOfLife{
	/* ######################### MAIN METHOD ######################### */
	public static void main(String[] args){

		int rpos=0, cpos=0;
		int rsize=0, csize=0;
		Maze maze = new Maze();
		
   		File f = new File (args[0]);
   		Scanner sc = null;

   		try{
    		 sc = new Scanner (f);
   		}
   		catch (FileNotFoundException fnfe){
    	 	System.err.println ("File did not exist");
    		 return;
   		}

   		// Read in size and checks for errors
		while (true){
			if( ! (sc.hasNextInt()) ){
				System.err.println("End of file reached. Exiting...");
				System.exit(0);
			}
    	 	rsize = sc.nextInt();
    	 	csize = sc.nextInt();
			if(rsize <= 0 || csize <= 0){
				System.err.println("Error: incorrect size");
				continue;
			}
			break;
   		}
		
		//maze.init(rsize, csize);
		int i, j, k, r, c;
		r = rsize+2;
		c = csize+2;
	  	boolean gridA[][];
  		boolean gridB[][];

		// GRID A
		gridA = new boolean[r][];
		for(i=0; i<r; i++){
			gridA[i] = new boolean[c];
		}
		for(i=0; i < r; i++){
			for(j=0; j < c; j++){
				gridA[i][j] = false;
			}
		}
	
		// GRID B
		gridB = new boolean[r][];
		for(i=0; i<r; i++){
			gridB[i] = new boolean[c];
		}
		for(i=0; i < r; i++){
			for(j=0; j < c; j++){
				gridB[i][j] = false;
			}
		}
	
   		// Read in blocked positions and checks for errors
		while (true){
			if( ! (sc.hasNextInt()) ){
				System.err.println("End of file reached.");
				break;
			}
    	 	rpos = sc.nextInt();
    	 	cpos = sc.nextInt();
			if(rpos < 1 || rpos > rsize){
				System.err.println("Error: incorrect rpos position");
				continue;
			}
			if(cpos < 1 || cpos > csize){
				System.err.println("Error: incorrect cpos position");
				continue;
			}
			gridA[rpos][cpos] = true;
			//maze.initPos(rpos, cpos);
		}
	
	/* ######################### STACK OPERATIONS METHOD ######################### */
		//int i, j, k, r, c;
		int count;
		Direction move = new Direction();
		GridDisplay disp = new GridDisplay(rsize+2, csize+2);
		
		move.reset();		
		//maze.display(disp);

		for(i=0; i < rsize+2; i++){
			for(j=0; j < csize+2; j++){
				System.out.print(gridA[i][j]);
				if( ! gridA[i][j] ){
					disp.setColor(i, j, Color.WHITE);
				}	
				if( gridA[i][j] ){
					disp.setColor(i, j, Color.BLACK);
				}
			}
			System.out.println();
		}

		while (true){

			for(r=1; r<rsize; r++){
				for(c=1; c<csize; c++){
					count = counter(r, c, gridA);
					// Any live cell with less than two live neighbors dies, as if caused by under-population.
					if( gridA[r][c] && (count < 2) ){
						gridB[r][c] = false;
						//disp.setColor(r,c, Color.WHITE);
					}
					else if( gridA[r][c] && (count == 2 || count == 3) ){
						gridB[r][c] = true;
						//disp.setColor(r,c, Color.BLACK);
					}
					else if( gridA[r][c] && (count > 3) ){
						gridB[r][c] = false;
						//disp.setColor(r,c, Color.WHITE);
					}
					else if( (! gridA[r][c]) && (count == 3) ){
						gridB[r][c] = true;
						//disp.setColor(r,c, Color.BLACK);
					}
				}
			}

			for(i=0; i < rsize+2; i++){
				for(j=0; j < csize+2; j++){
					System.out.print(gridA[i][j]);
					if( ! gridB[i][j] ){
						disp.setColor(i, j, Color.WHITE);
					}	
					if(gridB[i][j]){
						disp.setColor(i, j, Color.BLACK);
					}
				}
				System.out.println();
			}

			for(i=0; i<rsize; i++){
				for(j=0; j<csize; j++){
					gridA[i][j] = gridB[i][j];
				}
			}
			mySleep(200);

		} // MAIN LOOP
		

	} // END OF stackOps()


	public static int counter(int r, int c, boolean gridA[][]){
		int count = 0;
		if(gridA[r-1][c])	// NORTH
			count++;
		if(gridA[r-1][c+1])	// NORTH-EAST
			count++;
		if(gridA[r][c+1])	// EAST
			count++;
		if(gridA[r+1][c+1])	// SOUTH-EAST
			count++;
		if(gridA[r+1][c])	// SOUTH
			count++;
		if(gridA[r+1][c-1])	// SOUTH-WEST
			count++;
		if(gridA[r][c-1])	// WEST
			count++;
		if(gridA[r-1][c-1])	// NORTH-WEST
			count++;

		return count;
	}


	/* #################### SYSTEM SLEEP METHOD #################### */
	// puts the current thread to sleep for some number of milliseconds 
	// to allow for "animation"
    public static void mySleep( int milliseconds){
      try{
        Thread.sleep(milliseconds);
      }
      catch (InterruptedException ie){
      }
    }
}


/* ============================================================== */
/* ######################### MAZE CLASS ######################### */
class Maze{
  private char gridA[][];
  private char gridB[][];
  private int rSize, cSize; // Row and Col sizes
  private int rPos, cPos;
  private int x, y;

  public void init(int rsize, int csize){
	rSize = rsize;
	cSize = csize;

	int i, j;
	int r = rSize+2;
	int c = cSize+2;

	// GRID A
	gridA = new char[r][];
	for(i=0; i<r; i++){
		gridA[i] = new char[c];
	}
	for(i=0; i < r; i++){
		for(j=0; j < c; j++){
			gridA[i][j] = '.';
		}
	}

	// GRID B
	gridB = new char[r][];
	for(i=0; i<r; i++){
		gridB[i] = new char[c];
	}/*
	for(i=0; i < r; i++){
		for(j=0; j < c; j++){
			gridB[i][j] = '.';
		}
	}*/


	int row = rSize + 1;
	int col = cSize + 1;
	for(i=0; i < r; i++){
		gridB[i][0] = '.';
		gridB[i][col] = '.';
	}
	for(i=0; i < c; i++){
		gridB[0][i] = '.';
		gridB[row][i] = '.';
	}

  }

  public void initPos(int rpos, int cpos){
	rPos = rpos;
	cPos = cpos;
	gridA[rPos][cPos] = '*';
  }

  public void setGridB(int r, int c, char con){
  	gridB[r][c] = con;
  }

  public char GridA(int row, int col){
	x = row;
	y = col;
	return (this.gridA[x][y]);
  }

  public void copy(){
  	int i, j;
  	for(j=1; j<cSize; j++){
  		for(i=1; i<rSize; i++){
  			gridA[i][j] = gridB[i][j];
  		}
  	}
  }

  public void display(GridDisplay disp){
	int i, j;
	//System.out.println("Size: " + rSize + ", " + cSize);

	for(i=0; i < rSize+2; i++){
		for(j=0; j < cSize+2; j++){
			System.out.print(gridA[i][j]);
			if(gridA[i][j] == '.'){
				disp.setColor(i, j, Color.WHITE);
			}	
			if(gridA[i][j] == '*'){
				disp.setColor(i, j, Color.BLACK);
			}
		}
		System.out.println();
	}

  }

  public void displayB(GridDisplay disp){
	int i, j;
	//System.out.println("Size: " + rSize + ", " + cSize);

	for(i=0; i < rSize+2; i++){
		for(j=0; j < cSize+2; j++){
			System.out.print(gridB[i][j]);
			if(gridB[i][j] == '.'){
				disp.setColor(i, j, Color.WHITE);
			}	
			if(gridB[i][j] == '*'){
				disp.setColor(i, j, Color.BLACK);
			}
		}
		System.out.println();
	}

  }

}



/* ==================================================================== */
/* ######################### DIRECTIONS CLASS ######################### */
class Direction{
  private int i;
  private int j;
  private int n;

	public void reset(){
		i = 0;
		j = 1;
		n = 1;
	}

	public void changeDirection(){
		n++;
		if(n >= 9){
			i = 0;
			j = 1;
			n = 1;
		}
		else if(n == 1){
			i = 0; j = 1;
		}
		else if(n == 2){
			i = 1; j = 0;
		}
		else if(n == 3){
			i = 0; j = -1;
		}
		else if(n == 4){
			i = -1; j = 0;
		}
		else if(n == 5){	// NORTH-WEST
			i = -1; j = -1;
		}
		else if(n == 6){	// NORTH-EAST
			i = -1; j = 1;
		}
		else if(n == 7){	// SOUTH-WEST
			i = 1; j = -1;
		}
		else if(n == 8){	// SOUTH-EAST
			i = 1; j = 1;
		}

	}

	public int r(){
		return i;
	}
	public int c(){
		return j;
	}
}



/* ============================================================== */
/* ######################### GRID CLASS ######################### */
class GridDisplay extends JFrame
{
   private JLabel labels[];

   private Container container;
   private GridLayout grid1;
   int rowCount;
   int colCount;

   // set up GUI
   public GridDisplay(int rows, int cols)
   {
      super( "GridDisplay for CS211" );
      setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );

      // set up grid layout struture of the display
      rowCount = rows;
      colCount = cols;
      grid1 = new GridLayout( rows, cols );
      container = getContentPane();
      container.setLayout( grid1 );

      // create and add buttons
      labels = new JLabel[ rows * cols ];

      for ( int count = 0; count < labels.length; count++ ) {
         labels[ count ] = new JLabel( " " );
         labels[count].setOpaque(true);
         container.add( labels[ count ] );
      }

      // set up the size of the window and show it
      setSize( cols * 15 , rows * 15 );
      setVisible( true );

   } // end constructor GridLayoutDemo

   // display the given char in the (row,col) position of the display
   public void setChar (int row, int col, char c)
   {
     if ((row >= 0 && row < rowCount) && (col >= 0 && col < colCount) )
     {
       int pos = row * colCount + col;
       labels [pos].setText("" + c);  
     }
   }
   
   // display the given color in the (row,col) position of the display
   public void setColor (int row, int col, Color c)
   {
     if ((row >= 0 && row < rowCount) && (col >= 0 && col < colCount) )
     {
       int pos = row * colCount + col;
       labels [pos].setBackground(c);  
     }
   }
} // end class GridDisplay



