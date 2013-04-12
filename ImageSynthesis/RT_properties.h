#ifndef RT_PROPERTIES_H
#define RT_PROPERTIES_H

#include <cmath>
#include <iostream>
#include <string>
#include <Qstring>

/* Forward declarations - BEGIN */
class Ray;
class Point;
class Vector3D;
/* Forward declarations - END */

#define PI 3.14159265
#define DIMENSION 30
#define PERLIN(a) (3*a*a - 2*a*a*a)
#define JUMP(a) ( ((int)a)%2==0 ? 1 : 0 )
#define JUMP2(a,b) ( (((int)a)+((int)b))%2==0 ? 1 : 0 )
#define JUMP3(a,b,c) ( (((int)a)+((int)b)+((int)c))%2==0 ? 1 : 0 )
#define RINGS(a,b,c) ( ((int)(sqrt(a*a+b*b+c*c)))%2 ? 1 : 0 )
enum MaterialType{ diffusive, reflective, refractive, emissive };
enum TexType{ NOTEXTURE, IMAGE, PROCEDURE, SOLID };

typedef int TYPE_INTEGER;


class PixColor
{
public:
    float red;
    float green;
    float blue;
    float alpha;
    // Constructors
    PixColor();
    PixColor(float a,float b, float c, float d);
    PixColor(const PixColor& c);
    void set(float a,float b, float c, float d);
    void set(PixColor c);
    // Methods
    void add(PixColor c);
    void mix(PixColor c);
    void mul(float value);
    void div(float value);
    void HDR2LDR(float max_channel, float min_channel);
    PixColor& operator=(const PixColor& c);
};


class PerlinNoise
{
private:
    float gX[DIMENSION][DIMENSION];
    float gY[DIMENSION][DIMENSION];
    unsigned int FACTOR;
    PixColor* noiseColor;
public:
    PerlinNoise();    
    float noise2D(float u, float v);
    PixColor getNoiseColor();
    ~PerlinNoise();
};


class JuliaSet
{
private:
    float real_C;
    float img_C;
public:
    JuliaSet(float r, float i);
    bool isInside(float real_0, float img_0);
    bool isIndide3D(float x, float y, float z);
};


class Material
{
private:
    PerlinNoise* noise;
    JuliaSet* julia;    
public:
//Attributes
    PixColor* colour;
    TYPE_INTEGER materialType;
    TYPE_INTEGER textureType;
    std::string TextureFile;    
    float neta;
    PixColor** texels;
    unsigned int texRows;
    unsigned int texCols;
// Methods
    Material();
    Material(PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, std::string filename, float neta=0.0);

    void readTexture(void);
    PixColor computeTexColor(float u, float v);
    PixColor computeSolidTexColor(const Point& Ph);
    ~Material();
};


class SphereMap
{
private:
    Point* center;
    float radius;
    Material* matter;
    Vector3D* n;
    // Methods private to the class
    void UV(Point& Ph, float& u, float &v);
public:
    SphereMap(Point& cen, float r, std::string filename);
    void intersect(Ray& r, PixColor& ImgColor);
    ~SphereMap();
};


class CubeMap
{
private:
    PixColor** map[6];
    unsigned int xsize;
    unsigned int ysize;
    void readTexture(std::string f, unsigned int k);
    PixColor readCubeMap(unsigned int k, float u, float v);
public:
    CubeMap(std::string f[]);
    void intersect(Ray& r, PixColor& ImgColor);
    ~CubeMap();
};

PixColor addC(const PixColor& op1, const PixColor& op2);
PixColor mixC(const PixColor& op1, const PixColor& op2);
PixColor mulC(const PixColor& op1, const float op2);
PixColor divC(const PixColor& op1, const float op2);
void bilinearInterpolation(PixColor& ret_val, float u, float v, PixColor& c1,PixColor& c2,PixColor& c3,PixColor& c4);

#endif // RT_PROPERTIES_H
