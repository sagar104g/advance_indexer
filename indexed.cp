l#include <iostream>
#include <fstream>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include "index.h"

using namespace std;

int main()
{
	supermap index;
	int fileno=0;
	while(1)                                   //this loop will end when all pages will be visited
	{
	std::string ifn=toString(fileno);		   //converting file name to string
	std::string filename="file"+ifn;
	filename+=".txt";
	fileno++;
	words wordMiniMap(filename);
	if(filename=="NAN")						 //if all files are visited  then this loop will break
	    {
	    std::cout<<filename;
	    break;
	    }
	wordMiniMap.getWord(index);
	std::cout<<filename;
	}
	index.savetofile();                     //after visiting all pages data will be saved to file
	return 0;
}

