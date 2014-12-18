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

	string number;

	fin >> number;

	int a, b;

	if(number.length() % 2)
	{
		fout << "Not possible.\n";
		return 0;
	}

	string permutation = number;
	sort(permutation.begin(), permutation.end(), std::greater<char>());

	do {
		a = stoi(permutation.substr(0, permutation.length() / 2));
		b = stoi(permutation.substr(permutation.length()/2, permutation.length()/2));

		if(a * b == stoi(number))
		{
			fout << a << " " << b << "\n";
			fout.close();
			fin.close();
			return 0;
		}
	} while(prev_permutation(permutation.begin(), permutation.end()));

	fout << "Not possible.\n";

	fin.close();
	fout.close();

	return 0;
}