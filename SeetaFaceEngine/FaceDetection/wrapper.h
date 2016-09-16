#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "face_detection.h"

using namespace std;

namespace Accelerator
{
	class Rect {
	public:
		int x;
		int y;
		int width;
		int height;
		Rect(int X, int Y, int Width, int Height) :x(X), y(Y), width(Width), height(Height){};
	};

	class FaceInfo {
	public:
		Rect bbox;

		double roll;
		double pitch;
		double yaw;

		double score; /**< Larger score should mean higher confidence. */
		FaceInfo() :bbox(Rect(NULL, NULL, NULL, NULL)), roll(NULL), pitch(NULL), yaw(NULL), score(NULL){};
		FaceInfo(Rect Bbox, double Roll, double Pitch, double Yaw, double Score) :bbox(Bbox), roll(Roll), pitch(Pitch), yaw(Yaw), score(Score){};
	};

	class CLIDetector
	{
		seeta::FaceDetection *detector;
	public:
		CLIDetector(string modelpath);
		std::vector<FaceInfo> Detect(string imagepath);
		std::vector<FaceInfo> Detect(cv::Mat mat);
		~CLIDetector();
	};
}
