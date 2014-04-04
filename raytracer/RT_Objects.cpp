#include "RT_Objects.h"
#include "RT_math.h"
#include "RT_properties.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;


/* ------------- Methods of Primitive class ------------- */
Primitive::Primitive(char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, string filename, float neta, Vector3D& dir, float spd)
{
    this->type = t;
    this->id = id;
    this->matter = new Material(c, texEnable, val, filename, neta);
    this->Move = new Vector3D(dir);
    this->speed = spd;
}

float Primitive::intersect(Ray r) {
    //Do nothing
    std::cout<<"Primitive intersect reached, check the code\n";
    return 0.0;
}

bool Primitive::isVisible(const Point& p) {
    //Do nothing
    std::cout<<"Primitive isVisible reached, check the code\n";
    return false;
}

void Primitive::extract_Color_Normal(const Point& Ph, Vector3D &normal, PixColor &mycolor) {
    //Do nothing
    std::cout<<"Primitive extract_Color_Normal reached, check the code\n";
}

Vector3D Primitive::getNormal(const Point& Ph) {
    //Do nothing    
    std::cout<<"Primitive getNormal reached, check the code\n";
    return Vector3D (0,0,0);
}

PixColor Primitive::getColor(const Point& Ph) {
    //Do nothing
    std::cout<<"Primitive getColor reached, check the code\n";
    return PixColor(0,0,0,0);
}

void Primitive::computeUV(const Point& Ph, float&u, float& v)
{
    std::cout<<"Primitive computeUV reached, check the code\n";
    u = 0.0; v = 0.0;
}

char Primitive::getType() { return this->type; }

unsigned int Primitive::getId() { return this->id; }

float Primitive::getNeta() { return matter->neta; }

bool Primitive::isTextureEnabled() { return matter->textureType==IMAGE; }
bool Primitive::ifDiffusive() { return (matter->materialType == diffusive ? true : false ); }
bool Primitive::isReflective() { return (matter->materialType == reflective ? true : false ); }
bool Primitive::isRefractive() { return (matter->materialType == refractive ? true : false ); }
bool Primitive::isEmissive() { return (matter->materialType == emissive ? true : false ); }

void Primitive::translate(float time) {
    std::cout<<"Primitive translate reached, check the code\n";
}

void Primitive::moveAxis(float theta_abtY)
{
    std::cout<<"Primitive moveAxis reached, check the code\n";
}

Primitive::~Primitive() { delete matter; delete Move; }


/* ------------- Methods of Plane class ------------- */
Plane::Plane(Vector3D& nor, Point& p, Vector3D* n, float s[], char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, string filename, float neta, Vector3D& dir, float spd)
    : Primitive(t, id, c, texEnable, val, filename, neta, dir, spd)
{
    nor.normalize();
    this->normal = new Vector3D(nor);
    this->P0 = new Point(p);
    naxes = new Vector3D[3];
    this->naxes[0] = n[0];
    this->naxes[1] = n[1];
    this->naxes[2] = cross(n[0], n[1]);
    this->texScale[0] = s[0];
    this->texScale[1] = s[1];
    this->texScale[2] = s[2];
}

float Plane::intersect(Ray r)
{
    float ret_val;

    float tmp = dot(*normal, r.direction);
    if( tmp > 0)
        ret_val = -1.0;
    else
        ret_val = dot(*normal, (*P0)-r.origin)/tmp;
    return ret_val;
}

bool Plane::isVisible(const Point& p)
{
    return !(dot(*normal,p-(*P0)) < 0);
}

void Plane::extract_Color_Normal(const Point& Ph, Vector3D &norm, PixColor &mycolor)
{
    norm = *normal;
    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
        {
            mycolor = matter->computeSolidTexColor(Ph);
        }
        else
        {
            float u, v;
            this->computeUV(Ph, u, v);
            mycolor = matter->computeTexColor(u,v);
        }
    }
    else
        mycolor.set(*matter->colour);
}

Vector3D Plane::getNormal(const Point& Ph) { return *normal; }

PixColor Plane::getColor(const Point& Ph)
{
    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
        {
            return matter->computeSolidTexColor(Ph);
        }
        else
        {
            float u, v;
            this->computeUV(Ph, u, v);
            return matter->computeTexColor(u,v);
        }
    }
    else
        return *matter->colour;
}

void Plane::computeUV(const Point& Ph, float&u, float& v)
{
    float X,Y,Z;
    Vector3D temp = Ph-(*P0);
    X = dot(naxes[0], temp)/texScale[0];
    Y = dot(naxes[1], temp)/texScale[1];
    Z = dot(naxes[2], temp)/texScale[2];

    u = (normal->vec[0] == -1) ? Y - (int)Y : ( (normal->vec[1]== -1) ? Z-(int)Z : X-(int)X);
    if(u<0) u=1+u;
    v = (normal->vec[0] == -1) ? Z - (int)Z : ( (normal->vec[1]== -1) ? X-(int)X : Y-(int)Y);
    if(v<0) v=1+v;
}

void Plane::translate(float time)
{
	Point tmp = (*P0)+(*Move)*(speed*time);
    this->P0->set(tmp.x,tmp.y,tmp.z);
}

void Plane::moveAxis(float theta_abtY)
{
    ;//Do nothing
    float* array = new float[9];
    array[0] = cos(theta_abtY); array[1] = 0; array[2] = -1.0*sin(theta_abtY);
    array[3] = 0; array[4] = 1; array[5] = 0;
    array[6] = sin(theta_abtY); array[7] = 0; array[8] = cos(theta_abtY);    
    for(unsigned int k=0; k< 9; k++)
        if( abs(array[k]) < 1.0e-6 ) array[k]=0.0;
    Mat r1(array, 3, 3);
    Mat r2(array, 3, 3);
    Mat r3(array, 3, 3);
    naxes[0] = rotateVec(r1, naxes[0]);
    naxes[1] = rotateVec(r2, naxes[1]);
    naxes[2] = rotateVec(r3, naxes[2]);
    delete array;
}


/* ------------- Methods of Sphere class ------------- */
Sphere::Sphere(Point& cen, float r, Vector3D* n, float s[], char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, string filename, float neta, Vector3D& dir, float spd)
    :Primitive(t, id, c, texEnable, val, filename, neta, dir, spd)
{
    this->center = new Point(cen);
    this->radius = r;
    naxes = new Vector3D[3];
    this->naxes[0] = n[0];
    this->naxes[1] = n[1];
    this->naxes[2] = cross(n[0], n[1]);
    this->texScale[0] = s[0];
    this->texScale[1] = s[1];
    this->texScale[2] = s[2];
}

float Sphere::intersect(Ray r)
{
    float ret_val, b, c, discri;
    Vector3D Vce = *center - r.origin;

    b = dot( r.direction, Vce);
    c = dot(Vce,Vce) - radius*radius;
    discri = b*b-c;
    if( b > 0.0 && discri >= 0.0 )
        ret_val = (b-sqrt(discri));
    else
        ret_val = -1.0;
    return ret_val;
}

bool Sphere::isVisible(const Point& p)
{
    bool ret_val;
    Vector3D temp = *center-p;
    ret_val = ( dot(temp,temp) - (radius*radius) > 0 );
    return ret_val;
}

void Sphere::extract_Color_Normal(const Point& Ph, Vector3D &norm, PixColor &mycolor)
{
    norm = Ph - *center;
    norm.normalize();

    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
        {
            mycolor = matter->computeSolidTexColor(Ph);
        }
        else
        {
            float u, v;
            this->computeUV(Ph, u, v);
            mycolor = matter->computeTexColor(u,v);
        }
    }
    else
        mycolor.set(*matter->colour);
}

Vector3D Sphere::getNormal(const Point& Ph)
{
    Vector3D ret_val = Ph-*center;
    ret_val.normalize();
    return ret_val;
}

PixColor Sphere::getColor(const Point& Ph)
{
    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
        {
            return matter->computeSolidTexColor(Ph);
        }
        else
        {
            float u, v;
            this->computeUV(Ph, u, v);
            return matter->computeTexColor(u,v);
        }
    }
    else
        return *matter->colour;
}

void Sphere::computeUV(const Point& Ph, float&u, float& v)
{
    Vector3D temp = Ph-*center;
    float X = dot(naxes[0], temp)/radius;
    float Y = dot(naxes[1], temp)/radius;
    float Z = dot(naxes[2], temp)/radius;
    u = 0.5*atan2(Y,X)/PI+0.5;
    v = acos(Z) / PI;
}

void Sphere::translate(float time)
{
	Point tmp = (*center)+(*Move)*(speed*time);
    this->center->set(tmp.x,tmp.y,tmp.z);
}

void Sphere::moveAxis(float theta_abtY)
{
    ;//Do nothing
    float* array = new float[9];
    array[0] = cos(theta_abtY); array[1] = 0; array[2] = -1.0*sin(theta_abtY);    
    array[3] = 0; array[4] = 1; array[5] = 0;
    array[6] = sin(theta_abtY); array[7] = 0; array[8] = cos(theta_abtY);
    for(unsigned int k=0; k< 9; k++)
        if( abs(array[k]) < 1.0e-6 ) array[k]=0.0;
    Mat r1(array, 3, 3);
    Mat r2(array, 3, 3);
    Mat r3(array, 3, 3);
    naxes[0] = rotateVec(r1, naxes[0]);
    naxes[1] = rotateVec(r2, naxes[1]);
    naxes[2] = rotateVec(r3, naxes[2]);
    delete array;
}


/* ------------- Methods of Quadric class ------------- */
Quadric::Quadric(Vector3D& M, Vector3D& m, Point& p, float sval[], float ai[], char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, string filename, float neta, Vector3D& dir, float spd)
    :Primitive(t, id, c, texEnable, val, filename, neta, dir, spd)
{
    majorAX = new Vector3D(M);
    minorAX = new Vector3D(m);
    this->Pc = new Point(p);
    s[0] = sval[0]; s[1] = sval[1]; s[2] = sval[2];
    ai2[0] = ai[0]; ai2[1] = ai[1]; ai2[2] = ai[2];
    ai1[0] = ai[3]; ai1[1] = ai[4]; ai1[2] = ai[5];
    a00 = ai[6];

    // Generate the Local Coordinate space
    this->n = new Vector3D[3];
    n[0] = *majorAX;
    n[1] = *minorAX;
    n[2] = cross(n[0],n[1]);
    n[0].normalize();
    n[1].normalize();
    n[2].normalize();

    // Identify if the Quadric is a Sphere/Plane/Cone/Cylinder/Cone/Hyperboloid/Paraboloid.
    if(ai2[0]==0 && ai2[1]==0 && ai2[2]==0) {
        ObjType = 'P';       // Plane
    }else if(ai2[0]>0 && ai2[1]>0 && ai2[2]>0) {
        ObjType = 'E';       // Ellipsoid
    }else if( (ai2[0]>0 && ai2[1]>0 && ai2[2]==0) ||
              (ai2[1]>0 && ai2[2]>0 && ai2[0]==0) ||
              (ai2[2]>0 && ai2[0]>0 && ai2[1]==0)) {
        if(a00==0)
            ObjType='Y';     // Paraboloid
        else
            ObjType='C';     // Cylinder
    }else if((ai2[0]>0 && ai2[1]>0 && ai2[2]<0) ||
             (ai2[1]>0 && ai2[2]>0 && ai2[0]<0) ||
             (ai2[2]>0 && ai2[0]>0 && ai2[1]<0)) {
        if(a00==0)
            ObjType = 'K';     // Cone
        else
        {
            // Hyperboloid
            ObjType = 'H';
        }
    }
}

float Quadric::intersect(Ray r)
{
    float ret_val=-1.0;
    unsigned int i;
    Point Pe = r.origin;
    Vector3D n_pe = r.direction;

    float A,B,C,discri,t1,t2;
    A=B=C=0.0;
    // compute t^2 co-efficient
    for(i=0; i<3; i++)
    {
        A = A + ( (ai2[i] * pow(dot(n[i],n_pe),2)) / pow(s[i],2) );
    }
    // compute t co-efficient
    Vector3D temp = Pe-*Pc;
    for(i=0; i<3; i++)
    {
        float tmp = dot(n[i],n_pe);
        B = B + ( (2 * ai2[i] * tmp * dot(n[i], temp)) / pow(s[i],2) ) + ( (ai1[i]*tmp) / s[i] );
    }
    // compute constant
    for(i=0; i<3; i++)
    {
        float tmp = dot(n[i], temp);
        C = C + ( (ai2[i] * pow(tmp,2)) / pow(s[i],2) ) + ( (ai1[i]*tmp) / s[i] );
    }

    C = C + this->a00;
    discri  = B*B - 4*A*C;

    if(discri > 0)
    {
        if(A!=0)
        {
            t1 = ( (-1*B) - sqrt(discri) ) / (2*A);
            t2 = ( (-1*B) + sqrt(discri) ) / (2*A);
            if(t1>0 && t2>0)
                ret_val = min(t1,t2);
            else if(t1>0)
                ret_val = t1;
            else if(t2>0)
                ret_val = t2;
        }else{
            t1 = -1*(C/B);
            if(t1>=0)
                ret_val = t1;
        }
    }
    return ret_val;
}

bool Quadric::isVisible(const Point& p)
{
    float ret_val=0;
    Vector3D temp = p-*Pc;
    for(unsigned int i=0; i <3; i++)
    {
        float xi = dot(n[i], temp)/s[i];
        ret_val = ret_val + (ai2[i]*xi*xi) + (ai1[i]*xi);
    }
    return (( (ret_val+a00) <= 0 ) ? false : true );
}

void Quadric::extract_Color_Normal(const Point& Ph, Vector3D &norm, PixColor &mycolor)
{
    Vector3D v(0,0,0);
    Vector3D temp = Ph - *(this->Pc);
    for(unsigned int i=0; i<3; i++)
    {
        Vector3D vtmp = (n[i]*((2 * ai2[i] * dot(n[i],temp))/(s[i]*s[i])) + (n[i]*(ai1[i]/s[i])));
        v = v + vtmp;
    }
    v.normalize();
    norm = v;

    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
        {
            mycolor = matter->computeSolidTexColor(Ph);
        }
        else
        {
            float u, v;
            this->computeUV(Ph, u, v);
            mycolor = matter->computeTexColor(u,v);
        }
    }
    else
        mycolor.set(*matter->colour);
}

Vector3D Quadric::getNormal(const Point& Ph)
{
    Vector3D v(0,0,0);
    Vector3D temp = Ph - *(this->Pc);
    for(unsigned int i=0; i<3; i++)
    {
        Vector3D vtmp = (n[i]*((2 * ai2[i] * dot(n[i],temp))/(s[i]*s[i])))+(n[i]*((ai1[i]/s[i])));
        v = v + vtmp;
    }
    v.normalize();
    return v;
}

PixColor Quadric::getColor(const Point& Ph)
{
    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
        {
            return matter->computeSolidTexColor(Ph);
        }
        else
        {
            float u, v;
            this->computeUV(Ph, u, v);
            return matter->computeTexColor(u,v);
        }
    }
    else
        return *matter->colour;
}

void Quadric::computeUV(const Point& Ph, float&u, float& v)
{
    float X,Y,Z;
    Vector3D temp = Ph-*Pc;
    X = dot(n[0], temp)/s[0];
    Y = dot(n[1], temp)/s[1];
    Z = dot(n[2], temp)/s[2];
    /*
     * P -Plane; E -Ellipsoid; Y - Paraboloid;
     * C - Cylinder; K - Cone; H - Hyperboloid.
     */
    switch(ObjType)
    {
    case 'P':
        u = (ai1[0] == -1) ? Y - (int)Y : ( (ai1[1]== -1) ? Z-(int)Z : X-(int)X);
        if(u<0) u=1+u;
        v = (ai1[0] == -1) ? Z - (int)Z : ( (ai1[1]== -1) ? X-(int)X : Y-(int)Y);
        if(v<0) v=1+v;
        break;
    case 'E':
        u = acos( Y/sqrt( 1-Z*Z ) ) / (2 * PI);
        if(X<0) u=1-u;
        v = acos(Z)/PI;
        break;
    case 'Y':
        break;
    case 'C':
        if(ai2[2]==0)
        {
            u = acos(Y)/(2*PI);
            if(X<0)
                u = 1-u;
            v = Z - (int)Z;
        }
        else if(ai2[1]==0)
        {
            u = acos(Z)/(2*PI);
            if(Y<0)
                u = 1-u;
            v = Y - (int)Y;
        }
        else
        {
            u = acos(Z)/(2*PI);
            if(X<0)
                u = 1-u;
            v = X - (int)X;
        }
        if(v<0) v=1+v;
        break;
    case 'K':
        if(ai2[1]==-1)
        {
            u = acos(X/Y)/(2*PI);
            if(Z>=0) u=1-u;
            v = Y - (int)Y;
        }
        else if(ai2[0]==-1)
        {
            u = acos(Y/X)/(2*PI);
            if(Z>=0) u=1-u;
            v = X - (int)X;
        }
        else
        {
            u = acos(X/Z)/(2*PI);
            if(Y>=0) u=1-u;
            v = Z - (int)Z;
        }
        if(u<0) u=1+u;
        if(v<0) v=1+v;

        break;
    case 'H':
        if(a00==1)
        {
            float psi;

            if(ai2[1]==-1)
            {
                if(Y<0) Y = Y*-1.0;
                int err=-1;
                psi = acosh(Y,err);
                if(err==0)
                    u = acos(Z/sinh(psi))/(2*PI);
                else
                    std::cout<<"Domain error for acosh"<<std::endl;
                if((X/sinh(psi))<0) u=1-u;
            }
            else if(ai2[0]==-1)
            {
                if(X<0) X = X*-1.0;
                int err=-1;
                psi = acosh(X,err);
                if(err==0)
                    u = acos(Z/sinh(psi))/(2*PI);
                else
                    std::cout<<"Domain error for acosh"<<std::endl;
                if((Y/sinh(psi))<0) u=1-u;
            }
            else
            {
                if(Z<0) Z = Z*-1.0;
                int err=-1;
                psi = acosh(Z,err);
                if(err==0)
                    u = acos(Y/sinh(psi))/(2*PI);
                else
                    std::cout<<"Domain error for acosh"<<std::endl;
                if((X/sinh(psi))<0) u=1-u;
            }
            v = psi - (int)psi;
        }else{
            float psi;

            if(ai2[1]==-1)
            {
                psi = asinh(Y);
                u = acos(Z/cosh(psi))/(2*PI);
                if(X<0) u=1-u;
            }
            else if(ai2[0]==-1)
            {
                psi = asinh(X);
                u = acos(Z/cosh(psi))/(2*PI);
                if(Y<0) u=1-u;
            }
            else
            {
                psi = asinh(Z);
                u = acos(Y/cosh(psi))/(2*PI);
                if(X<0) u=1-u;
            }
            v = psi - (int)psi;
        }
        if(u<0) u=1+u;
        if(v<0) v=1+v;
        break;
    default:
        cout<<"Unrecoginzed object encountered; Stop execution"<<endl;
        break;
    }
}

void Quadric::translate(float time)
{
	Point tmp = (*Pc) + (*Move)*(speed*time);
	this->Pc->set(tmp.x,tmp.y,tmp.z);
}

void Quadric::moveAxis(float theta_abtY)
{
    ;//Do nothing
    float* array = new float[9];
    array[0] = cos(theta_abtY); array[1] = 0; array[2] = -1.0*sin(theta_abtY);
    array[3] = 0; array[4] = 1; array[5] = 0;
    array[6] = sin(theta_abtY); array[7] = 0; array[8] = cos(theta_abtY);    
    for(unsigned int k=0; k< 9; k++)
        if( abs(array[k]) < 1.0e-6 ) array[k]=0.0;
    Mat r1(array, 3, 3);
    Mat r2(array, 3, 3);
    Mat r3(array, 3, 3);
    n[0] = rotateVec(r1, n[0]);
    n[1] = rotateVec(r2, n[1]);
    n[2] = rotateVec(r3, n[2]);
    delete array;
}


/* ------------- Methods of TriangularMesh class ------------- */
TriangularMesh::TriangularMesh(string objfile, char t, unsigned int id, PixColor& c, TYPE_INTEGER texEnable, TYPE_INTEGER val, string filename, float neta, Vector3D& dir, float spd)
    :Primitive(t, id, c, texEnable, val, filename, neta, dir, spd)
{
    this->loadMesh(objfile.c_str());
}

void TriangularMesh::loadMesh(const char* filename)
{
    char buff[3], vtype; /* vtype helps in holding / character temporarily */
    float x,y,z;
    unsigned int v1,t1,n1;
    unsigned int v2,t2,n2;
    unsigned int v3,t3,n3;
    Point *ptemp;
    Vector3D *vtmp;
    couple *t2temp;
    triplet *t3temp[3];

    string inputLine;
    stringstream stream(stringstream::in | stringstream::out);
    this->minID = 0;

    ifstream ObjHandle(filename,ios::in);
    while(getline(ObjHandle,inputLine)) {
        stream.clear();                         /* clear internal error flags */
        stream.str("");                         /* to remove the old string. */
        buff[0]='\0';buff[1]='\0';buff[2]='\0'; /* clear old buff values */
        stream<<inputLine;
        stream>>buff;

        if(buff[0] == 'v')
        {
            if(buff[1] == 't')
            {
                stream>>x>>y;
                t2temp = new couple(x,y);
                this->vt.push_back(t2temp);
            }
            else if(buff[1] == 'n')
            {
                stream>>x>>y>>z;
                vtmp = new Vector3D(x,y,z);
                this->vn.push_back(vtmp);
            }
            else
            {
                stream>>x>>y>>z;
                ptemp = new Point(x,y,z);
                this->v.push_back(ptemp);
            }
        }
        else if(buff[0]=='f')
        {
            stream>>v1>>vtype>>t1>>vtype>>n1; //vtype holds the value of            
            stream>>v2>>vtype>>t2>>vtype>>n2;            
            stream>>v3>>vtype>>t3>>vtype>>n3;            
            t3temp[0] = new triplet(v1,v2,v3); this->triangles.push_back(t3temp[0]);
            t3temp[1] = new triplet(t1,t2,t3); this->vtIndices.push_back(t3temp[1]);
            t3temp[2] = new triplet(n1,n2,n3); this->vnIndices.push_back(t3temp[2]);
        }
        else
        {
            ;/*Do Nothing*/
        }
    }

    for(unsigned int i=0;i<triangles.size(); i++)
    {
        Vector3D vtmp  = *v[triangles[i]->a1-1] - *v[triangles[i]->a0-1];
        Vector3D vtmp2 = *v[triangles[i]->a2-1] - *v[triangles[i]->a0-1];
        Vector3D *fnorm = new Vector3D(cross( vtmp, vtmp2 ));
        faceNormals.push_back(fnorm);
    }
}

bool TriangularMesh::insideTriangle(const Point& Ph, unsigned int index)
{
    bool ret_val = false;
    float a,b,c;
    float mu,mv;
    float ut,vt,wt;

    Vector3D A = *faceNormals[index];
    Vector3D A1 = cross( (*v[triangles[index]->a1-1])-Ph, (*v[triangles[index]->a2-1])-Ph );
    Vector3D A2 = cross( (*v[triangles[index]->a2-1])-Ph, (*v[triangles[index]->a0-1])-Ph );

    // u computation
    a = (A1.vec[0] < 0) ? A1.vec[0]*-1.0 : A1.vec[0];
    b = (A1.vec[1] < 0) ? A1.vec[1]*-1.0 : A1.vec[1];
    c = (A1.vec[2] < 0) ? A1.vec[2]*-1.0 : A1.vec[2];
    mu = max(a,max(b,c));
    if( a == mu)
        ut = A1.vec[0]/A.vec[0];
    else if(b == mu)
        ut = A1.vec[1]/A.vec[1];
    else
        ut = A1.vec[2]/A.vec[2];
    // check for a miss to return preempt uneccessary calculations
    if(ut<0 || ut>1)
        return ret_val;

    // v computation
    a = (A2.vec[0] < 0) ? A2.vec[0]*-1.0 : A2.vec[0];
    b = (A2.vec[1] < 0) ? A2.vec[1]*-1.0 : A2.vec[1];
    c = (A2.vec[2] < 0) ? A2.vec[2]*-1.0 : A2.vec[2];
    mv = max(a,max(b,c));
    if( a == mv)
        vt = A2.vec[0]/A.vec[0];
    else if(b == mv)
        vt = A2.vec[1]/A.vec[1];
    else
        vt = A2.vec[2]/A.vec[2];
    // check for a miss to return preempt uneccessary calculations
    if(vt<0 || vt>1)
        return ret_val;

    // w computation
    wt = 1 - ut -vt;

    if( 0 <= wt && wt <= 1 )
        ret_val = true;
    return ret_val;
}

Vector3D TriangularMesh::trilinearInterpolation(float u, float v, const Vector3D *vn1, const Vector3D *vn2, const Vector3D *vn3)
{
    Vector3D norm;
    float x = u*vn1->vec[0] + v*vn2->vec[0] + (1-u-v)*vn3->vec[0];
    float y = u*vn1->vec[1] + v*vn2->vec[1] + (1-u-v)*vn3->vec[1];
    float z = u*vn1->vec[2] + v*vn2->vec[2] + (1-u-v)*vn3->vec[2];
    norm.set(x,y,z);
    norm.normalize();
    return norm;
}

float TriangularMesh::intersect(Ray r)
{
    float ret_val=0.0;
    float t=-1.0;
    Point Ph;
    unsigned int triangleEncounter=0;
    minPh = new Point;

    for(unsigned int i=0; i<triangles.size(); i++)
    {
        float tmp = dot(*faceNormals[i],r.direction);
        if( tmp <= 0)
        {
            // Compute corresponding Ph
            t = dot( *faceNormals[i], (*v[triangles[i]->a0-1] - r.origin)) / tmp;
            Ph = r.origin + r.direction*t;

            if(this->insideTriangle( Ph, i))
            {
                if(t < ret_val || !ret_val)
                {
                    ret_val = t;
                    minID = i;
                    minPh->operator=(Ph);
                }
                triangleEncounter++;
            }
        }
        // Assuming Convex shapes, hence only two intersections
        if(triangleEncounter > 1)
            break;
    }
    return ret_val;
}

bool TriangularMesh::isVisible(const Point& p)
{
    return true;
}

void TriangularMesh::extract_Color_Normal(const Point& Ph, Vector3D &norm, PixColor &mycolor)
{
    float u,v;
    this->computeUV(Ph, u, v);
    norm = trilinearInterpolation(u,v,vn[vnIndices[minID]->a0-1],vn[vnIndices[minID]->a1-1],vn[vnIndices[minID]->a2-1]);
    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
            mycolor = matter->computeSolidTexColor(Ph);
        else
            mycolor = matter->computeTexColor(u,v);
    }
    else
        mycolor.set(*matter->colour);
}

Vector3D TriangularMesh::getNormal(const Point& Ph)
{
    float u,v;
    this->computeUV(Ph, u, v);
    return trilinearInterpolation(u,v,vn[vnIndices[minID]->a0-1],vn[vnIndices[minID]->a1-1],vn[vnIndices[minID]->a2-1]);
}

PixColor TriangularMesh::getColor(const Point& Ph)
{
    if( isTextureEnabled() )
    {
        if(matter->textureType == SOLID)
            return matter->computeSolidTexColor(Ph);
        else
        {
            float u, v;
            this->computeUV(Ph, u, v);
            return matter->computeTexColor(u,v);
        }
    }
    else
        return *matter->colour;
}

void TriangularMesh::computeUV(const Point& Ph, float &u_tex, float &v_tex)
{
    float a,b,c;
    float mu,mv;

    Vector3D A = *faceNormals[minID];
    Vector3D A1 = cross( (*v[triangles[minID]->a1-1])-(*minPh), (*v[triangles[minID]->a2-1])-(*minPh) );
    Vector3D A2 = cross( (*v[triangles[minID]->a2-1])-(*minPh), (*v[triangles[minID]->a0-1])-(*minPh) );

    // u computation
    a = (A1.vec[0] < 0) ? A1.vec[0]*-1.0 : A1.vec[0];
    b = (A1.vec[1] < 0) ? A1.vec[1]*-1.0 : A1.vec[1];
    c = (A1.vec[2] < 0) ? A1.vec[2]*-1.0 : A1.vec[2];
    mu = max(a,max(b,c));
    if( a == mu)
        u_tex = A1.vec[0]/A.vec[0];
    else if(b == mu)
        u_tex = A1.vec[1]/A.vec[1];
    else
        u_tex = A1.vec[2]/A.vec[2];
    // v computation
    a = (A2.vec[0] < 0) ? A2.vec[0]*-1.0 : A2.vec[0];
    b = (A2.vec[1] < 0) ? A2.vec[1]*-1.0 : A2.vec[1];
    c = (A2.vec[2] < 0) ? A2.vec[2]*-1.0 : A2.vec[2];
    mv = max(a,max(b,c));
    if( a == mv)
        v_tex = A2.vec[0]/A.vec[0];
    else if(b == mv)
        v_tex = A2.vec[1]/A.vec[1];
    else
        v_tex = A2.vec[2]/A.vec[2];
}

void TriangularMesh::translate(float time)
{
    /* have to move each and every vertex at 'velocity' speed in 'Move' direction */;
}

void TriangularMesh::moveAxis(float theta_abtY)
{
    /* have to move each and every vertex at 'velocity' speed in 'Move' direction */;
}
