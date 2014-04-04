#include "RT_Scene.h"
#include "RT_math.h"
#include "RT_properties.h"
#include "RT_Objects.h"

#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <QtGui>
#include <QtXml>
#include <QTreeWidget>
#include <QTreeWidgetItem>

using namespace std;


/* ------------- AreaLight class ------------- */
AreaLight::AreaLight(Point& Pc, Vector3D& v2, Vector3D& up, unsigned int m, unsigned int n, PixColor& AC)
{
    unsigned int i,j;
    float len,brth;

    this->Pc = new Point(Pc);
    this->v2 = new Vector3D(v2);
    this->up = new Vector3D(up);
    this->m = m;
    this->n = n;
    this->AC = new PixColor(AC);

    this->mn_Lights = new Point*[m];
    for(i=0; i<m; i++)
        mn_Lights[i] = new Point[n];
    this->v2->normalize();
    this->up->normalize();
    Vector3D n0 = *this->v2;
    Vector3D n1 = *this->up;
    srand(time(NULL));

    len = this->v2->mag;
    brth = this->up->mag;
    Point Origin = (*this->Pc) - ((n0*len/2.0)+(n1*brth/2.0));

    for(i=0;i<m;i++)
    {
        for(j=0; j<n;j++)
            mn_Lights[i][j] = Origin + ((n0*(i*len/n+d_rand(0,0.2)))+(n1*(j*brth/m+d_rand(0,0.2))));
    }
}

AreaLight::~AreaLight()
{
    for(unsigned int c=0; c<m; c++)
        delete[] mn_Lights[c];
    delete[] mn_Lights;
    delete Pc;
    delete v2;
    delete up;
    delete AC;
}


#pragma region XML DomReader helpers

void PinHoleCam::parseSceneElement(const QDomElement &elt)
{
    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if( element.tagName() == "targetImage" ) {
            parseTargetImageElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "viewPlane" ) {
            parseTripletElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "camera" ) {
            parseCameraElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "pointLights" ) {
            parsePointLightsElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "areaLights" ) {
            parseAreaLightsElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "specularLight" ) {
            parseSpecularLightElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "ambientLight" ) {
            parseAmbientLightElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "goochGooch" ) {
            parseGoochandGoochElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "reflection" ) {
            parseCoupleElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "refraction" ) {
            parseCoupleElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "DOF" ) {
            parseTripletElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "ambientOcclusion" ) {
            parseCoupleElement(element,sceneTreeWidget->invisibleRootItem());
        }else if( element.tagName() == "envMap" ) {
			if( this->EnvironmentMapEnabled )
				parseEnvMapElement(element,sceneTreeWidget->invisibleRootItem());
        }else {
            std::cerr << "Scene description file: Unkown tag <"<<
				element.tagName().toStdString()<<"> encountered under tag <"<<
				elt.tagName().toStdString() <<">"<<std::endl;
        }
    }
}
void PinHoleCam::parseCoupleElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());

    QString temp1,temp2;

    if(elt.tagName()=="resolution") {
        temp1 = elt.attribute("width");
        temp2 = elt.attribute("height");
        this->Xmax = temp1.toUInt();
        this->Ymax = temp2.toUInt();
        temp1 = temp1 + "," + temp2;
        item->setText(1,temp1);
    }else if(elt.tagName()=="antiAlias") {
        temp1 = elt.attribute("m");
        temp2 = elt.attribute("n");
        this->m = temp1.toUInt();
        this->n = temp2.toUInt();
        temp1 = temp1 + "," + temp2;
        item->setText(1,temp1);
    }else if(elt.tagName()=="reflection") {
        temp1 = elt.attribute("depth");
        temp2 = elt.attribute("intensity");
        this->MaxReflectionDepth = temp1.toInt();
        this->Reflectance = temp2.toFloat();
        temp1 = temp1 + "," + temp2;
        item->setText(1,temp1);
    }else if(elt.tagName()=="refraction") {
        temp1 = elt.attribute("depth");
        temp2 = elt.attribute("intensity");
        this->MaxRefractionDepth = temp1.toInt();
        this->Refraction = temp2.toFloat();
        temp1 = temp1 + "," + temp2;
        item->setText(1,temp1);
    }else if(elt.tagName()=="ambientOcclusion") {
        temp1 = elt.attribute("HitEffectiveDistance");
        temp2 = elt.attribute("radialSource");
        this->HitEffectiveDistance = temp1.toFloat();
        this->RadialFileName = temp2.toStdString();
        temp1 = temp1 + "," + temp2;
        item->setText(1,temp1);
    }else {
        std::cerr << "Scene description file: Unkown tag encountered <"<<
                     elt.tagName().toStdString() <<">"<<std::endl;
    }
}

void PinHoleCam::parseTripletElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());

    QString temp1,temp2,temp3;

    if(elt.tagName()=="viewPlane") {
        temp1 = elt.attribute("sx");
        temp2 = elt.attribute("sy");
        temp3 = elt.attribute("eyeToViewPlane");
        this->Sx = temp1.toFloat();
        this->Sy = temp2.toFloat();
        this->d = temp3.toFloat();
        temp1 = temp1 + "," + temp2 + "," + temp3;
        item->setText(1,temp1);
    }else if(elt.tagName()=="eye") {
        temp1 = elt.attribute("x");
        temp2 = elt.attribute("y");
        temp3 = elt.attribute("z");
        this->Pe = new Point(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
        temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
        item->setText(1,temp1);
    }else if(elt.tagName()=="lookAt") {
        temp1 = elt.attribute("x");
        temp2 = elt.attribute("y");
        temp3 = elt.attribute("z");
        this->V_view = new Vector3D(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
        temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
        item->setText(1,temp1);
    }else if(elt.tagName()=="up") {
        temp1 = elt.attribute("x");
        temp2 = elt.attribute("y");
        temp3 = elt.attribute("z");
        this->V_up = new Vector3D(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
        temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
        item->setText(1,temp1);
    }else if(elt.tagName()=="DOF") {
        temp1 = elt.attribute("focalLength");
        temp2 = elt.attribute("apertureDiameter");
        temp3 = elt.attribute("numOfRays");
        this->FocalLength = temp1.toFloat();
        this->ApertureDiameter = temp2.toFloat();
        this->NumRays = temp3.toInt();
        temp1 = temp1 + "," + temp2 + "," + temp3;
        item->setText(1,temp1);
    }else {
        std::cerr << "Scene description file: Unkown tag encountered <"<<
                     elt.tagName().toStdString() <<">"<<std::endl;
    }
}

void PinHoleCam::parseTargetImageElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());
    item->setText(1,QString("Output Directory = ")+elt.attribute("outputDirectory"));

    for(QDomElement element=elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName() == "resolution") {
            parseCoupleElement(element,item);
        }else if(element.tagName() == "antiAlias") {
            parseCoupleElement(element,item);
        }else {
            std::cerr << "Scene description file: Unkown tag encountered under tag <"<<
                         elt.tagName().toStdString() <<">"<<std::endl;
        }
    }
    this->outputDirectory = elt.attribute("outputDirectory");
}

void PinHoleCam::parseCameraElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());
    for(QDomElement element=elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
        parseTripletElement(element,item);
}

void PinHoleCam::parsePointLightsElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());
    item->setText(1,elt.attribute("count"));
    for(QDomElement element=elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
        parsePLightElement(element,item);
}

void PinHoleCam::parsePLightElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *pointLightItem = new QTreeWidgetItem(parent);
    pointLightItem->setText(0,elt.tagName()+elt.attribute(" sno"));

    QTreeWidgetItem *positionItem = new QTreeWidgetItem(pointLightItem);
    QTreeWidgetItem *colorItem = new QTreeWidgetItem(pointLightItem);

    QString temp1,temp2,temp3;
    Point *ptemp;
    PixColor *ctmp;

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="position") {
            positionItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            ptemp = new Point(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            positionItem->setText(1,temp1);
        }else if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            ctmp = new PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0);
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }
    }
    this->Plights.push_back(ptemp);
    this->Clights.push_back(ctmp);
}

void PinHoleCam::parseAreaLightsElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());
    item->setText(1,elt.attribute("count"));
    for(QDomElement element=elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
        parseALightElement(element,item);
}

void PinHoleCam::parseALightElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *AreaLightItem = new QTreeWidgetItem(parent);
    AreaLightItem->setText(0,elt.tagName()+elt.attribute(" sno"));

    QTreeWidgetItem *centerItem = new QTreeWidgetItem(AreaLightItem);
    QTreeWidgetItem *horizontalItem = new QTreeWidgetItem(AreaLightItem);
    QTreeWidgetItem *verticalItem = new QTreeWidgetItem(AreaLightItem);
    QTreeWidgetItem *samplesItem = new QTreeWidgetItem(AreaLightItem);
    QTreeWidgetItem *colorItem = new QTreeWidgetItem(AreaLightItem);

    QString temp1,temp2,temp3;
    Point ptemp;
    Vector3D vtmp1;
    Vector3D vtmp2;
    unsigned int m,n;
    PixColor ctmp;

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="center") {
            centerItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            ptemp.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            centerItem->setText(1,temp1);
        }else if(element.tagName()=="horizontal") {
            horizontalItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            vtmp1.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            horizontalItem->setText(1,temp1);
        }else if(element.tagName()=="vertical") {
            verticalItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            vtmp2.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            verticalItem->setText(1,temp1);
        }else if(element.tagName()=="samples") {
            samplesItem->setText(0,element.tagName());
            temp1 = element.attribute("hSamples");
            temp2 = element.attribute("vSamples");
            m = temp1.toUInt();
            n = temp2.toUInt();
            temp1 = temp1 + "," + temp2;
            samplesItem->setText(1,temp1);
        }else if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            ctmp.set(PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0));
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }
    }

    AreaLight *AL = new AreaLight(ptemp,vtmp1,vtmp2,m,n,ctmp);
    this->Alights.push_back(AL);
}

void PinHoleCam::parseSpecularLightElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *specLItem = new QTreeWidgetItem(parent);
    specLItem->setText(0,elt.tagName());
    specLItem->setText(1,QString("Phong Exponent = ")+elt.attribute("phongExponent"));

    QTreeWidgetItem *colorItem = new QTreeWidgetItem(specLItem);

    QString temp1,temp2,temp3;

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            this->specL = new PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0);
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }
    }
    this->phongExp = elt.attribute("phongExponent").toFloat();
}

void PinHoleCam::parseAmbientLightElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *ambLItem = new QTreeWidgetItem(parent);
    ambLItem->setText(0,elt.tagName());
    ambLItem->setText(1,QString("Intensity = ")+elt.attribute("intensity"));

    QTreeWidgetItem *colorItem = new QTreeWidgetItem(ambLItem);

    QString temp1,temp2,temp3;

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            this->AmbientL = new PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0);
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }
    }
    this->amb_intensity = elt.attribute("intensity").toFloat();
}

void PinHoleCam::parseGoochandGoochElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="cool") {
            parseCoolElement(element,item);
        }else if(element.tagName()=="warm") {
            parseWarmElement(element,item);
        }
    }
}

void PinHoleCam::parseCoolElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *coolItem = new QTreeWidgetItem(parent);
    coolItem->setText(0,elt.tagName());
    coolItem->setText(1,QString("Factor = ")+elt.attribute("factor"));

    QTreeWidgetItem *colorItem = new QTreeWidgetItem(coolItem);

    QString temp1,temp2,temp3;

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            this->Cc = new PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0);
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }
    }
    this->alpha = elt.attribute("factor").toFloat();
}

void PinHoleCam::parseWarmElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *warmItem = new QTreeWidgetItem(parent);
    warmItem->setText(0,elt.tagName());
    warmItem->setText(1,QString("Factor = ")+elt.attribute("factor"));

    QTreeWidgetItem *colorItem = new QTreeWidgetItem(warmItem);

    QString temp1,temp2,temp3;

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            this->Cw = new PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0);
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }
    }
    this->beta = elt.attribute("factor").toFloat();
}

void PinHoleCam::parseEnvMapElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,elt.tagName());

    if(elt.attribute("whichOne")=="cubeMap")
    {
        parseCubeMapElement(elt.firstChildElement(),item);
    }else if(elt.attribute("whichOne")=="sphereMap")
    {
        parseSphereMapElement(elt.firstChildElement(),item);
    }
}

void PinHoleCam::parseSphereMapElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    ;
}

void PinHoleCam::parseCubeMapElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *cmapItem = new QTreeWidgetItem(parent);
    cmapItem->setText(0,elt.tagName());

    QTreeWidgetItem *mapItems[6];
    for(unsigned int i=0;i<6;i++)
        mapItems[i] = new QTreeWidgetItem(cmapItem);
    std::string file[6];

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="posx") {
            mapItems[0]->setText(0,element.tagName());
            file[0] = element.text().toStdString();
            mapItems[0]->setText(2,element.text());
        }else if(element.tagName()=="negx") {
            mapItems[1]->setText(0,element.tagName());
            file[1] = element.text().toStdString();
            mapItems[1]->setText(2,element.text());
        }else if(element.tagName()=="posy") {
            mapItems[2]->setText(0,element.tagName());
            file[2] = element.text().toStdString();
            mapItems[2]->setText(2,element.text());
        }else if(element.tagName()=="negy") {
            mapItems[3]->setText(0,element.tagName());
            file[3] = element.text().toStdString();
            mapItems[3]->setText(2,element.text());
        }else if(element.tagName()=="posz") {
            mapItems[4]->setText(0,element.tagName());
            file[4] = element.text().toStdString();
            mapItems[4]->setText(2,element.text());
        }else if(element.tagName()=="negz") {
            mapItems[5]->setText(0,element.tagName());
            file[5] = element.text().toStdString();
            mapItems[5]->setText(2,element.text());
        }
    }
    this->EnvironmentMap = new CubeMap(file);
}

void PinHoleCam::parsePlaneElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *planeItem = new QTreeWidgetItem(parent);
    planeItem->setText(0,elt.attribute("type"));
    planeItem->setText(1,QString("Object %1").arg(this->Obj_NO+1));
    planeItem->setExpanded(true);

    QTreeWidgetItem *normalItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *pointItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *haxisItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *vaxisItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *scaleItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *colorItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *texFileItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *motionItem = new QTreeWidgetItem(planeItem);
    QTreeWidgetItem *directionItem = new QTreeWidgetItem(motionItem);

    char type = 'P';
    TYPE_INTEGER tType,mType;
    float nta;
    Vector3D normal; Point ptmp;
    Vector3D vtmp; Vector3D axes[2];
    float s[3];
    PixColor ctmp;
    std::string texFile;
    float spd; Vector3D direc;
    QString temp1,temp2,temp3;

    QString tex = elt.attribute("TextureType");
    QString mat = elt.attribute("MaterialType");
    tType = ( tex=="PROCEDURE" ? PROCEDURE : (tex=="IMAGE" ? IMAGE : (tex=="SOLID" ? SOLID : NOTEXTURE)) );
    mType = ( mat == "reflective" ? reflective : ( mat == "refractive" ? refractive : ( mat == "emissive" ? emissive : diffusive ) ) );
    nta = elt.attribute("neta").toFloat();

    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if(element.tagName()=="normal") {
            normalItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            normal.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            normalItem->setText(1,temp1);
        }else if(element.tagName()=="point") {
            pointItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            ptmp.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            pointItem->setText(1,temp1);
        }else if(element.tagName()=="haxis") {
            haxisItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            axes[0].set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            haxisItem->setText(1,temp1);
        }else if(element.tagName()=="vaxis") {
            vaxisItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");            
            axes[1].set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            vaxisItem->setText(1,temp1);
        }else if(element.tagName()=="scale") {
            scaleItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            s[0] = temp1.toFloat(); s[1] = temp2.toFloat(); s[2] = temp3.toFloat();
            temp1 = temp1 + "," + temp2 + "," + temp3;
            scaleItem->setText(1,temp1);
        }else if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            ctmp.set(PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0));
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }else if(element.tagName()=="texFile") {
            texFileItem->setText(0,element.tagName());
            texFile = element.text().toStdString();
            texFileItem->setText(2,element.text());
        }else if(element.tagName()=="motion") {
            motionItem->setText(0,element.tagName());
            motionItem->setText(1,QString("Speed = ")+element.attribute("speed"));

            QDomElement sibling = element.firstChildElement();
            if(sibling.tagName()=="direction") {
                directionItem->setText(0,sibling.tagName());
                temp1 = sibling.attribute("x");
                temp2 = sibling.attribute("y");
                temp3 = sibling.attribute("z");
                direc.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
                temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
                directionItem->setText(1,temp1);
            } else {
                std::cerr << "Scene description file: Unkown tag encountered under tag <"<<
                             elt.tagName().toStdString() <<">"<<std::endl;
            }

            spd = element.attribute("speed").toFloat();
        }
    }
    Primitive *pl = new Plane(normal,ptmp,axes,s,type,this->Obj_NO,ctmp,tType,mType,texFile,nta,direc,spd);
    this->OBJ_LIST.push_back(pl);
    this->Obj_NO++;
}

void PinHoleCam::parseSphereElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *sphereItem = new QTreeWidgetItem(parent);
    sphereItem->setText(0,elt.attribute("type"));
    sphereItem->setText(1,QString("Object %1").arg(this->Obj_NO+1));
    sphereItem->setExpanded(true);

    QTreeWidgetItem *centerItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *radiusItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *haxisItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *vaxisItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *scaleItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *colorItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *texFileItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *motionItem = new QTreeWidgetItem(sphereItem);
    QTreeWidgetItem *directionItem = new QTreeWidgetItem(motionItem);

    char type = 'S';
    TYPE_INTEGER tType,mType;
    float nta;
    Point center; float radius;
    Vector3D vtmp; Vector3D axes[2];
    float s[3];
    PixColor ctmp;
    std::string texFile;
    float spd; Vector3D direc;
    QString temp1,temp2,temp3;

    QString tex = elt.attribute("TextureType");
    QString mat = elt.attribute("MaterialType");
    tType = ( tex=="PROCEDURE" ? PROCEDURE : (tex=="IMAGE" ? IMAGE : (tex=="SOLID" ? SOLID : NOTEXTURE)) );
    mType = ( mat == "reflective" ? reflective : ( mat == "refractive" ? refractive : ( mat == "emissive" ? emissive : diffusive ) ) );
    nta = elt.attribute("neta").toFloat();

    for(QDomElement element = elt.firstChildElement(); !element.isNull();  element = element.nextSiblingElement())
    {
        if(element.tagName()=="center") {
            centerItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            center.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            centerItem->setText(1,temp1);
        }else if(element.tagName()=="radius") {
            radiusItem->setText(0,element.tagName());
            radius = element.text().toFloat();
            radiusItem->setText(1,element.text());
        }else if(element.tagName()=="haxis") {
            haxisItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            axes[0].set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            haxisItem->setText(1,temp1);
        }else if(element.tagName()=="vaxis") {
            vaxisItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            axes[1].set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            vaxisItem->setText(1,temp1);
        }else if(element.tagName()=="scale") {
            scaleItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            s[0] = temp1.toFloat(); s[1] = temp2.toFloat(); s[2] = temp3.toFloat();
            temp1 = temp1 + "," + temp2 + "," + temp3;
            scaleItem->setText(1,temp1);
        }else if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            ctmp.set(PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0));
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }else if(element.tagName()=="texFile") {
            texFileItem->setText(0,element.tagName());
            texFile = element.text().toStdString();
            texFileItem->setText(2,element.text());
        }else if(element.tagName()=="motion") {
            motionItem->setText(0,element.tagName());
            motionItem->setText(1,QString("Speed = ")+element.attribute("speed"));

            QDomElement sibling = element.firstChildElement();
            if(sibling.tagName()=="direction") {
                directionItem->setText(0,sibling.tagName());
                temp1 = sibling.attribute("x");
                temp2 = sibling.attribute("y");
                temp3 = sibling.attribute("z");
                direc.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
                temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
                directionItem->setText(1,temp1);
            } else {
                std::cerr << "Scene description file: Unkown tag encountered under tag <"<<
                             elt.tagName().toStdString() <<">"<<std::endl;
            }

            spd = element.attribute("speed").toFloat();
        }
    }
    Primitive* sph = new Sphere(center,radius,axes,s,type,this->Obj_NO,ctmp,tType,mType,texFile,nta,direc,spd);
    this->OBJ_LIST.push_back(sph);
    this->Obj_NO++;
}

void PinHoleCam::parseQuadricElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *quadricItem = new QTreeWidgetItem(parent);
    quadricItem->setText(0,elt.attribute("type"));
    quadricItem->setText(1,QString("Object %1").arg(this->Obj_NO+1));
    quadricItem->setExpanded(true);

    QTreeWidgetItem *haxisItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *vaxisItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *pointItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *scaleItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *aValuesItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *colorItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *texFileItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *motionItem = new QTreeWidgetItem(quadricItem);
    QTreeWidgetItem *directionItem = new QTreeWidgetItem(motionItem);

    char type = 'Q';
    TYPE_INTEGER tType,mType;
    float nta;
    Vector3D haxis; Vector3D vaxis;
    Point ptmp;
    float s[3]; float a[7];
    PixColor ctmp;
    std::string texFile;
    float spd; Vector3D direc;
    QString temp1,temp2,temp3;

    QString tex = elt.attribute("TextureType");
    QString mat = elt.attribute("MaterialType");
    tType = ( tex=="PROCEDURE" ? PROCEDURE : (tex=="IMAGE" ? IMAGE : (tex=="SOLID" ? SOLID : NOTEXTURE)) );
    mType = ( mat == "reflective" ? reflective : ( mat == "refractive" ? refractive : ( mat == "emissive" ? emissive : diffusive ) ) );
    nta = elt.attribute("neta").toFloat();

    for(QDomElement element = elt.firstChildElement(); !element.isNull();  element = element.nextSiblingElement())
    {
        if(element.tagName()=="point") {
            pointItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            ptmp.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            pointItem->setText(1,temp1);
        }else if(element.tagName()=="aValues") {
            aValuesItem->setText(0,element.tagName());

            temp1 = element.attribute("a20");
            a[0] = temp1.toFloat();

            temp2 = element.attribute("a21");
            a[1] = temp2.toFloat();
            temp1 = temp1 + "," + temp2;

            temp2 = element.attribute("a22");
            a[2] = temp2.toFloat();
            temp1 = temp1 + "," + temp2;

            temp2 = element.attribute("a10");
            a[3] = temp2.toFloat();
            temp1 = temp1 + "," + temp2;

            temp2 = element.attribute("a11");
            a[4] = temp2.toFloat();
            temp1 = temp1 + "," + temp2;

            temp2 = element.attribute("a12");
            a[5] = temp2.toFloat();
            temp1 = temp1 + "," + temp2;

            temp2 = element.attribute("a00");
            a[6] = temp2.toFloat();
            temp1 = temp1 + "," + temp2;

            aValuesItem->setText(1,temp1);
        }else if(element.tagName()=="haxis") {
            haxisItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            haxis.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            haxisItem->setText(1,temp1);
        }else if(element.tagName()=="vaxis") {
            vaxisItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            vaxis.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            vaxisItem->setText(1,temp1);
        }else if(element.tagName()=="scale") {
            scaleItem->setText(0,element.tagName());
            temp1 = element.attribute("x");
            temp2 = element.attribute("y");
            temp3 = element.attribute("z");
            s[0] = temp1.toFloat(); s[1] = temp2.toFloat(); s[2] = temp3.toFloat();
            temp1 = temp1 + "," + temp2 + "," + temp3;
            scaleItem->setText(1,temp1);
        }else if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            ctmp.set(PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0));
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }else if(element.tagName()=="texFile") {
            texFileItem->setText(0,element.tagName());
            texFile = element.text().toStdString();
            texFileItem->setText(2,element.text());
        }else if(element.tagName()=="motion") {
            motionItem->setText(0,element.tagName());
            motionItem->setText(1,QString("Speed = ")+element.attribute("speed"));

            QDomElement sibling = element.firstChildElement();
            if(sibling.tagName()=="direction") {
                directionItem->setText(0,sibling.tagName());
                temp1 = sibling.attribute("x");
                temp2 = sibling.attribute("y");
                temp3 = sibling.attribute("z");
                direc.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
                temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
                directionItem->setText(1,temp1);
            } else {
                std::cerr << "Scene description file: Unkown tag encountered under tag <"<<
                             elt.tagName().toStdString() <<">"<<std::endl;
            }

            spd = element.attribute("speed").toFloat();
        }
    }
    Primitive* quadrc = new Quadric(haxis, vaxis, ptmp, s, a, type,this->Obj_NO,ctmp,tType,mType,texFile,nta,direc,spd);
    this->OBJ_LIST.push_back(quadrc);
    this->Obj_NO++;
}

void PinHoleCam::parseTriangularMeshElement(const QDomElement &elt, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *meshItem = new QTreeWidgetItem(parent);
    meshItem->setText(0,elt.attribute("type"));
    meshItem->setText(1,QString("Object %1").arg(this->Obj_NO+1));
    meshItem->setExpanded(true);

    QTreeWidgetItem *objFileItem = new QTreeWidgetItem(meshItem);
    QTreeWidgetItem *colorItem = new QTreeWidgetItem(meshItem);
    QTreeWidgetItem *texFileItem = new QTreeWidgetItem(meshItem);
    QTreeWidgetItem *motionItem = new QTreeWidgetItem(meshItem);
    QTreeWidgetItem *directionItem = new QTreeWidgetItem(motionItem);

    char type = 'M';
    TYPE_INTEGER tType,mType;
    float nta;
    std::string objFile;
    PixColor ctmp;
    std::string texFile;
    float spd; Vector3D direc;
    QString temp1,temp2,temp3;

    QString tex = elt.attribute("TextureType");
    QString mat = elt.attribute("MaterialType");
    tType = ( tex=="PROCEDURE" ? PROCEDURE : (tex=="IMAGE" ? IMAGE : (tex=="SOLID" ? SOLID : NOTEXTURE)) );
    mType = ( mat == "reflective" ? reflective : ( mat == "refractive" ? refractive : ( mat == "emissive" ? emissive : diffusive ) ) );
    nta = elt.attribute("neta").toFloat();

    for(QDomElement element = elt.firstChildElement(); !element.isNull();  element = element.nextSiblingElement())
    {
        if(element.tagName()=="objFile") {
            objFileItem->setText(0,element.tagName());
            objFile = element.text().toStdString();
            objFileItem->setText(2,element.text());
        }else if(element.tagName()=="color") {
            colorItem->setText(0,element.tagName());
            temp1 = element.attribute("red");
            temp2 = element.attribute("green");
            temp3 = element.attribute("blue");
            ctmp.set(PixColor(temp1.toFloat(),temp2.toFloat(),temp3.toFloat(),0.0));
            temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
            colorItem->setText(1,temp1);
        }else if(element.tagName()=="texFile") {
            texFileItem->setText(0,element.tagName());
            texFile = element.text().toStdString();
            texFileItem->setText(2,element.text());
        }else if(element.tagName()=="motion") {
            motionItem->setText(0,element.tagName());
            motionItem->setText(1,QString("Speed = ")+element.attribute("speed"));

            QDomElement sibling = element.firstChildElement();
            if(sibling.tagName()=="direction") {
                directionItem->setText(0,sibling.tagName());
                temp1 = sibling.attribute("x");
                temp2 = sibling.attribute("y");
                temp3 = sibling.attribute("z");
                direc.set(temp1.toFloat(),temp2.toFloat(),temp3.toFloat());
                temp1 = "(" + temp1 + "," + temp2 + "," + temp3 + ")";
                directionItem->setText(1,temp1);
            } else {
                std::cerr << "Scene description file: Unkown tag encountered under tag <"<<
                             elt.tagName().toStdString() <<">"<<std::endl;
            }

            spd = element.attribute("speed").toFloat();
        }
    }
    Primitive* msh = new TriangularMesh(objFile,type,this->Obj_NO,ctmp,tType,mType,texFile,nta,direc,spd);
    this->OBJ_LIST.push_back(msh);
    this->Obj_NO++;
}

void PinHoleCam::parseObjectsElement(const QDomElement &elt)
{
    for(QDomElement element = elt.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
    {
        if( element.attribute("type")=="Plane" ) {
            parsePlaneElement(element,objectsTreeWidget->invisibleRootItem());
        }else if( element.attribute("type")=="Sphere" ) {
            parseSphereElement(element,objectsTreeWidget->invisibleRootItem());
        }else if( element.attribute("type")=="Quadric" ) {
            parseQuadricElement(element,objectsTreeWidget->invisibleRootItem());
        }else if( element.attribute("type")=="Mesh" ) {
            parseTriangularMeshElement(element,objectsTreeWidget->invisibleRootItem());
        }else {
            std::cerr << "Scene description file: Unkown tag encountered under tag <"<<
                         elt.tagName().toStdString() <<">"<<std::endl;
        }        
    }
}

#pragma endregion XML DomReader helpers


PinHoleCam::PinHoleCam(QObject *parent, Production rendering, bool EMap, QStringList &input, QTreeWidget *stree, QTreeWidget *otree, Production ifAnimation)
    :QThread(parent)
{
    this->Obj_NO = 0;
    this->sceneTreeWidget = stree;
    this->objectsTreeWidget = otree;
    this->renderingType = rendering;
    this->ifAnimation = ifAnimation;
    this->EnvironmentMapEnabled = EMap;
    this->currPixel = 0;

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomElement root;

    // Code Block to read scene description
    QFile sceneFile(input.at(0));
    if( !sceneFile.open(QIODevice::ReadOnly) )
    {
        std::cerr << "Error: Cannot read file " << qPrintable(input.at(0))
                  << ": " << qPrintable(sceneFile.errorString())
                  << std::endl;
    }
    if (!sceneDoc.setContent(&sceneFile, false, &errorStr, &errorLine, &errorColumn))
    {
        sceneFile.close();
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
    }
    root = sceneDoc.documentElement();
    if (root.tagName() != "Scene") {
        std::cerr << "Error: Not a Scene description file" << std::endl;
    }
    this->parseSceneElement(root);
    sceneTreeWidget->show();
    this->readRadials(RadialFileName.c_str());
    sceneFile.close();

    // Code Block to objects data in the scene
    QFile objectFile(input.at(1));
    if( !objectFile.open(QIODevice::ReadOnly) )
    {
        std::cerr << "Error: Cannot read file " << qPrintable(input.at(1))
                  << ": " << qPrintable(objectFile.errorString())
                  << std::endl;
    }
    if (!objectsDoc.setContent(&objectFile, false, &errorStr, &errorLine, &errorColumn))
    {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
    }
    root = objectsDoc.documentElement();
    if (root.tagName() != "objects") {
        std::cerr << "Error: Not a Objects file" << std::endl;
    }
    this->parseObjectsElement(root);
    objectsTreeWidget->show();
    objectFile.close();

    /* Compute the orthogonal unit vectors to define space */    
    Vector3D n2 = *V_view;
    n2.normalize();
    SceneCenter = new Point((*Pe)+n2*d);
    n0 = new Vector3D(cross(*V_view,*V_up));
    n0->normalize();
    n1 = new Vector3D(cross(*n0,n2));

    /*  Compute the origin of the scene */
    SceneOrigin = new Point( (*SceneCenter) - ((*n0)*(Sx/2.0)+(*n1)*(Sy/2.0)) );

    // enum Production { IGNORE, RAYTRACE, DOF, MB, AMBOCC, ANIMATE, COMBINE };
    switch(renderingType)
    {
    case RAYTRACE:
        pixels = new PixColor*[Xmax];
        for(unsigned int col=0; col < Xmax; col++)
            pixels[col] = new PixColor[Ymax];
        break;
    case DOF:
        DOFpixels = new PixColor*[Xmax];
        for(unsigned int col=0; col < Xmax; col++)
            DOFpixels[col] = new PixColor[Ymax];
        break;
    case MB:
        pixels = new PixColor*[Xmax];
        MBpixels = new PixColor*[Xmax];
        for(unsigned int col=0; col < Xmax; col++)
        {
            pixels[col] = new PixColor[Ymax];
            MBpixels[col] = new PixColor[Ymax];
        }
        break;
    case AMBOCC:
        AMB_OCCpixels = new PixColor*[Xmax];
        for(unsigned int col=0; col < Xmax; col++)
            AMB_OCCpixels[col] = new PixColor[Ymax];
        break;
    case ANIMATE: {
        switch(ifAnimation)
        {
        case RAYTRACE:
            pixels = new PixColor*[Xmax];
            for(unsigned int col=0; col < Xmax; col++)
                pixels[col] = new PixColor[Ymax];
            break;
        case AMBOCC:
            AMB_OCCpixels = new PixColor*[Xmax];
            for(unsigned int col=0; col < Xmax; col++)
                AMB_OCCpixels[col] = new PixColor[Ymax];
            break;
        default:
            std::cout<<"No animation option"<<std::endl;
            exit(1);
            break;
        }
    }
    break;
    case COMBINE:
        pixels = new PixColor*[Xmax];
        AMB_OCCpixels = new PixColor*[Xmax];
        COMBINE_pixels = new PixColor*[Xmax];
        for(unsigned int col=0; col < Xmax; col++)
        {
            pixels[col] = new PixColor[Ymax];
            AMB_OCCpixels[col] = new PixColor[Ymax];
            COMBINE_pixels[col] = new PixColor[Ymax];
        }
        break;
    default:
        std::cerr<<"No rendering option"<<std::endl;
        exit(1);
        break;
    }
}

PinHoleCam::~PinHoleCam()
{
    // Free all the dynamic allocations
    switch(renderingType)
    {
    case RAYTRACE:
        for(unsigned int k=0; k<Xmax; k++)
            delete[] pixels[k];
        delete[] pixels;
        break;
    case DOF:
        for(unsigned int k=0; k<Xmax; k++)
            delete[] DOFpixels[k];
        delete[] DOFpixels;
        break;
    case MB:
        for(unsigned int k=0; k<Xmax; k++)
        {
            delete[] pixels[k];
            delete[] MBpixels[k];
        }
        delete[] pixels;
        delete[] MBpixels;
        break;
    case AMBOCC:
        for(unsigned int k=0; k<Xmax; k++)
            delete[] AMB_OCCpixels[k];
        delete[] AMB_OCCpixels;
        break;
    case ANIMATE: {
        switch(ifAnimation)
        {
        case RAYTRACE:
            for(unsigned int k=0; k<Xmax; k++)
                delete[] pixels[k];
            delete[] pixels;
            break;
        case AMBOCC:
            for(unsigned int k=0; k<Xmax; k++)
                delete[] AMB_OCCpixels[k];
            delete[] AMB_OCCpixels;
            break;
        default:
            std::cout<<"No animation option"<<std::endl;
            exit(1);
            break;
        }
    }
    break;
    case COMBINE:
        for(unsigned int k=0; k<Xmax; k++)
        {
            delete[] pixels[k];
            delete[] AMB_OCCpixels[k];
        }
        delete[] pixels;
        delete[] AMB_OCCpixels;
        break;
    default:
        std::cout<<"No rendering option"<<std::endl;
        exit(1);
        break;
    }

    delete Pe;
    delete V_view;
    delete SceneCenter;
    delete SceneOrigin;
    delete n0;
    delete n1;
    delete V_up;
	if(this->EnvironmentMapEnabled)
		delete EnvironmentMap;
    delete specL;
    delete AmbientL;
    delete Cc;
    delete Cw;

    for(unsigned int k=0; k < Plights.size(); k++ )
        delete Plights[k];
    for(unsigned int k=0; k < Clights.size(); k++ )
        delete Clights[k];
    for(unsigned int k=0; k < Alights.size(); k++ )
        delete Alights[k];
    for(unsigned int k=0; k < Radials.size(); k++ )
        delete Radials[k];
    for(unsigned int k=0; k < OBJ_LIST.size(); k++ )
        delete OBJ_LIST[k];	
}

void PinHoleCam::readRadials(const char* filename)
{
    char buff[3];
    float x,y,z;
    Vector3D* vtmp;
    string inputLine;
    stringstream stream(stringstream::in | stringstream::out);
    ifstream ObjFile(filename,ios::in);

    while(getline(ObjFile,inputLine)) {
        stream.clear();                         /* clear internal error flags */
        stream.str("");                         /* to remove the old string. */
        buff[0]='\0';buff[1]='\0';buff[2]='\0'; /* clear old buff values */
        stream<<inputLine;
        stream>>buff;
        if(buff[0]=='v')
        {
            stream>>x>>y>>z;
            vtmp = new Vector3D(x,y,z);
            vtmp->normalize();
            this->Radials.push_back(vtmp);
        }
    }
}

void PinHoleCam::resetChannel()
{
    this->max_channel = 1.0;
    this->min_channel = 0.0;

	Point tempP;
	Vector3D tempV;
    // Added here to recompute scene_origin while adding animate function
    Vector3D n2 = *V_view;
    n2.normalize();
	tempP = ((*Pe) + n2*d);
	SceneCenter->set(tempP.x,tempP.y,tempP.z);
	tempV = cross(*V_view,*V_up);
    n0->set(tempV.vec[0],tempV.vec[1],tempV.vec[2]);
    n0->normalize();
	tempV = cross(*n0,n2);
    n1->set(tempV.vec[0],tempV.vec[1],tempV.vec[2]);

    /*  Compute the origin of the scene */
	Point temp = (*SceneCenter) - ((*n0)*(Sx/2.0)+(*n1)*(Sy/2.0));
	SceneOrigin->set(tempP.x,tempP.y,tempP.z);
}

void PinHoleCam::moveCamera(float theta)
{
    float a,b,c,r = 8;
    a = r*sin(theta);
    b = -1.0;
    c = r*cos(theta);
    if( abs(a) < 1.0e-7 ) a = 0.0;
    if( abs(b) < 1.0e-7 ) b = 0.0;
    if( abs(c) < 1.0e-7 ) c = 0.0;
    this->Pe->set(a,b,c);

    a = sin(PI+theta);
    b = 0.0;
    c = cos(PI+theta);
    if( abs(a) < 1.0e-5 ) a = 0.0;
    if( abs(b) < 1.0e-5 ) b = 0.0;
    if( abs(c) < 1.0e-5 ) c = 0.0;
    this->V_view->set(a,b,c);
    cout<<"\nNew Eye Position :"<<Pe<<"\n New view direction"<<V_view<<endl;
}

PixColor PinHoleCam::rayCast(Ray r, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId)
{
    float tmin,t;
    int min_id=-1;
    PixColor ret_val;
    tmin = 0.0;
    t = 0.0;

    for(unsigned int k=0; k<OBJ_LIST.size(); k++)
    {
        if( OBJ_LIST[k]->isVisible(r.origin) )
        {
            t = OBJ_LIST[k]->intersect(r);
            if(t>1.0e-5 && (t < tmin || !tmin))
            {
                tmin = t;
                min_id = k;
            }
        }
    }

    if( tmin>0 && min_id != -1 )
    {
        Point Ph = r.origin + (r.direction*tmin);
        ret_val = computeEffectofLights(Ph, r, min_id, depth, reflnDepth, refrDepth, prevMinId);
    }
    else if(EnvironmentMapEnabled)
    {
        EnvironmentMap->intersect(r, ret_val);
    }
    return ret_val;
}

bool PinHoleCam::isReflectionObstructed(const Point& Ph, const Point& Pl, const Vector3D& n_lh, unsigned int min_id)
{
    bool ret_val=false;
    float t;
    float D2Light = (Ph-Pl).mag;
    Ray r(Ph,n_lh);

    for(unsigned int k=0; k<OBJ_LIST.size(); k++)
    {
        if( OBJ_LIST[k]->isVisible(Ph) )
        {
            t = OBJ_LIST[k]->intersect( r );
            if(t>1.0e-5 && t<D2Light)
            {
                ret_val=true;
                break;
            }
        }
    }
    return ret_val;
}

PixColor PinHoleCam::computeEffectofLights(const Point& Ph, Ray ry, unsigned int min_id, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId)
{
    PixColor PL_result, AL_result;
    PixColor Ch_tmin;
    Vector3D n_h;
    Vector3D r;
    Vector3D n_pe = ry.direction;
    float C;

    OBJ_LIST[min_id]->extract_Color_Normal(Ph, n_h, Ch_tmin);
    C = dot(n_pe,n_h);
    r = n_pe - (n_h*(2*C));
    r.normalize();

    if( !OBJ_LIST[min_id]->ifDiffusive() )
    {
        if( reflnDepth < MaxReflectionDepth && OBJ_LIST[min_id]->isReflective() )
        {
            Ray reflectedRay(Ph, r);
            PixColor RRComp = rayCast(reflectedRay,depth+1,reflnDepth+1,refrDepth,min_id);
            Ch_tmin = addC(mulC(RRComp, Reflectance),mulC(Ch_tmin, (1-Reflectance)));
        }
        else if( refrDepth < MaxRefractionDepth && OBJ_LIST[min_id]->isRefractive() )
        {            
            float neta, term;
            if( min_id == prevMinId ) {
                // for convex shapes you shouldn't have a problem with this condition for now
                n_h.invert();
                neta = 1/OBJ_LIST[min_id]->getNeta();
            }else {
                neta = OBJ_LIST[min_id]->getNeta();
            }
            term = (((C*C)-1)/(neta*neta)) + 1;
            if(term < 0)
            {
                // Total internal reflection happens
                r = n_pe - n_h*(2*C);
                r.normalize();
                Ray reflectedRay(Ph, r);
                PixColor RRComp = rayCast(reflectedRay,depth+1,reflnDepth+1,refrDepth,min_id);                
                Ch_tmin = addC(mulC(RRComp, Reflectance),mulC(Ch_tmin, (1-Reflectance)));
            }
            else
            {
                //Refraction Vector3D & component Calculation
                r = (n_h*((C/neta) + sqrt(term))) - (n_pe*(1.0/neta));
                r.normalize();
                Ray refractedRay(Ph, r);
                PixColor RRComp = rayCast(refractedRay,depth+1,reflnDepth,refrDepth+1,min_id);
                Ch_tmin = addC(mulC(RRComp, Refraction), mulC(Ch_tmin, (1-Refraction)));
            }
        }
    }

    if( Plights.size() > 0 )
    {
        float cos_theta, cos_phi;
        Vector3D n_lh;
        // Loop  through all point lights in the scene
        for(unsigned int light=0; light < Plights.size(); light++)
        {
            n_lh = (*Plights[light])-Ph;
            n_lh.normalize();
            if( !isReflectionObstructed(Ph, *Plights[light], n_lh, min_id) )
            {
                //Diffusive component computation
                cos_theta = dot(n_h,n_lh);   //cos_theta = (cos_theta<0) ? 0.0 : cos_theta;
                cos_theta = (1+cos_theta)/2;
                // Specular highlight computation
                cos_phi = dot(r,n_lh);
                cos_phi = (cos_phi<0) ? 0.0 : pow(cos_phi,phongExp);

                //Below one includes Lambert and Phong Illumination
                PL_result.add(addC(mulC(mixC(*Clights[light], Ch_tmin),(1-cos_phi)*cos_theta),mulC(mixC(*Clights[light], *specL),cos_phi)));
            }
        }
    }

    if( Alights.size() > 0 )
    {
        unsigned int m, n, i, j;
        Vector3D n_lh;
        Point Pl;
        PixColor Cl;

        float cos_theta,cos_phi;
        // Loop  through all area lights in the scene
        for(unsigned int light=0; light < Alights.size(); light++)
        {
            Cl = *(Alights[light]->AC);
            m = Alights[light]->m;
            n = Alights[light]->n;

            for(i=0; i < m; i++)
            {
                for(j=0; j < n; j++ )
                {
                    Pl = Alights[light]->mn_Lights[i][j];
                    n_lh = Pl-Ph;

                    if( !isReflectionObstructed(Ph, Pl, n_lh, min_id) )
                    {
                        // Diffusive component computation
                        cos_theta = dot(n_h,n_lh);
                        cos_theta = (cos_theta<0) ? 0.0 : cos_theta;
                        // Specular highlight computation
                        cos_phi = dot(r,n_lh);
                        cos_phi = (cos_phi<0) ? 0.0 : pow(cos_phi,phongExp);

                        //Below one includes Lambert and Phong Illumination
                        AL_result.add(addC(mulC(mixC(Cl,Ch_tmin),(1-cos_phi)*cos_theta),mulC(mixC(Cl,*specL),cos_phi)));
                    }
                }
            }
        }
        PL_result.add(AL_result);
    }
    return PL_result;
}

void PinHoleCam::render()
{
    unsigned int i, j, I, J;
    float temp, X, Y;
    Point Pp;

    for(I=0; I<Xmax; I++)
    {
        for(J=0; J<Ymax; J++)
        {
            PixColor clr;
            for(i=0; i<m; i++)
            {
                for(j=0; j<n; j++)
                {
                    X = I + (i + rand()%2)/(float)m;
                    Y = J + (j + rand()%2)/(float)n;
                    Pp = (*SceneOrigin) + ((*n0)*(Sx*X/Xmax) + (*n1)*(Sy*Y/Ymax));
                    Vector3D vtmp1 = Pp-*Pe;
                    vtmp1.normalize();
                    Ray ry(*Pe,vtmp1);
                    clr.add(rayCast(ry, 0, 0, 0, -1));
                }
            }

            temp = max( max(clr.red, clr.green), clr.blue);

            if(max_channel < temp)
                max_channel = temp;

            temp = min( min(clr.red, clr.green), clr.blue );

            if(min_channel > temp)
                min_channel = temp;
            pixels[I][J] = clr;
            emit sendPixelIndex(++currPixel);
        }// coloumn by coloumn
    }// row by row
}

void PinHoleCam::renderWithDOF()
{
    unsigned int i, j, I, J, alongX, alongY, XS, YS;
    float temp, X, Y, PeLen;
    Point Pp;
    Point pnt;
    Point O_Pe;
    Point** rPe;
    Vector3D CamN0(1,0,0);
    Vector3D CamN1(0,1,0);

    PeLen = ApertureDiameter;
    XS = (int)sqrt((float)NumRays);
    YS = XS;
    /* set image plane at focal plane; distance of focal plane(focal length) = Fnumber*ApertureDiameter */

    /* render using actual Pe */
    render();
    for(I=0;I<Xmax; I++)
        for(J=0;J<Ymax; J++)
            DOFpixels[I][J].add(pixels[I][J]);

    /* Create a 2d array of random points on aperture */
    rPe = (Point**)malloc( XS * sizeof(Point*));
    for(I=0; I< XS; I++)
        rPe[I] = (Point*)malloc(YS * sizeof(Point));

    O_Pe = (*this->Pe) + ( CamN0*(PeLen/2) + CamN1*(PeLen/2) );

    for(I=0; I < XS; I++)
    {
        for(J=0; J < YS; J++)
            rPe[I][J] = O_Pe + ((CamN0*(I*PeLen/XS + d_rand(0,PeLen/XS))) + (CamN1*((PeLen-J-1)*PeLen/YS+d_rand(0,PeLen/XS))));
    }

    /* Now use points distributed over rect. lens and do raycast/raytrace */
    for(alongX=0; alongX < XS; alongX++)
    {
        for(alongY=0; alongY < YS; alongY++)
        {
            for(I=0; I<Xmax; I++)
            {
                for(J=0; J<Ymax; J++)
                {
                    PixColor clr;
                    for(i=0; i<m; i++)
                    {
                        for(j=0; j<n; j++)
                        {
                            X = I + ((float)i)/m + (rand() % 2)/m;
                            Y = J + ((float)j)/n + (rand() % 2)/n;

                            Pp = (*SceneOrigin) + ((*n0)*(Sx*X/Xmax)+(*n1)*(Sy*Y/Ymax));
                            pnt = (*Pe) + (Pp-(*(this->Pe)))*FocalLength;
                            Vector3D vtmp1 = pnt - rPe[alongX][alongY];
                            vtmp1.normalize();
                            Ray ry(rPe[alongX][alongY], vtmp1);
                            clr.add(rayCast(ry, 0, 0, 0, -1));
                        }
                    }

                    temp = max( max( clr.red, clr.green ), clr.blue );

                    if(max_channel < temp)
                        max_channel = temp;

                    temp = min( min(clr.red, clr.green ), clr.blue );

                    if(min_channel > temp)
                        min_channel = temp;
                    DOFpixels[I][J] = clr;
                }// coloumn by coloumn
            }// row by row
        }
    }
}

void PinHoleCam::renderWithMB()
{
    unsigned int I,J, k;
    float time,TimeRange, temp;
    TimeRange = 1.0;
    for(time=0.0; time<TimeRange; time=time+0.05)
    {
        /* move objects */
        for(k=0; k < OBJ_LIST.size(); k++)
            OBJ_LIST[k]->translate(time);

        /* render */
        render();

        /* collect colors */
        for(I=0; I<Xmax; I++)
        {
            for(J=0; J<Ymax; J++)
            {
                MBpixels[I][J].add(pixels[I][J]);
                PixColor clr = MBpixels[I][J];
                temp = max( max( clr.red, clr.green ), clr.blue );
                if(max_channel < temp)
                    max_channel = temp;
                temp = min( min( clr.red, clr.green ), clr.blue );
                if(min_channel > temp)
                    min_channel = temp;
                MBpixels[I][J].set(clr);
            }
        }
    }
}

PixColor PinHoleCam::rayTraceAmbOcc(Ray r, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId)
{
    float tmin,t;
    unsigned int min_id,k;
    PixColor ret_val;
    tmin = 0.0;
    t = 0.0;

    for(k=0; k< OBJ_LIST.size(); k++)
    {
        if( OBJ_LIST[k]->isVisible(r.origin) )
        {
            t = OBJ_LIST[k]->intersect(r);
            if(t>1.0e-5 && (t < tmin || !tmin))
            {
                tmin = t;
                min_id = k;
            }
        }
    }

    if(tmin>0)
    {
        Point Ph = r.origin + r.direction*tmin;
        ret_val = findAmbientOcclusion( Ph, r, min_id, depth, reflnDepth, refrDepth, prevMinId);
    }
    /*else if(EnvironmentMapEnabled)
    {
        EnvironmentMap->intersect(r, ret_val);
        ret_val =89.0* ret_val;
    }*/
    return ret_val;
}

bool PinHoleCam::isRadialObstructed(Ray r, unsigned int min_id, PixColor& HitColor)
{
    bool ret_val=false;
    float t, tmin;
    tmin = 0.0;

    for(unsigned int k=0; k<OBJ_LIST.size(); k++)
    {
        if( OBJ_LIST[k]->isVisible(r.origin) )
        {
            t = OBJ_LIST[k]->intersect( r );
            if(t>1.0e-5 && (t < tmin || !tmin) && t < this->HitEffectiveDistance)
            {
                ret_val=true;
                HitColor = OBJ_LIST[k]->getColor(r.origin);
            }
        }
    }
    return ret_val;
}

PixColor PinHoleCam::findAmbientOcclusion(const Point& Ph, Ray r, unsigned int min_id, int depth, int reflnDepth, int refrDepth, unsigned int prevMinId)
{
    PixColor EnvMapClr;
    EnvMapClr.set(1.0,1.0,1.0,0.0);
    /*PixColor AC_result, HitColor;
    Vector3D n_r, n_h;
    float cos_theta = 0.0;


    Vector3D n_pe = ry.direction;
    float C;

    OBJ_LIST[min_id]->extract_Color_Normal(Ph, n_h, Ch_tmin);
    float C = dot(n_pe,n_h);
    Vector3D r = normalize(n_pe - (2*C)*n_h);

    n_h = OBJ_LIST[min_id]->getNormal(Ph);

    float C = dot(r.direction,n_h);
    Vector3D rec = normalize(r.direction - (2*C)*n_h);
    Ray ry(Ph, rec);
    EnvironmentMap->intersect(ry, AC_result);

    if( !OBJ_LIST[min_id]->ifDiffusive() )
    {
        if( reflnDepth < MaxReflectionDepth && OBJ_LIST[min_id]->isReflective() )
        {
            Ray reflectedRay(Ph, r);
            PixColor RRComp = rayCast(reflectedRay,depth+1,reflnDepth+1,refrDepth,min_id);
            Ch_tmin = Reflectance * RRComp + (1-Reflectance)*Ch_tmin;
        }
        else if( refrDepth < MaxRefractionDepth && OBJ_LIST[min_id]->isRefractive() )
        {
            float neta, term;
            if( min_id == prevMinId ) {
                // for convex shapes you shouldn't have a problem with this condition for now
                n_h = -1.0*n_h;
                neta = 1/OBJ_LIST[min_id]->getNeta();
            }else {
                neta = OBJ_LIST[min_id]->getNeta();
            }
            term = (((C*C)-1)/(neta*neta)) + 1;
            if(term < 0)
            {
                // Total internal reflection happens
                r = normalize(n_pe - (2*C)*n_h);
                Ray reflectedRay(Ph,r);
                PixColor RRComp = rayCast(reflectedRay,depth+1,reflnDepth+1,refrDepth,min_id);
                Ch_tmin = Reflectance * RRComp + (1-Reflectance)*Ch_tmin;
            }
            else
            {
                //Refraction Vector3D & component Calculation
                r = normalize( (1.0/neta) *n_pe - ((C/neta) + sqrt(term)) *n_h);
                Ray refractedRay(Ph, r);
                PixColor RRComp = rayCast(refractedRay,depth+1,reflnDepth,refrDepth+1,min_id);
                Ch_tmin = Refraction * RRComp + (1-Refraction)*Ch_tmin;
            }
        }
    }

    if(depth==0)
    {
        for(unsigned int radial=0; radial < this->Radials.size(); radial++)
        {
            n_r = Radials[radial];
            cos_theta = dot(n_r,n_h);
            if( cos_theta >= 0 &&  cos_theta <= 1 )
            {
                Ray ry(Ph,n_r);
                if( isRadialObstructed(ry, min_id, HitColor) )
                {
                    ;//AC_result = AC_result + HitColor;
                }
                else
                {
                    EnvironmentMap->intersect(ry, EnvMapClr);
                    AC_result = AC_result + EnvMapClr;
                }
                //AC_result = AC_result + rayCast(ry, 0, 0, -1);
            }
        }
    }
    else
    {
        EnvironmentMap->intersect(ry, EnvMapClr);
    }*/

    return EnvMapClr;
}

void PinHoleCam::renderAmbOcc()
{
    unsigned int i, j, I, J;
    Point Pp;
    float temp, X, Y;

    for(I=0; I<Xmax; I++)
    {
        for(J=0; J<Ymax; J++)
        {
            PixColor clr;
            for(i=0; i<m; i++)
            {
                for(j=0; j<n; j++)
                {
                    X = I + ((float)i)/m + (rand() % 2)/m;
                    Y = J + ((float)j)/n + (rand() % 2)/n;
                    Pp = (*SceneOrigin)+(((*n0)*(Sx*X/Xmax))+((*n1)*(Sy*Y/Ymax)));
                    Vector3D vtmp1 = Pp - *Pe;
                    vtmp1.normalize();
                    Ray ry(*Pe, vtmp1);
                    clr.add(rayTraceAmbOcc(ry,0,0,0,-1));
                }
            }

            temp = max( max( clr.red, clr.green ), clr.blue );
            if(max_channel < temp)
                max_channel = temp;
            temp = min( min( clr.red, clr.green ), clr.blue );
            if(min_channel > temp)
                min_channel = temp;

            AMB_OCCpixels[I][J] = clr;
        }// coloumn by coloumn
    }// row by row
}

void PinHoleCam::combine(PixColor **op1, PixColor **op2)
{
    float temp=0.0;
    unsigned int I, J;
    for(I=0; I<Xmax; I++)
    {
        for(J=0; J<Ymax; J++)
        {
            COMBINE_pixels[I][J].add(op1[I][J]);
            COMBINE_pixels[I][J].add(op2[I][J]);
            PixColor clr = COMBINE_pixels[I][J];
            temp = max( max( clr.red, clr.green ), clr.blue );
            if(max_channel < temp)
                max_channel = temp;
            temp = min( min( clr.red, clr.green ), clr.blue );
            if(min_channel > temp)
                min_channel = temp;
            COMBINE_pixels[I][J] = clr;
        }
    }
}


void PinHoleCam::writeImage2File(PixColor **result, QString OutputName)
{
    unsigned int I, J;
    cout<<"Channel max_channel, min_channel: "<<max_channel<<", "<<min_channel<<endl;

    QImage my_image(Xmax,Ymax, QImage::Format_ARGB32_Premultiplied);
    QColor temp_hold;
    //Cap the RGb channel values now and then write into image
    for(I=0; I<Xmax; I++)
    {
        for(J=0; J<Ymax; J++)
        {
            result[I][J].HDR2LDR(max_channel, min_channel);
            temp_hold.setRgbF(result[I][J].red, result[I][J].green, result[I][J].blue, result[I][J].alpha);
            my_image.setPixel(I, J, temp_hold.rgb());
        }
    }
    my_image.save(OutputName);
}

void PinHoleCam::animate(Production ifAnimation)
{
    QString file;
    unsigned int frames = 16;
    float step = (2*PI)/frames;
    float threshold = 2*PI;
    unsigned int k, curr_frame=0;
    QString ss;
    float time=0.0;

    for(float theta = 0; theta < threshold; theta+=step, time+=0.1)
    {
        for(k=0; k < OBJ_LIST.size(); k++)
        {
            OBJ_LIST[k]->translate(time);
            OBJ_LIST[k]->moveAxis(theta);
            //OBJ_LIST[k]->print();
        }
        //this->moveCamera(theta);
        ss = QString("%1").arg(curr_frame++);
#ifdef Q_OS_WIN
        file = outputDirectory + QString("animation\\") + ss + QString(".jpg");
#endif
#ifdef Q_OS_LINUX
        file = outputDirectory + QString("animation/") + ss + QString(".jpg");
#endif
        switch(ifAnimation)
        {
        case RAYTRACE:
            this->render();
            writeImage2File(pixels, file);
            break;
        case AMBOCC:
            this->renderAmbOcc();
            writeImage2File(AMB_OCCpixels, file);
            break;
        default:
            std::cout<<"No animation option"<<std::endl;
            exit(1);
            break;
        }
        ss.clear();
        resetChannel();
    }
}

void PinHoleCam::run(void)
{
    time_t start, end;
    resetChannel();

    // enum Production { RAYTRACE, DOF, MB, AMBOCC, ANIMATE, COMBINE };
    start=time(NULL);
    switch(renderingType)
    {    
    case RAYTRACE:
        render();
        output = outputDirectory+QString("Normal.jpg");
        writeImage2File(pixels, output);
        break;
    case DOF:
        renderWithDOF();
        output = outputDirectory+QString("DOF.jpg");
        writeImage2File(DOFpixels, output);
        break;
    case MB:
        renderWithMB();
        output = outputDirectory+QString("MB.jpg");
        writeImage2File(MBpixels, output);
        break;
    case AMBOCC:
        renderAmbOcc();
        output = outputDirectory+QString("AmbOcc.jpg");
        writeImage2File(AMB_OCCpixels, output);
        break;
    case ANIMATE:
        animate(ifAnimation);
        break;
    case COMBINE:
        render();
        writeImage2File(pixels, outputDirectory+QString("Normal.jpg"));

        resetChannel();
        renderAmbOcc();
        writeImage2File(AMB_OCCpixels, outputDirectory+QString("AmbOcc.jpg"));

        resetChannel();
        combine(AMB_OCCpixels, pixels);
        output = outputDirectory+QString("Combined.jpg");
        writeImage2File(COMBINE_pixels, output);
        break;
    default:
        std::cout<<"No rendering option"<<std::endl;
        break;
    }
    end = time(NULL);
    std::cout << "Rendering time: "<< (end-start)<<" seconds"<< "\n\n";
}

couple PinHoleCam::getOutImgDimensions() { return couple(Xmax,Ymax); }

QString PinHoleCam::getOutputName() { return output; }
