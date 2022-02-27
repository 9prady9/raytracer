#include "RT_properties.h"
#include "RT_math.h"
#include <iostream>

using namespace std;

/* ------------- Methods of SphereMap class ------------- */
SphereMap::SphereMap(Point& cen, float r, std::string filename)
{
    PixColor temp(1.0,1.0,1.0,0.0);
    this->center = new Point(cen);
    this->radius = r;
    this->matter = new Material(temp, IMAGE, diffusive, filename, 0.0);
    n[0] = glm::highp_vec3(0, 0, -1);
    n[1] = glm::highp_vec3(-1, 0, 0);
    n[2] = glm::highp_vec3(0, -1, 0);
}

void SphereMap::UV(Point& Ph, float& u, float &v)
{
    Vector3D temp;
    temp = Ph-*center;
    float X = dot(n[0], temp)/radius;
    float Y = dot(n[1], temp)/radius;
    float Z = dot(n[2], temp)/radius;
    u = 0.5*atan2(Y,X)/PI+0.5;
    v = acos(Z) / PI;
}

void SphereMap::intersect(Ray& r, PixColor& ImgColor)
{
    float t, b, c;

    // Move out of sphere to get a proper intersection
    Point newPh; newPh = r.origin + r.direction*(3*radius);
    Vector3D newDir; newDir = r.direction*-1.0;

    // Now compute the intersection as though a ray is hitting env map from outside
    Vector3D Vce; Vce = (*center)-newPh;
    b = dot( newDir, Vce);
    c = dot(Vce,Vce) - radius*radius;
    t = (b-sqrt(b*b-c));
    Point Ph; Ph = newPh+newDir*t;

    float u,v;
    this->UV(Ph, u, v);
    ImgColor = matter->computeTexColor( u,v );
}

SphereMap::~SphereMap()
{
    delete center;
    delete matter;
    delete[] n;
}


/* ------------- Methods of CubeMap class ------------- */
CubeMap::CubeMap(std::string f[])
{
    for(unsigned int k=0; k<6; k++)
    {
        this->readTexture(f[k],k);
    }
}

void CubeMap::readTexture(std::string f, unsigned int k)
{
    unsigned int I,J;
    QColor c;
    QImage texture(f.c_str());
    xsize = texture.height();
    ysize = texture.width();

    map[k] = new PixColor*[xsize];
    for(I=0; I<xsize; I++)
        map[k][I] = new PixColor[ysize];

    for(I=0; I<xsize; I++)
    {
        for(J=0; J<ysize; J++)
        {
            c = texture.pixel(I,J);
            if(k!=2)
                map[k][I][J].set( c.redF(), c.greenF(), c.blueF(), c.alphaF());
            else
                map[k][I][ysize-1-J].set(c.redF(), c.greenF(), c.blueF(), c.alphaF());
        }
    }
}

PixColor CubeMap::readCubeMap(unsigned int k, float u, float v)
{
    u = abs(u);
    v = abs(v);
    int umin = int(xsize * u);
    int vmin = int(ysize * v);
    int umax = int(xsize * u) + 1;
    int vmax = int(ysize * v) + 1;
    float ucoef = abs(xsize * u - umin);
    float vcoef = abs(ysize * v - vmin);

    // The texture is being addressed on [0,1]
    // There should be an addressing type in order to
    // determine how we should access texels when
    // the coordinates are beyond those boundaries.

    // Clamping is our current default and the only
    // implemented addressing type for now.
    // Clamping is done by bringing anything below zero
    // to the coordinate zero
    // and everything beyond one, to one.
    umin = min(max(umin, 0), xsize - 1);
    umax = min(max(umax, 0), xsize - 1);
    vmin = min(max(vmin, 0), ysize - 1);
    vmax = min(max(vmax, 0), ysize - 1);

    // What follows is a bilinear interpolation
    // along two coordinates u and v
    PixColor ret_val;
    bilinearInterpolation(ret_val, ucoef,vcoef, map[k][umin][vmin], map[k][umax][vmin], map[k][umax][vmax], map[k][umin][vmax]);
    return ret_val;
}

void CubeMap::intersect(Ray& r, PixColor& ImgColor)
{
    float u,v;
    Vector3D n_pe; n_pe = r.direction;

    if ((fabsf(n_pe.vec[0]) >= fabsf(n_pe.vec[1])) && (fabsf(n_pe.vec[0]) >= fabsf(n_pe.vec[2])))
    {
        if (n_pe.vec[0] > 0.0)
        {
            u = 1.0 - (n_pe.vec[2]/n_pe.vec[0]+ 1.0) * 0.5;
            v = (n_pe.vec[1]/n_pe.vec[0]+ 1.0) * 0.5;
            ImgColor = readCubeMap(0,u,v);
        }
        else if (n_pe.vec[0] < 0.0)
        {
            u = 1.0 - (n_pe.vec[2]/n_pe.vec[0]+ 1.0) * 0.5;
            v = 1.0 - (n_pe.vec[1]/n_pe.vec[0]+ 1.0) * 0.5;
            ImgColor = readCubeMap(1,u,v);
        }
    }
    else if ((fabsf(n_pe.vec[1]) >= fabsf(n_pe.vec[0])) && (fabsf(n_pe.vec[1]) >= fabsf(n_pe.vec[2])))
    {
        if (n_pe.vec[1] > 0.0)
        {
            u = (n_pe.vec[0]/n_pe.vec[1]+ 1.0) * 0.5;
            v = 1.0 - (n_pe.vec[2]/n_pe.vec[1]+ 1.0) * 0.5;
            ImgColor = readCubeMap(3,u,v);
        }
        else if (n_pe.vec[1] < 0.0)
        {
            u = 1.0 - (n_pe.vec[0]/n_pe.vec[1]+ 1.0) * 0.5;
            v = (n_pe.vec[2]/n_pe.vec[1]+ 1.0) * 0.5;
            ImgColor = readCubeMap(2,u,v);
        }
    }
    else if ((fabsf(n_pe.vec[2]) >= fabsf(n_pe.vec[0])) && (fabsf(n_pe.vec[2]) >= fabsf(n_pe.vec[1])))
    {
        if (n_pe.vec[2] > 0.0)
        {
            u = (n_pe.vec[0]/n_pe.vec[2]+ 1.0) * 0.5;
            v = (n_pe.vec[1]/n_pe.vec[2]+ 1.0) * 0.5;
            ImgColor = readCubeMap(4,u,v);
        }
        else if (n_pe.vec[2] < 0.0)
        {
            u = (n_pe.vec[0]/n_pe.vec[2]+ 1.0) * 0.5;
            v = 1.0 - (n_pe.vec[1]/n_pe.vec[2]+ 1.0) * 0.5;
            ImgColor = readCubeMap(5,u,v);
        }
    }
}

CubeMap::~CubeMap()
{
    for(unsigned int k=0; k<6; k++)
    {
        for(unsigned int i=0; i < xsize; i++)
            delete[] map[k][i];
        delete[] map[k];
    }
}




/* ------------- other non-class Methods ------------- */

PixColor addC(const PixColor& op1, const PixColor& op2) {
    PixColor ret_val(op1.red+op2.red, op1.green+op2.green, op1.blue+op2.blue, op1.alpha+op2.alpha);
    return ret_val;
}

PixColor mixC(const PixColor& op1, const PixColor& op2) {
    PixColor ret_val(op1.red*op2.red, op1.green*op2.green, op1.blue*op2.blue, op1.alpha*op2.alpha);
    return ret_val;
}

PixColor mulC(const PixColor& op1, const float op2) {
    PixColor ret_val(op2*op1.red, op2*op1.green, op2*op1.blue, op2*op1.alpha);
    return ret_val;
}

PixColor divC(const PixColor& op1, const float op2) {
    PixColor ret_val(op1.red/op2, op1.green/op2, op1.blue/op2, op1.alpha/op2);
    return ret_val;
}

void bilinearInterpolation(PixColor& ret_val, float u, float v, PixColor& c1, PixColor& c2, PixColor& c3, PixColor& c4)
{
    float r,g,b,a;
    r = ((1-u)*(1-v)*c1.red + u*(1-v)*c2.red + u*v*c3.red + (1-u)*v*c4.red);
    g = ((1-u)*(1-v)*c1.green + u*(1-v)*c2.green + u*v*c3.green + (1-u)*v*c4.green);
    b = ((1-u)*(1-v)*c1.blue + u*(1-v)*c2.blue + u*v*c3.blue + (1-u)*v*c4.blue);
    a = ((1-u)*(1-v)*c1.alpha + u*(1-v)*c2.alpha + u*v*c3.alpha + (1-u)*v*c4.alpha);
    ret_val.set(r,g,b,a);
}
