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

	this->trace_h_ = NULL;
	this->trace_w_ = NULL;
	this->delta_trace_h_ = NULL;
	this->delta_trace_w_ = NULL;
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

	if (this->trace_h_ != NULL)
	{
		delete[](this->trace_h_);
		this->trace_h_ = NULL;
	}
	if (this->trace_w_ != NULL)
	{
		delete[](this->trace_w_);
		this->trace_w_ = NULL;
	}
	if (this->delta_trace_h_ != NULL)
	{
		delete[](this->delta_trace_h_);
		this->delta_trace_h_ = NULL;
	}
	if (this->delta_trace_w_ != NULL)
	{
		delete[](this->delta_trace_w_);
		this->delta_trace_w_ = NULL;
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
		ErrorHandling("Error in find config.xml: " + CONFIG_PATHNAME);
	}
	else
	{
		string main_path;
		string data_set_path;
		if (kPlatformFlag == WINDOWS)
		{
			fs["main_path_win"] >> main_path;
		}
		else if (kPlatformFlag == UBUNTU)
		{
			fs["main_path_linux"] >> main_path;
		}
		else
		{
			return false;
		}
		fs["data_set_path"] >> data_set_path;

		string tmp;
		fs["ipro_mat_path"] >> tmp;
		this->ipro_mat_path_ = main_path + data_set_path + tmp;
		fs["ipro_mat_name"] >> this->ipro_mat_name_;
		fs["ipro_mat_suffix"] >> this->ipro_mat_suffix_;

		fs["depth_mat_path"] >> tmp;
		this->depth_mat_path_ = main_path + data_set_path + tmp;
		fs["depth_mat_name"] >> this->depth_mat_name_;
		fs["depth_mat_suffix"] >> this->depth_mat_suffix_;
		CreateDir(this->depth_mat_path_);

		fs["ipro_output_path"] >> tmp;
		this->ipro_output_path = main_path + data_set_path + tmp;
		fs["ipro_output_name"] >> this->ipro_output_name;
		fs["ipro_output_suffix"] >> this->ipro_output_suffix;
		CreateDir(this->ipro_output_path);

		fs["point_cloud_path"] >> tmp;
		this->point_cloud_path_ = main_path + data_set_path + tmp;
		fs["point_cloud_name"] >> this->point_cloud_name_;
		fs["point_cloud_suffix"] >> this->point_cloud_suffix_;
		CreateDir(this->point_cloud_path_);

		fs["point_show_path"] >> tmp;
		this->point_show_path_ = main_path + data_set_path + tmp;
		fs["point_show_name"] >> this->point_show_name_;
		fs["point_show_suffix"] >> this->point_show_suffix_;
		CreateDir(this->point_show_path_);

		fs["trace_path"] >> tmp;
		this->trace_path_ = main_path + data_set_path + tmp;
		fs["trace_name"] >> this->trace_name_;
		fs["trace_suffix"] >> this->trace_suffix_;
		CreateDir(this->trace_path_);

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
	this->m_jPro = new Mat[DYNAFRAME_MAXNUM];
	this->m_lineA = new Mat[DYNAFRAME_MAXNUM];
	this->m_lineB = new Mat[DYNAFRAME_MAXNUM];
	this->m_lineC = new Mat[DYNAFRAME_MAXNUM];
	this->trace_h_ = new Mat[DYNAFRAME_MAXNUM];
	this->trace_w_ = new Mat[DYNAFRAME_MAXNUM];
	this->delta_trace_h_ = new Mat[DYNAFRAME_MAXNUM];
	this->delta_trace_w_ = new Mat[DYNAFRAME_MAXNUM];
	this->holes_mark_ = new Mat[DYNAFRAME_MAXNUM];
	printf("finished.\n");

	printf("\tAllocating mats...");
	for (int i = 0; i < DYNAFRAME_MAXNUM; i++)
	{
		this->m_xMat[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_yMat[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_zMat[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->m_iPro[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		
		
		this->trace_h_[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->trace_w_[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->delta_trace_h_[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->delta_trace_w_[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
		this->holes_mark_[i].create(CAMERA_RESROW, CAMERA_RESLINE, CV_8UC1);
	}
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

	if (status)
	{
		printf("\tFillFirstProjectorU...");
		status = this->FillFirstProjectorU();
		printf("finished.\n");
	}
	
	if (status)
	{
		printf("\tFill depth_mat from ipro_mat...");
		status = this->Ipro2Depth(0);
		printf("finished.\n");
	}

	if (status)
	{
		printf("\tFill coordinate...");
		status = this->FillCoordinate(0);
		printf("finished.\n");
	}

	// 根据数值填充jProjector
	if (status)
	{
		printf("\tFiil jpro_mat...");
		status = this->FilljPro(0);
		printf("finished.\n");
	}

	myDebug.ShowPeriod(this->m_iPro[0], 100, 0.5, true, 
		this->point_show_path_
		+ this->point_show_name_
		+ "0"
		+ this->point_show_suffix_
		);

	if (status)
	{
		printf("\tCalculate epipolar line...");
		status = this->CalculateEpipolarLine(0);
		printf("finished.\n");
	}

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

		// trace point according to key_frame：
		if (status)
		{
			printf("\tPointTracking:");
			status = this->TrackPoints(frameNum);
			printf("finished.\n");
		}

		// fill ipro_maT
		if (status)
		{
			printf("\tFillOtherProU:");
			int key_frame_num = frameNum - ((frameNum - 1) % 5 + 1);
			this->FillOtherProU(frameNum, key_frame_num);
			printf("finished.\n");
		}

		// process key frame
		if (status)
		{
			if (frameNum % 5 == 0)
			{
				printf("\tProcess key frame:");
				status = this->ProcessFrame(frameNum);
				printf("finished.\n");
			}
		}

		// fill z_mat
		if (status)
		{
			printf("\tFill zmat:");
			status = this->Ipro2Depth(frameNum);
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

			// Output (x,y,z)
			file << this->m_xMat[i].at<double>(v, u) << ' ';
			file << this->m_yMat[i].at<double>(v, u) << ' ';
			file << this->m_zMat[i].at<double>(v, u) << endl;
			//printf("(%f, %f, %f)\n", this->m_xMat.at<double>(v, u), this->m_yMat.at<double>(v, u), this->m_zMat.at<double>(v, u));
		}
	}
	file.close();
	
	if (i >= 0)
	{
		string idx2str;
		stringstream ss;
		ss << i;
		ss >> idx2str;

		// save depth mat
		WriteMatData(this->depth_mat_path_,
			this->depth_mat_name_,
			idx2str,
			this->depth_mat_suffix_, this->m_zMat[i]);

		// save ipro mat
		WriteMatData(this->ipro_output_name,
			this->ipro_output_name,
			idx2str,
			this->ipro_output_suffix, this->m_iPro[i]);

		// save iH,iW
		//Mat iHv = this->trace_h_[i](Range(this->m_hBegin, this->m_hEnd), Range(this->m_wBegin, this->m_wEnd));
		//Mat iWv = this->trace_w_[i](Range(this->m_hBegin, this->m_hEnd), Range(this->m_wBegin, this->m_wEnd));
		WriteMatData(this->trace_path_,
			this->trace_name_ + "_iH",
			idx2str,
			this->trace_suffix_, this->trace_h_[i]);
		WriteMatData(this->trace_path_,
			this->trace_name_ + "_iW",
			idx2str,
			this->trace_suffix_, this->trace_w_[i]);
	}
	
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


// 已有x,y,z的情况下，填充jPro
bool CCalculation::FilljPro(int frame_num)
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
	this->m_jPro[frame_num].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_jPro[frame_num].setTo(-100);
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			// 判断是否有值
			double z = this->m_zMat[frame_num].at<double>(h, w);
			if (z < 0)
			{
				continue;
			}
			double x = this->m_xMat[frame_num].at<double>(h, w);
			double y = this->m_yMat[frame_num].at<double>(h, w);

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
			this->m_jPro[frame_num].at<double>(h, w) = j / omega;
		}
	}

	return status;
}


// 计算每个点对应的极线
bool CCalculation::CalculateEpipolarLine(int frame_num)
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
	this->m_lineA[frame_num].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_lineB[frame_num].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_lineC[frame_num].create(CAMERA_RESROW, CAMERA_RESLINE, CV_64FC1);
	this->m_lineA[frame_num].setTo(0);
	this->m_lineB[frame_num].setTo(0);
	this->m_lineC[frame_num].setTo(1);
	double di = this->m_proMatrix.at<double>(0, 2);
	double dj = this->m_proMatrix.at<double>(1, 2);
	double fi = this->m_proMatrix.at<double>(0, 0);
	double fj = this->m_proMatrix.at<double>(1, 1);
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			// 假设Z的情况下，计算对应的X,Y,Z
			double i = this->m_iPro[frame_num].at<double>(h, w);
			if (i < 0)
			{
				continue;
			}
			double j = this->m_jPro[frame_num].at<double>(h, w);
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
			this->m_lineA[frame_num].at<double>(h, w) = A;
			this->m_lineB[frame_num].at<double>(h, w) = B;
			this->m_lineC[frame_num].at<double>(h, w) = C;
		}
	}

	return status;
}


// 填充后续帧中的ProU。直接将视口中的每个点映射到后续帧中。
bool CCalculation::FillOtherProU(int frame_num, int key_frame_num)
{
	bool status = true;

	// 填充ProU[k]
	this->m_iPro[frame_num].setTo(-100);
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			int h_now = (int)this->trace_h_[frame_num].at<double>(h, w);
			int w_now = (int)this->trace_w_[frame_num].at<double>(h, w);
			if ((h_now > 0) && (w_now > 0))
			{
				this->m_iPro[frame_num].at<double>(h_now, w_now) = this->m_iPro[key_frame_num].at<double>(h, w);
			}
		}
	}

	return status;
}


bool CCalculation::Ipro2Depth(int frame_num)
{
	bool status = true;

	this->m_zMat[frame_num].setTo(-100);
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			double z = -100;
			// 如果是没有值的部分，z就暂时不填
			if (this->m_iPro[frame_num].at<double>(h, w) < 0)
			{
				z = -100;
				continue;
			}
			// 有值的部分，计算深度
			else
			{
				double para_A = this->m_cA;
				double para_B = this->m_cB;
				double para_C = this->m_cC.at<double>(h, w);
				double para_D = this->m_cD.at<double>(h, w);
				z = -(para_A - para_B*this->m_iPro[frame_num].at<double>(h, w))
					/ (para_C - para_D*this->m_iPro[frame_num].at<double>(h, w));
			}

			// 判断z是否在合法范围内
			if ((z < FOV_MIN_DISTANCE) || (z > FOV_MAX_DISTANCE))
			{
				z = -100;
				continue;
			}
			else
			{
				this->m_zMat[frame_num].at<double>(h, w) = z;
			}
		}
	}

	return status;
}


bool CCalculation::Depth2Ipro(int frame_num)
{
	bool status = true;

	this->m_iPro[frame_num].setTo(-100);
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			double ipro = -100;
			// check if this point is avaliable
			if (this->m_zMat[frame_num].at<double>(h, w) < 0)
			{
				continue;
			}
			else
			{
				double para_A = this->m_cA;
				double para_B = this->m_cB;
				double para_C = this->m_cC.at<double>(h, w);
				double para_D = this->m_cD.at<double>(h, w);
				ipro = (para_A + para_C*this->m_zMat[frame_num].at<double>(h, w))
					/ (para_B + para_D*this->m_zMat[frame_num].at<double>(h, w));
			}

			this->m_iPro[frame_num].at<double>(h, w) = ipro;
		}
	}

	return status;
}


bool CCalculation::MarkHoles(int frame_num)
{
	bool status = true;

	int kMaxHoleSize = 300;
	// 0: UnChecked
	// 1: Holes
	// 2: Not_Holes
	// 3: Checking
	this->holes_mark_[frame_num].setTo(0);

	// if ipro have value, then not holes
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			if (this->m_iPro[frame_num].at<double>(h, w) > 0)
			{
				this->holes_mark_[frame_num].at<uchar>(h, w) = 2;
			}
		}
	}

	// Flood fills
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			if (this->holes_mark_[frame_num].at<uchar>(h, w) == 0)
			{
				// Search, BFS
				queue<Point2i> my_queue;
				my_queue.push(Point2i(h, w));
				int hole_size = 0;	// Count for the size of hole
				while (!my_queue.empty())
				{
					Point2i pos = my_queue.front();
					my_queue.pop();

					if (this->holes_mark_[frame_num].at<uchar>(pos.x, pos.y) == 0)
					{
						this->holes_mark_[frame_num].at<uchar>(pos.x, pos.y) = 3;
						hole_size += 1;
						if (pos.x - 1 > 0)
						{
							my_queue.push(Point2i(pos.x - 1, pos.y));
						}
						if (pos.x + 1 < CAMERA_RESROW)
						{
							my_queue.push(Point2i(pos.x + 1, pos.y));
						}
						if (pos.y - 1 > 0)
						{
							my_queue.push(Point2i(pos.x, pos.y - 1));
						}
						if (pos.y + 1 < CAMERA_RESLINE)
						{
							my_queue.push(Point2i(pos.x, pos.y + 1));
						}
					}
				}

				// Decide whether this is a hole
				uchar to_value = 1;
				if (hole_size > kMaxHoleSize)
				{
					to_value = 2;
				}
				else
				{
					to_value = 1;
				}

				// Change checking status
				for (int h_idx = 0; h_idx < CAMERA_RESROW; h_idx++)
				{
					for (int w_idx = 0; w_idx < CAMERA_RESLINE; w_idx++)
					{
						if (this->holes_mark_[frame_num].at<uchar>(h_idx, w_idx) == 3)
						{
							this->holes_mark_[frame_num].at<uchar>(h_idx, w_idx) = to_value;
						}
					}
				}
			}
		}
	}


	return status;
}


bool CCalculation::ProcessFrame(int frame_num)
{
	bool status = true;

	// Check the hole
	if (status)
	{
		status = this->MarkHoles(frame_num);
	}

	// Hole filling
	int ave_half_win = 20;
	for (int h = ave_half_win; h < CAMERA_RESROW - ave_half_win; h++)
	{
		for (int w = ave_half_win; w < CAMERA_RESLINE - ave_half_win; w++)
		{
			//printf("val(%d, %d) = %f\n", h, w, val);
			if (this->holes_mark_[frame_num].at<uchar>(h, w) == 1) // is hole
			{
				int num_of_none_hole = 0;
				double value_of_none_hole = 0;
				for (int u = -ave_half_win; u <= ave_half_win; u++)
				{
					for (int v = -ave_half_win; v <= ave_half_win; v++)
					{
						if (this->holes_mark_[frame_num].at<uchar>(h + u, w + v) == 1)
						{
							num_of_none_hole += 1;
							value_of_none_hole += this->m_iPro[frame_num].at<double>(h + u, w + v);
						}
					}
				}
				if (num_of_none_hole != 0)
				{
					this->m_iPro[frame_num].at<double>(h, w) = value_of_none_hole / num_of_none_hole;
				}
			}
		}
	}

	// fix ipro
	Mat ipro_tmp;
	if (status)
	{
		stringstream ss;
		string idx2str;
		ss << frame_num;
		ss >> idx2str;

		FileStorage fs;
		fs.open(this->ipro_mat_path_
			+ this->ipro_mat_name_
			+ idx2str
			+ this->ipro_mat_suffix_, FileStorage::READ);
		if (fs.isOpened())
		{
			fs["ipro_mat"] >> ipro_tmp;
			fs.release();
		}
		else
		{
			status = false;
		}
	}
	if (status)
	{
		for (int h = 0; h < CAMERA_RESROW; h++)
		{
			for (int w = 0; w < CAMERA_RESLINE; w++)
			{
				if (this->m_iPro[frame_num].at<double>(h, w) > 0)
				{
					this->m_iPro[frame_num].at<double>(h, w) = ipro_tmp.at<double>(h, w);
				}
			}
		}
	}
	if (status)
	{
		status = Ipro2Depth(frame_num);
	}
	

	// fill epipolar line
	if (status)
	{
		status = this->FillCoordinate(frame_num);
	}
	if (status)
	{
		status = this->FilljPro(frame_num);
	}
	if (status)
	{
		status = this->CalculateEpipolarLine(frame_num);
	}

	// fill trace_h_, trace_w_
	if (status)
	{
		for (int h = 0; h < CAMERA_RESROW; h++)
		{
			for (int w = 0; w < CAMERA_RESLINE; w++)
			{
				if (this->m_iPro[frame_num].at<double>(h, w) > 0)
				{
					this->trace_h_[frame_num].at<double>(h, w) = (double)h;
					this->trace_w_[frame_num].at<double>(h, w) = (double)w;
				}
				else
				{
					this->trace_h_[frame_num].at<double>(h, w) = -100;
					this->trace_w_[frame_num].at<double>(h, w) = -100;
				}
			}
		}
	}

	return status;
}


bool CCalculation::FillCoordinate(int i)
{
	for (int h = 0; h < CAMERA_RESROW; h++)
	{
		for (int w = 0; w < CAMERA_RESLINE; w++)
		{
			// x = z * u_c/f_u
			double z = this->m_zMat[i].at<double>(h, w);
			if (z < 0)
			{
				continue;
			}
			double w_center = w - this->m_C.at<double>(0, 2);
			double h_center = h - this->m_C.at<double>(1, 2);
			double w_f = this->m_C.at<double>(0, 0);
			double h_f = this->m_C.at<double>(1, 1);
			this->m_xMat[i].at<double>(h, w) = z * w_center / w_f;
			this->m_yMat[i].at<double>(h, w) = z * h_center / h_f;
		}
	}

	return true;
}


// 填充iX和iY，deltaX和deltaY
bool CCalculation::TrackPoints(int frameNum)
{
	this->trace_h_[frameNum].setTo(-100);
	this->trace_w_[frameNum].setTo(-100);
	this->delta_trace_h_[frameNum].setTo(0);	
	this->delta_trace_w_[frameNum].setTo(0);

	if (frameNum == 0)
	{
		// first frame: view port
		for (int h0 = this->m_hBegin; h0 < this->m_hEnd; h0++)
		{
			for (int w0 = this->m_wBegin; w0 < this->m_wEnd; w0++)
			{
				this->trace_h_[frameNum].at<double>(h0, w0) = (double)h0;
				this->trace_w_[frameNum].at<double>(h0, w0) = (double)w0;
			}
		}
	}
	else
	{
		// Calculate key frame num
		int key_frame = 1;
		key_frame = frameNum - (frameNum % 5);
		if (frameNum % 5 == 0)
		{
			key_frame -= 5;
		}

		// Get key frame and now frame
		this->m_sensor->SetProPicture(key_frame);
		Mat tmpMat, key_frame_mat, now_frame_mat;
		tmpMat = this->m_sensor->GetCamPicture();
		tmpMat.copyTo(key_frame_mat);
		this->m_sensor->SetProPicture(frameNum);
		tmpMat = this->m_sensor->GetCamPicture();
		tmpMat.copyTo(now_frame_mat);

		/// Test: Binary image
		/*Mat mean_value_mat;
		blur(key_frame_mat, mean_value_mat, Size(21, 21), Point(-1, -1));
		for (int h = 0; h < CAMERA_RESROW; h++)
		{
			for (int w = 0; w < CAMERA_RESLINE; w++)
			{
				if (key_frame_mat.at<uchar>(h, w) < mean_value_mat.at<uchar>(h, w))
				{
					key_frame_mat.at<uchar>(h, w) = 0;
				}
				else
				{
					key_frame_mat.at<uchar>(h, w) = 255;
				}
			}
		}
		blur(now_frame_mat, mean_value_mat, Size(21, 21), Point(-1, -1));
		for (int h = 0; h < CAMERA_RESROW; h++)
		{
			for (int w = 0; w < CAMERA_RESLINE; w++)
			{
				if (now_frame_mat.at<uchar>(h, w) < mean_value_mat.at<uchar>(h, w))
				{
					now_frame_mat.at<uchar>(h, w) = 0;
				}
				else
				{
					now_frame_mat.at<uchar>(h, w) = 255;
				}
			}
		}*/
		/// Test: Binary image

		// bound parameters for patch, search area
		int search_half_win = (SEARCH_WINDOW_SIZE - 1) / 2;
		int now_search_h_begin, now_search_h_end, now_search_w_begin, now_search_w_end;		// xk,yk的搜索范围（给定中心点）
		int patch_half_win = (MATCH_WINDOW_SIZE - 1) / 2;
		Mat key_patch;
		int key_patch_h_begin, key_patch_h_end, key_patch_w_begin, key_patch_w_end;
		Mat now_patch;
		int now_patch_h_begin, now_patch_h_end, now_patch_w_begin, now_patch_w_end;

		double hSize = this->m_hEnd - this->m_hBegin;
		int k = 0;

		double maxMinVal = 0;
		for (int h_key = 0; h_key < CAMERA_RESROW; h_key++)
		{
			if (h_key % 100 == 0)
			{
				cout << '.' << flush;
			}
			for (int w_key = 0; w_key < CAMERA_RESLINE; w_key++)
			{
				if (this->m_iPro[key_frame].at<double>(h_key, w_key) <= 0)
				{
					// no data point in key_frame, set to invalid
					continue;
				}
				if (this->trace_h_[frameNum - 1].at<double>(h_key, w_key) <= 0)
				{
					// last frame is invalid then discard
					continue;
				}

				// find key patch in key frame
				key_patch_h_begin = h_key - patch_half_win;
				key_patch_h_end = h_key + patch_half_win + 1;
				key_patch_w_begin = w_key - patch_half_win;
				key_patch_w_end = w_key + patch_half_win + 1;
				tmpMat = key_frame_mat(Range(key_patch_h_begin, key_patch_h_end),
					Range(key_patch_w_begin, key_patch_w_end));
				tmpMat.convertTo(key_patch, CV_64FC1);

				// Normalized
				double min_val, max_val;
				minMaxLoc(key_patch, &min_val, &max_val);
				key_patch = (key_patch - min_val) / (max_val - min_val) * 255;

				// find search area in now frame, according to last frame position
				int h_last = (int)this->trace_h_[frameNum - 1].at<double>(h_key, w_key);
				int w_last = (int)this->trace_w_[frameNum - 1].at<double>(h_key, w_key);
				now_search_h_begin = h_last - search_half_win;
				now_search_h_end = h_last + search_half_win + 1;
				now_search_w_begin = w_last - search_half_win;
				now_search_w_end = w_last + search_half_win + 1;

				// result for storage
				Mat error_result(2, now_search_w_end - now_search_w_begin, CV_64FC1);
				Mat h_mark_mat(2, now_search_w_end - now_search_w_begin, CV_16UC1);
				error_result.setTo(65536.0);
				h_mark_mat.setTo(0);

				// 在范围内进行查找匹配
				double A = this->m_lineA[key_frame].at<double>(h_key, w_key);
				double B = this->m_lineB[key_frame].at<double>(h_key, w_key);
				double C = this->m_lineC[key_frame].at<double>(h_key, w_key);
				double N = (double)SEARCH_WINDOW_SIZE * (double)SEARCH_WINDOW_SIZE;
				Point minLoc;// Loc.x:W, Loc.y:H
				double minVal = 99999999.0;
				double kMatchThrehold = 6000.0;

				/// Debug
				if (h_key == 527 && w_key == 822)
				{
					//printf("Debug.\n");
				}
				/// Debug

				for (int w = now_search_w_begin; w < now_search_w_end; w++)
				{
					// Calculate h for match according to epipolar constraint
					double h_double = (-A / B)*w + (-C / B);

					for (int h = (int)h_double; h <= (int)h_double + 1; h++)
					{
						// find frame patch in now frame
						now_patch_h_begin = h - patch_half_win;
						now_patch_h_end = h + patch_half_win + 1;
						now_patch_w_begin = w - patch_half_win;
						now_patch_w_end = w + patch_half_win + 1;
						tmpMat = now_frame_mat(Range(now_patch_h_begin, now_patch_h_end),
							Range(now_patch_w_begin, now_patch_w_end));
						tmpMat.convertTo(now_patch, CV_64FC1);

						// Normalized
						minMaxLoc(now_patch, &min_val, &max_val);
						now_patch = (now_patch - min_val) / (max_val - min_val) * 255;

						// Calculate error and mark down
						tmpMat = (now_patch - key_patch).mul((now_patch - key_patch), 1.0 / N);
						Scalar error_val = sum(tmpMat);
						error_result.at<double>(h - (int)h_double, w - now_search_w_begin) = error_val[0];
						h_mark_mat.at<ushort>(h - (int)h_double, w - now_search_w_begin) = h;
					}
				}

				// find min_value point position
				minMaxLoc(error_result, &minVal, NULL, &minLoc, NULL);
				if (minVal >= kMatchThrehold)
				{
					// match badly, discard
					continue;
				}
				/// Debug
				if (h_key == 527 && w_key == 822)
				{
					//cout << error_result << endl;
					//cout << h_mark_mat << endl;
				}
				/// Debug
				if (minVal > maxMinVal)
				{
					maxMinVal = minVal;
				}
				
				// mark now h, w
				this->delta_trace_h_[frameNum].at<double>(h_key, w_key) = h_mark_mat.at<ushort>(minLoc.y, minLoc.x) - h_last;
				this->delta_trace_w_[frameNum].at<double>(h_key, w_key) = minLoc.x - search_half_win;
				
			}
		}
		printf("\n\t\tMaxMinVal: %0.2f", maxMinVal);

		// 更新新的坐标对应关系
		this->trace_h_[frameNum] = this->trace_h_[frameNum - 1] + this->delta_trace_h_[frameNum];
		this->trace_w_[frameNum] = this->trace_w_[frameNum - 1] + this->delta_trace_w_[frameNum];
	}

	return true;
}