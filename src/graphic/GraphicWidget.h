#pragma once

#include <QWidget>

#include "GraphicAxis.h"

class GraphicWidget : public QWidget {
	Q_OBJECT
public:
    typedef std::list<GraphicLine> lines_t;

public:
    GraphicAxis<x_axis_type_tag> x;
    GraphicAxis<y_axis_type_tag> y;
	QPen axisframepen;
	QBrush background;
	bool on_paint;

private:
	QPointF mpos;
	lines_t lines;
	QVector<QPointF> vpoint;

public:
	GraphicWidget(QWidget* parent = 0) : QWidget(parent), 
        x(), y(), axisframepen(), background(),
        on_paint(),
        mpos(), lines(), vpoint() { clear(); }

	void clear() {
		this->setMouseTracking(true);
		axisframepen = QColor(0,0,0); 
		background = QColor(255,255,255,255);
		on_paint = true;
        mpos_clear();
		x.clear();
		y.clear();
        lines = decltype(lines)();
        vpoint = decltype(vpoint)();
	}

    void addline(const GraphicLine& ln, const limits& exxlim = limits(), const limits& exylim = limits() ) {
        if( valid(ln) ) {
            x.addline(ln, exxlim);
            y.addline(ln, exylim);
            vpoint.reserve(ln.n);
			lines.push_back(ln);
		}
	}

	lines_t& graphic_lines() { return lines; }

    bool zoom(double nZoom);

protected:
	virtual void paintEvent(QPaintEvent*);
    virtual void mouseDoubleClickEvent(QMouseEvent* ev);
    virtual void wheelEvent(QWheelEvent* ev);
	virtual void mouseMoveEvent(QMouseEvent* ev);
    virtual void mouseReleaseEvent(QMouseEvent* ev);

private:
    void mpos_clear() { mpos = decltype(mpos)(); }
};

