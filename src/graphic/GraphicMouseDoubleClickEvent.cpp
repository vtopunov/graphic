#include"GraphicWidget.h"

void GraphicWidget::mouseDoubleClickEvent(QMouseEvent* ev) {
#if QT_VERSION >= 0x050000
    QPointF pos( ev->localPos() );
#else
    QPointF pos( ev->posF() );
#endif

    if( x.window().in( pos.x() ) && y.window().in( pos.y() ) ) {
        x.restore_limits();
        y.restore_limits();
        this->repaint();
    }
}
