QT += widgets
QT       += core gui
HEADERS += \
    QTGame.h \
    QTPrivatisation.h \
    graphwidget.h \
    privatisation.h

SOURCES += \
        QTPrivatisation.cpp \
        main.cpp \
        graphwidget.cpp \
        privatisation.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/elasticnodes
INSTALLS += target
