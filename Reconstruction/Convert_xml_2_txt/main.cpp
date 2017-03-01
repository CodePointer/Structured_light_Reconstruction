#include <iostream>
#include <string>
#include <strstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	string trace_path = "D:/Structured_Light_Data/20170213/StatueForward/result/trace/";
	string trace_m_path = "D:/Structured_Light_Data/20170213/StatueForward/result/trace_m/";

	for (int frameNum = 1; frameNum < 50; frameNum++)
	{
		strstream ss;
		string idx2str;
		ss << frameNum;
		ss >> idx2str;

		Mat iX, iY, deltaX, deltaY;

		FileStorage fs;
		fs.open(trace_path + "trace" + idx2str + ".txt", FileStorage::READ);
		if (fs.isOpened())
		{
			fs["iX"] >> iX;
			fs["iY"] >> iY;
			fs["deltaX"] >> deltaX;
			fs["deltaY"] >> deltaY;
			fs.release();

			fstream file;
			file.open(trace_m_path + "iH" + idx2str + ".txt", ios::out);
			if (file.is_open())
			{
				Size mat_size = iX.size();
				for (int h = 0; h < mat_size.height; h++)
				{
					for (int w = 0; w < mat_size.width; w++)
					{
						file << iX.at<double>(h, w) << " ";
					}
					file << endl;
				}
				file.close();
			}
			else
			{
				printf("Error.\n");
				break;
			}
			
			file.open(trace_m_path + "iW" + idx2str + ".txt", ios::out);
			if (file.is_open())
			{
				Size mat_size = iY.size();
				for (int h = 0; h < mat_size.height; h++)
				{
					for (int w = 0; w < mat_size.width; w++)
					{
						file << iY.at<double>(h, w) << " ";
					}
					file << endl;
				}
				file.close();
			}
			else
			{
				printf("Error.\n");
				break;
			}

		}
		else
		{
			printf("Error.\n");
			break;
		}

		printf("Frame %d finished.\n", frameNum);
	}

	system("PAUSE");
	return 0;
}