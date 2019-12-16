#include <iostream>
#include <utility>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <numeric>
#include <iterator>
#include <random>


using namespace std;
using namespace cv;

class DataLoader
{ //test mode and train mode
private:
    size_t dataset_length{0}; //root/classname/xxx.png
    
	vector<Mat> batch;
	vector<int> label;
    
	int batchsize{1};
    int num_classes{1};
    
	vector<size_t> length_classes;
    vector<vector<filesystem::path>> image_path;
    
	bool test{false};
    bool toshuffle{true}; //to be completed
    
	size_t index_track{0};
    
	vector<size_t> permutation;
    vector<filesystem::path> classpath;
    filesystem::path directory;
    std::pair<filesystem::path,int> path_generator(size_t index)
    
	{
        int class_count{0};
        for (auto p : length_classes)
        {
            if (index < p)
            {
                return std::make_pair(filesystem::path(image_path[class_count][index]),class_count);
            }
            else
            {
                ++class_count;
                index -= p;
            }
        }
    }

public:
	int getNumOfClasses()const { return num_classes; }
    size_t getdatasetsize() const { return dataset_length; }
	int getBatchSize()const { return batchsize; }
	size_t getNumOfIteration() const{ return dataset_length / batchsize; }
	DataLoader(bool test, bool toshuffle, int batchsize, filesystem::path directory);
	std::pair<Mat, Mat> operator[](size_t index) //meaning: the indexth batch
    {
        if (index * batchsize + batchsize > dataset_length)
        {
            exit(-1);
        }
        for (size_t i{index * batchsize}; i < index * batchsize + batchsize; ++i)
        {
			std::pair<filesystem::path, int> result = path_generator(i);
            Mat temp = imread(result.first.string(), IMREAD_GRAYSCALE).reshape(1, 1);
			label.push_back(result.second);
			normalize(temp, temp, 0.0, 1.0, NORM_MINMAX, CV_32F);
            batch.push_back(temp);
        }
        Mat imageBatch;
		Mat labelMatrix = Mat(label,true).t();//[]
		labelMatrix.convertTo(labelMatrix, CV_8U);
        vconcat(batch, imageBatch);
        return make_pair(imageBatch, labelMatrix);
    }
};
