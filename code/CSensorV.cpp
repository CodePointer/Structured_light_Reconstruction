#include "CSensorV.h"


// 传感器模块。
// 用于模拟传感器传入数据。
// 为方便起见，相关的数据已经离线采集好，该模块则是模拟传感器模块进行数据的读入。


CSensor::CSensor()
{
	this->m_dataNum = 0;
	this->m_nowNum = 0;
	this->m_filePath = "";
	this->m_fileName = "";
	this->m_fileSuffix = "";

	this->m_dataMats = 0;

}


CSensor::~CSensor()
{
	if (this->m_dataMats != NULL)
	{
		delete[]this->m_dataMats;
		this->m_dataMats = NULL;
	}
}


// 初始化传感器：设定一些参数
// 主要是读取的文件路径参数
bool CSensor::InitSensor()
{
	bool status = true;

	this->m_groupDataPath = DATA_PATH + "20170213\\StatueForward\\";
	this->m_dynaPath = "dyna\\";
	this->m_dynaName = "dyna_mat";
	this->m_dataFileSuffix = ".png";

	return status;
}


// 关闭传感器
bool CSensor::CloseSensor()
{
	bool status = true;
	
	this->UnloadDatas();

	return status;
}


// Read data into memory
bool CSensor::LoadDatas(int max_frame_num)
{
	// if avaliable
	if (this->m_dataMats != NULL)
	{
		this->UnloadDatas();
	}

	// Read dynaMats from files
	this->m_dataNum = max_frame_num;
	this->m_nowNum = 0;
	this->m_filePath = this->m_groupDataPath + this->m_dynaPath;
	this->m_fileName = this->m_dynaName;
	this->m_fileSuffix = this->m_dataFileSuffix;

	// Read files
	this->m_dataMats = new Mat[this->m_dataNum];
	for (int i = 0; i < this->m_dataNum; i++)
	{
		Mat tempMat;
		string idx2Str;
		strstream ss;
		ss << i ;
		ss >> idx2Str;
		
		tempMat = imread(this->m_filePath
			+ this->m_fileName
			+ idx2Str
			+ this->m_fileSuffix, CV_LOAD_IMAGE_GRAYSCALE);
		tempMat.copyTo(this->m_dataMats[i]);

		/*cout << this->m_filePath
			+ this->m_fileName
			+ idx2Str
			+ this->m_fileSuffix << endl;*/

		if (tempMat.empty())
		{
			ErrorHandling("CSensor::LoadPatterns::<Read>, imread error: "
				+ this->m_filePath
				+ this->m_fileName
				+ idx2Str
				+ this->m_fileSuffix);
		}
	}

	return true;
}


// Release data
bool CSensor::UnloadDatas()
{
	if (this->m_dataMats != NULL)
	{
		delete[]this->m_dataMats;
		this->m_dataMats = NULL;
	}
	
	this->m_dataNum = 0;
	this->m_nowNum = 0;
	this->m_filePath = "";
	this->m_fileName = "";
	this->m_fileSuffix = "";

	return true;
}


// 设置投影仪投影的图像
bool CSensor::SetProPicture(int nowNum)
{
	bool status = true;

	// 检查参数是否合法
	if (nowNum >= this->m_dataNum)
	{
		status = false;
		return status;
	}

	this->m_nowNum = nowNum;

	return status;
}


// 获取相机图像
Mat CSensor::GetCamPicture()
{
	bool status = true;

	Mat tempMat;
	this->m_dataMats[this->m_nowNum].copyTo(tempMat);

	return tempMat;
}