#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char ** argv)
{
	int alphabet[26] = {0};

	string magazine, ransom;

	ifstream fin(argv[1]);
	ofstream fout(argv[2]);

	getline(fin, magazine);
	getline(fin, ransom);

	if(magazine.length() < ransom.length() || !magazine.length())
	{
		fout << "Not possible.\n";
		fout.close();
		return 0;
	}

	for(auto it = magazine.begin(); it != magazine.end(); it++)
	{
		if(isalpha(*it))
			alphabet[tolower(*it) - 97]++;
	}

	for(auto it = ransom.begin(); it != ransom.end(); it++)
	{
		if(!alphabet[tolower(*it) - 97])
		{
			fout << "Not possible.\n";
			fout.close();
			return 0;
		} else {
			alphabet[tolower(*it) - 97]--;
		}		
	}

	fout << "Possible.\n";

	fin.close();
	fout.close();
	
	return 0;
}