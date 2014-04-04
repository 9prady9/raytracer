/********************************************************************************
** Form generated from reading UI file 'viewer.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWER_H
#define UI_VIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "canvas.h"

QT_BEGIN_NAMESPACE

class Ui_RootGUI
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_6;
    QTabWidget *mTabGrp;
    QWidget *Loader;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *mSceneFileLoc;
    QPushButton *mSceneFileButton;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *mObjFileLoc;
    QPushButton *mObjFileButton;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *mEffectLabel;
    QComboBox *mEffectComboBox;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *mEvnMapLabel;
    QComboBox *mEnvMapComboBox;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *mLoadInputsButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *mRenderButton;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_9;
    Canvas *Viewer;
    QWidget *SceneDesc;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *mSceneTree;
    QWidget *ObjectList;
    QVBoxLayout *verticalLayout_4;
    QTreeWidget *mObjTree;
    QProgressBar *mProgressBar;

    void setupUi(QWidget *RootGUI)
    {
        if (RootGUI->objectName().isEmpty())
            RootGUI->setObjectName(QStringLiteral("RootGUI"));
        RootGUI->resize(828, 705);
        QIcon icon;
        icon.addFile(QStringLiteral(":/raytracer/Resources/Icon.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        RootGUI->setWindowIcon(icon);
        gridLayout = new QGridLayout(RootGUI);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        mTabGrp = new QTabWidget(RootGUI);
        mTabGrp->setObjectName(QStringLiteral("mTabGrp"));
        Loader = new QWidget();
        Loader->setObjectName(QStringLiteral("Loader"));
        verticalLayout_2 = new QVBoxLayout(Loader);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mSceneFileLoc = new QLineEdit(Loader);
        mSceneFileLoc->setObjectName(QStringLiteral("mSceneFileLoc"));

        horizontalLayout->addWidget(mSceneFileLoc);

        mSceneFileButton = new QPushButton(Loader);
        mSceneFileButton->setObjectName(QStringLiteral("mSceneFileButton"));
        mSceneFileButton->setMinimumSize(QSize(151, 23));
        mSceneFileButton->setMaximumSize(QSize(151, 23));

        horizontalLayout->addWidget(mSceneFileButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        mObjFileLoc = new QLineEdit(Loader);
        mObjFileLoc->setObjectName(QStringLiteral("mObjFileLoc"));

        horizontalLayout_2->addWidget(mObjFileLoc);

        mObjFileButton = new QPushButton(Loader);
        mObjFileButton->setObjectName(QStringLiteral("mObjFileButton"));
        mObjFileButton->setMinimumSize(QSize(151, 23));
        mObjFileButton->setMaximumSize(QSize(151, 23));

        horizontalLayout_2->addWidget(mObjFileButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        mEffectLabel = new QLabel(Loader);
        mEffectLabel->setObjectName(QStringLiteral("mEffectLabel"));
        mEffectLabel->setMinimumSize(QSize(151, 16));
        mEffectLabel->setMaximumSize(QSize(151, 16));

        horizontalLayout_3->addWidget(mEffectLabel);

        mEffectComboBox = new QComboBox(Loader);
        mEffectComboBox->setObjectName(QStringLiteral("mEffectComboBox"));
        mEffectComboBox->setMinimumSize(QSize(151, 22));
        mEffectComboBox->setMaximumSize(QSize(151, 22));

        horizontalLayout_3->addWidget(mEffectComboBox);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        mEvnMapLabel = new QLabel(Loader);
        mEvnMapLabel->setObjectName(QStringLiteral("mEvnMapLabel"));
        mEvnMapLabel->setMinimumSize(QSize(151, 16));
        mEvnMapLabel->setMaximumSize(QSize(151, 16));

        horizontalLayout_4->addWidget(mEvnMapLabel);

        mEnvMapComboBox = new QComboBox(Loader);
        mEnvMapComboBox->setObjectName(QStringLiteral("mEnvMapComboBox"));
        mEnvMapComboBox->setMinimumSize(QSize(151, 22));
        mEnvMapComboBox->setMaximumSize(QSize(151, 22));

        horizontalLayout_4->addWidget(mEnvMapComboBox);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_8->addLayout(verticalLayout);

        horizontalSpacer_5 = new QSpacerItem(308, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_8);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 0, 0, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_7, 0, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(308, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_8, 1, 0, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(308, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_10, 1, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_5, 2, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_6, 2, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        mLoadInputsButton = new QPushButton(Loader);
        mLoadInputsButton->setObjectName(QStringLiteral("mLoadInputsButton"));
        mLoadInputsButton->setMinimumSize(QSize(75, 23));
        mLoadInputsButton->setMaximumSize(QSize(75, 23));

        horizontalLayout_5->addWidget(mLoadInputsButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        mRenderButton = new QPushButton(Loader);
        mRenderButton->setObjectName(QStringLiteral("mRenderButton"));
        mRenderButton->setMinimumSize(QSize(75, 23));
        mRenderButton->setMaximumSize(QSize(75, 23));

        horizontalLayout_5->addWidget(mRenderButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        horizontalLayout_7->addLayout(horizontalLayout_5);

        horizontalSpacer_9 = new QSpacerItem(128, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_9);


        verticalLayout_2->addLayout(horizontalLayout_7);

        mTabGrp->addTab(Loader, QString());
        Viewer = new Canvas();
        Viewer->setObjectName(QStringLiteral("Viewer"));
        mTabGrp->addTab(Viewer, QString());
        SceneDesc = new QWidget();
        SceneDesc->setObjectName(QStringLiteral("SceneDesc"));
        verticalLayout_3 = new QVBoxLayout(SceneDesc);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        mSceneTree = new QTreeWidget(SceneDesc);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(2, QStringLiteral("Value"));
        __qtreewidgetitem->setText(1, QStringLiteral("Attributes"));
        __qtreewidgetitem->setText(0, QStringLiteral("Property Name"));
        mSceneTree->setHeaderItem(__qtreewidgetitem);
        mSceneTree->setObjectName(QStringLiteral("mSceneTree"));
        mSceneTree->setColumnCount(3);

        verticalLayout_3->addWidget(mSceneTree);

        mTabGrp->addTab(SceneDesc, QString());
        ObjectList = new QWidget();
        ObjectList->setObjectName(QStringLiteral("ObjectList"));
        verticalLayout_4 = new QVBoxLayout(ObjectList);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        mObjTree = new QTreeWidget(ObjectList);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("Property Name"));
        mObjTree->setHeaderItem(__qtreewidgetitem1);
        mObjTree->setObjectName(QStringLiteral("mObjTree"));
        mObjTree->setColumnCount(3);

        verticalLayout_4->addWidget(mObjTree);

        mTabGrp->addTab(ObjectList, QString());

        horizontalLayout_6->addWidget(mTabGrp);

        mProgressBar = new QProgressBar(RootGUI);
        mProgressBar->setObjectName(QStringLiteral("mProgressBar"));
        mProgressBar->setMinimumSize(QSize(16, 0));
        mProgressBar->setMaximumSize(QSize(16, 16777215));
        mProgressBar->setValue(0);
        mProgressBar->setOrientation(Qt::Vertical);
        mProgressBar->setInvertedAppearance(false);
        mProgressBar->setTextDirection(QProgressBar::BottomToTop);

        horizontalLayout_6->addWidget(mProgressBar);


        gridLayout->addLayout(horizontalLayout_6, 0, 0, 1, 1);


        retranslateUi(RootGUI);

        mTabGrp->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(RootGUI);
    } // setupUi

    void retranslateUi(QWidget *RootGUI)
    {
        RootGUI->setWindowTitle(QApplication::translate("RootGUI", "Form", 0));
        mSceneFileLoc->setText(QString());
        mSceneFileButton->setText(QApplication::translate("RootGUI", "Select scene properties XML", 0));
        mObjFileLoc->setText(QString());
        mObjFileButton->setText(QApplication::translate("RootGUI", "Select objects XML", 0));
        mEffectLabel->setText(QApplication::translate("RootGUI", "Select Rendering Effect", 0));
        mEffectComboBox->clear();
        mEffectComboBox->insertItems(0, QStringList()
         << QApplication::translate("RootGUI", "Simple Ray Trace", 0)
         << QApplication::translate("RootGUI", "Depth of field", 0)
         << QApplication::translate("RootGUI", "Motion Blur effect", 0)
         << QApplication::translate("RootGUI", "Ambient occlusion", 0)
         << QApplication::translate("RootGUI", "Animation", 0)
         << QApplication::translate("RootGUI", "All Effects", 0)
        );
        mEvnMapLabel->setText(QApplication::translate("RootGUI", "Environment map availability", 0));
        mEnvMapComboBox->clear();
        mEnvMapComboBox->insertItems(0, QStringList()
         << QApplication::translate("RootGUI", "disable", 0)
         << QApplication::translate("RootGUI", "enable", 0)
        );
        mLoadInputsButton->setText(QApplication::translate("RootGUI", "Load Inputs", 0));
        mRenderButton->setText(QApplication::translate("RootGUI", "Render", 0));
        mTabGrp->setTabText(mTabGrp->indexOf(Loader), QApplication::translate("RootGUI", "File Loader", 0));
        mTabGrp->setTabText(mTabGrp->indexOf(Viewer), QApplication::translate("RootGUI", "Render Viewer", 0));
        mTabGrp->setTabText(mTabGrp->indexOf(SceneDesc), QApplication::translate("RootGUI", "Scene Description", 0));
        QTreeWidgetItem *___qtreewidgetitem = mObjTree->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("RootGUI", "Value", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("RootGUI", "Attributes", 0));
        mTabGrp->setTabText(mTabGrp->indexOf(ObjectList), QApplication::translate("RootGUI", "Objects", 0));
    } // retranslateUi

};

namespace Ui {
    class RootGUI: public Ui_RootGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
