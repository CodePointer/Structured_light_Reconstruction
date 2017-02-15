#include "CCalculation.h"

using namespace cv;
using namespace std;

int main()
{
	bool status = true;

	CCalculation myCalculation;

	if (status)
	{
		myCalculation.Init();
	}
	if (status)
	{
		myCalculation.CalculateFirst();
	}
	if (status)
	{
		myCalculation.CalculateOther();
	}
	
	return 0;
}