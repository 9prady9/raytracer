#include "RT_math.h"
#include <malloc.h>
#include <cstdlib>


/* ------------- Methods of Point class ------------- */
Point::Point(){}
Point::Point(float a,float b, float c) { x=a; y=b; z=c; }
Point::Point(const Point& p) { x=p.X(); y=p.Y(); z=p.Z(); }

void Point::set(float a,float b, float c) { x=a; y=b; z=c; }
void Point::set(Point p) { x=p.X(); y=p.Y(); z=p.Z(); }

float Point::X() const { return x; }
float Point::Y() const { return y; }
float Point::Z() const { return z; }

Point& Point::operator =(const Point& p)
{
    if(this != &p)
    {
        x=p.X(); y=p.Y(); z=p.Z();
    }
    return *this;
}


/* ------------- Methods of Vector3D class ------------- */
Vector3D::Vector3D() { vec[0]=0.0; vec[1]=0.0; vec[2]=0.0; mag=0.0; }
Vector3D::Vector3D(float x,float y,float z) { vec[0]=x; vec[1]=y; vec[2]=z; mag=sqrt(x*x+y*y+z*z); }

Vector3D::Vector3D(const Vector3D& v)
{
    vec[0]=v.DX(); vec[1]=v.DY(); vec[2]=v.DZ();
    mag = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

void Vector3D::set(float x,float y,float z) { vec[0]=x; vec[1]=y; vec[2]=z; mag=sqrt(x*x+y*y+z*z); }

void Vector3D::set(Vector3D v)
{
    vec[0]=v.DX(); vec[1]=v.DY(); vec[2]=v.DZ();
    mag = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

float Vector3D::DX() const { return vec[0]; }
float Vector3D::DY() const { return vec[1]; }
float Vector3D::DZ() const { return vec[2]; }
float Vector3D::magnitude() { return mag; }
int Vector3D::length() { return 3; }

void Vector3D::add(Vector3D& v)
{
    this->vec[0] = this->vec[0]+v.DX();
    this->vec[1] = this->vec[1]+v.DY();
    this->vec[2] = this->vec[2]+v.DZ();
    mag = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

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
        vec[0]=v.DX(); vec[1]=v.DY(); vec[2]=v.DZ();
        mag = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
    }
    return *this;
}


/* ------------- Methods of Ray class ------------- */
Ray::Ray() {}

Ray::Ray(Point o, Vector3D dir)
{
    this->origin = new Point;
    this->origin->set(o);
    this->direction = new Vector3D;
    this->direction->set(dir);
}

Point Ray::getOrigin() { return *origin; }
Vector3D Ray::getDirection() { return *direction; }
Point Ray::pointAtDist(float t) { return addP2V(*origin, mulV(*direction,t)); }
Ray::~Ray() { delete origin; delete direction; }


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

Point addP2V(const Point& op1, const Vector3D& op2) {
    Point ret_val(op1.X()+op2.DX(), op1.Y()+op2.DY(), op1.Z()+op2.DZ());
    return ret_val;
}

Vector3D subP4P(const Point& op1, const Point& op2) {
    Vector3D ret_val(op1.X()-op2.X(), op1.Y()-op2.Y(), op1.Z()-op2.Z());
    return ret_val;
}

Point subP4V(const Point& op1, const Vector3D& op2) {
    Point ret_val(op1.X()-op2.DX(), op1.Y()-op2.DY(), op1.Z()-op2.DZ());
    return ret_val;
}

Vector3D addV2V(const Vector3D& op1, const Vector3D& op2) {
    Vector3D ret_val(op1.DX()+op2.DX(), op1.DY()+op2.DY(), op1.DZ()+op2.DZ());
    return ret_val;
}

Vector3D subV4V(const Vector3D& op1, const Vector3D& op2) {
    Vector3D ret_val(op1.DX()-op2.DX(), op1.DY()-op2.DY(), op1.DZ()-op2.DZ());
    return ret_val;
}

Vector3D mulV(const Vector3D& op1, const float op2) {
    Vector3D ret_val(op1.DX()*op2, op1.DY()*op2, op1.DZ()*op2);
    return ret_val;
}

Vector3D divV(const Vector3D& op1,const float op2) {
    Vector3D ret_val(op1.DX()/op2, op1.DY()/op2, op1.DZ()/op2);
    return ret_val;
}

Vector3D crossProduct(const Vector3D& v0, const Vector3D& v1) {
    Vector3D ret_val(v0.DY()*v1.DZ()-v0.DZ()*v1.DY(), v0.DZ()*v1.DX()-v0.DX()*v1.DZ(), v0.DX()*v1.DY()-v0.DY()*v1.DX());
    return ret_val;
}

float dotProduct(const Vector3D& v0, const Vector3D& v1) {
    return (v0.DX()*v1.DX()+v0.DY()*v1.DY()+v0.DZ()*v1.DZ());
}

Mat toMatrix(Vector3D& v)
{
    /* convert a type Vector3D to type Mat with dimensions 3 by 1 matrix(coloumn vector) */
    Mat m(v.length(),1);
    m.setElement(0,0,v.DX());
    m.setElement(1,0,v.DY());
    m.setElement(2,0,v.DZ());
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
