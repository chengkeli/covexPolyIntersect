#ifndef _KL_CONVEX_POLYGON_H_
#define _KL_CONVEX_POLYGON_H_

#include "KL_Common.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MaxNode 0xFFFF
#define INF 0xFFFFFFFF
#define eps 1e-10

typedef struct TPoint
{
	double x;
	double y;
}TPoint;

typedef struct TConvexHull
{
	TPoint * p;
	int n;
}TConvexHull;

typedef struct TSegment
{
	TPoint p1;
	TPoint p2;
}TSegment;

static int stack[MaxNode]; // index of vertices composing convex hull
static int top; // top of stack
static TPoint p0; // convex hull base

inline void swap(TPoint point[], int i, int j) {
	TPoint tmp;
	tmp = point[i];
	point[i] = point[j];
	point[j] = tmp;
}

inline double max(double x, double y) { return x > y ? x : y; }
inline double min(double x, double y) { return x < y ? x : y; }

inline double cross2d(TPoint p1, TPoint p2, TPoint p0)
{
	return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y); 
}

inline double dist2d(TPoint p1, TPoint p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); 
}

inline int cmp(const void *a, const void *b)
{
	TPoint *c = (TPoint *)a;
	TPoint *d = (TPoint *)b;
	double k = cross2d(*c, *d, p0);
	if (k < 0) return 1;
	else if (k == 0 && dist2d(*c, p0) >= dist2d(*d, p0)) return 1;
	else return -1;
}

void grahamScan(TPoint point[], int n, TConvexHull * CH); // find convex hull with Graham scan

bool isLineSegmentsIntersected(TPoint s1, TPoint e1, TPoint s2, TPoint e2);

bool isLineSegmentsIntersected(TSegment L1, TSegment L2);

bool isPointOnLineSegment(TSegment L, TPoint p);

bool isPointsOnSameSideOfLineSegment(TSegment L, TPoint p1, TPoint p2);

bool isPointInConvexHull_RayCast(TPoint q, const TConvexHull * CH);

bool isPointInConvexHull(TPoint q, const TConvexHull * CH);

int isConvexPolygonIntersected(const TConvexHull* CH1, const TConvexHull* CH2);

int isInsideConvexPolygon(const TConvexHull* CH1, const TConvexHull* CH2);

#endif