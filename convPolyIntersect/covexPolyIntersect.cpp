#include "KL_Common.h"
#include "KL_ConvexPolygon.h"

cv::Mat Canvas;
int tracker[4] = {220, 180, 420, 300};
vector<TPoint> trustZonePtSet;
char nameWin[500] = "Check Convex Polygon Intersection";

void drawTracker(cv::Mat img)
{
	cv::rectangle(img,cv::Point(tracker[0], tracker[1]), cv::Point(tracker[2], tracker[3]), Scalar(0,255,0,0),1,8,0);
}

void drawConvexHull(cv::Mat img, const TConvexHull * CH)
{
	size_t N = CH->n;
	cv::line(img, cv::Point(CH->p[N - 1].x, CH->p[N - 1].y), cv::Point(CH->p[0].x, CH->p[0].y), Scalar(0,255,0,0),1,8,0);
	cv::putText(img,"0",cv::Point(CH->p[0].x, CH->p[0].y),FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255,0));  
	while(N > 1){
		cv::line(img, cv::Point(CH->p[N - 1].x, CH->p[N - 1].y), cv::Point(CH->p[N - 2].x, CH->p[N - 2].y), Scalar(0,255,0,0),1,8,0);
		char num[100]; itoa(N - 1, num, 10);
		cv::putText(img,num,cv::Point(CH->p[N - 1].x, CH->p[N - 1].y),FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255,0));  
		N--;
	}
}

void on_mouse(int event,int x,int y,int flags,void *ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		TPoint tmpPt; tmpPt.x = x; tmpPt.y = y;
		trustZonePtSet.push_back(tmpPt);
		cv::Mat img; Canvas.copyTo(img);

		size_t N = trustZonePtSet.size();
		while(N > 1){
			cv::line(img, cv::Point(trustZonePtSet[N - 1].x, trustZonePtSet[N - 1].y), cv::Point(trustZonePtSet[N - 2].x, trustZonePtSet[N - 2].y), Scalar(0,255,0,0),1,8,0);
			N--;
		}
		cv::imshow(nameWin,img);  
	}
	else if(event == CV_EVENT_RBUTTONDOWN)
	{
		size_t N = trustZonePtSet.size(); 
		if(N < 3) return;

		TPoint * ptSet = (TPoint*)malloc(sizeof(TPoint) * N);
		for(int i = 0; i < N; i++) ptSet[i] = trustZonePtSet[i];
		TConvexHull trustZoneConvexHull;
		trustZoneConvexHull.p = (TPoint*)malloc(sizeof(TPoint) * N);
		grahamScan(ptSet, N, &trustZoneConvexHull);
		free(ptSet);

		ptSet = (TPoint*)malloc(sizeof(TPoint) * 4);
		ptSet[0].x = tracker[0]; ptSet[0].y = tracker[1]; ptSet[1].x = tracker[2]; ptSet[1].y = tracker[1];
		ptSet[2].x = tracker[0]; ptSet[2].y = tracker[3]; ptSet[3].x = tracker[2]; ptSet[3].y = tracker[3];
		TConvexHull trackerConvexHull;
		trackerConvexHull.p = (TPoint*)malloc(sizeof(TPoint) * 4);
		grahamScan(ptSet, 4, &trackerConvexHull);
		free(ptSet);

		cv::Mat img; Canvas.copyTo(img);
		if(trustZoneConvexHull.n < N) cv::putText(img,"Trust Zone Must be Convex!",cv::Point(10, 80),FONT_HERSHEY_SIMPLEX,.7f,Scalar(0,255,0,0), 3);  
		drawConvexHull(img, &trustZoneConvexHull);
		drawConvexHull(img, &trackerConvexHull);

		if(isConvexPolygonIntersected(&trustZoneConvexHull, &trackerConvexHull))
			cv::putText(img,"Intersected",cv::Point(10, 40),FONT_HERSHEY_SIMPLEX,1.5,Scalar(0,0,255,0), 3); 

		else
			cv::putText(img,"Not Intersected",cv::Point(10,40),FONT_HERSHEY_SIMPLEX,1.5,Scalar(255,0,0,0), 3);

		if(isInsideConvexPolygon( &trackerConvexHull, &trustZoneConvexHull))
			cv::putText(img,", and tracker is in trust zone",cv::Point(270, 40),FONT_HERSHEY_SIMPLEX,.7f,Scalar(0,0,255,0), 2.5f);


		cv::imshow(nameWin,img); 
		free(trustZoneConvexHull.p);
		free(trackerConvexHull.p);
		trustZonePtSet.clear();
	}
}

int main(int argc, char ** argv)
{
	/*int i, d, t1, t2, p, test = 1;
	double x1, y1, x2, y2;
	while (scanf("%d %d ", &d, &p) && d && p){
		printf("Polygon A has %d vertices, B has %p vertices.\n", d, p);

		if (test != 1) printf("\n");

		t1 = 0;
		for (i = 0;i < d;i++){
			scanf("%lf %lf %lf %lf ",&x1, &y1, &x2, &y2);
			black[t1].x = x1;
			black[t1].y = y1;
			t1++;
			black[t1].x = x2;
			black[t1].y = y2;
			t1++;
			black[t1].x = x1;
			black[t1].y = y2;
			t1++;
			black[t1].x = x2;
			black[t1].y = y1;
		}

		t2 = 0;
		for (i = 0;i < p;i++){
			scanf("%lf %lf %lf %lf ", &x1, &y1, &x2, &y2);
			white[t2].x = x1;
			white[t2].y = y1;
			t2++;
			white[t2].x = x2;
			white[t2].y = y2;
			t2++;
			white[t2].x = x1;
			white[t2].y = y2;
			t2++;
			white[t2].x = x2;
			white[t2].y = y1;
		}

		grahamScan(black, t1 + 1);
		for (i = 0;i <= top;i++){
			covtexb.p[i].x = black[stack[i]].x;
			covtexb.p[i].y = black[stack[i]].y;
		}
		covtexb.n = top + 1;

		grahamScan(white, t2 + 1);
		for (i = 0;i <= top;i++){
			covtexw.p[i].x = white[stack[i]].x;
			covtexw.p[i].y = white[stack[i]].y;
		}
		covtexw.n = top + 1;

		if (isConvexPolygonIntersected())
			printf("Case %d: It is possible to separate the two groups of vendors.\n", test++); else
			printf("Case %d: It is not possible to separate the two groups of vendors.\n", test++); 
	}*/

	size_t W = 640, H = 480, Dim = W * H * 3;
	uchar * data = (uchar*)malloc(sizeof(uchar) * Dim);
	memset(data, 0, sizeof(uchar) * Dim);
	Canvas = cv::Mat(cv::Size(W, H), CV_8UC3, data);
	drawTracker(Canvas);

	cv::namedWindow(nameWin);
	setMouseCallback(nameWin,on_mouse,0);
	cv::imshow(nameWin,Canvas);  
    cv::waitKey(-1);

	free(data);  
	return 0;
}