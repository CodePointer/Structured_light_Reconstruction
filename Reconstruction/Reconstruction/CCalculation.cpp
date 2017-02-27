#include "CCalculation.h"

using namespace cv;
using namespace std;

CVisualization myDebug("Debug");


CCalculation::CCalculation()
{
	this->m_sensor = NULL;
	this->m_iPro = NULL;
	this->m_jPro = NULL;

	this->m_lineA = NULL;
	this->m_lineB = NULL;
	this->m_lineC = NULL;

	this->m_xMat = NULL;
	this->m_yMat = NULL;
	this->m_zMat = NULL;

	this->m_iH = NULL;
	this->m_iW = NULL;
	this->m_deltaH = NULL;
	this->m_deltaW = NULL;
}


CCalculation::~CCalculation()
{
	this->ReleaseSpace();
}


bool CCalculation::ReleaseSpace()
{
	if (this->m_sensor != NULL)
	{
		this->m_sensor->UnloadDatas();
		delete(this->m_sensor);
		this->m_sensor = NULL;
	}
	if (this->m_iPro != NULL)
	{
		delete[](this->m_iPro);
		this->m_iPro = NULL;
	}
	if (this->m_jPro != NULL)
	{
		delete[](this->m_jPro);
		this->m_jPro = NULL;
	}

	if (this->m_lineA != NULL)
	{
		delete[](this->m_lineA);
		this->m_lineA = NULL;
	}
	if (this->m_lineB != NULL)
	{
		delete[](this->m_lineB);
		this->m_lineB = NULL;
	}
	if (this->m_lineC != NULL)
	{
		delete[](this->m_lineC);
		this->m_lineC = NULL;
	}

	if (this->m_xMat != NULL)
	{
		delete[](this->m_xMat);
		this->m_xMat = NULL;
	}
	if (this->m_yMat != NULL)
	{
		delete[](this->m_yMat);
		this->m_yMat = NULL;
	}
	if (this->m_zMat != NULL)
	{
		delete[](this->m_zMat);
		this->m_zMat = NULL;
	}

	if (this->m_iH != NULL)
	{
		delete[](this->m_iH);
		this->m_iH = NULL;
	}
	if (this->m_iW != NULL)
	{
		delete[](this->m_iW);
		this->m_iW = NULL;
	}
	if (this->m_deltaH != NULL)
	{
		delete[](this->m_deltaH);
		this->m_deltaH = NULL;
	}
	if (this->m_deltaW != NULL)
	{
		delete[](this->m_deltaW);
		this->m_deltaW = NULL;
	}

	return true;
}


bool CCalculation::Init()
{
	bool status = true;

	printf("Initialization:\n");

	// 确保参数合法
	if ((this->m_sensor != NULL))
		return false;

	// 设定相关路径参数
	this->inner_para_path_ = "";
	this->inner_para_name_ = "parameters";
	this->inner_para_suffix_ = ".yml";
	FileStorage fs;
	fs.open(CONFIG_PATHNAME, FileStorage::READ);
	if (!fs.isOpened())
	{
		status = false;
		printf("Error in find config.xml: %s", CONFIG_PATHNAME);
	}
	else
	{
		string main_path;
		string data_set_path;
		fs["main_path"] >> main_path;
		fs["data_set_path"] >> data_set_path;

		string tmp;
		fs["ipro_mat_path"] >> tmp;
		this->ipro_mat_path_ = main_path + data_set_path + tmp;
		fs["ipro_mat_name"] >> this->ipro_mat_name_;
		fs["ipro_mat_suffix"] >> this->ipro_mat_suffix_;

		fs["point_cloud_path"] >> tmp;
		this->point_cloud_path_ = main_path + data_set_path + tmp;
		fs["point_cloud_name"] >> this->point_cloud_name_;
		fs["point_cloud_suffix"] >> this->point_cloud_suffix_;

		fs["point_show_path"] >> tmp;
		this->point_show_path_ = main_path + data_set_path + tmp;
		fs["point_show_name"] >> this->point_show_name_;
		fs["point_show_suffix"] >> this->point_show_suffix_;

		fs["trace_path"] >> tmp;
		this->trace_path_ = main_path + data_set_path + tmp;
		fs["trace_name"] >> this->trace_name_;
		fs["trace_suffix"] >> this->trace_suffix_;

		fs.release();
	}

	// 根据数据设定视口大小：
	this->m_hBegin = 450;
	this->m_hEnd = 650;
	this->m_wBegin = 700;
	this->m_wEnd = 900;

	// 创建新元素
	printf("\tCreating sensor...");
	if (status)
	{
		this->m_sensor = new CSensor;
		status = this->m_sensor->InitSensor();
	}
	if (status)
	{
		status = this->m_sensor->LoadDatas(DYNAFRAME_MAXNUM);
	}
	printf("finished.\n");

	// 申请空间:uxyz
	printf("\tAllocating pointers...");
	this->m_xMat = new Mat[DYNAFRAME_MAXNUM];
	this->m_yMat = new Mat[DYNAFRAME_MAXNUM];
	this->m_zMat = new Mat[DYNAFRAME_MAXNUM];
	this->m_iPro = new Mat[DYNAFRAME_MAXNUM];
	this->m_jPro = new Mat[1];
	this->m_lineA = new Mat[1];
	this->m_lineB = new Mat[1];
	this->m_lineC = new Mat[1];
	this->m_iH = new Mat[DYNAFRAME_MAXNUM];
	this->m_iW = new Mat[DYNAFRAME_MAXNUM];
	this->m_deltaH = new Mat[DYNAFRAME_MAXNUM];
	this->m_deltaW = new Mat[DYNAFRAME_MAXNUM];
	printf("finished.\n");

	printf("\tAllocating mats...");
	for (int i = 0; i < DYNAFRAME_MAXNUM; i++)
	{
		this->m_xMat[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_yMat[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_zMat[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_iPro[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		
		
		this->m_iH[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_iW[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_deltaH[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_deltaW[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	}
	this->m_jPro[0].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_lineA[0].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_lineB[0].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_lineC[0].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	printf("finished.\n");
	
	// 导入标定的系统参数
	printf("\tImporting system parameters...");
	fs.open(this->inner_para_path_ 
		+ this->inner_para_name_ 
		+ this->inner_para_suffix_, FileStorage::READ);
	fs["CamMat"] >> this->m_camMatrix;
	fs["ProMat"] >> this->m_proMatrix;
	fs["R"] >> this->m_R;
	fs["T"] >> this->m_T;
	fs.release();

	// 填充C
	this->m_C.create(3, 4, CV_64FC1);
	this->m_C.setTo(0);
	this->m_camMatrix.copyTo(this->m_C.colRange(0, 3));
	//cout << this->m_C << endl;
	
	// 填充P
	Mat temp;
	temp.create(3, 4, CV_64FC1);
	this->m_R.copyTo(temp.colRange(0, 3));
	this->m_T.copyTo(temp.colRange(3, 4));
	this->m_P = this->m_proMatrix * temp;
	/*cout << this->m_proMatrix << endl;
	cout << temp << endl;
	cout << this->m_P << endl;*/

	// 填充ABCD参数
	this->m_cA = this->m_C.at<double>(0, 0) * this->m_C.at<double>(1, 1) * this->m_P.at<double>(0, 3);
	this->m_cB = this->m_C.at<double>(0, 0) * this->m_C.at<double>(1, 1) * this->m_P.at<double>(2, 3);
	this->m_cC.create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_cD.create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	for (int u = 0; u < CAMERA_RESLINE; u++)
	{
		for (int v = 0; v < CAMERA_RESROW; v++)
		{
			this->m_cC.at<double>(v, u) = (u - this->m_C.at<double>(0, 2))*this->m_C.at<double>(1, 1) * this->m_P.at<double>(0, 0)
				+ (v - this->m_C.at<double>(1, 2))*this->m_C.at<double>(0, 0) * this->m_P.at<double>(0, 1)
				+ this->m_C.at<double>(0, 0) * this->m_C.at<double>(1, 1) * this->m_P.at<double>(0, 2);
			this->m_cD.at<double>(v, u) = (u - this->m_C.at<double>(0, 2))*this->m_C.at<double>(1, 1) * this->m_P.at<double>(2, 0)
				+ (v - this->m_C.at<double>(1, 2))*this->m_C.at<double>(0, 0) * this->m_P.at<double>(2, 1)
				+ this->m_C.at<double>(0, 0) * this->m_C.at<double>(1, 1) * this->m_P.at<double>(2, 2);
		}
	}
	printf("finished.\n");

	return true;
}


bool CCalculation::CalculateFirst()
{
	bool status = true;

	// 判断参数是否合法
	if ((this->m_sensor == NULL))
		return false;
	if (this->m_C.empty() || this->m_P.empty())
		return false;

	printf("First frame:\n");

	// 根据数值计算ProjectorU
	if (status)
	{
		printf("\tFillFirstProjectorU...");
		status = this->FillFirstProjectorU();
		printf("finished.\n");
	}
	
	//myDebug.Show(this->m_iPro[0], 0, true, 0.5);

	// 根据ProjectorU和参数计算空间坐标
	if (status)
	{
		printf("\tFillCoordinate...");
		status = this->FillCoordinate(0);
		printf("finished.\n");
	}

	// 根据数值填充jProjector
	if (status)
	{
		printf("\tFiiljPro...");
		status = this->FilljPro(0);
		printf("finished.\n");
	}

	myDebug.ShowPeriod(this->m_iPro[0], 100, 0.5, true, 
		this->point_show_path_
		+ this->point_show_name_
		+ "0"
		+ this->point_show_suffix_
		);

	// 计算每个点的极线值
	if (status)
	{
		printf("\tCalculate epipolar line...");
		status = this->CalculateEpipolarLine();
		printf("finished.\n");
	}

	// 输出、保存结果
	if (status)
	{
		printf("\tBegin writing...");
		/*status = this->Result(DATA_PATH
			+ this->m_resPath
			+ this->m_pcPath
			+ this->m_pcName
			+ "0"
			+ this->m_pcSuffix, 0, false);*/
		printf("finished.\n");
	}
	
	// 进行第一帧的准备工作：填充初始的iX,iY对应关系
	if (status)
	{
		printf("\tFilling iX & iY...");
		this->TrackPoints(0);
		printf("finished.\n");
	}
	
	return true;
}


bool CCalculation::CalculateOther()
{
	bool status = true;

	// 判断参数是否合法
	if ((this->m_sensor == NULL))
		return false;
	if (this->m_C.empty() || this->m_P.empty())
		return false;

	// 逐帧进行计算：
	for (int frameNum = 1; frameNum < DYNAFRAME_MAXNUM; frameNum += 1)
	{
		string idx2str;
		stringstream ss;
		ss << frameNum;
		ss >> idx2str;

		printf("No.%d frame:\n", frameNum);

		// 对视口中的每个点进行追踪：
		if (status)
		{
			printf("\tPointTracking:");
			status = this->TrackPoints(frameNum);
			printf("finished.\n");
		}
		
		// 追踪之后填充每个点
		if (status)
		{
			printf("\tFillOtherDeltaProU:");
			this->FillOtherProU(frameNum);
			printf("finished.\n");
		}

		// 使用对应之后的点进行还原
		if (status)
		{
			printf("\tFillCoordinate:");
			this->FillCoordinate(frameNum);
			printf("finished.\n");
		}

		// 测试输出：
		myDebug.ShowPeriod(this->m_iPro[frameNum], 100, 0.5, true,
			this->point_show_path_
			+ this->point_show_name_
			+ idx2str
			+ this->point_show_suffix_
		);

		// 显示
		//Mat colorShow;
		//colorShow = Mat::zeros(temp.size(), CV_8UC3);
		//for (int h = 0; h < hTo - hFrom; h++)
		//{
		//	for (int w = 0; w < wTo - wFrom; w++)
		//	{
		//		double value = temp.at<double>(h, w);
		//		
		//		if (value > 0)
		//		{
		//			// 蓝移
		//			uchar uVal = uchar(value * 255 / 6);
		//			colorShow.at<Vec3b>(h, w) = Vec3b(255, 255 - uVal, 255 - uVal);
		//		}
		//		else if (value < 0)
		//		{
		//			// 红移
		//			uchar uVal = uchar(-value * 255 / 6);
		//			colorShow.at<Vec3b>(h, w) = Vec3b(255 - uVal, 255 - uVal, 255);
		//		}
		//		else
		//		{
		//			colorShow.at<Vec3b>(h, w) = Vec3b(255, 255, 255);
		//		}
		//	}
		//}

		//myDebug.Show(colorShow, 200, false, 1.0, true, DATA_PATH  + this->m_resPath + "DeltaZImg\\" + "dZ" + idx2str + ".jpg");

		/*fstream file;
		file.open(DATA_PATH + this->m_resPath + "DeltaZ\\" + "dZ" + idx2str + ".txt", ios::out);
		if (!file)
		{
		system("PAUSE");
		return false;
		}
		for (int h = hFrom; h < hTo; h++)
		{
		for (int w = wFrom; w < wTo; w++)
		{
		file << temp.at<double>(h - hFrom, w - wFrom) << " ";
		}
		file << endl;
		}
		file.close();*/


		// 保存数据
		if (status)
		{
			printf("\tWriteData...");
			this->Result(this->point_cloud_path_
				+ this->point_cloud_name_
				+ idx2str
				+ this->point_cloud_suffix_, frameNum, false);
			printf("finished.\n");
		}
		
	}

	return true;
}


// 存储、记录结果
bool CCalculation::Result(string fileName, int i, bool view_port_only)
{
	fstream file;
	file.open(fileName, ios::out);
	if (!file)
	{
		ErrorHandling("CCalculation::Result() OpenFile Error:" + fileName);
		return false;
	}

	int uFrom, uTo, vFrom, vTo;
	if (view_port_only)
	{
		vFrom = this->m_hBegin;
		vTo = this->m_hEnd;
		uFrom = this->m_wBegin;
		uTo = this->m_wEnd;
	}
	else
	{
		uFrom = 0;
		uTo = CAMERA_RESLINE;
		vFrom = 0;
		vTo = CAMERA_RESROW;
	}

	for (int u = uFrom; u < uTo; u++)
	{
		for (int v = vFrom; v < vTo; v++)
		{
			// 根据z值进行筛选
			double valZ = this->m_zMat[i].at<double>(v, u);
			if ((valZ < FOV_MIN_DISTANCE) || (valZ > FOV_MAX_DISTANCE))
			{
				continue;
			}

			// 输出
			file << this->m_xMat[i].at<double>(v, u) << ' ';
			file << this->m_yMat[i].at<double>(v, u) << ' ';
			file << this->m_zMat[i].at<double>(v, u) << endl;
			//printf("(%f, %f, %f)\n", this->m_xMat.at<double>(v, u), this->m_yMat.at<double>(v, u), this->m_zMat.at<double>(v, u));
		}
	}

	// 保存iX,iY,deltaX,deltaY
	if (i >= 0)
	{
		string idx2str;
		stringstream ss;
		ss << i;
		ss >> idx2str;

		// 提取视口
		Mat iXv = this->m_iH[i](Range(this->m_hBegin, this->m_hEnd), Range(this->m_wBegin, this->m_wEnd));
		Mat iYv = this->m_iW[i](Range(this->m_hBegin, this->m_hEnd), Range(this->m_wBegin, this->m_wEnd));
		Mat deltaXv = this->m_deltaH[i](Range(this->m_hBegin, this->m_hEnd), Range(this->m_wBegin, this->m_wEnd));
		Mat deltaYv = this->m_deltaW[i](Range(this->m_hBegin, this->m_hEnd), Range(this->m_wBegin, this->m_wEnd));

		FileStorage fs((this->trace_path_ + this->trace_name_ + idx2str + this->trace_suffix_), FileStorage::WRITE);
		fs << "iX" << iXv;
		fs << "iY" << iYv;
		fs << "deltaX" << deltaXv;
		fs << "deltaY" << deltaYv;
		fs.release();
	}


	file.close();
	return true;
}


// 完成第一帧的vGray和vPhase解码工作，还原出P_0。原理等同于标定中的过程
bool CCalculation::FillFirstProjectorU()
{
	bool status = true;

	// 创建临时变量
	Mat sensorMat;
	Mat vGrayMat;
	Mat vPhaseMat;
	Mat vProjectorMat;

	FileStorage fs(this->ipro_mat_path_
		+ this->ipro_mat_name_
		+ "0"
		+ this->ipro_mat_suffix_, FileStorage::READ);
	fs["ipro_mat"] >> vProjectorMat;
	fs.release();

	// 填充部分细小缝隙
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 1; w < CAMERA_RESLINE - 1; w++)
		{
			double val = vProjectorMat.at<double>(h, w);
			double val_left = vProjectorMat.at<double>(h, w - 1);
			double val_right = vProjectorMat.at<double>(h, w + 1);

			if ((val < 0) && (val_left > 0) && (val_right > 0))
			{
				vProjectorMat.at<double>(h, w) = (val_left + val_right) / 2;
			}
		}
	}

	vProjectorMat.copyTo(this->m_iPro[0]);

	return true;
}


// 已有iPro的情况下，填充jPro
bool CCalculation::FilljPro(int fN)
{
	bool status = true;

	Mat Rt;
	Rt.create(4, 4, CV_64FC1);
	Rt.setTo(0);
	this->m_R.copyTo(Rt.rowRange(0, 3).colRange(0, 3));
	this->m_T.copyTo(Rt.rowRange(0, 3).colRange(3, 4));
	Rt.at<double>(3, 3) = 1;

	Mat Pm;
	Pm.create(3, 4, CV_64FC1);
	Pm.setTo(0);
	this->m_proMatrix.copyTo(Pm.colRange(0, 3));

	Mat Pp_oc;
	Pp_oc.create(4, 1, CV_64FC1);
	Mat Pp_op;
	Mat Pp_p;

	// 逐个像素点筛查
	Mat PmRt = Pm * Rt;
	this->m_jPro[fN].setTo(-100);
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			// 判断是否有值
			double z = this->m_zMat[fN].at<double>(h, w);
			if (z < 0)
			{
				continue;
			}
			double x = this->m_xMat[fN].at<double>(h, w);
			double y = this->m_yMat[fN].at<double>(h, w);

			// 填充相机世界坐标系中的3D点坐标
			Pp_oc.at<double>(0, 0) = x;
			Pp_oc.at<double>(1, 0) = y;
			Pp_oc.at<double>(2, 0) = z;
			Pp_oc.at<double>(3, 0) = 1.0;

			// 转换到投影仪世界坐标系中

			// 投影到投影仪2D坐标系中
			Pp_p = PmRt * Pp_oc;

			// 填充j
			double i = Pp_p.at<double>(0, 0);
			double j = Pp_p.at<double>(1, 0);
			double omega = Pp_p.at<double>(2, 0);
			this->m_jPro[fN].at<double>(h, w) = j / omega;
		}
	}

	return status;
}


// 计算每个点对应的极线
bool CCalculation::CalculateEpipolarLine()
{
	bool status = true;

	// 计算相关矩阵
	Mat Rt;
	Rt.create(4, 4, CV_64FC1);
	Rt.setTo(0);
	this->m_R.copyTo(Rt.rowRange(0, 3).colRange(0, 3));
	this->m_T.copyTo(Rt.rowRange(0, 3).colRange(3, 4));
	Rt.at<double>(3, 3) = 1;

	Mat Rt_1;
	Rt_1 = Rt.inv();

	Mat Cm;
	Cm.create(3, 4, CV_64FC1);
	Cm.setTo(0);
	this->m_camMatrix.copyTo(Cm.colRange(0, 3));

	Mat CmRt_1;
	CmRt_1 = Cm * Rt_1;

	// 投影投影仪原点到相机平面
	Mat Op_op;
	Op_op.create(4, 1, CV_64FC1);
	Op_op.at<double>(0, 0) = 0;
	Op_op.at<double>(1, 0) = 0;
	Op_op.at<double>(2, 0) = 0;
	Op_op.at<double>(3, 0) = 1;
	Mat Op_oc;
	Op_oc = CmRt_1 * Op_op;
	double x1 = Op_oc.at<double>(0, 0) / Op_oc.at<double>(2, 0);
	double y1 = Op_oc.at<double>(1, 0) / Op_oc.at<double>(2, 0);

	// 逐个像素查询其在相机空间中的投影
	Mat Pp_op, Pp_oc;
	Pp_op.create(4, 1, CV_64FC1);
	double assumeZ = 1.0;
	this->m_lineA[0].setTo(0);
	this->m_lineB[0].setTo(0);
	this->m_lineC[0].setTo(1);
	double di = this->m_proMatrix.at<double>(0, 2);
	double dj = this->m_proMatrix.at<double>(1, 2);
	double fi = this->m_proMatrix.at<double>(0, 0);
	double fj = this->m_proMatrix.at<double>(1, 1);
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			// 假设Z的情况下，计算对应的X,Y,Z
			double i = this->m_iPro[0].at<double>(h, w);
			if (i < 0)
			{
				continue;
			}
			double j = this->m_jPro[0].at<double>(h, w);
			double x = assumeZ * (i - di) / fi;
			double y = assumeZ * (j - dj) / fj;
			double z = assumeZ;
			Pp_op.at<double>(0, 0) = x;
			Pp_op.at<double>(1, 0) = y;
			Pp_op.at<double>(2, 0) = z;
			Pp_op.at<double>(3, 0) = 1.0;

			// 投影至相机平面
			Pp_oc = CmRt_1 * Pp_op;

			// 计算极线：ABC
			double x2 = Pp_oc.at<double>(0, 0) / Pp_oc.at<double>(2, 0);
			double y2 = Pp_oc.at<double>(1, 0) / Pp_oc.at<double>(2, 0);
			double A = y2 - y1;
			double B = x1 - x2;
			double C = x2 * y1 -x1 * y2;
			this->m_lineA[0].at<double>(h, w) = A;
			this->m_lineB[0].at<double>(h, w) = B;
			this->m_lineC[0].at<double>(h, w) = C;
		}
	}

	return status;
}


// 填充后续帧中的ProU。直接将视口中的每个点映射到后续帧中。
bool CCalculation::FillOtherProU(int fN)
{
	bool status = true;

	// 对于视口中每个点，找到在k时刻对应的像素位置
	
	// 填充ProU[k]
	this->m_iPro[fN].setTo(-100);
	for (int x0 = this->m_hBegin; x0 < this->m_hEnd; x0++)
	{
		for (int y0 = this->m_wBegin; y0 < this->m_wEnd; y0++)
		{
			int xk = (int)this->m_iH[fN].at<double>(x0, y0);
			int yk = (int)this->m_iW[fN].at<double>(x0, y0);
			/*printf("\t\txk = %d, yk = %d\n", xk, yk);
			printf("\t\tx0 = %d, y0 = %d\n", x0, y0);*/
			this->m_iPro[fN].at<double>(xk, yk) = this->m_iPro[0].at<double>(x0, y0);
		}
	}

	return status;
}


// 根据系统的三角约束，完成ProjectorU至Z的计算
bool CCalculation::FillCoordinate(int i)
{
	double min = 60;
	double max = 0;

	// 根据ip坐标求得深度z
	this->m_zMat[i].setTo(-100);
	for (int u = 0; u < CAMERA_RESLINE; u++)
	{
		for (int v = 0; v < CAMERA_RESROW; v++)
		{
			double z = -100;
			// 如果是没有值的部分，z就暂时不填
			if (this->m_iPro[i].at<double>(v, u) < 0)
			{
				z = -100;
				continue;
			}
			// 有值的部分，计算深度
			else
			{
				z = -(this->m_cA - this->m_cB*this->m_iPro[i].at<double>(v, u))
					/ (this->m_cC.at<double>(v, u) - this->m_cD.at<double>(v, u)*this->m_iPro[i].at<double>(v, u));
			}

			// 判断z是否在合法范围内
			if ((z < FOV_MIN_DISTANCE) || (z > FOV_MAX_DISTANCE))
			{
				z = -100;
				continue;
			}

			// 可视化部分：收集最大、最小值
			if (z < min)
			{
				min = z;
			}
			if (z > max)
			{
				max = z;
			}

			this->m_zMat[i].at<double>(v, u) = z;
		}
	}

	// 根据z求得x和y
	for (int u = 0; u < CAMERA_RESLINE; u++)
	{
		for (int v = 0; v < CAMERA_RESROW; v++)
		{
			// x = z * u_c/f_u
			double z = this->m_zMat[i].at<double>(v, u);
			if (z < 0)
			{
				continue;
			}
			double uc = u - this->m_C.at<double>(0, 2);
			double vc = v - this->m_C.at<double>(1, 2);
			double fu = this->m_C.at<double>(0, 0);
			double fv = this->m_C.at<double>(1, 1);
			this->m_xMat[i].at<double>(v, u) = z * uc / fu;
			this->m_yMat[i].at<double>(v, u) = z * vc / fv;
			
			//printf("(%f, %f, %f)\n", this->m_xMat.at<double>(v, u), this->m_yMat.at<double>(v, u), this->m_zMat.at<double>(v, u));
		}
	}
	/*if (i > 0)
	{
		this->m_deltaZ[i] = this->m_zMat[i] - this->m_zMat[i - 1];
	}*/
	
	//Mat temp;
	//Mat zTemp = (this->m_zMat[i] - min) / (max - min) * 255;
	//zTemp.convertTo(temp, CV_16U);
	//imwrite("test.bmp", temp);

	//myDebug.Show(this->m_zMat[i], 100, true, 0.5);

	return true;
}


// 填充iX和iY，deltaX和deltaY
bool CCalculation::TrackPoints(int frameNum)
{
	this->m_iH[frameNum].setTo(0);
	this->m_iW[frameNum].setTo(0);
	this->m_deltaH[frameNum].setTo(0);	// 蛋疼，反了，最后翻过来了一次，函数内部这里是正确的
	this->m_deltaW[frameNum].setTo(0);

	if (frameNum == 0)
	{
		// 第0帧，初帧情况：
		for (int h0 = this->m_hBegin; h0 < this->m_hEnd; h0++)
		{
			for (int w0 = this->m_wBegin; w0 < this->m_wEnd; w0++)
			{
				this->m_iH[frameNum].at<double>(h0, w0) = (double)h0;
				this->m_iW[frameNum].at<double>(h0, w0) = (double)w0;
			}
		}
	}
	else
	{
		// 后续帧情况

		// 计算比较帧帧数
		int compare_frame = 1;
		compare_frame = frameNum - (frameNum % 5);
		if (frameNum % 5 == 0)
		{
			compare_frame -= 5;
		}

		// 获取图像，0时刻的iniGreyMat，t时刻的nowGreyMat
		//this->m_sensor->SetProPicture(0); // 目前使用的是和0时刻对比
		this->m_sensor->SetProPicture(compare_frame); // 目前使用的是和某个比较帧对比
		Mat tmpMat, iniGreyMat, nowGreyMat;
		tmpMat = this->m_sensor->GetCamPicture();
		tmpMat.copyTo(iniGreyMat);
		this->m_sensor->SetProPicture(frameNum);
		tmpMat = this->m_sensor->GetCamPicture();
		tmpMat.copyTo(nowGreyMat);
		//myDebug.Show(nowGreyMat, 100, false);

		// 设定参数
		int sHalfWin = (SEARCH_WINDOW_SIZE - 1) / 2;
		int mHalfWin = (MATCH_WINDOW_SIZE - 1) / 2;
		int hk_sBegin, hk_sEnd, wk_sBegin, wk_sEnd;		// xk,yk的搜索范围（给定中心点）
		Mat xk_1_mMat;
		int hk_1_mBegin, hk_1_mEnd, wk_1_mBegin, wk_1_mEnd;
		//Mat x0_mMat;
		//int h0_mBegin, h0_mEnd, w0_mBegin, w0_mEnd;		// 0时刻的匹配矩阵
		Mat xk_mMat;
		int wk_mBegin, wk_mEnd;		// k时刻的匹配矩阵

		double hSize = this->m_hEnd - this->m_hBegin;
		int k = 0;

		double maxMinVal = 0;
		for (int h0 = this->m_hBegin; h0 < this->m_hEnd; h0++)
		{
			if (h0%10 == 0)
			{
				cout << '.';
			}
			for (int w0 = this->m_wBegin; w0 < this->m_wEnd; w0++)
			{
				if (this->m_iPro[0].at<double>(h0, w0) <= 0)
				{
					// 这会导致部分点跟踪出错。就是一直原地趴窝 (WaitForFixed)
					continue;
				}

				/*if ((h0 == 405) && (w0 == 632) && (frameNum == 4))
				{
					cout << endl;
				}*/

				// 获取xk，yk的搜索中心并圈定搜索范围
				//int hk_1 = (int)this->m_iH[0].at<double>(h0, w0);
				//int wk_1 = (int)this->m_iW[0].at<double>(h0, w0);
				int hk_1 = (int)this->m_iH[compare_frame].at<double>(h0, w0);
				int wk_1 = (int)this->m_iW[compare_frame].at<double>(h0, w0);
				hk_sBegin = hk_1 - sHalfWin;	// 目前的搜索范围仅为一个邻域
				hk_sEnd = hk_1 + sHalfWin + 1;
				wk_sBegin = wk_1 - sHalfWin;
				wk_sEnd = wk_1 + sHalfWin + 1;
				
				// 获取k-1时刻的，中心为xk-1，yk-1的匹配矩阵
				hk_1_mBegin = h0 - mHalfWin;
				hk_1_mEnd = h0 + mHalfWin + 1;
				wk_1_mBegin = w0 - mHalfWin;
				wk_1_mEnd = w0 + mHalfWin + 1;
				tmpMat = iniGreyMat(Range(hk_1_mBegin, hk_1_mEnd), Range(wk_1_mBegin, wk_1_mEnd));
				tmpMat.convertTo(xk_1_mMat, CV_64FC1);

				// 灰度归一化
				double min_val, max_val;
				minMaxLoc(xk_1_mMat, &min_val, &max_val);
				xk_1_mMat = (xk_1_mMat - min_val) / (max_val - min_val) * 255;

				// 获取0时刻的，中心为x0,y0的匹配矩阵
				//h0_mBegin = h0 - mHalfWin;
				//h0_mEnd = h0 + mHalfWin + 1;
				//w0_mBegin = w0 - mHalfWin;
				//w0_mEnd = w0 + mHalfWin + 1;
				//tmpMat = iniGreyMat(Range(h0_mBegin, h0_mEnd), Range(w0_mBegin, w0_mEnd));
				////myDebug.Show(tmpMat, 0, false, 10.0);
				//tmpMat.convertTo(x0_mMat, CV_64FC1);

				// 创建一个存储数据的矩阵
				Mat resMat(2, wk_sEnd - wk_sBegin, CV_64FC1);
				Mat hMat(2, wk_sEnd - wk_sBegin, CV_16UC1);
				resMat.setTo(65536.0);
				hMat.setTo(0);

				// 在范围内进行查找匹配
				double A = this->m_lineA[0].at<double>(h0, w0);
				double B = this->m_lineB[0].at<double>(h0, w0);
				double C = this->m_lineC[0].at<double>(h0, w0);
				double N = (double)SEARCH_WINDOW_SIZE * (double)SEARCH_WINDOW_SIZE;

				// 设定最小值阈值
				Point minLoc;// Loc.x:W, Loc.y:H
				double minVal = 99999999.0;
				double kMatchThrehold = 0.0;

				for (int w = wk_sBegin; w < wk_sEnd; w++)
				{
					wk_mBegin = w - mHalfWin;
					wk_mEnd = w + mHalfWin + 1;

					//// 检查目标点是否有值
					//if (A == 0 && B == 0)
					//{
					//	resMat.at<double>(0, w - wk_sBegin) = 65536.0;
					//	resMat.at<double>(1, w - wk_sBegin) = 65536.0;
					//	hMat.at<ushort>(0, w - wk_sBegin) = -100;
					//	hMat.at<ushort>(1, w - wk_sBegin) = -100;
					//	break;
					//}

					// 根据极限约束，考察对应的h值（极线的上、下）
					double h = (-A / B)*w + (-C / B);
					int h1 = (int)h;
					int h2 = (int)h + 1;

					// h1进行匹配
					int h1_mBegin = h1 - mHalfWin;
					int h1_mEnd = h1 + mHalfWin + 1;
					tmpMat = nowGreyMat(Range(h1_mBegin, h1_mEnd), Range(wk_mBegin, wk_mEnd));
					//test->Show(tmpMat, 100, false, 10.0);
					tmpMat.convertTo(xk_mMat, CV_64FC1);
					// 归一化
					minMaxLoc(xk_mMat, &min_val, &max_val);
					xk_mMat = (xk_mMat - min_val) / (max_val - min_val) * 255;
					tmpMat = (xk_mMat - xk_1_mMat).mul((xk_mMat - xk_1_mMat), 1.0 / N);
					//tmpMat = (xk_mMat - x0_mMat).mul((xk_mMat - x0_mMat), 1.0 / N);
					Scalar err = sum(tmpMat);
					resMat.at<double>(0, w - wk_sBegin) = err.val[0];
					hMat.at<ushort>(0, w - wk_sBegin) = h1;
					/*if ((h0 == 405) && (w0 == 632) && (frameNum == 4))
					{
						cout << "(" << h1 << ", " << w << "): " << err.val[0] << endl;
						imwrite("h0.bmp", iniGreyMat(Range(hk_1_mBegin, hk_1_mEnd), Range(wk_1_mBegin, wk_1_mEnd)));
						imwrite("h1.bmp", nowGreyMat(Range(h1_mBegin, h1_mEnd), Range(wk_mBegin, wk_mEnd)));
					}*/
					if (err.val[0] < kMatchThrehold)
					{
						minVal = err.val[0];
						minLoc = Point(w - wk_sBegin, 0);
						break;
					}

					// h2进行匹配
					int h2_mBegin = h2 - mHalfWin;
					int h2_mEnd = h2 + mHalfWin + 1;
					tmpMat = nowGreyMat(Range(h2_mBegin, h2_mEnd), Range(wk_mBegin, wk_mEnd));
					//test->Show(tmpMat, 100, false, 10.0);
					tmpMat.convertTo(xk_mMat, CV_64FC1);
					// 归一化
					minMaxLoc(xk_mMat, &min_val, &max_val);
					xk_mMat = (xk_mMat - min_val) / (max_val - min_val) * 255;
					tmpMat = (xk_mMat - xk_1_mMat).mul((xk_mMat - xk_1_mMat), 1.0 / N);
					//tmpMat = (xk_mMat - x0_mMat).mul((xk_mMat - x0_mMat), 1.0 / N);
					err = sum(tmpMat);
					resMat.at<double>(1, w - wk_sBegin) = err.val[0];
					hMat.at<ushort>(1, w - wk_sBegin) = h2;
					/*if ((h0 == 405) && (w0 == 632) && (frameNum == 4))
					{
						cout << "(" << h2 << ", " << w << "): " << err.val[0] << endl;
						imwrite("h2.bmp", nowGreyMat(Range(h2_mBegin, h2_mEnd), Range(wk_mBegin, wk_mEnd)));
						cout << endl;
					}*/
					if (err.val[0] < kMatchThrehold)
					{
						minVal = err.val[0];
						minLoc = Point(w - wk_sBegin, 1);
						break;
					}
					
				}

				// 查找最小值并检查
				/*cout << "resMat" << resMat << endl;
				cout << "hMat" << hMat << endl;*/
				if (minVal >= kMatchThrehold)
				{
					minMaxLoc(resMat, &minVal, NULL, &minLoc, NULL);
				}
				if (minVal > maxMinVal)
				{
					maxMinVal = minVal;
				}
				//int minThrehold = 100;
				//if (minVal > minThrehold)	// 没有找到匹配
				//{

				//}
				
				// 赋值
				//cout << minLoc << endl;
				this->m_deltaH[frameNum].at<double>(h0, w0) = hMat.at<ushort>(minLoc.y, minLoc.x) - hk_1;
				this->m_deltaW[frameNum].at<double>(h0, w0) = minLoc.x - sHalfWin;
				//printf("dH:%f,dW:%f\n", this->m_deltaH[frameNum].at<double>(h0, w0), this->m_deltaW[frameNum].at<double>(h0, w0));

				// Debug部分
				//if ((h0 == 405) && (w0 == 632))
				//{
				//	// 找到当前匹配的坐标与图像
				//	int hk = this->m_deltaH[frameNum].at<double>(h0, w0) + this->m_iH[frameNum - 1].at<double>(h0, w0);
				//	int wk = this->m_deltaW[frameNum].at<double>(h0, w0) + this->m_iW[frameNum - 1].at<double>(h0, w0);
				//	int hk_mBegin = hk - mHalfWin;
				//	int hk_mEnd = hk + mHalfWin + 1;
				//	int wk_mBegin = wk - mHalfWin;
				//	int wk_mEnd = wk + mHalfWin + 1;
				//	tmpMat = nowGreyMat(Range(hk_mBegin, hk_mEnd), Range(wk_mBegin, wk_mEnd));
				//	stringstream ss;
				//	ss << frameNum;
				//	string idx2str;
				//	ss >> idx2str;

				//	imwrite("MidValue/tmpMat" + idx2str + ".bmp", tmpMat);
				//}
			}
		}
		printf("\n\t\tMaxMinVal: %0.2f", maxMinVal);

		// 更新新的坐标对应关系
		this->m_iH[frameNum] = this->m_iH[frameNum - 1] + this->m_deltaH[frameNum];
		this->m_iW[frameNum] = this->m_iW[frameNum - 1] + this->m_deltaW[frameNum];
	}

	return true;
}