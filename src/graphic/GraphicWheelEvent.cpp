#include "GraphicWidget.h"

void GraphicWidget::wheelEvent(QWheelEvent* ev)
{
    enum{ min_zDelta = 120 };
    const double nZoom = static_cast<double>(ev->delta()) / min_zDelta ;
    zoom(nZoom);
}
