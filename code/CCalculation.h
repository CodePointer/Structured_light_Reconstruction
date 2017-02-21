#ifndef _CCALCULATION_H_
#define _CCALCULATION_H_

#include <fstream>
#include "CSensorV.h"
#include "CVisualization.h"

using namespace std;

class CCalculation
{
private:
	// ������������
	CSensor * m_sensor;

	// �ӿڴ�С
	int m_hBegin;
	int m_hEnd;
	int m_wBegin;
	int m_wEnd;

	// �������i,j
	Mat * m_iPro;
	Mat * m_jPro;

	// ��������ļ���
	Mat * m_lineA;
	Mat * m_lineB;
	Mat * m_lineC;

	// ���ٵõ���Ŀ���ֵ
	Mat * m_iH;	// iH
	Mat * m_iW;	// iW

	// �仯��
	Mat * m_deltaH;
	Mat * m_deltaW;

	// ÿ֡�ĵ�����Ϣ
	Mat * m_xMat;
	Mat * m_yMat;
	Mat * m_zMat;

	// �궨�ľ���
	string m_paraPath;
	string m_paraName;
	string m_paraSuffix;
	Mat m_camMatrix;
	Mat m_proMatrix;
	Mat m_R;
	Mat m_T;
	Mat m_C;
	Mat m_P;
	double m_cA;
	double m_cB;
	Mat m_cC;
	Mat m_cD;

	// ������ƵĲ���
	string m_resPath;
	string m_pcPath;
	string m_pcName;
	string m_pcSuffix;

	// ����FloodFill��ȫ����ʱ����
	Mat m_tempMat;

	// ���ù��ܵ�С����
	bool ReleaseSpace();				// �ͷſռ�
	int FloodFill(int h, int w, uchar from, uchar to);	// ��亯��
	
	// ��֡P����Ľ���
	bool FillFirstProjectorU();				// ��֡�Ľ���
	bool FilljPro(int fN);	// ����iPro������£����jPro
	bool CalculateEpipolarLine();
	
	// ������֡�е�ProU��
	bool FillOtherProU(int fN);
	
	// ��������
	bool FillCoordinate(int i);

	// ���iX��iY��deltaX��deltaY
	bool TrackPoints(int frameNum);

public:
	CCalculation();
	~CCalculation();
	bool Init();				// ��ʼ��
	bool CalculateFirst();		// ������֡
	bool CalculateOther();		// ���㶯̬֡
	bool Result(std::string fileName, int i, bool view_port_only);	// ��¼���
};


#endif