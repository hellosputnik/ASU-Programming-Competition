#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char ** argv)
{
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);

	int stacks;
	int * sheets;
	int total;

	fin >> stacks;

	sheets = new int[stacks];
	for(int n = 0; n < stacks; n++)
	{
		fin >> sheets[n];
	}

	fin >> total;

	int count = 0;

	int a, b;
	for(a = 0; a < stacks - 1; a++)
	{
		b = a + 1;
		while(b < stacks)
		{
			if(sheets[a] + sheets[b] == total)
			{
				fout << a << " " << b << "\n";
				count++;
			}
			b++;
		}
	}

	if(!count)
		fout << "Not possible.\n";

	fin.close();
	fout.close();

	return 0;
}