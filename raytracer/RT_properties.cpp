#include "RT_properties.h"
#include "RT_math.h"
#include <iostream>
#include <QImage>
#include <QColor>

using namespace std;

/* ------------- Methods of PixColor class ------------- */
PixColor::PixColor() { this->red=0.0; this->green=0.0; this->blue=0.0; this->alpha=0.0; }

PixColor::PixColor(float a,float b, float c, float d) {
    this->red=a; this->green=b; this->blue=c; this->alpha=d;
}

PixColor::PixColor(const PixColor& c) {
    this->red=c.red; this->green=c.green; this->blue=c.blue; this->alpha=c.alpha;
}

void PixColor::set(float a,float b, float c, float d) {
    this->red=a; this->green=b; this->blue=c; this->alpha=d;
}

void PixColor::set(PixColor c) {
    this->red=c.red; this->green=c.green; this->blue=c.blue; this->alpha=c.alpha;
}

void PixColor::add(PixColor c) {
    this->red = this->red + c.red;
    this->green = this->green + c.green;
    this->blue = this->blue + c.blue;
    this->alpha = this->alpha + c.alpha;
}

void PixColor::mix(PixColor c) {
    this->red = this->red * c.red;
    this->green = this->green * c.green;
    this->blue = this->blue * c.blue;
    this->alpha = this->alpha * c.alpha;
}

void PixColor::mul(float value) {
    this->red = this->red * value;
    this->green = this->green * value;
    this->blue = this->blue * value;
    this->alpha = this->alpha * value;
}

void PixColor::div(float value) {
    this->red = this->red / value;
    this->green = this->green / value;
    this->blue = this->blue / value;
    this->alpha = this->alpha / value;
}

void PixColor::HDR2LDR(float max_channel, float min_channel) {
    this->red = (this->red - min_channel)/(max_channel-min_channel);
    this->green = (this->green - min_channel)/(max_channel-min_channel);
    this->blue = (this->blue - min_channel)/(max_channel-min_channel);
    this->alpha = (this->alpha - min_channel)/(max_channel-min_channel);
}

PixColor& PixColor::operator =(const PixColor& c)
{
    if( this != &c )
    {
        this->red = c.red;
        this->green = c.green;
        this->blue = c.blue;
        this->alpha = c.alpha;
    }
    return *this;
}


/* ------------- Methods of PerlinNoise class ------------- */
PerlinNoise::PerlinNoise()
    :FACTOR(DIMENSION-1)
{
    float x,y;
    unsigned int i,j;
    for(i=0; i < DIMENSION; i++)
    {
        for(j=0; j < DIMENSION; j++)
        {
            x = d_rand(1,100); y = d_rand(1,100);
            gX[i][j] = x/100;
            gY[i][j] = y/100;
        }
    }
    this->noiseColor = new PixColor(0.361,0.298,0.102,0.0);
}

float PerlinNoise::noise2D(float u, float v)
{
    float X = FACTOR * u;
    float Y = FACTOR * v;

    float Iv;
    float Jv;
    float i = modf(X,&Iv);
    float j = modf(Y,&Jv);
    int I = (int)Iv;
    int J = (int)Jv;

    float C00 = gX[I][J]*i + gY[I][J]*j;
    float C10 = gX[I+1][J]*(i-1) + gY[I+1][J]*j;
    float alpha_i0 = (1-PERLIN(i)) * C00 + PERLIN(i) * C10;
    float C01 = gX[I][J+1]*i + gY[I][J+1]*(j-1);
    float C11 = gX[I+1][J+1]*(i-1) + gY[I+1][J+1]*(j-1);
    float alpha_i1 = (1-PERLIN(i)) * C01 + PERLIN(i) * C11;
    float alpha = alpha_i0*(1-PERLIN(j))+alpha_i1*PERLIN(j);
    alpha+=0.5;
    return alpha;
}

PixColor PerlinNoise::getNoiseColor() { return *noiseColor; }
PerlinNoise::~PerlinNoise() { delete noiseColor; }


/* ------------- Methods of JuliaSet class ------------- */
JuliaSet::JuliaSet(float r, float i)
    :real_C(r), img_C(i)
{ }

bool JuliaSet::isInside(float real_0, float img_0)
{
    float real_N, img_N;
    // Transform params into (-1,1) range // 1.5 is scale factor
    // Julia Set
    real_0 = 2*(real_0 - 0.5);
    img_0 = 2*(img_0 - 0.5);
    /*// Manderlot set
    real_C = 2*(real_0 - 0.5); img_C = 2*(img_0 - 0.5);
    real_0 = 0.0; img_0 = 0.0;*/

    for(unsigned int k=0; k<200; k++)
    {
        real_N = (real_0*real_0-img_0*img_0) + real_C;
        img_N = 2*real_0*img_0 + img_C;
        real_0 = real_N;
        img_0 = img_N;

        if( (real_N*real_N+img_N*img_N) > 1000 )
            return false;
    }
    return true;
}

bool JuliaSet::isIndide3D(float x, float y, float z)
{
    return false;
}


/* ------------- Methods of Material class ------------- */
Material::Material() {}

Material::Material(PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, string filename, float neta)
{
    this->colour = new PixColor(c);
    this->textureType = texEnable;
    this->materialType = val;
    this->TextureFile = filename;
    this->neta = neta;
    this->readTexture();
    //this->julia = new JuliaSet(-0.5,0.5);
    this->julia = new JuliaSet(-0.8,0.156);
    this->noise = new PerlinNoise();
}

void Material::readTexture()
{
    if(textureType == IMAGE)
    {
        unsigned int I,J;
        QColor c;
        QImage texture(TextureFile.c_str());
        if( texture.isNull() )
        {
            std::cerr<<" Given texture is not read properly; Check file path."<<std::endl;
        }
        this->texRows = texture.height();
        this->texCols = texture.width();

        texels = new PixColor*[texRows];
        for(I=0; I<texRows; I++)
            texels[I] = new PixColor[texCols];

        for(I=0; I<texRows; I++)
        {
            for(J=0; J<texCols; J++)
            {
                c = texture.pixel(I,J);
                texels[I][J].set(c.redF(), c.greenF(), c.blueF(), c.alphaF());
            }
        }
    }
}

PixColor Material::computeTexColor(float u, float v)
{
    if(textureType == IMAGE)
    {
        if( abs(u - 1.0) < 1.0e-6 ) u = 0.0;
        if(abs(v - 1.0) < 1.0e-6 ) v = 0.0;
        float X = u*(texRows-1);
        float Y = v*(texCols-1);
        int i = (int)X;
        int j = (int)Y;
        u = X - i;
        v = Y - j;
        PixColor ret_val;
        bilinearInterpolation(ret_val,u,v, texels[i][j], texels[i+1][j], texels[i+1][j+1], texels[i][j+1]);
        return ret_val;
    }
    else if(textureType == PROCEDURE)
    {
        if( abs(u - 1.0) < 1.0e-6 ) u = 0.0;
        if(abs(v - 1.0) < 1.0e-6 ) v = 0.0;

        // ( u-0.5)*(u-0.5)+(v-0.5)*(v-0.5) < 0.15
        // (u < 0.5 && v < 0.5) || (u > 0.5 && v > 0.5)

        /*float fraction, intpart;
        fraction = modf(noise->noise2D(u,v), &intpart);
        return (fraction*noise->getNoiseColor() + (1-fraction)*this->colour);*/

        if( julia->isInside(u,v) )         {
            PixColor ret_val(0.0,0.0,1.0,0.0);
            return ret_val;
        }
        else {
            PixColor ret_val(1.0,1.0,1.0,0.0);
            return ret_val;
        }
            //return this->colour;
    }
    else
    {
        std::cout<<"Texture is not enables, some wrong call occured, chk code\n";
        PixColor ret_val(0.0,0.0,0.0,0.0);
        return ret_val;
    }
}

PixColor Material::computeSolidTexColor(const Point& Ph)
{
    PixColor RED(0.0,0.0,1.0,0.0);
    PixColor WHITE(1.0,1.0,1.0,0.0);

    /*if( JUMP(Ph.X()) )
        return RED;
    else
        return WHITE;*/

    /*if( JUMP2(Ph.X(),Ph.Y()) )
        return RED;
    else
        return WHITE;*/

    /*if( RINGS(Ph.X(),Ph.Y(),Ph.Z()) )
        return RED;
    else
        return WHITE;*/

    float xFrac, xInt;
    xFrac = modf(Ph.x, &xInt);
    float yFrac, yInt;
    yFrac = modf(Ph.y, &yInt);

    if( julia->isInside(abs(xFrac),abs(yFrac)) )
        return RED;
    else
        return WHITE;
}

Material::~Material()
{
    if(textureType == IMAGE)
    {
        for(unsigned int i=0; i < texRows; i++)
            delete[] texels[i];
        delete[] texels;
    }
    delete noise;
    delete julia;
    delete colour;
}


/* ------------- Methods of SphereMap class ------------- */
SphereMap::SphereMap(Point& cen, float r, std::string filename)
{
    PixColor temp(1.0,1.0,1.0,0.0);
    this->center = new Point(cen);
    this->radius = r;
    this->matter = new Material(temp, IMAGE, diffusive, filename, 0.0);
    n = new Vector3D[3];
    this->n[0].set(0,0,-1);
    this->n[1].set(-1,0,0);
    this->n[2].set(0,-1,0);
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
