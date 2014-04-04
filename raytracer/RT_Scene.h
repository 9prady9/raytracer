#ifndef RT_SCENE_H
#define RT_SCENE_H

#include <vector>
#include <iostream>
#include <QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QThread>

/* Forward declarations - BEGIN */
class Ray;
class Point;
class couple;
class PixColor;
class Vector3D;
class Material;
class Primitive;
class CubeMap;
class SphereMap;

class QTreeWidget;
class QTreeWidgetItem;
class QProgressBar;
/* Forward declarations - END */

typedef int TYPE_INTEGER;

class AreaLight
{
public:
    // Varibles
    Point* Pc;
    Point** mn_Lights;
    Vector3D* v2;
    Vector3D* up;
    unsigned int m,n;
    PixColor* AC;
    // Constructors
    AreaLight(Point& Pc, Vector3D& v2, Vector3D& up, unsigned int m, unsigned int n, PixColor& AC);
    ~AreaLight();
};

enum Production { IGNORE, RAYTRACE, DOF, MB, AMBOCC, ANIMATE, COMBINE };

class PinHoleCam : public QThread
{
    Q_OBJECT
signals:
    void sendPixelIndex(int index);
public:
    Production renderingType;
    Production ifAnimation;
    bool EnvironmentMapEnabled;
    // Final pixels
    PixColor **pixels;
    PixColor **DOFpixels;
    PixColor **MBpixels;
    PixColor **AMB_OCCpixels;
    PixColor **COMBINE_pixels;
    // Environment
    Point* Pe;
    Vector3D* V_view;    
    Vector3D* V_up;
    // View plane
    float Sx,Sy;
    unsigned int Xmax,Ymax;
    unsigned int m,n;
    float d;
    Point* SceneCenter;
    Point* SceneOrigin;
    Vector3D* n0;
    Vector3D* n1;

    // Lights
    std::vector<Point*> Plights;
    std::vector<PixColor*> Clights;
    std::vector<AreaLight*> Alights;
    // Ambient occlusion Geodesic Dome    
    std::string RadialFileName;
    std::vector<Vector3D*> Radials;
    float HitEffectiveDistance;

    CubeMap* EnvironmentMap;
    //SphereMap* EnvironmentMap;

    PixColor* specL;
    float phongExp;
    PixColor* AmbientL;
    float amb_intensity;    
    PixColor* Cc;
    float alpha;  /* Diffuse color factor used in cool color computation */
    PixColor* Cw;
    float beta;   /* Diffuse color factor used in warm color computation */

    int MaxReflectionDepth;
    float Reflectance;
    int MaxRefractionDepth;
    float Refraction;

    float FocalLength;
    float ApertureDiameter;
    int NumRays;

    float max_channel; /* RGB Channel related; Computed on-fly */
    float min_channel; /* RGB Channel related; Computed on-fly */

    /* Object details will follow from here */
    unsigned int Obj_NO;
    std::vector<Primitive*> OBJ_LIST;


    /* Constructors,Destructors & Methods */
    PinHoleCam(QObject *parent, Production rendering, bool EMap, QStringList &input, QTreeWidget *stree, QTreeWidget *otree, Production ifAnimation=IGNORE);
    couple getOutImgDimensions();
    void run(void);
    QString getOutputName();
    ~PinHoleCam();
private:
    void readRadials(const char* filename);
    void resetChannel();
    void moveCamera(float theta);
/* Actual Ray tracing Methods */
    PixColor rayCast(Ray r, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId);
    PixColor rayTraceAmbOcc(Ray r, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId);
    bool isReflectionObstructed(const Point& Ph, const Point& Pl, const Vector3D& n_lh, unsigned int id);
    bool isRadialObstructed(Ray ry, unsigned int id, PixColor& HitColor);
    PixColor computeEffectofLights(const Point& Ph, Ray r, unsigned int min_id, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId);
    PixColor findAmbientOcclusion(const Point& Ph, Ray r, unsigned int min_id, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId);
    void render();
    void renderWithDOF();
    void renderWithMB();
    void renderAmbOcc();
/* Methods to output the rendered Ray traced scene */
    void combine(PixColor **op1, PixColor **op2);
    void writeImage2File(PixColor **result, QString OutputName);
    void animate(Production ifAnimation);
/* Methods to parse input xml files */
    void parseSceneElement(const QDomElement& elt);
    void parseObjectsElement(const QDomElement& elt);
    void parseCoupleElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseTripletElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseTargetImageElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseCameraElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parsePointLightsElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parsePLightElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseAreaLightsElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseALightElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseSpecularLightElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseAmbientLightElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseGoochandGoochElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseCoolElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseWarmElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseEnvMapElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseSphereMapElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseCubeMapElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parsePlaneElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseSphereElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseQuadricElement(const QDomElement& elt, QTreeWidgetItem *parent);
    void parseTriangularMeshElement(const QDomElement& elt, QTreeWidgetItem *parent);

    QTreeWidget *sceneTreeWidget;
    QTreeWidget *objectsTreeWidget;
    QDomDocument sceneDoc;
    QDomDocument objectsDoc;
    QString outputDirectory;
    QString output;
    unsigned int currPixel;
};

#endif // RT_SCENE_H
