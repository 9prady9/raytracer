#ifndef __CANVAS_H__
#define __CANVAS_H__

#include <QImage>
#include <QPainter>
#include <QWidget>

class Canvas: public QWidget
{
	Q_OBJECT
public:
	Canvas(QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~Canvas();
	void updateImage(const QImage &);
	
protected:
	void paintEvent(QPaintEvent* fEvent);

private:
	QImage		mPaintImage;
};

#endif //__CANVAS_H__