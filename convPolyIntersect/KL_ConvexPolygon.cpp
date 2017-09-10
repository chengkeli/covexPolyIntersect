#include "KL_ConvexPolygon.h"

void grahamScan(TPoint point[], int n, TConvexHull * CH)
{	
	int i, u;
	
	u = 0;
	for (i = 1;i <= n - 1;i++){
		if ((point[i].y < point[u].y) ||
			(point[i].y == point[u].y && point[i].x < point[u].x))
			u = i;
	}
	swap(point, 0, u);
	p0 = point[0]; // 1. set the point at left bottom as p0

	qsort(point + 1, n - 1, sizeof (point[0]), cmp); //2. sort vertices by increasing angle

	// 3. find convex hull
	for (i = 0;i <= 2;i++) stack[i] = i;
	top = 2;
	for (i = 3;i <= n - 1;i++){
		while (cross2d(point[i], point[stack[top]], point[stack[top - 1]]) >= 0)
		{ 
			if (top == 0)
				break ;
			top--;
		}
		top++;
		stack[top] = i;
	}

	for (i = 0;i <= top;i++){
		CH->p[i].x = point[stack[i]].x;
		CH->p[i].y = point[stack[i]].y;
	}
	CH->n = top + 1;
}

bool isLineSegmentsIntersected(TPoint s1, TPoint e1, TPoint s2, TPoint e2)
{
	//check intersection between two line segment
	if (
		//1.line segments are intersected only if the two rectanges are intersected. For quick rejection.
		(max(s1.x, e1.x) >= min(s2.x, e2.x)) &&
		(max(s2.x, e2.x) >= min(s1.x, e1.x)) &&
		(max(s1.y, e1.y) >= min(s2.y, e2.y)) &&
		(max(s2.y, e2.y) >= min(s1.y, e1.y)) &&
		//2.line segments are intersected if and only if following two requirements are satisfied:
		(cross2d(s2, e1, s1) * cross2d(e1, e2, s1) >= 0) && //1. rotation form segment (s2->s1) to segment (e1->s1) has the same direction as the rotation from segment (e1->s1) to segment (e2->s1)
		(cross2d(s1, e2, s2) * cross2d(e2, e1, s2) >= 0)) //2. rotation from segment (s1->s2) to segment (e2->s2) has the same direction as rotation from segment (e2->s2) to segment (e1->s2)
		return true ;
	return false ;
}

bool isLineSegmentsIntersected(TSegment L1, TSegment L2)
{
	TPoint s1 = L1.p1;
	TPoint e1 = L1.p2;
	TPoint s2 = L2.p1;
	TPoint e2 = L2.p2;

	if (
		(max(s1.x, e1.x) >= min(s2.x, e2.x)) &&
		(max(s2.x, e2.x) >= min(s1.x, e1.x)) &&
		(max(s1.y, e1.y) >= min(s2.y, e2.y)) &&
		(max(s2.y, e2.y) >= min(s1.y, e1.y)) &&
		(cross2d(s2, e1, s1) * cross2d(e1, e2, s1) >= 0) &&
		(cross2d(s1, e2, s2) * cross2d(e2, e1, s2) >= 0)
		) return true ;
	return false ;
}

bool isPointOnLineSegment(TSegment L, TPoint p)
{
	double dx, dy, dx1, dy1;
	dx = L.p2.x - L.p1.x;
	dy = L.p2.y - L.p1.y;
	dx1 = p.x - L.p1.x;
	dy1 = p.y - L.p1.y;
	if (dx * dy1 - dy * dx1 != 0) 
		return false ; // 1. point is on line segment only if point is on the line
	if (dx1 * (dx1 - dx) < 0 || dy1 * (dy1 - dy) < 0) 
		return true ; // 2. point is on line segment only if point is in the rectangle defined by 'L'
	return false ;
}

bool isPointsOnSameSideOfLineSegment(TSegment L, TPoint p1, TPoint p2)
{
	//return true if 'p1' and 'p2' are on the same side of line segment 'L'
	if (cross2d(p1, L.p2, L.p1) * cross2d(L.p2, p2, L.p1)< 0) 
		return true ; 
	return false ;
}

bool isPointInConvexHull_RayCast(TPoint q, const TConvexHull * CH)
{
	// check point in polygon by ray casting algorithm: https://en.wikipedia.org/wiki/Point_in_polygon
	int c, i, n;
	TSegment L1, L2;
	c = 0; // count the time of intersect between edges of polygon and horizontal line passing point 'q'
	n = CH->n;
	L1.p1 = q;
	L1.p2 = q;
	L1.p2.x = INF;
	
	for (i = 0;i < CH->n;i++){
		L2.p1 = CH->p[i];
		L2.p2 = CH->p[(i + 1) % n];
		if (isLineSegmentsIntersected(L1, L2)){
			c++;
			continue ;
		}
	}
	if (c % 2 == 0) return false ;
	else return true ;
}

bool isPointInConvexHull(TPoint q, const TConvexHull * CH)
{
	int n = n = CH->n;
	if(n<3) return false;
    if(cross2d(q, CH->p[1]  , CH->p[0]) > -eps) return false;
    if(cross2d(q, CH->p[n-1], CH->p[0]) <  eps) return false;
 
    int i = 2,j = n - 1;
    int line = -1;
 
    while(i<=j)
    {
        int mid = (i + j) >> 1;
        if(cross2d(q, CH->p[mid], CH->p[0]) > -eps)
        {
            line=mid;
            j=mid-1;
        }
        else i=mid+1;
    }
    return cross2d(q, CH->p[line], CH->p[line-1]) < -eps;
}

int isConvexPolygonIntersected(const TConvexHull* CH1, const TConvexHull* CH2)
{
	// 1. check intersection of every edge between two convex polygon
	int i, j, s, t;
	for (i = 0;i < CH1->n;i++){
		j =i + 1;
		for (s = 0;s < CH2->n;s++){
			t = s + 1;
			if (isLineSegmentsIntersected(CH1->p[i], CH1->p[j%CH1->n], CH2->p[s],CH2->p[t%CH2->n])) 
				return 1;
		}
	}
	// 2. 
	if (isPointInConvexHull(CH1->p[CH1->n - 1], CH2) || isPointInConvexHull(CH2->p[CH2->n - 1], CH1)) 
		return 1;
	else 
		return 0;
}

int isInsideConvexPolygon(const TConvexHull* CH1, const TConvexHull* CH2)
{
	// 1. check intersection of every edge between two convex polygon
	int i, j, s, t;
	for (i = 0;i < CH1->n;i++){
		j =i + 1;
		for (s = 0;s < CH2->n;s++){
			t = s + 1;
			if (isLineSegmentsIntersected(CH1->p[i], CH1->p[j%CH1->n], CH2->p[s],CH2->p[t%CH2->n])) 
				return 0;
		}
	}
	// 2. 
	if (isPointInConvexHull(CH1->p[CH1->n - 1], CH2)) 
		return 1;
	else 
		return 0;
}