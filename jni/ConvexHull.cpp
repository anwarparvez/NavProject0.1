#include "ConvexHull.h"
#include "voxel.h"

bool sortOutPoints(const CvPoint p1, const CvPoint p2){
	return  p1.x == p2.x ? p1.y < p2.y : p1.x < p2.x;
}
int getCrossProduct(CvPoint Xi,CvPoint next,CvPoint P){
	int x1 = Xi.x - P.x;
	int y1 = Xi.y - P.y;
	int x2 = next.x - P.x;
	int y2 = next.y - P.y;
	return x1*y2 - x2*y1;
}
double getNormCrossProduct(CvPoint Xi, CvPoint next, CvPoint P){
	double x1 = (double)(Xi.x - P.x);
	double y1 = (double)(Xi.y - P.y);
	double x2 = (double)(next.x - P.x);
	double y2 = (double)(next.y - P.y);
	double scalar1 = sqrt(x1*x1 + y1*y1);
	double scalar2 = sqrt(x2*x2 + y2*y2);
	x1 = x1/scalar1;
	y1 = y1/scalar1;
	x2 = x2/scalar2;
	y2 = y2/scalar2;
   return fabs(x1*y2 - x2*y1);
}
int perpLexU(vector<CvPoint> X1){
	int A,B;
	CvPoint2D32f Vec1, Vec2;
	//sort(X.begin(),X.end(),sortOutPoints);
	int start = 0;
	vector<CvPoint> X;
	int p = 0;
	int *used = new int[X1.size()+1];
	memset(used,0,sizeof(used));
	int counter = 0;
	Voxel *voxelGrid = new Voxel( &X1, 25, 25, 0, X1.size(), 2, 10);
	X.clear();
	X.insert(X.begin(),voxelGrid->listofPoints.begin(),voxelGrid->listofPoints.end());
	do{
	 int next = -1;
	 int dist = 0;
	 for(int i = 0; i < X.size(); i++){
		 if(i == p)continue;
		 if(used[i]== 1)continue;
		 if(next == -1) next = i;
		/*if(i == 0){
			Vec1.x = fabs((float) X.at(X.size() - 1).x - (float)X.at(i).x);
			Vec1.y = fabs((float)X.at(X.size() - 1).y - (float)X.at(i).y);
			Vec2.x = fabs((float)X.at(i+1).x - (float)X.at(i).x);
			Vec2.y = fabs((float)X.at(i+1).y - (float)X.at(i).y);
		}else if(i == (X.size() - 1)){
		    Vec1.x = fabs((float) X.at(i - 1).x - (float)X.at(i).x);
			Vec1.y = fabs((float)X.at(i - 1).y - (float)X.at(i).y);
			Vec2.x = fabs((float)X.at(0).x - (float)X.at(i).x);
			Vec2.y = fabs((float)X.at(0).y - (float)X.at(i).y);
		}//*/
		//else{
			Vec1.x = fabs((float)X.at(i).x - (float)X.at(p).x);
			Vec1.y = fabs((float)X.at(i).y - (float)X.at(p).y);
			Vec2.x = fabs((float)X.at(next).x - (float)X.at(p).x);
			Vec2.y = fabs((float)X.at(next).y - (float)X.at(p).y);
		//}
		double cross = getNormCrossProduct(X[i],X[next],X[p]);
		printf("Cross Product %lf\n",cross);
		double scalar1 = sqrt(Vec1.x*Vec1.x+Vec1.y*Vec1.y);
		double scalar2 = sqrt(Vec2.x*Vec2.x+Vec2.y*Vec2.y);
		Vec1.x = Vec1.x/scalar1;
		Vec1.y = Vec1.y/scalar1;
		Vec2.x = Vec2.x/scalar2;
		Vec2.y = Vec2.y/scalar2;
		double val = Vec1.x*Vec2.x + Vec1.y*Vec2.y;
		printf("Val is %lf\n",val);
		if(val <= 0.18){
			next = i;
			counter++;
			if(counter == 2)
				return 1;
			else
				break;
		}else if(cross < 0.20){
		    next = i;
		}
		if(counter == 4)
			return 1;
	 }
	 p = next;
	 used[p] = 1;
	}while(start != p);
	return 0;
}
int perpLex(vector<CvPoint> X1){
	int A,B;
	CvPoint2D32f Vec1, Vec2;
	vector<CvPoint> X;

	Voxel *voxelGrid = new Voxel( &X1, 25, 25, 0, X1.size(), 2, 10);
	X.clear();
	X.insert(X.begin(),voxelGrid->listofPoints.begin(),voxelGrid->listofPoints.end());
	//sort(X.begin(),X.end(),sortOutPoints);
	int counter = 0;
	for(int i = 0; i < X.size(); i++){
		if(i == 0){
			Vec1.x = fabs((float) X.at(X.size() - 1).x - (float)X.at(i).x);
			Vec1.y = fabs((float)X.at(X.size() - 1).y - (float)X.at(i).y);
			Vec2.x = fabs((float)X.at(i+1).x - (float)X.at(i).x);
			Vec2.y = fabs((float)X.at(i+1).y - (float)X.at(i).y);
		}else if(i == (X.size() - 1)){
		    Vec1.x = fabs((float) X.at(i - 1).x - (float)X.at(i).x);
			Vec1.y = fabs((float)X.at(i - 1).y - (float)X.at(i).y);
			Vec2.x = fabs((float)X.at(0).x - (float)X.at(i).x);
			Vec2.y = fabs((float)X.at(0).y - (float)X.at(i).y);
		}
		else{
			Vec1.x = fabs((float)X.at(i - 1).x - (float)X.at(i).x);
			Vec1.y = fabs((float)X.at(i - 1).y - (float)X.at(i).y);
			Vec2.x = fabs((float)X.at(i+1).x - (float)X.at(i).x);
			Vec2.y = fabs((float)X.at(i+1).y - (float)X.at(i).y);
		}
		double scalar1 = sqrt(Vec1.x*Vec1.x+Vec1.y*Vec1.y);
		double scalar2 = sqrt(Vec2.x*Vec2.x+Vec2.y*Vec2.y);
		Vec1.x = Vec1.x/scalar1;
		Vec1.y = Vec1.y/scalar1;
		Vec2.x = Vec2.x/scalar2;
		Vec2.y = Vec2.y/scalar2;
		double val = Vec1.x*Vec2.x + Vec1.y*Vec2.y;
		printf("Val is %lf\n",val);
		if(val <= 0.2)
			counter++;
		if(counter == 3)
			return 1;
	}
	return 0;
}

int getDotProduct(CvPoint Xi,CvPoint P){
	int x1 = Xi.x - P.x;
	int y1 = Xi.y - P.y;
	return x1*x1 + y1*y1; 
}
int convexHull(vector<CvPoint> X,bool onEdge){
	vector<CvPoint> container;
	container.clear();
	sort(X.begin(),X.end(),sortOutPoints);
	int N = X.size();
	int p = 0;
	int *used = new int[N + 1];
	int start = p;
	memset(used,0,sizeof(used));
	container.push_back(X.at(p));
	do{
	 int next = -1;
	 int dist = onEdge == true ? INF : 0;

	 for(int i = 0; i < N; i++){
	    if(i == p) continue;
		if(used[i] == 1) continue;
		if(next  == -1) next = i;
		int cross = getCrossProduct(X[i],X[next],X[p]);
		int dot = getDotProduct(X[i],X[p]);
		if(cross < 0){
		  next = i;
		  dist = dot;
		}else if(cross == 0){
			if(onEdge == true && dot < dist){
				next  = i;
				dist = dot;
			}
			else if(!onEdge && dot > dist){
				next = i;
				dist = dot;
			}
		}
	 }// end of For loop
	 p = next;
	 used[p] = true;
	 container.push_back(X.at(p));
	}while(start != p);
	if((container.size() - 1 ) == X.size()){
		printf("Size of the ConvexHull %d\n",container.size() - 1);
		container.push_back(cvPoint(-1,-1));
		return  container.size();
	}
	return 0;
}
