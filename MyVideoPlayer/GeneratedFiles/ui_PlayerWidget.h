/********************************************************************************
** Form generated from reading UI file 'PlayerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYERWIDGET_H
#define UI_PLAYERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "slider.h"
#include "videowidget.h"

QT_BEGIN_NAMESPACE

class Ui_PlayerWidget
{
public:
    QGridLayout *gridLayout;
    VideoWidget *videoWidget;
    QListWidget *listWidget;
    QFrame *toolFrame;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    Slider *positionSlider;
    QPushButton *pushButton;
    Slider *volumeSlider;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *positionLabel;
    QLabel *durationLabel;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *searchComboBox;
    QPushButton *searchButton;
    QSpacerItem *horizontalSpacer_4;
    QCheckBox *cb_isCachedPlay;

    void setupUi(QWidget *PlayerWidget)
    {
        if (PlayerWidget->objectName().isEmpty())
            PlayerWidget->setObjectName(QStringLiteral("PlayerWidget"));
        PlayerWidget->resize(821, 507);
        gridLayout = new QGridLayout(PlayerWidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        videoWidget = new VideoWidget(PlayerWidget);
        videoWidget->setObjectName(QStringLiteral("videoWidget"));

        gridLayout->addWidget(videoWidget, 0, 0, 1, 1);

        listWidget = new QListWidget(PlayerWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setMinimumSize(QSize(200, 0));
        listWidget->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(listWidget, 0, 1, 2, 1);

        toolFrame = new QFrame(PlayerWidget);
        toolFrame->setObjectName(QStringLiteral("toolFrame"));
        toolFrame->setMinimumSize(QSize(0, 60));
        toolFrame->setMaximumSize(QSize(16777215, 60));
        toolFrame->setFrameShape(QFrame::StyledPanel);
        toolFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(toolFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        positionSlider = new Slider(toolFrame);
        positionSlider->setObjectName(QStringLiteral("positionSlider"));
        positionSlider->setMinimumSize(QSize(0, 12));
        positionSlider->setMaximumSize(QSize(16777215, 12));
        positionSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(positionSlider);

        pushButton = new QPushButton(toolFrame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(20, 12));
        pushButton->setMaximumSize(QSize(20, 12));

        horizontalLayout->addWidget(pushButton);

        volumeSlider = new Slider(toolFrame);
        volumeSlider->setObjectName(QStringLiteral("volumeSlider"));
        volumeSlider->setMinimumSize(QSize(80, 12));
        volumeSlider->setMaximumSize(QSize(80, 12));
        volumeSlider->setValue(80);
        volumeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(volumeSlider);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(80, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        positionLabel = new QLabel(toolFrame);
        positionLabel->setObjectName(QStringLiteral("positionLabel"));
        positionLabel->setMinimumSize(QSize(51, 16));
        positionLabel->setMaximumSize(QSize(51, 16));
        positionLabel->setStyleSheet(QStringLiteral("color:rgb(204, 204, 204);"));

        horizontalLayout_2->addWidget(positionLabel);

        durationLabel = new QLabel(toolFrame);
        durationLabel->setObjectName(QStringLiteral("durationLabel"));
        durationLabel->setMinimumSize(QSize(61, 16));
        durationLabel->setMaximumSize(QSize(61, 16));
        durationLabel->setStyleSheet(QStringLiteral("color:rgb(111, 111, 111);"));

        horizontalLayout_2->addWidget(durationLabel);

        horizontalSpacer_2 = new QSpacerItem(60, 0, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        searchComboBox = new QComboBox(toolFrame);
        searchComboBox->addItem(QString());
        searchComboBox->setObjectName(QStringLiteral("searchComboBox"));
        searchComboBox->setMinimumSize(QSize(75, 20));
        searchComboBox->setMaximumSize(QSize(75, 20));

        horizontalLayout_2->addWidget(searchComboBox);

        searchButton = new QPushButton(toolFrame);
        searchButton->setObjectName(QStringLiteral("searchButton"));
        searchButton->setMinimumSize(QSize(45, 22));
        searchButton->setMaximumSize(QSize(45, 22));

        horizontalLayout_2->addWidget(searchButton);

        horizontalSpacer_4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        cb_isCachedPlay = new QCheckBox(toolFrame);
        cb_isCachedPlay->setObjectName(QStringLiteral("cb_isCachedPlay"));
        cb_isCachedPlay->setMinimumSize(QSize(68, 22));
        cb_isCachedPlay->setMaximumSize(QSize(68, 22));
        cb_isCachedPlay->setStyleSheet(QLatin1String("QCheckBox#cb_isCachedPlay:!hover{\n"
"	background-color:rgba(255, 255, 255, 0); \n"
"	color:rgb(170, 170, 170); \n"
"}\n"
"\n"
"QCheckBox#cb_isCachedPlay:hover{\n"
"	background-color:rgba(255, 255, 255, 0); \n"
"	color:rgb(250, 250, 250); \n"
"}"));

        horizontalLayout_2->addWidget(cb_isCachedPlay);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addWidget(toolFrame, 1, 0, 1, 1);


        retranslateUi(PlayerWidget);

        QMetaObject::connectSlotsByName(PlayerWidget);
    } // setupUi

    void retranslateUi(QWidget *PlayerWidget)
    {
        PlayerWidget->setWindowTitle(QApplication::translate("PlayerWidget", "PlayerWidget", nullptr));
        pushButton->setText(QString());
        positionLabel->setText(QApplication::translate("PlayerWidget", "00:00:00", nullptr));
        durationLabel->setText(QApplication::translate("PlayerWidget", "/00:00:00", nullptr));
        searchComboBox->setItemText(0, QApplication::translate("PlayerWidget", "\350\205\276\350\256\257\350\247\206\351\242\221", nullptr));

        searchButton->setText(QApplication::translate("PlayerWidget", "search", nullptr));
        cb_isCachedPlay->setText(QApplication::translate("PlayerWidget", "\350\276\271\344\270\213\350\276\271\346\222\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlayerWidget: public Ui_PlayerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYERWIDGET_H
