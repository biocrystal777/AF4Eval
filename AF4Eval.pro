#-------------------------------------------------
#
# AF4 Evaluation software
#
#  \author    Benedikt Häusele, Emre Brooks
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
    ./src/Gui/fffmainwindow.cpp \
    ./src/Gui/Calibration/af4calibsettingsframe.cpp \
    ./src/Gui/Calibration/af4stokeseinsteincalculatorwidget.cpp \
    ./src/Gui/Calibration/af4channelconfigurationwidget.cpp \
    ./src/Gui/Calibration/af4channeldimswidget.cpp \
    ./src/Gui/Calibration/af4channelcalibwidget.cpp \
    ./src/Gui/Calibration/af4calibplotwidget.cpp \
    ./src/Gui/DiffEvaluation/fffdiffevaluationwidget.cpp \
    ./src/Gui/DiffEvaluation/fffexpsettingsframe.cpp \ 
    ./src/Gui/GeneralWidgets/ffflog.cpp \
    ./src/Gui/GeneralWidgets/ffffileinoutwidget.cpp \
    ./src/Gui/GeneralWidgets/fffmultiplefilewidget.cpp \
    ./src/Gui/smallQDerivates/ffftwoboxwidget.cpp \
    ./src/Gui/LightScattering/fffpeakparameterframe.cpp \
    ./src/Gui/LightScattering/fffslscalibrationframe.cpp \
    ./src/Gui/LightScattering/fffslsevaluationwidget.cpp \
    ./src/Gui/LightScattering/fffassigndatadialog.cpp \
    ./src/Gui/LightScattering/fffslsparameterframe.cpp \
    ./src/Gui/smallQDerivates/fffnumberedcheckbox.cpp \
    ./src/Gui/smallQDerivates/fffnumberedcombobox.cpp \
    ./src/Gui/smallQDerivates/fffnumberedtoolbutton.cpp

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
    ./src/Gui/fffmainwindow.h \
    ./src/Gui/Calibration/af4calibsettingsframe.h \
    ./src/Gui/Calibration/af4calibplotwidget.h \
    ./src/Gui/Calibration/af4channelcalibwidget.h \
    ./src/Gui/Calibration/af4channeldimswidget.h \
    ./src/Gui/Calibration/af4channelconfigurationwidget.h \
    ./src/Gui/Calibration/af4stokeseinsteincalculatorwidget.h \
    ./src/Gui/DiffEvaluation/fffdiffevaluationwidget.h \
    ./src/Gui/DiffEvaluation/fffexpsettingsframe.h \
    ./src/Gui/GeneralWidgets/ffflog.h \
    ./src/Gui/GeneralWidgets/ffffileinoutwidget.h \
    ./src/Gui/GeneralWidgets/fffmultiplefilewidget.h \
    ./src/Gui/LightScattering/fffassigndatadialog.h \
    ./src/Gui/LightScattering/fffslscalibrationframe.h \
    ./src/Gui/LightScattering/fffslsevaluationwidget.h \
    ./src/Gui/LightScattering/fffpeakparameterframe.h \
    ./src/Gui/LightScattering/posPeakPair.h \
    ./src/Gui/LightScattering/fffslsparameterframe.h \
    ./src/Gui/smallQDerivates/ffftwoboxwidget.h \
    ./src/Gui/smallQDerivates/fffnumberedcombobox.h \
    ./src/Gui/smallQDerivates/fffnumberedcheckbox.h \
    ./src/Gui/smallQDerivates/fffnumberedtoolbutton.h


RESOURCES += \
    ./Resources/images.qrc
