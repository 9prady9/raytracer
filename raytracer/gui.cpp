#include "gui.h"

#include <RT_math.h>

#include <QtWidgets\qfiledialog.h>
#include <QtWidgets\qtreewidget.h>
#include <QtWidgets\qmessagebox.h>
#include <ctime>

UserInterface::UserInterface(QWidget* parent) :
	QWidget(parent)
{
	mUI.setupUi(this);
	setWindowTitle(tr("Raytracer"));
	connect(mUI.mSceneFileButton, SIGNAL(clicked()), this, SLOT(loadSceneProperties()));
	connect(mUI.mObjFileButton, SIGNAL(clicked()), this, SLOT(loadSceneObjects()));
	connect(mUI.mLoadInputsButton, SIGNAL(clicked()), this, SLOT(loadInputFiles()));
	connect(mUI.mEffectComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderMode(int)));
	connect(mUI.mEnvMapComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEnvMapMode(int)));
	connect(mUI.mRenderButton, SIGNAL(clicked()), this, SLOT(renderImage()));
	mCanvas = (Canvas*)mUI.mTabGrp->widget(1);
	productionType = RAYTRACE;
	envMapEnable = false;
	args.push_back("F:/CodeWorks/GitHub/raytracer/input/sceneproperties.xml");
	args.push_back("F:/CodeWorks/GitHub/raytracer/input/objectlist.xml");
}

UserInterface::~UserInterface()
{
}

void UserInterface::loadSceneProperties(void)
{
	args.clear();
    QString name = QFileDialog::getOpenFileName(this,QObject::tr("Open Scene properties file"),
                                            "E:\\CODE WORK\\Ray Tracer\\ImageSynthesis\\input\\",
                                            QObject::tr("Xml files(*.xml)"));
	if( !name.isNull() ) {
		args.push_back(name);
		mUI.mSceneFileLoc->setText(name);
	}
}

void UserInterface::loadSceneObjects(void)
{
	QString name = QFileDialog::getOpenFileName(this,QObject::tr("Open Objects file"),
                                            "E:\\CODE WORK\\Ray Tracer\\ImageSynthesis\\input\\",
                                            QObject::tr("Xml files(*.xml)"));
	if( !name.isNull() ) {
		args.push_back(name);
		mUI.mObjFileLoc->setText(name);
	}
}

void UserInterface::changeRenderMode(int index)
{
    switch(++index)
    {
    case 0:
        productionType = IGNORE;
        break;
    case 1:
        productionType = RAYTRACE;
        break;
    case 2:
        productionType = DOF;
        break;
    case 3:
        productionType = MB;
        break;
    case 4:
        productionType = AMBOCC;
        break;
    case 5:
        productionType = ANIMATE;
        break;
    case 6:
        productionType = COMBINE;
        break;
    default:
        break;
    }
}

void UserInterface::setEnvMapMode(int index)
{
	envMapEnable = (index==1?true:false);
}

void UserInterface::loadInputFiles(void)
{
    QTreeWidget *sceneTree = mUI.mSceneTree;
	sceneTree->header()->setSectionResizeMode(QHeaderView::Stretch);
    QTreeWidget *objectsTree = mUI.mObjTree;
    objectsTree->header()->setSectionResizeMode(QHeaderView::Stretch);

	sceneTree->clear();
	objectsTree->clear();

	// if renderthread already available
	// destroy old one and create new one
	if(rayTraceThread) {
		disconnect(mUI.mProgressBar,SLOT(setValue(int)));
		disconnect(this,SLOT(getRendering()));
		delete rayTraceThread;
	}
    rayTraceThread = new PinHoleCam(this, productionType,envMapEnable,args,sceneTree,objectsTree);
	
    couple dimen = rayTraceThread->getOutImgDimensions();
    mUI.mProgressBar->setRange(0,dimen.a1*dimen.a2);
    mUI.mProgressBar->setValue(0);
    connect(rayTraceThread,SIGNAL(sendPixelIndex(int)),mUI.mProgressBar,SLOT(setValue(int)));
    connect(rayTraceThread,SIGNAL(finished()),this,SLOT(getRendering()));
	sceneTree->updateGeometry();
	objectsTree->updateGeometry();
}

void UserInterface::renderImage(void)
{
	srand(time(NULL));
	if(rayTraceThread!=0) {
		//rayTraceThread->start();	
		rayTraceThread->run();
		this->getRendering();
	} else {
		QMessageBox msgBox;
		msgBox.setText("Render thread not created");
		msgBox.exec();
	}
}

void UserInterface::getRendering(void)
{
    output = rayTraceThread->getOutputName();
    if (!output.isEmpty()) {
		mCanvas->updateImage(QImage(output));
    }
}