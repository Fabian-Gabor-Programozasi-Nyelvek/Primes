TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS +=  -fopenmp
QMAKE_CFLAGS_DEBUG += -fopenmp
QMAKE_CFLAGS_RELEASE += -fopenmp
LIBS += -fopenmp

SOURCES += \
        main.c
