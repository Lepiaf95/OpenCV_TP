#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



int main()
{
	VideoCapture cap("vid1.MOV");
	Mat frame, frame_hsv, frame_close, frame_canny, frame_contour;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

		while (true)
		{
			
			cap >> frame;
			if (frame.empty())
				cout << "Cette image est vide !" << endl;


			// Partie 1 : Segmentation couleur de l'image pour la reconnaissance d'objet

			resize(frame, frame, Size(340,480),0,0, INTER_LINEAR); // Interpolation linéaire
			cvtColor(frame, frame_hsv, COLOR_BGR2HSV); // HSV = TSL
			inRange(frame_hsv, Scalar(170, 160, 60), Scalar(180, 255, 255), frame_hsv); // la couleur rouge
			

			// Partie 2 : Détection d'objet et reconstitution de la forme

			morphologyEx(frame_hsv, frame_close, MORPH_CLOSE, kernel, Point(3, 3));
			Canny(frame_close, frame_canny, 100, 200, 3);
			dilate(frame_canny, frame_contour, kernel, Point(3, 3));
			findContours(frame_contour, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

			RNG rng(12345);
			Mat drawing = Mat::zeros(frame_contour.size(), CV_8UC3);
			for (int i = 0; i < contours.size(); i++)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
			}


			// Partie 3 : Affinage de la détection pour le suivi robuste




			imshow("video", frame);
			imshow("video_hsv", frame_hsv);
			imshow("video_close", frame_close);
			imshow("video_canny", frame_canny);
			imshow("contours", drawing);
			if (waitKey(40) == 27) // C'est le code ASCII de la touche "Echap" du clavier
				break;
		}
}