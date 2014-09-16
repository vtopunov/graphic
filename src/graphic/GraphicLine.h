#pragma once

#include<QtGui>

struct GraphicLine {
public:
	QPen linepen;
    const double* x;
    const double* y;
    int n;

    GraphicLine() : linepen(QColor(0, 0, 255)), x(0), y(0), n(0) {}
};

inline bool valid(const GraphicLine& ln) { return ln.x && ln.y && ln.n > 0; }

