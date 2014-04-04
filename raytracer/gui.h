#ifndef __GUI_H__
#define __GUI_H__

#include <QtWidgets\qwidget.h>

#include <ui_viewer.h>
#include <canvas.h>

#include "RT_Scene.h"

class UserInterface : public QWidget
{
	Q_OBJECT
public:
	explicit UserInterface(QWidget* parent=0);
	~UserInterface();

private slots:
	void loadSceneProperties(void);
    void loadSceneObjects(void);
    void changeRenderMode(int index);
    void setEnvMapMode(int index);
	void loadInputFiles(void);
    void renderImage(void);
	void getRendering(void);

private:
	Ui::RootGUI mUI;
	Canvas* mCanvas;

	PinHoleCam *rayTraceThread;
    Production productionType;
    bool envMapEnable;
    QStringList args;
    QString output;
};

#endif//__GUI_H__