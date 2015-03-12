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

	int length;

	int number;
	string buffer;

	fin >> length;
	for(int n = 0; n < length; n++)
	{
		fin >> number;
		buffer.append(to_string(number));
	}

	sort(buffer.begin(), buffer.end(), std::greater<char>());

	int count = 0;
	bool success;
	do {
		success = true;
		for(int n = 0; n < buffer.length() - 1; n++)
		{
			if(!(n % 2))
			{
				if(buffer.at(n) <= buffer.at(n + 1))
				{
					success = false;
					break;
				}
			} else {
				if(buffer.at(n) >= buffer.at(n + 1))
				{
					success = false;
					break;
				}
			}
		}
		if(success)
		{
			count++;
			for(auto it = buffer.begin(); it != buffer.end(); it++)
			{
				fout << *it << " ";
			}
			fout << "\n";
		}
	} while(prev_permutation(buffer.begin(), buffer.end()));

	if(!count)
	{
		fout << "Not possible.\n";
	}

	fin.close();
	fout.close();

	return 0;
}