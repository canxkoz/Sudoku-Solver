/*
 *=====================
 *   Sudoku Solver
 *=====================
 *
 * Started On - May 15th, 2019
 * Author - Can Koz
 * Objective - Takes in Sudoku puzzles and outputs the solution.
*/

#include<iostream>
#include<fstream>
#include <cstdlib>
#include <Windows.h>
using namespace std;

/**
  *	This class provides a data structure which can hold and manipulate the values in a sudoku puzzle.
  *	In this file, we shall call this data structure the 'Sudoku Grid'.
*/
class SudokuGrid{

	int grid[9][9]; //This pointer will hold all the values in the matrix.

	/**
	  *	@desc This constructor calls the menu() func to provide the menu.
	  *	@param none
	  *	@return none
	*/
	public:SudokuGrid(){
		menu();
	}

	/**
	  *	@desc Displays a menu to the user when the SudokuGrid objects in instantiated
	  *	(which is basically at the start of the program) to display all possible options
	  *	from the user.
	  *
	  *	@param none
	  *	@return none
	*/
	private:void menu(){

		cout<<"\n======================\n";
		cout<<"    Sudoku Solver\n";
		cout<<"======================\n\n";

		cout<<"Welcome to Sudoku Solver!\n";
		cout<<"Before we start, you will have to input the puzzle into this program.\n\n";
		cout<<"You can either:-\n";
		cout<<"\t1. Input the puzzle by entering the values manually. (Enter 1)\n";
		cout<<"\t2. Input the puzzle by reading a file with values in it. (Enter 2)\n";
		cout<<"\t   The file must not have a name > 20 characters.\n";
		cout<<"\t   The file must be in the same directory as this C++ file.\n";
		cout<<"\t   The file must have all 81 values seperated with spaces.\n";
		cout<<"\t   Blank cells must be filled in as 0 (eg; 1 0 0  2 0 0 ...).\n";
		cout<<"\t   --> ";

		int option;
		cin>>option;

		if(option==1) readFromConsole();
		else if(option==2) readFromFile();
		else{
			while(true){
				cout<<"\nYou seem to have entered an invalid option. Try again.\n";
				cout<<"\t   --> ";
				cin>>option;

				if(option==1) readFromConsole();
				else if(option==2) readFromFile();
				else continue;

				break;
			}
		}

	}

	/**
	  *	@desc Reads the values for the Sudoku Grid cell-by-cell.
	  *	@param none
	  *	@return none
	*/
	private:void readFromConsole(){
		cout<<"\nEnter the specified value when prompted.\n";
		cout<<"Enter 0 if cell is empty.\n\n";

		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){ //Iterating over cells to read vals.
			for(colIter=0; colIter<9; colIter++){
				int enteredValue;

				cout<<"Enter value for cell["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
				cin>>enteredValue;

				if(!(enteredValue>=0 && enteredValue<=9)){ //Checking for bounds in input.
					while(true){ //We loop until valid input is read from user.
						cout<<"Oops! You seem to have entered a wrong value! Try again.\n";
						cout<<"Enter value for cell ["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
						cin>>enteredValue;

						if(enteredValue>=0 && enteredValue<=9) break;
					}
				}

				grid[rowIter][colIter]=enteredValue;
			}
			cout<<"-------\n"; //Display a break after every row for convenience.
		}
		cout<<endl;
	}

	/**
	  *	@desc Reads the values from a file containing the grid values seperated by whitespaces.
	  *	@param none
	  *	@return none
	*/
	private:void readFromFile(){

		char filename[30]; //Getting filename.

		cout<<"\nEnter the name of the file that contains the Sudoku Puzzle.\n";
		cout<<"\t   --> ";
		cin>>filename;

		ifstream input_file; //Opening file for reading.
		input_file.open(filename,ios::in);

		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){ //Iterating over file values.
			for(colIter=0; colIter<9; colIter++){
				int readValue;

				input_file>>readValue;

				if(!(readValue>=0 && readValue<=9)){ //Checking bounds for input.
					cout<<"\nValue "<<((rowIter*9)+colIter+1)<<" in "<<filename;
					cout<<" seems to be wrong! Correct the value and try again!\n";
					exit(EXIT_FAILURE);
				}

				grid[rowIter][colIter]=readValue;

				if(input_file.eof()) break; //Breaking if EOF is reached.
			}
		}

		//Gotta have a line which lets us check the frame for any errors

		input_file.close();
		cout<<endl;
	}

	/**
	  *	@desc Assigns the passed-in number to the specified row and col.
	  *	@param row (int) row of the specified cell
	  *	@param col (int) col of the specified cell
	  *	@return none
	*/
	public:void setCellValue(int row, int col, int key){
		grid[row][col]=key;
	}

	/**
	  *	@desc Returns the value of the cell at the specified row and col.
	  *	@param row (int) row of the specified cell
	  *	@param col (int) col of the specified cell
	  *	@return (int) cellValue value at the specified cell
	*/
	public:int getCellValue(int row, int col){
		int cellValue=grid[row][col];
		return cellValue;
	}


	/**
	  *	@desc Displays the values stored in the grid with designs. We also use
	  *	ANSI colors, using escape sequences, to display the grid.
	  *	@param none
	  *	@return none
	*/
	public:void displayGrid(){

		cout<<"++=====================================++";
		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){
			int cellIter=1;

			cout<<"\n||";
			for(colIter=0; colIter<9; colIter++){
				if(cellIter==3){
					cout<<" ";
					if(grid[rowIter][colIter] != 0)
						cout<<grid[rowIter][colIter]<<" ";
					else
						cout<<"  ";
					cout<<"||";
					cellIter=1;
				}
				else{
					cout<<" ";
					if(grid[rowIter][colIter] != 0)
						cout<<grid[rowIter][colIter]<<"  ";
					else
						cout<<"   ";
					cellIter++;
				}
			}

			if(rowIter%3!=2) cout<<"\n++-----------++-----------++-----------++";
			else cout<<"\n++=====================================++";
		}

		cout<<" ";
	}

};


/**
  *	Takes in the SudokuGrid object and solves the Sudoku Puzzle.
*/
class SudokuSolver{

	int recursiveCount; //Stats variable
	SudokuGrid grid; //The grid object

	/**
	  *	@desc The constructor initialises the recursiveCount variable and also calls
	  *	the solve() function which solves the puzzle. It also displays the grids
	  *	before and after the solving.
	  *	@param none
	*/
	public:SudokuSolver(){
		recursiveCount=0;

		displayGrid();
		cout<<"\n\nCalculating possibilities...\n";
		cout<<"Backtracking across puzzle....\n";
		cout<<"Validating cells and values...\n\n";

		solve();

		displayGrid();
		cout<<"\n\n";
	}

	/**
	  *	@desc Checks if the value in the specified cell is valid or not.
	  *	@param row (int) row of the required value
	  *	@param col (int) col of the required value
	  *	@param keyValue (int) the required value
	  *	@return (bool) whether the value is valid or not in the sudoku grid
	*/
	private:bool cellValueValid(int row, int col, int keyValue){
		int rowIter, colIter;

		//Checking if value exists in same column
		for(rowIter=0; rowIter<9; rowIter++){
			if(rowIter == row)
				continue;

			if(grid.getCellValue(rowIter, col) == keyValue)
				return false;
		}

		//Checking if value exists in same row
		for(colIter=0; colIter<9; colIter++){
			if(colIter == col)
				continue;

			if(grid.getCellValue(row, colIter) == keyValue)
				return false;
		}

		//Checking if value exists in the same 3x3 square block
		if( !ThreeByThreeGridValid(row, col, keyValue) )
			return false;

		// It is a valid key
		return true;
	}

	/**
	  *	@desc Checks if the same value is also present in the same 3x3 grid block.
	  *	@param row (int) row of the required cell
	  *	@param col (int) col of the required cell
	  *	@param keyValue (int) required value
	  *	@return (bool) whether the value is valid or not in the 3x3 sudoku grid
	*/
	private:bool ThreeByThreeGridValid(int row, int col, int keyValue){
		int rowIter, colIter;

		int s_start[3] = {0, 3, 6};
		int s_end[3] = {3, 6, 9};
		int s_r = int(row / 3);
		int s_c = int(col / 3);


		for(rowIter = s_start[s_r]; rowIter < s_end[s_r]; rowIter++){
			for(colIter = s_start[s_c]; colIter < s_end[s_c]; colIter++){
				if(rowIter == row && colIter == col){
					continue;
				}

				if(grid.getCellValue(rowIter, colIter) == keyValue){
					return false;
				}
			}
		}

		return true;
	}

	/**
	  *	@desc Checks if the every cell of grid is valid or not.
	  *	@param none
	  *	@return (bool) whether the grid is valid or not
	*/
	private:bool gridSolved(){

		int flag;

		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				if(grid.getCellValue(i, j) == 0)
					return false;

				flag = cellValueValid(i, j, grid.getCellValue(i, j));

				if(flag == 0){
					return false;
				}
			}
		}

		return true;
	}


	/**
	  *	@desc The recursive function which does all the work, this iterates over the
	  *	possible values for the specified cell one-by-one. Once a value has been filled, it
	  *	goes to the next cell. Here, the same thing happens. If none of the possible values
	  *	work out, then the function backtracks to the previous cell.
  	  *
	  *	@param none
	  *	@return (bool) whether the value put in the cell made it a SUCCESS or NOT
	*/
	private:bool singleCellSolve(){

		statsIncrement(); //This is used to see how many times the func is called.

		// If the grid full and correct return true and break the recursive calls
		if(gridSolved())
			return true;

        //statsPrint();
        //displayGrid();
        //cin.get();

		int rowIter, colIter, keyValueIter;

		for(rowIter=0; rowIter<9; rowIter++){
			for(colIter=0; colIter<9; colIter++){

				// Check empty cells and do the rest only for them
				// If cell's value is 0, it is empty
				if(grid.getCellValue(rowIter, colIter) == 0){

					// Check all the possible numbers as keys
					for(keyValueIter=1; keyValueIter<10; keyValueIter++){

						if(cellValueValid(rowIter, colIter, keyValueIter)){

							// Assign the key value
							grid.setCellValue(rowIter, colIter, keyValueIter);

							// Calling recursive function
							if(singleCellSolve())
								return true;

							// If grid is not solved, the key is not correct one, delete it and continue
							grid.setCellValue(rowIter, colIter, 0);
						}
					}

					// If there is not any valid for that cell program should go to back stage, [backtracking]
					if(grid.getCellValue(rowIter, colIter) == 0)
						return false;
				}
			}
		}

		// Grid did not solved
		return false;

	}

	/**
	  *	@desc Calls the singleCellSolve() func and prints a success/fail mesg.
	  *	@param none
	  *	@return none
	*/
	private:void solve(){
		bool success=singleCellSolve();

		statsPrint();
		if(success)
			cout<<"QED. Your puzzle has been solved!\n\n";		// quod erat demonstrandum [what was to be shown]
		else
			cout <<"There is not any correct answer for this puzzle :( \n";
	}

	/**
	  *	@desc Displays the sudoku grid by calling the displayGrid() func of the
	  *	SudokuGrid object.
	  *	@param none
	  *	@return none
	*/
	private:void displayGrid(){
		grid.displayGrid();
	}

	/**
	  *	@desc This increments the count variable to keep track of the recursions done.
	  *	@param none
	  *	@return none
	*/
	private:void statsIncrement(){
		recursiveCount++;
	}

	/**
	  *	@desc This displays the number of times recursion has happened.
	  *	@param none
	  *	@return none
	*/
	public:void statsPrint(){
		cout<<"\nThe singleCellSolve() function was recursively called "<<recursiveCount<<" times.\n";
	}

};

int main(){
	SudokuSolver ss;
	return 0;
}
