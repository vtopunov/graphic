/********************************************************************************
** Form generated from reading UI file 'graphic.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHIC_H
#define UI_GRAPHIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "graphicwidget.h"

QT_BEGIN_NAMESPACE

class Ui_graphicClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *plot;
    QSpacerItem *horizontalSpacer;
    GraphicWidget *graphic;

    void setupUi(QWidget *graphicClass)
    {
        if (graphicClass->objectName().isEmpty())
            graphicClass->setObjectName(QStringLiteral("graphicClass"));
        graphicClass->resize(600, 400);
        verticalLayout = new QVBoxLayout(graphicClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        plot = new QPushButton(graphicClass);
        plot->setObjectName(QStringLiteral("plot"));

        horizontalLayout->addWidget(plot);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        graphic = new GraphicWidget(graphicClass);
        graphic->setObjectName(QStringLiteral("graphic"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphic->sizePolicy().hasHeightForWidth());
        graphic->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(graphic);


        retranslateUi(graphicClass);
        QObject::connect(plot, SIGNAL(clicked()), graphicClass, SLOT(plot_clicked()));

        QMetaObject::connectSlotsByName(graphicClass);
    } // setupUi

    void retranslateUi(QWidget *graphicClass)
    {
        graphicClass->setWindowTitle(QApplication::translate("graphicClass", "graphic", 0));
        plot->setText(QApplication::translate("graphicClass", "\320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321\214", 0));
    } // retranslateUi

};

namespace Ui {
    class graphicClass: public Ui_graphicClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHIC_H
