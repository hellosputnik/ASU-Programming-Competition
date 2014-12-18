#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char ** argv)
{
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);

	unsigned int numbers[51];

	numbers[0] = 1;
	numbers[1] = 1;

	for(int n = 2; n < 51; n++)
	{
		numbers[n] = numbers[n - 2] + numbers[n - 1];
	}

	string buffer;
	while(getline(fin, buffer))
	{
		if(buffer == "0")
			break;

		fout << numbers[stoi(buffer)] << "\n";
	}

	fin.close();
	fout.close();

	return 0;
}