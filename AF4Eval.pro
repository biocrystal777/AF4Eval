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

SOURCES += ./src/Core/main.cpp\
    ./src/Core/fffcalibrator.cpp \
    ./src/Core/fffcalculator.cpp \
    ./src/Core/fffcsvwriter.cpp \
    ./src/Core/fffslsevaluator.cpp \
    ./src/Core/fffdiffevaluator.cpp \
    ./src/Gui/fffmainwindow.cpp \
    ./src/Core/fffcsvparser.cpp \
    ./src/Gui/DiffEvaluation/fffdiffevaluationwidget.cpp \
    ./src/Gui/DiffEvaluation/fffexpsettingsframe.cpp \
    ./src/Gui/Calibration/fffdiffcoeffcalcwidget.cpp \
    ./src/Gui/Calibration/fffchannelcalconfwidget.cpp \
    ./src/Gui/Calibration/fffchannelconfigwidget.cpp \
    ./src/Gui/Calibration/fffchannelcalibwidget.cpp \
    ./src/Gui/Calibration/fffcalibsettingsframe.cpp \
    ./src/Gui/Calibration/fffcalibplotwidget.cpp \
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


HEADERS  += ./src/Core/fffparameterstructs.h \
    ./src/Core/fffconstants.h \
    ./src/Core/fffcalibrator.h  \
    ./src/Core/fffcalculator.h \
    ./src/Core/fffcsvwriter.h \
    ./src/Core/fffslsevaluator.h \
    ./src/Core/fffdiffevaluator.h \
    ./src/Gui/fffmainwindow.h \
    ./src/Gui/DiffEvaluation/fffdiffevaluationwidget.h \
    ./src/Gui/DiffEvaluation/fffexpsettingsframe.h \
    ./src/Gui/Calibration/fffchannelcalconfwidget.h \
    ./src/Gui/Calibration/fffchannelconfigwidget.h \
    ./src/Gui/Calibration/fffchannelcalibwidget.h \
    ./src/Gui/Calibration/fffcalibsettingsframe.h \
    ./src/Gui/Calibration/fffcalibplotwidget.h \
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
    ./src/Gui/smallQDerivates/fffnumberedtoolbutton.h \
    ./src/Gui/Calibration/fffdiffcoeffcalcwidget.h \
    ./src/Core/fffcsvparser.h \
    ./src/Core/omp_qvector.h \
    ./src/Core/fffdatatypes.h

RESOURCES += \
    ./Resources/images.qrc
