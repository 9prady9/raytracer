#ifndef RT_MATH_H
#define RT_MATH_H

#include <cmath>
#include <iostream>

#ifdef _WIN32
    inline float asinh(float value) {
        return log(value+sqrt(1+value*value));
    }

    inline float acosh(float value, int &error) {
        error = 0;
        if(value>=1)
            return log(value+sqrt(value*value-1));
        else
            error=1;
        return 0;
    }
#endif

class couple
{
public:
    float a1;
    float a2;
    couple() {}
    couple(float a,float b) {
        a1=a; a2=b;
    }
    void set(float a,float b) {
        a1=a; a2=b;
    }
};


class triplet
{
public:
    unsigned int a0;
    unsigned int a1;
    unsigned int a2;
    triplet() {}
    triplet(unsigned int a,unsigned int b, unsigned int c) {
        a0=a; a1=b; a2=c;
    }
    void set(unsigned int a,unsigned int b, unsigned int c) {
        a0=a; a1=b; a2=c;
    }
};


class Point
{
public:
    float x,y,z;

	Point();
    Point(float a,float b, float c);
    Point(const Point& p);
    void set(float a,float b, float c);
    Point& operator=(const Point& p);
};

class Vector3D
{
public:
    float vec[3];
    float mag;

    Vector3D();
    Vector3D(float x,float y,float z);
    Vector3D(const Vector3D& v);
    void set(float x,float y,float z);
    void normalize();
    void invert();
    Vector3D& operator=(const Vector3D& v);
};

class Ray
{
public:
    Point origin;
    Vector3D direction;

    Ray(const Point &o, const Vector3D &dir);
	Ray(const Ray& r);
    Point pointAtDist(float t);
};


Point operator+(const Point& op1, const Vector3D& op2);
Vector3D operator-(const Point& op1, const Point& op2);
Point operator-(const Point& op1, const Vector3D& op2);
Vector3D operator+(const Vector3D& op1, const Vector3D& op2);
Vector3D operator-(const Vector3D& op1, const Vector3D& op2);
Vector3D operator*(const Vector3D& op1, const float op2);
Vector3D operator/(const Vector3D& op1,const float op2);
Vector3D cross(const Vector3D& v0, const Vector3D& v1);
float dot(const Vector3D& v0, const Vector3D& v1);


class Mat
{
private:
    float** matrix;
    int DimenX;
    int DimenY;
public:
    Mat();
    Mat(int rows, int cols);
    Mat(float* array, int rows, int cols);
    int getDimenX();
    int getDimenY();
    void setElement(int i, int j, float value);
    float getElement(int i, int j);
    void displayMatrix();
    void mul(Mat& op);
    void add(Mat& op);
    void scale(float value);
    ~Mat();
};


float d_rand(float low, float high);
float min(float a, float b);
float max(float a, float b);
void copyArray(int* dest, int* source, int size);
Mat toMatrix(Vector3D& v);
Vector3D rotateVec(Mat& t, Vector3D& v);
Vector3D toVector(Mat& m);

#endif // RTMATH_H
