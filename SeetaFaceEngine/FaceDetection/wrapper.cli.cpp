#pragma once
#include <msclr\marshal_cppstd.h>
#include "wrapper.h"

using namespace std;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

#define TO_NATIVE_STRING(str) msclr::interop::marshal_as<std::string>(str)
#define MARSHAL_ARRAY(n_array, m_array) \
  auto m_array = gcnew array<float>(n_array.Size); \
  pin_ptr<float> pma = &m_array[0]; \
  memcpy(pma, n_array.Data, n_array.Size * sizeof(float));

namespace FaceDetectDotNet
{
	public value struct FaceInfo
	{
		double roll;
		double pitch;
		double yaw;
		double score;
		System::Drawing::Rectangle r;
		FaceInfo(System::Drawing::Rectangle R, double Roll, double Pitch, double Yaw, double Score) :r(R), roll(Roll), pitch(Pitch), yaw(Yaw), score(Score){}
	};

	public ref class DetectModel
	{
	private:
		Accelerator::CLIDetector *d_model;

		int ConvertBitmapToMat(System::Drawing::Bitmap^ bmpImg, cv::Mat& cvImg)
		{
			int retVal = 0;

			System::Drawing::Imaging::BitmapData^ bmpData = bmpImg->LockBits(
				System::Drawing::Rectangle(0, 0, bmpImg->Width, bmpImg->Height),
				System::Drawing::Imaging::ImageLockMode::ReadWrite, bmpImg->PixelFormat);

			if (!cvImg.empty())
			{
				cvImg.release();
			}
			if (bmpImg->PixelFormat == System::Drawing::Imaging::PixelFormat::Format8bppIndexed)  
			{
				cvImg = cv::Mat(bmpImg->Height, bmpImg->Width, CV_8UC1, (char*)bmpData->Scan0.ToPointer());
			}
			else if (bmpImg->PixelFormat == System::Drawing::Imaging::PixelFormat::Format24bppRgb)  
			{
				cvImg = cv::Mat(bmpImg->Height, bmpImg->Width, CV_8UC3, (char*)bmpData->Scan0.ToPointer());
			}
			else if (bmpImg->PixelFormat == System::Drawing::Imaging::PixelFormat::Format32bppArgb)	
			{
				cvImg = cv::Mat(bmpImg->Height, bmpImg->Width, CV_8UC4, (char*)bmpData->Scan0.ToPointer());
			}
			else
			{
				retVal = -1;
			}
 
			bmpImg->UnlockBits(bmpData);

			return (retVal);
		}
	public:
		DetectModel(String ^modelFile)
		{
			d_model = new Accelerator::CLIDetector(TO_NATIVE_STRING(modelFile));
		}

		array<FaceInfo> ^Detect(String ^imageFile)
		{
			std::vector<Accelerator::FaceInfo> temp = d_model->Detect(TO_NATIVE_STRING(imageFile));
			auto output = gcnew array<FaceInfo>(temp.size());
			for (size_t i = 0; i < temp.size(); i++)
			{
				output[i] = FaceInfo(System::Drawing::Rectangle(temp[i].bbox.x, temp[i].bbox.y, temp[i].bbox.width, temp[i].bbox.height), temp[i].roll, temp[i].pitch, temp[i].yaw, temp[i].score);

			}
			return output;
		}

		array<FaceInfo> ^Detect(System::Drawing::Bitmap ^bmp)
		{
			cv::Mat mat;
			int a = ConvertBitmapToMat(bmp, mat);
			if (a==0)
			{
				std::vector<Accelerator::FaceInfo> temp = d_model->Detect(mat);
				auto output = gcnew array<FaceInfo>(temp.size());
				for (size_t i = 0; i < temp.size(); i++)
				{
					output[i] = FaceInfo(System::Drawing::Rectangle(temp[i].bbox.x, temp[i].bbox.y, temp[i].bbox.width, temp[i].bbox.height), temp[i].roll, temp[i].pitch, temp[i].yaw, temp[i].score);

				}
				return output;
			}
			else
			{
				return gcnew array<FaceInfo>(0);
			}
		}
	};


}