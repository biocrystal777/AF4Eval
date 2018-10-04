#/-------------------------------------------------
#
# AF4 Evaluation software
#
#  \author    Benedikt Häusele, (Emre Brooks)
#  \version   1.0
#  \date      2018
#  \copyright CC CC BY-NC-ND 4.0
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = AF4Eval
TEMPLATE = app
DESTDIR = out
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = Resources/qrc_src

#%%%% Add openmp paralelization
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
CONFIG += c++14

#QMAKE_CXXFLAGS += -fno-exceptions
#QMAKE_LFLAGS += -fopenmp

linux {
QWTDIR  = /usr/include/qwt
GSLDIR = /usr/lib
}

win32|win64 {
QWTDIR  = C:/Qwt-6.1.2
}

linux {
include ( /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qwt.prf )
}

INCLUDEPATH += ./src/Gui \
               ./src/Gui/Calibration \
               ./src/Gui/Deconvolution \
               ./src/Gui/smallQDerivates \
               ./src/Core \
               $${QWTDIR}


DEPENDPATH += ./src/Gui \
              ./src/Gui/Calibration \
              ./src/Gui/Deconvolution \
              ./src/Gui/smallQDerivates \
              ./src/Core

SOURCES += ./src/Core/main.cpp \
    ./src/Core/af4calibrator.cpp \
    ./src/Core/af4calculator.cpp \
    ./src/Core/af4csvparser.cpp \
    ./src/Core/af4csvwriter.cpp \
    ./src/Core/af4diffevaluator.cpp \
    ./src/Core/af4slsevaluator.cpp \
    ./src/Gui/af4mainwindow.cpp \
    ./src/Gui/Calibration/af4calibsettingsframe.cpp \
    ./src/Gui/Calibration/af4stokeseinsteincalculatorwidget.cpp \
    ./src/Gui/Calibration/af4channelconfigurationwidget.cpp \
    ./src/Gui/Calibration/af4channeldimswidget.cpp \
    ./src/Gui/Calibration/af4channelcalibwidget.cpp \
    ./src/Gui/Calibration/af4signalplot.cpp \
    ./src/Gui/DiffEvaluation/af4diffevaluationwidget.cpp \
    ./src/Gui/DiffEvaluation/af4expsettingsframe.cpp \
    ./src/Gui/GeneralWidgets/af4log.cpp \
    ./src/Gui/GeneralWidgets/af4fileinoutwidget.cpp \
    ./src/Gui/smallQDerivates/af4scinotspinbox.cpp \
    ./src/Gui/LightScattering/af4assigndatadialog.cpp \
    ./src/Gui/LightScattering/af4peakparameterframe.cpp \
    ./src/Gui/LightScattering/af4slscalibrationframe.cpp \
    ./src/Gui/LightScattering/af4slsevaluationwidget.cpp \
    ./src/Gui/LightScattering/af4slsparameterframe.cpp \
    ./src/Gui/smallQDerivates/af4numberedcheckbox.cpp \
    ./src/Gui/smallQDerivates/af4numberedcombobox.cpp \
    ./src/Gui/smallQDerivates/af4numberedtoolbutton.cpp \
    src/Gui/Calibration/af4calibplotwidget.cpp \
    src/Gui/Calibration/af4calibrationdialogs.cpp

HEADERS  += ./src/Core/af4parameterstructs.h \
    ./src/Core/af4constants.h \
    ./src/Core/af4calibrator.h \
    ./src/Core/af4calculator.h \
    ./src/Core/af4csvparser.h \
    ./src/Core/af4csvwriter.h \
    ./src/Core/af4datatypes.h \
    ./src/Core/af4diffevaluator.h \
    ./src/Core/af4slsevaluator.h \
    ./src/Core/omp_qvector.h \
    ./src/Gui/af4mainwindow.h \
    ./src/Gui/Calibration/af4calibsettingsframe.h \    
    ./src/Gui/Calibration/af4channelcalibwidget.h \
    ./src/Gui/Calibration/af4channeldimswidget.h \
    ./src/Gui/Calibration/af4channelconfigurationwidget.h \
    ./src/Gui/Calibration/af4stokeseinsteincalculatorwidget.h \
    ./src/Gui/Calibration/af4signalplot.h \
    ./src/Gui/DiffEvaluation/af4diffevaluationwidget.h \
    ./src/Gui/DiffEvaluation/af4expsettingsframe.h \
    ./src/Gui/GeneralWidgets/af4log.h \
    ./src/Gui/GeneralWidgets/af4fileinoutwidget.h \
    ./src/Gui/LightScattering/af4assigndatadialog.h \
    ./src/Gui/LightScattering/af4peakparameterframe.h \
    ./src/Gui/LightScattering/af4slscalibrationframe.h \
    ./src/Gui/LightScattering/af4slsevaluationwidget.h \    
    ./src/Gui/LightScattering/af4slsparameterframe.h \
    ./src/Gui/smallQDerivates/af4scinotspinbox.h \
    ./src/Gui/smallQDerivates/af4numberedcheckbox.h \
    ./src/Gui/smallQDerivates/af4numberedcombobox.h \
    ./src/Gui/smallQDerivates/af4numberedtoolbutton.h \
    src/Gui/Calibration/af4calibplotwidget.h \
    src/Gui/Calibration/af4calibrationdialogs.h

RESOURCES += \
    ./Resources/images.qrc
