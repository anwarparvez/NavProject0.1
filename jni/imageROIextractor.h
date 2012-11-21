#include<cv.h>
#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<math.h>
#include<highgui.h>
#include<stack>
#include <jni.h>
using namespace std;
typedef pair<int,int> pairing;
typedef vector<pairing> nodes;
struct setStruct{
int X1,X2;
int Y1,Y2;
CvPoint pp;
int oddEvenX;
int oddEvenY;
nodes Node;
int degree;
setStruct(CvPoint p) : pp(p),degree(0){
	//this->X1 = this->X2 = this->Y1 = this->Y2 = 0;		
	this->X1 = 0;//400;
	this->Y1 = 0;//600;
}
	int dataFill(CvPoint p1){
	/*	if( this->X1 == 0){
			if(abs(p1.x - pp.x) > 150){
			 this->X1 = abs(p1.x - pp.x);
			 p1.x = pp.x;
			 //return 1;
			}
		}
		else if(this->X2 == 0){
			if(abs(p1.x - pp.x) > 150){
			 this->X2 = abs(p1.x - pp.x);
			 p1.x = pp.x;
			// return 1;
			}
			
		}
		else{
			if(abs(p1.x - pp.x) > 150){
			  if(this->oddEvenX == 0){
				this->X1 = 0;
				this->oddEvenX = (this->oddEvenX + 1)% 2;
				dataFill(p1);
			  }else{
				this->X2 = 0;
				this->oddEvenX =  (this->oddEvenX + 1) % 2;
				dataFill(p1);
			  }
			}
		} 
		if(this->Y1 == 0){
			if(abs(p1.y - pp.y) > 300){
				this->Y1 = abs(p1.y - pp.y);
				p1.y = pp.y;
			//	return 1;
			}
			
		}
		else if(this->Y2 == 0){
			if(abs(p1.y - pp.y) > 300){
				this->Y2 = abs(p1.y - pp.y);
				p1.y = pp.y;
			//	return 1;
			}
			
		}
		else{
			if(abs(p1.y - pp.y) > 300){
			  if(this->oddEvenY == 0){
				this->Y1 = 0;
				this->oddEvenY = (this->oddEvenY + 1) % 2;
				dataFill(p1);
			  }
			  else{
				this->Y2 = 0;
				this->oddEvenY = (this->oddEvenY + 1) % 2;
				dataFill(p1);
			  }
			}
		}*/
		/*int distX = abs(this->pp.x - p1.x);
		int distY = abs(this->pp.y - p1.y);
		float tempFloat = (float)distX/(float)this->X1;
		float tempFloatY = (float)distY/(float)this->Y1;
		printf("%f\n",tempFloat);
		int meetX = 1;
		int meetY = 1;
		if(this->X1 == 400){
			
			if( tempFloat > 0.3 &&  tempFloat < 0.65){
		    	  this->X1 = distX;
			}
			else
				meetX = 0;
		}else{
			if(tempFloat > 0.9 && tempFloat< 1.1){
	     		  this->X1 = distX;
			}
			else
				meetX = 0;
 		}
		if(this->Y1 == 600){
			if(tempFloatY > 0.49 && tempFloatY < 0.81 )
			  this->Y1 = distY;
			else
				meetY = 0;
		}
		else{
			if(tempFloatY  > 0.9 && tempFloatY  < 1.2){
				this->Y1 = distY;			  
			}
			else
				meetY = 0;
		}
		if(meetX == 1 || meetY == 1)
			return 1;*/
		int distX = abs(this->pp.x - p1.x);
		int distY = abs(this->pp.y - p1.y);
		//float tempFloat = (float)distX/(float)this->X1;
		//float tempFloatY = (float)distY/(float)this->Y1;
		printf("distX %d distY %d\n",distX,distY);
		int meetX = 1;
		int meetY = 1;			
		if( distX > 125 && distX <= 240 &&  distY < 25 && this->X1 < distX){
		    this->X1 = distX;
			meetY = 0;
		}
		else if(distY > 330 && distY < 450 && distX < 25 && this->Y1 < distY  ){
			  this->Y1 = distY;
			  meetX = 0;
		}
		else if(distY > 330 && distY < 450 && distX > 125 && distX <= 240 ){
				this->Y1 = distY;
				this->X1 = distX;
		}
		else{
				meetY = 0;
				meetX = 0;
		}
		if(meetX == 1 && meetY == 1){
            printf("Got the hello out of here\n");		
			return 2;
		}
		if(meetX == 1 )
			return 1;
		if(meetY == 1)
			return 1;
		
		return 0;
	}

};
bool isSameline(CvPoint p1, CvPoint p2, vector<CvPoint*> &lineContainer);
void mergeSet(int X,int Y,vector<pair<int,pair<int,setStruct> > > &contains,CvPoint p1,CvPoint p2);
int  findSet(int X,vector<pair<int,pair<int,setStruct> > > &contains);
float lineLenght(CvPoint* p);
void initGraph(void);
void SSC(int node);
vector<vector<pair<CvPoint,int > > > imageROIExtractor(JNIEnv *env,IplImage* pImage,vector<CvPoint*>&lineContainer,vector<CvPoint> cornerPoints);
