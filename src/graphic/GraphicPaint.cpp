#include"GraphicWidget.h"

inline void drawRotatedText(QPainter *painter, qreal degrees, const QRectF& r, const QString &text) {
    painter->save();
    painter->translate(r.x(), r.y());
    painter->rotate(degrees);
    painter->drawText(QRectF(0, 0, r.width(), r.height()), text );
    painter->restore();
}

void GraphicWidget::paintEvent(QPaintEvent*) {
	if( !on_paint ) return;

	QPainter paint(this);

	// 1. Вычисление сетки
	x.prepaint();
	y.prepaint();

	// 2. Вычисление области отображения
	const QRectF rc = geometry();
	const QFontMetricsF xtm(x.font());
	const QFontMetricsF ytm(y.font());
	const QSizeF xsztxt = maxGraphicSizeText(x.text().cbegin(), x.text().cend(), xtm );
	const QSizeF ysztxt = maxGraphicSizeText(y.text().cbegin(), y.text().cend(), ytm );
	const limits winx(ysztxt.width() + ytm.width("-")+5, rc.width() - xsztxt.width()/2);
    const limits winy(xsztxt.height()+17, rc.height()-ysztxt.height()-2*xsztxt.height());
	if( !winx.valid() || !winy.valid() ) return;

	x.window( winx );
	y.window( winy );

	paint.setPen(axisframepen);
	paint.setBrush(background);
	paint.drawRect(winx.min()-1, winy.min()-1, winx.length()+2, winy.length()+2);
    paint.setClipRect(winx.min(), winy.min(), winx.length(), winy.length() );


	// 3. Рисование сетки и надписей

	// 3.1 По оси Y
	paint.setFont( y.font() );
	QRectF oldrc(0, 0, 1, 1);
	for(auto i = y.text().cbegin(); i != y.text().cend(); ++i ) {
		if( y.real().in(i->first) ) {
			const double dlg = y.to_window(i->first);
			paint.setPen( y.gridpen(i->second) );
			paint.setClipping(true);
            paint.drawLine(winx.min(), dlg, winx.max(), dlg);
			
			if (y.showlable_) {
				double cx = ytm.width(i->second);
				QRectF rc(winx.min() - cx - 3, dlg - ysztxt.height() / 2, cx, ysztxt.height());
				paint.setPen(y.textpen_);
				paint.setClipping(false);
				drawRotatedText(&paint, y.textangle_, rc, i->second);
				oldrc = rc;
			}
		}

		if( i->first > y.real().max() ) break;
	}

	// 3.2 По оси X
	paint.setFont( x.font() );
	oldrc.setRect(0, 0, 1, 1);
	for(auto i = x.text().cbegin(); i != x.text().cend(); ++i ) {
		if( x.real().in(i->first) ) {
			const double dlg = x.to_window(i->first);
			paint.setPen( x.gridpen(i->second) );
			paint.setClipping(true);
            paint.drawLine(dlg, winy.min()+1, dlg, winy.max() );
			double cx = xtm.width(i->second);
			QRectF rc(dlg-cx/2, winy.max()+xsztxt.height()/3, cx+3, xsztxt.height() );
            if( std::fabs(x.textangle_) > 0.1 || !rc.intersects(oldrc) ) {
				paint.setPen( x.textpen_ );
				paint.setClipping(false);
				drawRotatedText(&paint, x.textangle_, rc, i->second );
				oldrc = rc;
			}
		} 

		if( i->first > x.real().max() ) break;
	}

	// 4. Рисование линий графиков
	paint.setRenderHint(QPainter::Antialiasing, true );
	paint.setClipping(true);

	QPointF* vpt = vpoint.data();
    for(const auto& lin : lines ) {
        const double* xarray = lin.x;
        const double* yarray = lin.y;
        const int narray = lin.n;
	
		// !! critical runtime
        for(int j = 0; j < narray; ++j ) {
            vpt[j].setX( x.to_window(xarray[j]) );
            vpt[j].setY( y.to_window(yarray[j]) );
        }
        // !! end critical runtime

        paint.setPen(lin.linepen);
        paint.drawPolyline(vpt, narray);
	}
}
