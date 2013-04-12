#ifndef MYVIEWER_H
#define MYVIEWER_H

#include <QMainWindow>
#include <QPrinter>
#include "RT_Scene.h"

class QPushButton;
class QComboBox;
class QLabel;
class QScrollArea;
class QToolBar;
class QProgressBar;
class QPrinter;
class QScrollBar;

class MyViewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyViewer(QWidget *parent = 0);

private slots:
    void loadProperties();
    void loadObjects();
    void prodComboChanged(int index);
    void envMapEnableChanged(int index);
    void renderImage();
    void loadImage();
    void print();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

private:
    void createActions();
    void createMenus();
    void updateActions();
    void prepareToolBar();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
    QPushButton *loadPropertiesButton;
    QPushButton *loadObjectsButton;
    QLabel *prodLabel;
    QComboBox *productionVarBox;
    QLabel *envMapLabel;
    QComboBox *envMapEnabledBox;
    QPushButton *renderImageButton;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    QAction *printAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *myBar;
    QProgressBar *progress;

    PinHoleCam *rayTraceThread;
    Production productionType;
    bool envMapEnable;
    QStringList args;
    QString output;
};

#endif // MYVIEWER_H
