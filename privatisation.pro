QT += widgets
QT       += core gui
HEADERS += \
    QTPrivatisation.h \
    QTPrivatisationGame.h \
    graphwidget.h \
    privatisation.h

SOURCES += \
        QTPrivatiosationGame.cpp \
        QTPrivatisation.cpp \
        main.cpp \
        graphwidget.cpp \
        privatisation.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/elasticnodes
INSTALLS += target
