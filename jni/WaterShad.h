#pragma once
#include<math.h>
#include<algorithm>
#include<vector>
#include<list>
#include<stdio.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

class WaterShad {
public:
    WaterShad(void);
    ~WaterShad(void);
    vector<pair<pair<int, int>, vector<cv::Point > > > ultimateContainer;
    vector<int> indexes;
/*
    CvSeq* getContoursequence(Mat& objects) {

        IplImage* imageOb = &IplImage(objects);
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* seq = 0;
        cvFindContours(imageOb, storage, &seq, sizeof (CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		   // findContours((Mat)imageOb,storage,*seq,RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        return seq;
    }*/

    /*return  maximum x coordinate value from Point vector*/
    int maxLocation(vector<cv::Point> contains) {
        int max = -1;
        for (int i = 0; i < contains.size(); i++) {
            if (max < contains.at(i).x)
                max = contains.at(i).x;
        }
        return max;
    }


    /*return  minimum x coordinate value from Point vector*/
    int minLocation(vector<cv::Point> contains) {
        int min = 50000;
        for (int i = 0; i < contains.size(); i++) {
                if (min > contains.at(i).x)
                    min = contains.at(i).x;
        }
        return min;
    }

	void drawContour(Mat& image)
    {

        cv::Mat binary;
        cv::cvtColor(image, binary, CV_BGR2GRAY);


        cv::threshold(binary, binary, 100, 255, THRESH_BINARY_INV);

        // Eliminate noise and smaller objects
        cv::Mat fg;
        binary.copyTo(fg);

        cv::Mat bg;
        cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 2);
        cv::threshold(bg, bg, 100, 128, cv::THRESH_BINARY_INV);


        // Create markers image

    	std::vector<std::vector<cv::Point> > contours;
    	cv::findContours(binary,
    	contours, // a vector of contours
    	CV_RETR_EXTERNAL, // retrieve the external contours
    	CV_CHAIN_APPROX_NONE); // all pixels of each contours

    	// Draw black contours on a white image
		//cv::Mat result(binary.size(),CV_8U,cv::Scalar(0));
		for(int i=0;i<contours.size();i++){
			cv::drawContours(image,contours,
			i, // draw all contours
			cv::Scalar(0,0,255), // in Red
			1); // with a thickness of 2

		}
    }
/*
    void drawContours(Mat Images, Mat &srcImg) {
        ultimateContainer.reserve(2500);
        ultimateContainer.resize(2500);
        Mat mask = (Images == 255);
        Mat getTemp;
        CvMemStorage* memory = cvCreateMemStorage(0);
        vector<vector<cv::Point> > contoursPoint;
        vector<cv::Point> contourP;
        getTemp.create(mask.rows, mask.cols, CV_8UC1);
        mask.copyTo(getTemp);
        CvSeq* sequence = getContoursequence(mask);
        vector<double> areas;
        CvSeq* maxSequence = NULL;
        vector<CvSeq*> seqHolder;
        double maxArea = 0;
        for (unsigned int ai = 0; sequence != 0; sequence = sequence->h_next, ai++) {

            contourP.resize(sequence->total);
            printf("The number of Points %d\n", sequence->total);
            for (int i1 = 0; i1 < sequence->total; i1++) {
                CvPoint* cvPoints = (CvPoint*) cvGetSeqElem(sequence, i1);
                contourP[i1] = Point(cvPoints->x, cvPoints->y);
            }
            contoursPoint.push_back(contourP);
            contourP.clear();
            areas.push_back(cvContourArea(sequence));
            seqHolder.push_back(sequence);
            if (maxArea < areas.at(ai)) {
                maxArea = areas.at(ai);
                maxSequence = sequence;
            }
            printf("area Size %lf\n", areas.at(ai));
        }

        //find largest connected component
        double max;
        Point maxLoc, minLoc;
        minMaxLoc(Mat(areas), 0, &max, 0, &maxLoc);
        mask.setTo(Scalar(0)); //clear...
        try {

            int k = 0;
            for (int i = 0; i < seqHolder.size(); i++) {
                int min = minLocation(contoursPoint.at(i));
                int max = maxLocation(contoursPoint.at(i));
                int mid = (min + max) / 2;
                for (k = min; k <= max; k++) {
                    if (ultimateContainer.at(k).second.size() != 0) {
                        if (ultimateContainer.at(k).first.first < mid && ultimateContainer.at(k).first.second > mid) {
                            mid = k;
                            k = -1;
                            break;
                        }
                    }
                }
                if (k != -1) {
                    ultimateContainer.at(mid).first.first = min;
                    ultimateContainer.at(mid).first.second = max;
                }
                for (int j = 0; j < contoursPoint.at(i).size(); j++)
                    ultimateContainer.at(mid).second.push_back(contoursPoint.at(i).at(j));
                if (k != -1)
                    indexes.push_back(mid);
            }
            for (int i = 0; i < indexes.size(); i++) {
                for (int j = 0; j < ultimateContainer.at(indexes.at(i)).second.size(); j++)

                    cvDrawContours(&IplImage(srcImg), seqHolder.at(i), Scalar(0,0,255), Scalar(0), 0);


            }

            printf("number of Characters %d", indexes.size());

        } catch (Exception ex) {
            printf("%s\n", ex.err);
        }
    }
	*/
    int waterShade(Mat &wsImg) {
        cv::Mat image = wsImg.clone();

        cv::Mat binary;
        cv::cvtColor(image, binary, CV_BGR2GRAY);

        cv::threshold(binary, binary, 100, 255, THRESH_BINARY_INV);


        // Eliminate noise and smaller objects
        cv::Mat fg;
        binary.copyTo(fg);

        cv::Mat bg;
        cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 2);
        cv::threshold(bg, bg, 100, 128, cv::THRESH_BINARY_INV);


        // Create markers image
        cv::Mat markers(binary.size(), CV_8U, cv::Scalar(0));
        markers = fg + bg;
        // Create watershed segmentation object
		cv::Mat rmarkers;
        markers.convertTo(rmarkers, CV_32S);
	    cv::watershed(image, rmarkers);

		cv::Mat result ;
        rmarkers.convertTo(result, CV_8U);
        imshow("final_result", result);
		cv::waitKey(0);


       // drawContours(result, wsImg);
       // imshow("final_result2", wsImg);

        // drawContours(result);

        cv::waitKey(0);

        return 0;
    }


};

