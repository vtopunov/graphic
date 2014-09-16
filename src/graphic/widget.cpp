#include "widget.h"
#include "ui_graphic.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::graphicClass),
    x(), y()
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::plot_clicked()
{
    enum{ n_point = 3000 };
    ui->graphic->clear();
    x.clear();
    y.clear();

    x.reserve(n_point);
    y.reserve(n_point);
    for(int i = 0; i < n_point; ++i ) {
        const double x_ = ( 5 * i * M_PI ) / n_point;
        x.push_back( x_ );
        y.push_back( (i) ? sin(x_)/x_ : 1 );
    }


    GraphicLine ln;
    ln.n = n_point;
    ln.x = &x[0];
    ln.y = &y[0];
    ui->graphic->addline(ln);
    ui->graphic->repaint();
}

