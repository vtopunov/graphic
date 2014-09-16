#include"GraphicWidget.h"

bool GraphicWidget::zoom(double nZoom) {
    auto xreal = x.real();
    if( x.scale(nZoom) ) {
        if( y.scale(nZoom) ) {
            this->repaint();
            return true;
        }

        x.real(xreal);
    }

    return false;
}
