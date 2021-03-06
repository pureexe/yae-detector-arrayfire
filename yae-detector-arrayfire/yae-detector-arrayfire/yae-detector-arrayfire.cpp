// yae-detector-arrayfire.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

#include "OldYaeAlgorithm.h"

void TaskTest();
void Application();
void viewImage(array image);

int main()
{
	setBackend(AF_BACKEND_OPENCL);
	Application();
	//TaskTest();
    return 0;
}

void Application() {
	std::string inputPath = "../../images/case03.jpg";
	array inputImage = loadImage(inputPath.c_str(),true);
	seq row(660, 719), col(450, 840);
	array subtitleFrame = inputImage(row, col);
	array subtitleFrameMask;
	timer::start();
	subtitleFrameMask = OldYaeAlgorithm(subtitleFrame);
	printf("subtitle search time %g seconds\n", timer::stop());
	viewImage(subtitleFrameMask *255);
}

void viewImage(array image)
{
	Window resultWindows(image.dims(1), image.dims(0), "ResultImage");
	do {
		resultWindows.image(image.as(u8));
	} while (!resultWindows.close());
}

void TaskTest() {
	float aData[] = { 1,2,3,4 };
	array setA = array(4, 1, aData).as(s32);
	array setB = setA(1).as(s32);
	int* data = setB.host<int>();
	int u = 3;
	u += data[0];
	printf("%d", u);
}