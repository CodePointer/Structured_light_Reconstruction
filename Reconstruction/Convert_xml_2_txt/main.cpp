#include <iostream>
#include <fstream>
#include <string>
#include <strstream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


bool xml2txt(string xml_path, 
	string xml_name, 
	string txt_path,  
	string * mat_names,
	int num_mat_name,
	int num_frame)
{
	bool status = true;

	Mat * mats;
	mats = new Mat[num_mat_name];

	for (int idx_frame = 1; idx_frame < num_frame; idx_frame++)
	{
		strstream ss;
		string idx2str;
		ss << idx_frame;
		ss >> idx2str;

		FileStorage fs;
		fs.open(xml_path + xml_name + idx2str + ".xml", FileStorage::READ);
		if (fs.isOpened())
		{
			for (int idx_mat = 0; idx_mat < num_mat_name; idx_mat++)
			{
				fs[mat_names[idx_mat]] >> mats[idx_mat];

				fstream file;
				file.open(txt_path + mat_names[idx_mat] +idx2str + ".txt", ios::out);
				if (file.is_open())
				{
					Size mat_size = mats[idx_mat].size();
					for (int h = 0; h < mat_size.height; h++)
					{
						for (int w = 0; w < mat_size.width; w++)
						{
							file << mats[idx_mat].at<double>(h, w) << " ";
						}
						file << endl;
					}
					file.close();
				}
				else
				{
					printf("Error in open txt file.\n");
					break;
				}
			}
			fs.release();
		}
		else
		{
			printf("Error in open xml file.\n");
			break;
		}

		printf("Frame %d finished.\n", idx_frame);
	}

	delete[]mats;

	return status;
}


int main()
{
	string xml_path = "D:/Structured_Light_Data/20170213/StatueForward/result/trace/";
	string xml_name = "trace";
	string txt_path = "D:/Structured_Light_Data/20170213/StatueForward/result/trace_m/";
	string mat_names[2] = { "iH", "iW" };

	xml2txt(xml_path,
		xml_name,
		txt_path,
		mat_names,
		2,
		50);

	string mat_names_depth[1] = { "depth_mat" };

	xml2txt("D:/Structured_Light_Data/20170213/StatueForward/result/depth_mat/",
		"depth_mat",
		"D:/Structured_Light_Data/20170213/StatueForward/result/depth_mat_m/",
		mat_names_depth,
		1,
		50);

	system("PAUSE");
	return 0;
}