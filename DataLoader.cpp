#include "DataLoader.h"

DataLoader::DataLoader(bool test, 
                       bool toshuffle, 
					   int batchsize, 
					       filesystem::path directory) 
					  : directory(directory), 
					    toshuffle(toshuffle), 
						batchsize(batchsize)
{
	if (test)
	{ //test mode
		directory /= "testing";
	}
	else
	{ //train mode
		directory /= "training";
	}
    //****************
	for (vector<int>::const_iterator &p : filesystem::directory_iterator(directory))
	{
		//
		classpath.push_back(p.path());
	}

	num_classes = classpath.size();

	for (vector<filesystem::path>::const_iterator &path : classpath)
	{
		vector<filesystem::path> subpath;
		size_t class_entry_count{0};
		
		//***************
		for (vector<int>::const_iterator &file : filesystem::directory_iterator(path))
		{
			++class_entry_count;
			subpath.push_back(file.path());
		}
		//using filesystem::directory_iterator;
		//class_entry_count = distance(directory_iterator(path), directory_iterator{});
		length_classes.push_back(class_entry_count);
		image_path.push_back(subpath);
	}

	for (vector<size_t>::const_iterator &p : length_classes)
	{
		dataset_length += p;
	}

	permutation.resize(dataset_length);
	iota(permutation.begin(), permutation.end(), 0); //fill the vector with 0, 1, 2, ... , dataset_length-1
		
	if (toshuffle)
	{
		random_device rd;
		default_random_engine generator(rd());
		shuffle(permutation.begin(), permutation.end(), generator);
	}
}
