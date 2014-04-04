#include "RT_math.h"
#include <malloc.h>
#include <cstdlib>


/* ------------- Methods of Point class ------------- */
Point::Point() { Point(0.0f,0.0f,0.0f); }
Point::Point(float a,float b, float c) { x=a; y=b; z=c; }
Point::Point(const Point& p) { x=p.x; y=p.y; z=p.z; }

void Point::set(float a,float b, float c) { x=a; y=b; z=c; }

Point& Point::operator =(const Point& p)
{
    if(this != &p)
    {
        x=p.x; y=p.y; z=p.z;
    }
    return *this;
}


/* ------------- Methods of Vector3D class ------------- */
Vector3D::Vector3D() { vec[0]=0.0; vec[1]=0.0; vec[2]=0.0; mag=0.0; }
Vector3D::Vector3D(float x,float y,float z) { vec[0]=x; vec[1]=y; vec[2]=z; mag=sqrt(x*x+y*y+z*z); }

Vector3D::Vector3D(const Vector3D& v)
{
    vec[0]=v.vec[0]; vec[1]=v.vec[1]; vec[2]=v.vec[2];
    mag = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

void Vector3D::set(float x,float y,float z) { vec[0]=x; vec[1]=y; vec[2]=z; mag=sqrt(x*x+y*y+z*z); }

void Vector3D::normalize()
{
    if( !(vec[0]==0 && vec[1]==0 && vec[2]==0) )
    {
        vec[0] = vec[0]/mag;
        vec[1] = vec[1]/mag;
        vec[2] = vec[2]/mag;
        mag = 1.0;
    }
}

void Vector3D::invert()
{
    vec[0] = -1.0*vec[0]; vec[1] = -1.0*vec[1]; vec[2] = -1.0*vec[2];
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
    if( this != &v )
    {
        vec[0]=v.vec[0]; vec[1]=v.vec[1]; vec[2]=v.vec[2];
        mag = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
    }
    return *this;
}


/* ------------- Methods of Ray class ------------- */
Ray::Ray(const Point &o, const Vector3D &dir)
{
	this->origin = o;
    this->direction = dir;
}

Ray::Ray(const Ray& r)
{
	origin = r.origin;
	direction = r.direction;
}

Point Ray::pointAtDist(float t) { return origin+direction*t; }



Point operator+(const Point& op1, const Vector3D& op2) {
    return Point(op1.x+op2.vec[0], op1.y+op2.vec[1], op1.z+op2.vec[2]);
}

Vector3D operator-(const Point& op1, const Point& op2) {
    return Vector3D(op1.x-op2.x, op1.y-op2.y, op1.z-op2.z);
}

Point operator-(const Point& op1, const Vector3D& op2) {
    return Point(op1.x-op2.vec[0], op1.y-op2.vec[1], op1.z-op2.vec[2]);
}

Vector3D operator+(const Vector3D& op1, const Vector3D& op2) {
    return Vector3D(op1.vec[0]+op2.vec[0], op1.vec[1]+op2.vec[1], op1.vec[2]+op2.vec[2]);
}

Vector3D operator-(const Vector3D& op1, const Vector3D& op2) {
    return Vector3D(op1.vec[0]-op2.vec[0], op1.vec[1]-op2.vec[1], op1.vec[2]-op2.vec[2]);
}

Vector3D operator*(const Vector3D& op1, const float op2) {
    return Vector3D(op1.vec[0]*op2, op1.vec[1]*op2, op1.vec[2]*op2);
}

Vector3D operator/(const Vector3D& op1,const float op2) {
	return Vector3D(op1.vec[0]/op2, op1.vec[1]/op2, op1.vec[2]/op2);
}

Vector3D cross(const Vector3D& v0, const Vector3D& v1) {
    return Vector3D(v0.vec[1]*v1.vec[2]-v0.vec[2]*v1.vec[1], v0.vec[2]*v1.vec[0]-v0.vec[0]*v1.vec[2], v0.vec[0]*v1.vec[1]-v0.vec[1]*v1.vec[0]);
}

float dot(const Vector3D& v0, const Vector3D& v1) {
    return (v0.vec[0]*v1.vec[0]+v0.vec[1]*v1.vec[1]+v0.vec[2]*v1.vec[2]);
}



/* ------------- Methods of Mat class ------------- */
Mat::Mat() {}

Mat::Mat(int rows, int cols)
{
    DimenX = rows;
    DimenY = cols;
    matrix = NULL;
    matrix = (float**)malloc(DimenX*sizeof(float*));
    for(int x=0; x<DimenX; x++)
        matrix[x] = (float*)malloc(DimenY*sizeof(float));
}

Mat::Mat(float *array, int rows, int cols)
{
    DimenX = rows;
    DimenY = cols;
    matrix = NULL;
    matrix = (float**)malloc(DimenX*sizeof(float*));
    for(int x=0; x<DimenX; x++)
        matrix[x] = (float*)malloc(DimenY*sizeof(float));

    for(int x=0; x<DimenX; x++)
        for(int y=0; y<DimenY; y++)
            matrix[x][y] = array[x*DimenY+y];
}

int Mat::getDimenX() { return DimenX; }
int Mat::getDimenY() { return DimenY; }

void Mat::setElement(int i, int j, float value) { matrix[i][j] = value; }
float Mat::getElement(int i, int j) { return matrix[i][j]; }

void Mat::displayMatrix()
{
    for(int x=0; x<DimenX; x++)
    {
        for(int y=0; y<DimenY; y++)
            std::cout<<matrix[x][y]<<" ";
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void Mat::add(Mat& op)
{
    if(DimenX != op.getDimenX() || DimenY != op.getDimenY())
    {
        std::cerr<<"("<<DimenX<<","<<DimenY<<") != ("<<op.getDimenX()<<","<<op.getDimenY()<<")";
        std::cerr<<"    Unmatched dimensions - ADD;"<<std::endl;
        return;
    }
    for(int x=0; x<DimenX; x++)
        for(int y=0; y<DimenY; y++)
            matrix[x][y] = matrix[x][y] + op.getElement(x,y);
}

void Mat::mul(Mat& op)
{
    if(DimenY != op.getDimenX())
    {
        std::cerr<<DimenY<<" != "<<op.getDimenX();
        std::cerr<<"    Unmatched dimensions - MULTIPLY;"<<std::endl;
        return;
    }

    float** temp;
    temp = (float**)malloc(DimenX*sizeof(float*));
    for(int x=0; x<DimenX; x++)
        temp[x] = (float*)malloc(op.getDimenY()*sizeof(float));

    for(int x=0; x<DimenX; x++)
    {
        for(int y=0; y<op.getDimenY(); y++)
        {
            temp[x][y]=0.0;
            for(int k=0; k<DimenY; k++)
            {
                float t = matrix[x][k] * op.getElement(k,y);
                if( abs(t) > 1.0e-6 )
                    temp[x][y] = temp[x][y] + t;
            }
        }
    }
    free(matrix);
    matrix = temp;
    DimenY = op.getDimenY();
}

void Mat::scale(float value)
{
    for(int x=0; x<DimenX; x++)
        for(int y=0; y<DimenY; y++)
            matrix[x][y] = value * matrix[x][y];
}

Mat::~Mat()
{
    for(int x=0; x<DimenX; x++)
        delete[] matrix[x];
    delete[] matrix;
}




/* ------------- other non-class Methods ------------- */

float d_rand(float low, float high)
{
    float num = rand() * (high-low) / RAND_MAX + low ;
    return(num);
}

float min(float a, float b) { return (a>b ? b : a);}
float max(float a, float b) { return (b>a ? b : a);}

void copyArray(int* dest, int* source, int size)
{
        int i;
        for(i=0; i<size; i++)
                dest[i] = source[i];
}

Mat toMatrix(Vector3D& v)
{
    /* convert a type Vector3D to type Mat with dimensions 3 by 1 matrix(coloumn vector) */
    Mat m(3,1);
    m.setElement(0,0,v.vec[0]);
    m.setElement(1,0,v.vec[1]);
    m.setElement(2,0,v.vec[2]);
    return m;
}

Vector3D rotateVec(Mat& t, Vector3D& v)
{
    Mat m = toMatrix(v);
    t.mul(m);
    return toVector(t);
}

Vector3D toVector(Mat& m)
{
    /* convert a 3 by 1 matrix(coloumn vector) to type Vector3D */
    return Vector3D(m.getElement(0,0), m.getElement(1,0), m.getElement(2,0));
}
