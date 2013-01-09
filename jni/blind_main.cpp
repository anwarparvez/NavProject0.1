#include "EdgeFinder.h"
#include "CornerFinder.h"
#include "ROIExtractor.h"
#include "WaterShad.h"
#define FILEPATH  "D:/others/Blind_Navigation_Latest_Home/ImageInput/SM9.jpg"

int main() {

	//WaterShad ws;
	//ws.waterShade();


    Mat src; 
    Mat dst;
	Mat *p;
	
    src=imread(FILEPATH);
	//p=&src;
	//IplImage* img = &(IplImage )(p->clone());
	//rectangle(*p, cvPoint(20, 15), cvPoint(100, 70), cvScalar(255, 0, 0, 0),1, 8, 0);

	//dst=p->clone();
	//src.copyTo(*p,NULL);
     IplImage* img = &(IplImage)src;
    EdgeFinder ef(img);
    imshow("EdgeFinder",(Mat) ef.doProcess());
	//imshow("hi",dst);
	 waitKey(0);

   CornerFinder cf;
    cf.setVeticalLineHolder(ef.getVeticalLineHolder());
    cf.setOtherLineHolder(ef.getOtherLineHolder());
    cf.getCornerpoints(img);

	vector<CvPoint> superContainer=cf.getVoxelGrid()->listofPoints;
	for(int i = 0; i < superContainer.size(); i++){
	cvCircle(img,cvPoint(superContainer.at(i).x,superContainer.at(i).y),1,cvScalar(25,i*200,255),2);
	}

	

	/*vector<CvPoint*> &lineContainer=cf.getVeticalLineHolder();

		for(int i =0; i < lineContainer.size(); i++){
		CvPoint* points = lineContainer.at(i);

		cvLine(img,points[0],points[1],cvScalar(255,0,0),2);
		}

	imshow("Line",src);*/



	/*waitKey(0);

  ROIExtractor roiExtractor; 
    vector < vector < pair<CvPoint, int >> > container = roiExtractor.imageROIExtractor(cf.getVeticalLineHolder(), cf.getVoxelGrid()->listofPoints, img);
  */
    return 0;
}