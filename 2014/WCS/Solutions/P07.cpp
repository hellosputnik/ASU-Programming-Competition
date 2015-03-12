#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char ** argv)
{
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);

	int m, n;

	fin >> m;
	fin >> n;

	int ** room = new int * [m];
	for(int size = 0; size < m; size++)
	{
		room[size] = new int[n];
	}

	for(int out = 0; out < m; out++)
	{
		for(int in = 0; in < n; in++)
		{
			room[out][in] = 0;
		}
	}

	int direction, row, column;

	row = 0, column = 0, direction = 4;

	room[row][column] = 1;

	do
	{
		switch(direction)
		{
		case 1:
			if(!column)
			{
				direction = 2;
				break;
			}
			if(!row)
			{
				direction = 3;
				break;
			}
			row = row - 1, column = column - 1;
			room[row][column] ^= 1;
			break;
		case 2:
			if(column == n - 1)
			{
				direction = 1;
				break;
			}
			if(!row)
			{
				direction = 4;
				break;
			}
			row = row - 1, column = column + 1;
			room[row][column] ^= 1;
			break;
		case 3:
			if(!column)
			{
				direction = 4;
				break;
			}
			if(row == m - 1)
			{
				direction = 1;
				break;
			}
			row = row + 1, column = column - 1;
			room[row][column] ^= 1;
			break;
		case 4:
			if(column == n - 1)
			{
				direction = 3;
				break;
			}
			if(row == m - 1)
			{
				direction = 2;
				break;
			}
			row = row + 1, column = column + 1;
			room[row][column] ^= 1;
			break;
		}
	} while((row != 0 || column != 0) && (row != 0 || column != n - 1) && (row != m - 1 || column != 0) && (row != m - 1 || column != n - 1));
	
	
	int sum = 0;
	for(int out = 0; out < m; out++)
	{

		for(int in = 0; in < n; in++)
		{
			if(room[out][in])
				sum++;
		}
	}

	fout << sum << "\n";

	fin.close();
	fout.close();

	return 0;
}