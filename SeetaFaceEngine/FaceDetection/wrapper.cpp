#include "wrapper.h"

using namespace Accelerator;

CLIDetector::CLIDetector(string modelpath)
{
	detector = &seeta::FaceDetection(modelpath.c_str());
	detector->SetMinFaceSize(40);
	detector->SetScoreThresh(2.f);
	detector->SetImagePyramidScaleFactor(0.8f);
	detector->SetWindowStep(4, 4);
}

CLIDetector::~CLIDetector()
{
	free(detector);
}

std::vector<FaceInfo> CLIDetector::Detect(string imagepath)
{
	cv::Mat img = cv::imread(imagepath, cv::IMREAD_UNCHANGED);
	cv::Mat img_gray;

	if (img.channels() != 1)
		cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
	else
		img_gray = img;

	seeta::ImageData img_data;
	img_data.data = img_gray.data;
	img_data.width = img_gray.cols;
	img_data.height = img_gray.rows;
	img_data.num_channels = 1;

	std::vector<seeta::FaceInfo> faces = detector->Detect(img_data);
	std::vector<Accelerator::FaceInfo> faceinfo(faces.size());
	for (size_t i = 0; i < faces.size(); i++)
	{
		Accelerator::Rect temprect(faces[i].bbox.x, faces[i].bbox.y, faces[i].bbox.width, faces[i].bbox.height);
		Accelerator::FaceInfo tempinfo(temprect, faces[i].roll, faces[i].pitch, faces[i].yaw, faces[i].score);
		faceinfo.push_back(tempinfo);
	}
	return faceinfo;
}

std::vector<FaceInfo> CLIDetector::Detect(cv::Mat mat)
{
	cv::Mat img_gray;

	if (mat.channels() != 1)
		cv::cvtColor(mat, img_gray, cv::COLOR_BGR2GRAY);
	else
		img_gray = mat;

	seeta::ImageData img_data;
	img_data.data = img_gray.data;
	img_data.width = img_gray.cols;
	img_data.height = img_gray.rows;
	img_data.num_channels = 1;

	std::vector<seeta::FaceInfo> faces = detector->Detect(img_data);
	std::vector<Accelerator::FaceInfo> faceinfo(faces.size());
	for (size_t i = 0; i < faces.size(); i++)
	{
		Accelerator::Rect temprect(faces[i].bbox.x, faces[i].bbox.y, faces[i].bbox.width, faces[i].bbox.height);
		Accelerator::FaceInfo tempinfo(temprect, faces[i].roll, faces[i].pitch, faces[i].yaw, faces[i].score);
		faceinfo[i] = tempinfo;
	}
	return faceinfo;
}
