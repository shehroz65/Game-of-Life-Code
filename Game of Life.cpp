#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;
const int row = 10;
const int col = 10;
int numofgens;
int numcells;
int Grid[col][row];
int *unInitx;
int *unInity;


int temp[col][row];
int gen = 0;
//function for printing on screen
void WriteGrid(fstream& datafile)
{
	datafile << gen++ << "th Gen" << endl;
	cout << " 0123456789-----" << endl;
	for (int y = 0; y <row; y++)
	{
		cout << y;
		for (int x = 0; x < col; x++)
		{
			//gotoxy(x, y);
			if (Grid[x][y] == 1)
			{
				datafile << "O";
				cout << "O";
			}
			else
			{
				datafile << " ";
				cout << " ";
			}
		}
		datafile << endl;
		cout << endl;
	}
}

//function to initialize Grids values to 0
void ZeroGrid()
{
	for (int x = 0; x < col ; x++)
	{
		for (int y = 0; y < row ; y++)
		{
			Grid[x][y] = 0;
			temp[x][y] = 0;
		}
	}
}

//function to read file co ordinates
void ReadConfig(string filename)
{
	fstream datafile;
	
	datafile.open(filename, ios::in); //opening the user input file in input mode
	datafile >> numofgens; // grabs the number of generations

	//if it fails to grab the numofgens from the first line then that means the file isn't available
	if (datafile.fail())
	{
		cout << "File not available " << endl;
		exit(0);
	}
	//grabs the number of cells
	datafile >> numcells;

	//dynamic arrays having size equal to num of cells, containing x and y coordinates respectively
	unInitx = new int[numcells];
	unInity = new int[numcells];
	
	//calling function to initialize Grid array
	ZeroGrid();

	//for loop for reading values from file, and to print the co ordinates on screen too
	for (int i = 0; i < numcells; i++)
	{
		datafile >> unInitx[i];
		cout << unInitx[i] << " ";
		datafile >> unInity[i];
		cout << unInity[i] << " ";
		cout << endl;		
	}
	datafile.close();
	
	//giving the respective values of the file to the Grid array to make the required cells alive
	for (int i = 0; i < numcells; i++)
	{
		Grid[unInitx[i]][unInity[i]] = 1;
	}
}

//function for counting neighbors
int count_neighbours(int x, int y)
{
	int count = 0;
	if (Grid[x - 1][y - 1] == 1)  // top left
		count++;
	if (Grid[x][y - 1] == 1) //top
		count++;
	if (Grid[x + 1][y - 1] == 1) //top right
		count++;
	if (Grid[x - 1][y] == 1) //left
		count++;
	if (Grid[x + 1][y] == 1) //right
		count++;
	if (Grid[x - 1][y + 1] == 1) //bottom left
		count++;
	if (Grid[x][y + 1] == 1) //bottom
		count++;
	if (Grid[x + 1][y + 1] == 1) //bottom right
		count++;
	return count;
}

int main(int argc, char *argv[])
{
	fstream datafile;
	string inputfile, outputfile;	

	//if the argument count is greater than 1, then the input path file is assigned to the inputfile string
	if (argc > 1)
		inputfile.assign(argv[1]);

	//if the arg count is greater than 2, then the output path file is assigned to the outputfile string
	if (argc > 2)
		outputfile.assign(argv[2]);

	
	if (inputfile == "")
	{
		cout << "Enter input file name " << endl; 
		getline(cin, inputfile);
	}
	if (outputfile == "")
	{
		cout << "Enter output file name ";
		getline(cin, outputfile);		
	}
	ReadConfig(inputfile);

	datafile.open(outputfile, ios::out);


	int minx = 700, miny = 700, maxX = 0, maxY = 0;

	// check for min, max X and Y coordinates and initialize them with values according to the co ordinates from the file
	for (int i = 0; i < numcells; i++)
	{
		if (unInitx[i] < minx)
		{
			minx = unInitx[i]-1;
		}
		if (unInity[i] < miny)
		{
			miny = unInity[i]-1;
		}
		if (unInitx[i] > maxX)
		{
			maxX = unInitx[i]+1;
		}
		if (unInity[i] > maxY)
		{
			maxY = unInity[i]+1;
		}
	}
	
	//calling function to write on screen and file
	WriteGrid(datafile);
	for (int i = 0; i < numofgens; i++)
	{
		int count = 0;
		
		for (int y = miny; y < maxY+2; y++)
		{
				
			for (int x = minx; x < maxX+2; x++)
			{
							
				count = count_neighbours(x, y);
				//..if (count != 0)
					//..count = count;
				//1. Birth: if exactly three of its neighbors are alive, the cell will become alive at the next step.		
				if ((Grid[x][y] == 0) && (count == 3))
					temp[x][y] = 1;
				//2. Survival: if the cell has two or three live neighbors, the cell remains alive.					
				else if ((Grid[x][y] == 1) && ((count == 2) || (count == 3)))
					temp[x][y] = 1; // remain alive
				//3. Death by loneliness: if the cell has only zero or one live neighbors, the cell will become dead 
				else if ((Grid[x][y] == 1) && ((count == 0) || (count == 1)))
					temp[x][y] = 0;
				//4. Death by overcrowding: if the cell alive and has more than three live neighbors, the cell also dies.
				else if ((Grid[x][y] == 1) && (count > 3))
					temp[x][y] = 0;
			}

		}
		minx = 1000;
		miny = 1000;
		maxY = 0;
		maxX = 0;

		//this for loop is used to update the min, max x and Y coordinates as the Grid is being updated.
		for (int y = 0; y < row; y++)
		{
			for (int x = 0; x < col; x++)
			{
				Grid[x][y] = temp[x][y];
				if (Grid[x][y] == 1)					
				{					
					if (x < minx-1)
						minx = x-2;					
					if (x > maxX-1)
						maxX = x+1;
					if (y < miny-1)
						miny = y-1;
					if (y > maxY-1)
						maxY = y+1;
				}
			}
			
		}
		cout << " " << i << endl;

		//writing the values to datafile and screen
		WriteGrid(datafile);
	}

	datafile.close();

	//deleting the pointer array
	delete unInitx, unInity;
	

}
