#ifndef RT_OBJECTS_H
#define RT_OBJECTS_H

#include <iostream>
#include <vector>

/* Forward declarations - BEGIN */
class Ray;
class Point;
class PixColor;
class Vector3D;
class Material;
class couple;
class triplet;
/* Forward declarations - END */

typedef int TYPE_INTEGER;

class Primitive
{
protected:
    char type;
    unsigned int id;
    Material* matter;
    Vector3D* Move;    // direction of movement
    float speed;
public:
    Primitive(char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, std::string filename, float neta, Vector3D& dir, float spd);
    // All below virtual methods are implemented by sub classes
    virtual float intersect(Ray r);
    virtual bool isVisible(const Point& p);
    virtual void extract_Color_Normal(const Point& Ph, Vector3D& norm, PixColor& mycolor);
    virtual Vector3D getNormal(const Point& Ph);
    virtual PixColor getColor(const Point& Ph);
    virtual void computeUV(const Point& Ph, float&u, float& v);
    virtual void translate(float time);
    virtual void moveAxis(float theta_abtY);
    // common methods implementations
    char getType();
    unsigned int getId();
    float getNeta();
    bool isTextureEnabled();
    bool ifDiffusive();
    bool isReflective();
    bool isRefractive();
    bool isEmissive();
    ~Primitive();
};

class Plane: public Primitive
{
private:
    Vector3D* normal;
    Point* P0;
    Vector3D* naxes;
    float texScale[3];
public:
    Plane(Vector3D& nor, Point& p, Vector3D* n, float s[], char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, std::string filename, float neta, Vector3D& dir, float spd);
    float intersect(Ray r);
    bool isVisible(const Point& p);
    void extract_Color_Normal(const Point& Ph, Vector3D& norm, PixColor& mycolor);
    Vector3D getNormal(const Point& Ph);
    PixColor getColor(const Point& Ph);
    void computeUV(const Point& Ph, float&u, float& v);
    void translate(float time);
    void moveAxis(float theta_abtY);
    ~Plane();
};
typedef class Plane* PLANE;

class Sphere: public Primitive
{
private:
    Point* center;
    float radius;
    Vector3D* naxes;
    float texScale[3];
public:
    Sphere(Point& cen, float r, Vector3D* n, float s[], char t, unsigned int id,  PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, std::string filename, float neta, Vector3D& dir, float spd);
    float intersect(Ray r);
    bool isVisible(const Point& p);
    void extract_Color_Normal(const Point& Ph, Vector3D& norm, PixColor& mycolor);
    Vector3D getNormal(const Point& Ph);
    PixColor getColor(const Point& Ph);
    void computeUV(const Point& Ph, float&u, float& v);
    void translate(float time);
    void moveAxis(float theta_abtY);
};
typedef class Sphere* SPHERE;

class Quadric: public Primitive
{
private:
    Vector3D* majorAX;
    Vector3D* minorAX;
    Point* Pc;
    float s[3];
    float ai2[3], ai1[3], a00;
    char ObjType;     /* Calculated based on data read */
    Vector3D* n;      /* Calculated based on data read */
public:
    Quadric(Vector3D& M, Vector3D& m, Point& p, float sval[], float ai[], char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, std::string filename, float neta, Vector3D& dir, float spd);
    float intersect(Ray r);
    bool isVisible(const Point& p);
    void extract_Color_Normal(const Point& Ph, Vector3D& norm, PixColor& mycolor);
    Vector3D getNormal(const Point& Ph);
    PixColor getColor(const Point& Ph);
    void computeUV(const Point& Ph, float&u, float& v);
    void translate(float time);
    void moveAxis(float theta_abtY);
};

class TriangularMesh: public Primitive
{
private:
    std::vector<Point*> v;
    std::vector<Vector3D*> vn;
    std::vector<couple*> vt;
    // Variables for indices of vertices, normals and texture coordinates
    std::vector<triplet*> triangles;
    std::vector<Vector3D*> faceNormals;
    std::vector<triplet*> vnIndices;
    std::vector<triplet*> vtIndices;

    void loadMesh(const char* filename);
    bool insideTriangle(const Point& Ph, unsigned int index);
    Vector3D trilinearInterpolation(float u, float v, const Vector3D* vn1, const Vector3D* vn2, const Vector3D* vn3);
public:
    /* normal on the surface for current point Ph
     * TUPLE2 variable to hold texture u,v for same current Ph
     * these two variables are updated for each call of intersect,
     * so that subsequent variable access will give current Ph's normal and u,v*/
    couple* UV;
    Point* minPh;
    unsigned int minID;

    // Actual class constructors/destructors and methods
    TriangularMesh(std::string objfile, char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, std::string filename, float neta, Vector3D& dir, float spd);
    float intersect(Ray r);
    bool isVisible(const Point& p);
    void extract_Color_Normal(const Point& Ph, Vector3D& norm, PixColor& mycolor);
    Vector3D getNormal(const Point& Ph);
    PixColor getColor(const Point& Ph);
    void computeUV(const Point& Ph, float&u, float& v);
    void translate(float time);
    void moveAxis(float theta_abtY);
    ~TriangularMesh();
};

#endif // RT_OBJECTS_H
