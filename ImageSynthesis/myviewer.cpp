#include "myviewer.h"
#include "RT_math.h"
#include <QtGui>
#include <cstdlib>
#include <ctime>

MyViewer::MyViewer(QWidget *parent) :
    QMainWindow(parent)
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    setWindowTitle(tr("RT Output Viewer"));
    resize(640,480);
}

void MyViewer::createActions()
{
    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MyViewer::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
    this->prepareToolBar();
}

void MyViewer::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void MyViewer::prepareToolBar()
{
    QStringList prodCombo, enableCombo;
    prodCombo << "Select a effect" << "IGNORE - No Effect"
              << "Std. Ray Trace" << "Depth of Field" << "Motion Blur"
              << "Ambient Occlusion" <<"Animation"<<"Combination";
    enableCombo << "Select to Enable/Disable" << "True" << "False";
    myBar = new QToolBar;
    myBar->addWidget(new QLabel(" "));

    loadPropertiesButton = new QPushButton(tr(" Load Scene Properties "));
    connect(loadPropertiesButton,SIGNAL(clicked()),this,SLOT(loadProperties()));
    myBar->addWidget(loadPropertiesButton);
    myBar->addWidget(new QLabel("  "));

    loadObjectsButton = new QPushButton(tr("Load Objects"));
    loadObjectsButton->setEnabled(false);
    connect(loadObjectsButton,SIGNAL(clicked()),this,SLOT(loadObjects()));
    myBar->addWidget(loadObjectsButton);
    myBar->addWidget(new QLabel("  "));
    myBar->addSeparator();
    myBar->addWidget(new QLabel("  "));

    prodLabel = new QLabel(tr("Rendering Effect"));
    prodLabel->setEnabled(false);
    myBar->addWidget(prodLabel);
    myBar->addWidget(new QLabel(" "));
    productionVarBox = new QComboBox;
    productionVarBox->insertItems(-1,prodCombo);
    productionVarBox->setEnabled(false);
    connect(productionVarBox,SIGNAL(activated(int)),this,SLOT(prodComboChanged(int)));
    myBar->addWidget(productionVarBox);
    myBar->addWidget(new QLabel("  "));

    envMapLabel = new QLabel(tr("Environment Map"));
    envMapLabel->setEnabled(false);
    myBar->addWidget(envMapLabel);
    myBar->addWidget(new QLabel(" "));
    envMapEnabledBox = new QComboBox;
    envMapEnabledBox->insertItems(-1,enableCombo);
    envMapEnabledBox->setEnabled(false);
    connect(envMapEnabledBox,SIGNAL(activated(int)),this,SLOT(envMapEnableChanged(int)));
    myBar->addWidget(envMapEnabledBox);
    myBar->addWidget(new QLabel("  "));
    myBar->addSeparator();

    renderImageButton = new QPushButton(tr("Ray Trace"));
    renderImageButton->setEnabled(false);
    connect(renderImageButton,SIGNAL(clicked()),this,SLOT(renderImage()));
    myBar->addWidget(renderImageButton);
    myBar->addWidget(new QLabel("  "));

    progress = new QProgressBar;
    progress->setEnabled(false);
    myBar->addWidget(progress);

    addToolBar(myBar);
}

void MyViewer::zoomIn() { scaleImage(1.25); }

void MyViewer::zoomOut() { scaleImage(0.8); }

void MyViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor  = 1.0;
}

void MyViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void MyViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
                       tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
                          "and QScrollArea to display an image. QLabel is typically used "
                          "for displaying a text, but it can also display an image. "
                          "QScrollArea provides a scrolling view around another widget. "
                          "If the child widget exceeds the size of the frame, QScrollArea "
                          "automatically provides scroll bars. </p><p>The example "
                          "demonstrates how QLabel's ability to scale its contents "
                          "(QLabel::scaledContents), and QScrollArea's ability to "
                          "automatically resize its contents "
                          "(QScrollArea::widgetResizable), can be used to implement "
                          "zooming and scaling features. </p><p>In addition the example "
                          "shows how to use QPainter to print an image.</p>"));
}

void MyViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void MyViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MyViewer::loadImage()
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    output = rayTraceThread->getOutputName();
    if (!output.isEmpty()) {
        QImage image(output);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(output));
            return;
        }
        imageLabel->setPixmap(QPixmap::fromImage(image));
        scaleFactor = 1.0;

        printAct->setEnabled(true);
        fitToWindowAct->setEnabled(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();
    }
}

void MyViewer::print()
{
    Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}

void MyViewer::loadProperties()
{
    QString name;
    while(name.isEmpty())
    {
        name = QFileDialog::getOpenFileName(this,QObject::tr("Open Scene properties file"),
                                            "E:\\CODE WORK\\Ray Tracer\\ImageSynthesis\\input\\",
                                            QObject::tr("Xml files(*.xml)"));
    }
    args.push_back(name);
    loadObjectsButton->setEnabled(true);
}

void MyViewer::loadObjects()
{
    QString name;
    while(name.isEmpty())
    {
        name = QFileDialog::getOpenFileName(this,QObject::tr("Open Objects file"),
                                            "E:\\CODE WORK\\Ray Tracer\\ImageSynthesis\\input\\",
                                            QObject::tr("Xml files(*.xml)"));
    }
    args.push_back(name);
    prodLabel->setEnabled(true);
    productionVarBox->setEnabled(true);
    envMapLabel->setEnabled(true);
    envMapEnabledBox->setEnabled(true);
    renderImageButton->setEnabled(true);
}

void MyViewer::prodComboChanged(int index)
{
    index--;
    switch(index)
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

void MyViewer::envMapEnableChanged(int index)
{
    envMapEnable = (index==1?true:false);
}

void MyViewer::renderImage()
{
    QStringList headerLabels;
    headerLabels << QObject::tr("Property Name") <<
                    QObject::tr(" Attributes") << QObject::tr("Value");
    QTreeWidget *sceneTree = new QTreeWidget;
    sceneTree->setColumnCount(3);
    sceneTree->setHeaderLabels(headerLabels);
    sceneTree->header()->setResizeMode(QHeaderView::Stretch);
    sceneTree->setWindowTitle(QObject::tr("Scene Description"));
    QTreeWidget *objectsTree = new QTreeWidget;
    objectsTree->setColumnCount(3);
    objectsTree->setHeaderLabels(headerLabels);
    objectsTree->header()->setResizeMode(QHeaderView::Stretch);
    objectsTree->setWindowTitle(QObject::tr("Objects"));

    rayTraceThread = new PinHoleCam(this, productionType,envMapEnable,args,sceneTree,objectsTree);

    couple dimen = rayTraceThread->getOutImgDimensions();
    progress->setRange(0,dimen.a1*dimen.a2);
    progress->setValue(0);
    progress->setEnabled(true);
    connect(rayTraceThread,SIGNAL(sendPixelIndex(int)),progress,SLOT(setValue(int)));
    connect(rayTraceThread,SIGNAL(finished()),this,SLOT(loadImage()));
    QMessageBox::information(this, tr("RT Viewer"),tr("Loading input files complete. Rendering progress can be followed on progressbar."));
    srand(time(NULL));
    rayTraceThread->start();
    this->setCursor(QCursor(Qt::WaitCursor));
}

// //IGNORE, RAYTRACE, DOF, MB, AMBOCC, ANIMATE, COMBINE };
//PinHoleCam viewBox(COMBINE, true, argv[1],argv[2]);
//PinHoleCam viewBox(ANIMATE, false, argv[1],argv[2], RAYTRACE);
//PinHoleCam viewBox(AMBOCC, true, argv[1],argv[2]);
