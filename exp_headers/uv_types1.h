#ifndef UV_TYPES1_H
#define UV_TYPES1_H


#define uv_Size UvSize
typedef struct uv_Size
{
	int width;
	int height;
} uv_Size;
static uv_Size uvSize(int a, int b)
{
    uv_Size output;
    output.width = a;
    output.height = b;
    return output;
}
//uv_Size uvSize(int a, int b);

typedef struct UvPoint2D16
{
    short x;
    short y;
} UvPoint2D16;

#define uv_Point UvPoint
typedef struct uv_Point
{
	int x;
	int y;
} uv_Point;
static UvPoint uvPoint(int x, int y)
{
    UvPoint output;
    output.x = x;
    output.y = y;
    return output;
}

#define uv_Point32f UvPoint2D32f
typedef struct uv_Point32f
{
  	float x;
  	float y;

} uv_Point32f;

typedef struct UvPoint2D64f
{
  	double x;
  	double y;

} UvPoint2D64f;
static UvPoint2D64f uvPoint2D64f(double x, double y)
{
    UvPoint2D64f output;
    output.x = x;
    output.y = y;
    return output;
}

typedef struct UvPoint3D64f
{
  	double x;
  	double y;
  	double z;

} UvPoint3D64f;
static UvPoint3D64f uvPoint3D64f(double x, double y, double z)
{
    UvPoint3D64f output;
    output.x = x;
    output.y = y;
    output.z = z;
    return output;
}

typedef struct uv_Keypoint
{
  	int x;
  	int y;
  	int response;
  	float angle;

} uv_Keypoint;

typedef struct uv_TermCriteria
{
	int type;
	int max_iter;
	double epsilon;

} uv_TermCriteria;

typedef struct UvScalar
{
    int val[4];
}UvScalar;
static UvScalar uvScalar(int val0, int val1, int val2, int val3)
{
    UvScalar scalar;
    scalar.val[0] = val0;
    scalar.val[1] = val1;
    scalar.val[2] = val2;
    scalar.val[3] = val3;
    return scalar;
}
static  UvScalar  uvRealScalar( int val0 )
{
    UvScalar scalar;
    scalar.val[0] = val0;
    scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
    return scalar;
}
static  UvScalar  uvScalarAll( int valAll )
{
    UvScalar scalar;
    scalar.val[0] = valAll;
    scalar.val[1] = valAll;
    scalar.val[2] = valAll;
    scalar.val[3] = valAll;
    return scalar;
}



#endif

