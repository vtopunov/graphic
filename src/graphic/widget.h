#pragma once

#include <QWidget>
#include <vector>

namespace Ui {
	class graphicClass;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void plot_clicked();

private:
    Ui::graphicClass *ui;
    std::vector<double> x;
    std::vector<double> y;
};

