#include"GraphicWidget.h"

void GraphicWidget::mouseMoveEvent(QMouseEvent* ev) {
#if QT_VERSION >= 0x050000
	QPointF pos( ev->localPos() );
#else
	QPointF pos( ev->posF() );
#endif

	if( ev->buttons() == Qt::LeftButton  ) {
		if( mpos != QPointF() ) {
			QPointF dxy = mpos - pos;
			x.move(dxy.x());
			y.move(dxy.y());
			this->repaint();
		}

		mpos = pos;
	} else {
        mpos_clear();
	}
}

void GraphicWidget::mouseReleaseEvent(QMouseEvent* ) {
    mpos_clear();
}
