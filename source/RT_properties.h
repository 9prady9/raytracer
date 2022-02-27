#pragma once

#include <array>
#include <cmath>
#include <string>

#include <glm/glm.hpp>

/* Forward declarations - BEGIN */
class Ray;
class Point;
class Vector3D;
/* Forward declarations - END */

#define PERLIN(a) (3*a*a - 2*a*a*a)
#define JUMP(a) ( ((int)a)%2==0 ? 1 : 0 )
#define JUMP2(a,b) ( (((int)a)+((int)b))%2==0 ? 1 : 0 )
#define JUMP3(a,b,c) ( (((int)a)+((int)b)+((int)c))%2==0 ? 1 : 0 )
#define RINGS(a,b,c) ( ((int)(sqrt(a*a+b*b+c*c)))%2 ? 1 : 0 )

class SphereMap
{
private:
    Point* center;
    float radius;
    Material* matter;
    std::array<glm::highp_vec3 , 3> n;
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
