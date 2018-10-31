#include "af4channelconfigurationwidget.h"

using std::string;

AF4ChannelConfigurationWidget::AF4ChannelConfigurationWidget(QWidget *parent) :
   QWidget(parent), // settingsWriter (QSharedPointer<QPushButton>(new QPushButton("Save Parameters", this)))
   settingsWriter(new QPushButton("Save Parameters", this))
{
   layout = new QGridLayout(this);
   channelDimsOrgFrame = new AF4ChannelDimsOrgFrame(this);
   //qDebug() << "master1";
   calibsOrgFrame = new AF4CalibOrgFrame(channelDimsOrgFrame->getChannelSelection(),
                                         channelDimsOrgFrame->getChannelConfigWidgets(),
                                         this);
   //qDebug() << "master2";
   connect(channelDimsOrgFrame, &AF4ChannelDimsOrgFrame::configWidgetIdsAdapted,
           calibsOrgFrame,      &AF4CalibOrgFrame::adaptAllCalibWidgetIds);
   connect(channelDimsOrgFrame, &AF4ChannelDimsOrgFrame::configWidgetNamesAdapted,
           calibsOrgFrame,      &AF4CalibOrgFrame::adaptAllCalibWidgetNames);
   connect(channelDimsOrgFrame, &AF4ChannelDimsOrgFrame::channelAdded,        // ...(const QString oldName, const QString newName)
           calibsOrgFrame,      &AF4CalibOrgFrame::addConnectedChannel);
   connect(channelDimsOrgFrame, &AF4ChannelDimsOrgFrame::channelRenamed,      // ...(const QString oldName, const QString newName)
           calibsOrgFrame,      &AF4CalibOrgFrame::renameConnectedChannel);
   connect(channelDimsOrgFrame, &AF4ChannelDimsOrgFrame::channelDeleted,      // ...(const QString oldName, const QString newCurrentName)
           calibsOrgFrame,      &AF4CalibOrgFrame::deleteConnectedChannel);
   connect(channelDimsOrgFrame, &AF4ChannelDimsOrgFrame::channelSwitched,     // ...(const QString channelName)
           calibsOrgFrame,      &AF4CalibOrgFrame::switchToFirstCalibWidget);
   //qDebug() << "master5";
   layout->addWidget(channelDimsOrgFrame, 0, 0, 2, 10);
   layout->addWidget(calibsOrgFrame, 3, 0, 12, 10);

   //connect(settingsWriter.data(), &QPushButton::clicked,
   //        this,                  &AF4ChannelConfigurationWidget::saveParameters);
   connect(settingsWriter, &QPushButton::clicked,
           channelDimsOrgFrame,   &AF4ChannelDimsOrgFrame::saveButtonClicked);
   connect(settingsWriter, &QPushButton::clicked,
           calibsOrgFrame,        &AF4CalibOrgFrame::saveButtonClicked);
   //qDebug() << "master8";
   layout->addWidget(settingsWriter, 15, 0);
   //qDebug() << "master10";
}


auto AF4ChannelConfigurationWidget::getChannelConfigWidgets() const
-> QSharedPointer<QMap<QString, AF4ChannelDimsWidget *> >
{
   return channelDimsOrgFrame->getChannelConfigWidgets();
}

auto AF4ChannelConfigurationWidget::getChannelCalibWidgets() const
-> QSharedPointer<QMap<QString, QMap<QString, AF4ChannelCalibWidget *> > >
{
   return calibsOrgFrame->getChannelCalibWidgets();
}

AF4ChannelConfigurationWidget::~AF4ChannelConfigurationWidget()
{
   //writeSettings();
}




void AF4ChannelConfigurationWidget::calibrateChannnel()
{
   AF4Log::logText(tr("Start calibration..."));
   QGuiApplication::setOverrideCursor(Qt::WaitCursor);
   //ChannelDims chDims  = currentChConfigWidget->getChannelDimensions();
   ChannelDims chDims = channelDimsOrgFrame->getChannelDimensions();
   ParametersForCalibration params = calibsOrgFrame->getParamsForCalibration();
   CalibModeSettings cModes = calibsOrgFrame->getCalibModes();

   calibRealMeaurement(chDims, params, cModes);
   if(cModes.checkUncertainty){
      calibUncertaintyGrid(chDims, params, cModes);
      AF4Log::logText(tr(""));
   }
   QGuiApplication::restoreOverrideCursor();
}

void AF4ChannelConfigurationWidget::calibRealMeaurement(const ChannelDims &chDims, const ParametersForCalibration &params, const CalibModeSettings &cModes)
{
   AF4Log::logText(tr("Calibrate with measured values..."));
   CalibResult result;
   if(cModes.classical){
      AF4Log::logText(tr("Try \"Classical\" calibration..."));
      result = calibSingleParamSet(chDims, params, CalibMode::classical);
      if(result.errorCode == CalibErrorCode::noError){
         AF4Log::logText(tr("\"Classical\" calibration Finished. w_cla set to %1, V_cla set to %2.").arg(result.width).arg(result.volume));
         //currentCalibWidget->setChannelWidthClassical(result.width);
         //currentCalibWidget->setClassicalVolume(result.volume);
      }
      else      {
         AF4Log::logError(tr("\"Classical\" calibration could not be conducted."));
         logErrorMessage(result.errorCode);
      }
   }
   if(cModes.geometric){
      AF4Log::logText(tr("Try \"Geometric\" calibration..."));
      result = calibSingleParamSet(chDims, params, CalibMode::geometric);
      if(result.errorCode == CalibErrorCode::noError){
         AF4Log::logText(tr("\"Geometric\" calibration finished. w_geo set to %1, V_geo set to %2.").arg(result.width).arg(result.volume));
         //currentCalibWidget->setChannelWidthGeo(result.width);
         //currentCalibWidget->setGeometVolume(result.volume);
      }
      else {
         AF4Log::logError(tr("\"Geometric\" calibration could not be conducted."));
         logErrorMessage(result.errorCode);
      }
   }
   if(cModes.hydrodynamic){
      AF4Log::logText(tr("Try \"Hydrodynamic\" calibration..."));
      result = calibSingleParamSet(chDims, params, CalibMode::hydrodynamic);
      if(result.errorCode == CalibErrorCode::noError){
         AF4Log::logText(tr("\"Hydrodynamic\" calibration Finished. w_hyd set to %1, V_hyd set to %2.").arg(result.width).arg(result.volume));
         //currentCalibWidget->setChannelWidthHydro(result.width);
         //currentCalibWidget->setChannelWidthGeo(result.volume);
      }
      else {
         AF4Log::logError(tr("\"Hydrodynamic\" calibration could not be conducted."));
         logErrorMessage(result.errorCode);
      }
   }
}



CalibResult AF4ChannelConfigurationWidget::calibSingleParamSet(ChannelDims chDims, ParametersForCalibration params, CalibMode mode)
{
   AF4Calibrator calibrator(chDims, params);
   CalibResult result;
   result.errorCode = calibrator.checkParameters();
   if(result.errorCode != CalibErrorCode::noError) return result;

   switch (mode) {
   case CalibMode::classical:
      result = calibrator.calibrate_classic();
      break;
   case CalibMode::hydrodynamic:
      result = calibrator.calibrate_hydrodynamic();
      break;
   case CalibMode::geometric:
      result = calibrator.calibrate_geometric();
      break;
   }
  return result;
}



void AF4ChannelConfigurationWidget::calibUncertaintyGrid(const ChannelDims &chDims, const ParametersForCalibration &params, const CalibModeSettings &cModes)
{

   //-////////////////////////////////////////
   // non-const parameter structures for
   // Macro-based parameter modifications
   //-////////////////////////////////////////

   struct ChannelDims_nonConst {
      double length1;
      double length2;
      double length3;
      double chLength;
      double b0;
      double bL;
   };
   struct ParamsForCalibration_nonConst {
      double elutionFlow;
      double crossFlow;
      double relFocusPoint;
      //double leftOffsetTime;
      double voidPeakTime;
      double elutionTime;
      double diffCoeff;
   };

   //-/////////////////////////////////////////////////////////////////
   //
   // converters for const-to-nonConst containers and vice versa
   //
   //-/////////////////////////////////////////////////////////////////

   auto dimsToNonConst = [](const ChannelDims &constDims)
         -> ChannelDims_nonConst {
      return ChannelDims_nonConst
      {        constDims.length1,
               constDims.length2,
               constDims.length3,
               constDims.chLength,
               constDims.b0,
               constDims.bL
      };
   };
   auto dimsToConst = [](const ChannelDims_nonConst &nonConstDims)
         -> ChannelDims {
      return ChannelDims
      {        nonConstDims.length1,
               nonConstDims.length2,
               nonConstDims.length3,
               nonConstDims.chLength,
               nonConstDims.b0,
               nonConstDims.bL
      };
   };
   auto paramsToNonConst = [](const ParametersForCalibration &nonConstParams)
         -> ParamsForCalibration_nonConst {
      return ParamsForCalibration_nonConst
      {        nonConstParams.elutionFlow,
               nonConstParams.crossFlow,
               nonConstParams.relFocusPoint,
               //nonConstParams.leftOffsetTime,
               nonConstParams.voidPeakTime,
               nonConstParams.elutionTime,
               nonConstParams.diffCoeff
      };
   };
   auto paramsToConst = [](const ParamsForCalibration_nonConst &constParams)
         ->ParametersForCalibration {
      return  ParametersForCalibration
      {        constParams.elutionFlow,
               constParams.crossFlow,
               constParams.relFocusPoint,
               //constParams.leftOffsetTime,
               constParams.voidPeakTime,
               constParams.elutionTime,
               constParams.diffCoeff
      };
   };

   //-/////////////////////////////////////////////////////////////////////////////
   //
   // Start of actual function
   // void .....::  calibUncertaintyGrid(const ChannelDims &chDims,
   //                                    const ParametersForCalibration &params,
   //                                    const CalibModeSettings &cModes)
   //
   //-//////////////////////////////////////////////////////////////////////////////

   const double deltaMax = cModes.uncertRange / 100.0; // from "%" to factor
   //const double deltaMin = - deltaMax;
   const uint   gridMidPos = cModes.uncertGridSize;
   const uint   gridSize(2 * gridMidPos + 1);
   vecD devXRel;                 // relative deviating sizes within the defined range [X - X⋅δmax, X + X⋅δmax]
   devXRel.resize(gridSize);
   vecD deltaWidth;              // relative deviations of channel volume from  ( (Y(X ± δX) / Y(X)) - 1 )
   deltaWidth.resize(gridSize);
   vecD deltaVolume;             // relative deviations of channel width from  ( (Y(X ± δX) / Y(X)) - 1 )
   deltaVolume.resize(gridSize);

/*
 * DELTAMAGN         = size in one of the containers
 * CONSTCONTAINER    = conatainer of DELTAMAGN (params or chDims)
 * NONCONSTCONTAINER = corresponding modifyable version ()
 * CALIBMODE         = one of the three CalibModes (classical, geometric or hydrodynamic)
 * FILENAMESUFF      = "_*.csv"
 *
*/
#define ITERATIVE_PARAMETER_DELTA_ANALYSIS(DELTAMAGN, CONSTCONTAINER, NONCONSTCONTAINER, CALIBMODE , FILENAMESUFFIX) \
   {                                                                           \
   const double X = CONSTCONTAINER . DELTAMAGN;                                \
   for(uint i = 0; i < gridSize; ++i){                                         \
   /* create modified parameter structures, for both containers,               \
    *  so no macro parameter required */                                       \
   ParamsForCalibration_nonConst paramsDeltaMod = paramsToNonConst(params);    \
   ChannelDims_nonConst chDimsDeltaMod  = dimsToNonConst(chDims);              \
   /* get the modifier between δmin to δmax */                                \
   devXRel[i] =  static_cast<double>(i) - static_cast<double>(gridMidPos);    \
   devXRel[i] *= deltaMax / static_cast<double>(gridMidPos);                  \
   /* modify δsize parameter to X ± δX, specific in NONCONSTCONTAINER REQUIRED */ \
   NONCONSTCONTAINER.DELTAMAGN = (1.0 + devXRel[i]) * X;                      \
   /* conduct calibration */                                                  \
   CalibResult deltaResult = calibSingleParamSet(dimsToConst(chDimsDeltaMod), \
   paramsToConst(paramsDeltaMod),                                             \
   CalibMode::CALIBMODE);                                                     \
   deltaWidth[i]  = (deltaResult.width  / widthRefY  ) - 1.0;                 \
   deltaVolume[i] = (deltaResult.volume / VolumeRefY ) - 1.0;                 \
   AF4Log::logText(tr("Tested for modified %1 = %2 , with δX = %3\%")         \
                   .arg(#DELTAMAGN)                                           \
                   .arg(paramsDeltaMod.DELTAMAGN)                             \
                   .arg(devXRel[i] * 100.0));                                 \
}                                                                             \
/* write results */                                                           \
string outFile = filePathPrefix + string(FILENAMESUFFIX);                     \
AF4Log::logText(tr("Write result to %1").arg(outFile.c_str()));               \
AF4CsvWriter csvWriter(outFile);                                              \
csvWriter.writeFile(matD{devXRel, deltaWidth, deltaVolume} , header);         \
};

/*----------------------------------------------------------------------------------------------------------------
// Expanded example for Macro with arguments (elutionFlow, params, paramsDeltaMod, classical,"_elutionFlow.csv")
// ==> Iterates over a linearly distributed of the parameter elutionFlow and conducts the calibration with
// the respective values
{
   const double X = params.elutionFlow;
   for(uint i = 0; i < gridSize; ++i){
      // create modified parameter structures:
      ParamsForCalibration_nonConst paramsDeltaMod = paramsToNonConst(params);
      ChannelDims_nonConst chDimsDeltaMod  = dimsToNonConst(chDims);
      // get the modifier between δmin to δmax
      devXRel[i] =  static_cast<double>(i) - static_cast<double>(gridMidPos);
      devXRel[i] *= deltaMax / static_cast<double>(gridMidPos);
      modify δsize parameter to X ± δX
      paramsDeltaMod.elutionFlow = (1.0 + devXRel[i]) * X;
      // conduct calibration
      CalibResult deltaResult = calibSingleParamSet(dimsToConst(chDimsDeltaMod),
                                                    paramsToConst(paramsDeltaMod),
                                                    CalibMode::classical);
      deltaWidth[i]  = (deltaResult.width  / widthRefY  ) - 1.0;
      deltaVolume[i] = (deltaResult.volume / VolumeRefY ) - 1.0;
      AF4Log::logText(tr("Tested for modified %1 = %2 , with δX = %3\%")
                      .arg("elutionFlow")
                      .arg(paramsDeltaMod.elutionFlow)
                      .arg(devXRel[i] * 100.0));
   }
   // write results
   string outFile = filePathPrefix + string("_elutionFlow.csv");
   AF4Log::logText(tr("Write result to %1").arg(outFile.c_str()));
   AF4CsvWriter csvWriter(outFile);
   csvWriter.writeFile(matD{devXRel, deltaWidth, deltaVolume} , header);
};
-------------------------------------------------------------------------------------------------------------------*/

   CalibResult refResult;
   if(cModes.classical){
      // try a single test calibration to
      refResult = calibSingleParamSet(chDims, params, CalibMode::classical);
      if(refResult.errorCode != CalibErrorCode::noError){
         logErrorMessage(refResult.errorCode);
         return;
      }
      const double widthRefY = refResult.width;   // Y_width(X)
      const double VolumeRefY = refResult.volume;  // Y_vol(X)
      const string filePathPrefix("/home/bluemage/tests/deltaTests/delta");
      const std::vector<string> header = {string("delta X"), string("delta width"), string("delta volume") };

      ITERATIVE_PARAMETER_DELTA_ANALYSIS(elutionFlow,   params, paramsDeltaMod, classical, "_elutionFlow_cla.csv"   );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(crossFlow,     params, paramsDeltaMod, classical, "_crossFlow_cla.csv"     );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(relFocusPoint, params, paramsDeltaMod, classical, "_relFocusPoint_cla.csv" );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(voidPeakTime,  params, paramsDeltaMod, classical, "_voidPeakTime_cla.csv"  );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(elutionTime,   params, paramsDeltaMod, classical, "_elutionTime_cla.csv"   );
      ITERATIVE_PARAMETER_DELTA_ANALYSIS(diffCoeff,     params, paramsDeltaMod, classical, "_diffCoeff_cla.csv"     );

   }
   if(cModes.geometric){
      refResult = calibSingleParamSet(chDims, params, CalibMode::geometric);
      if(refResult.errorCode != CalibErrorCode::noError){
         logErrorMessage(refResult.errorCode);
         return;
      }
      else
         logErrorMessage(refResult.errorCode);
   }
   if(cModes.hydrodynamic){
      refResult = calibSingleParamSet(chDims, params, CalibMode::hydrodynamic);
      if(refResult.errorCode != CalibErrorCode::noError){
         logErrorMessage(refResult.errorCode);
         return;
      }
      else
         logErrorMessage(refResult.errorCode);
   }
#undef ITERATIVE_PARAMETER_DELTA_ANALYSIS
}

void AF4ChannelConfigurationWidget::logErrorMessage(CalibErrorCode errorCode)
{
   switch(errorCode){
   case CalibErrorCode::noError:
      break;
   case CalibErrorCode::voidTimeZero:
      AF4Log::logError(tr("tVoid = 0!"));
   case CalibErrorCode::eluFlowZero:
      AF4Log::logError(tr("Ve = 0!"));
   case CalibErrorCode::eluTimeZero:
      AF4Log::logError(tr("te = 0!"));
   case CalibErrorCode::crossFlowZero:
      AF4Log::logError(tr("Vc = 0!"));
   case CalibErrorCode::diffCoeffZero:
      AF4Log::logError(tr("D = 0!"));
   case CalibErrorCode::voidTimeTooSmall:
      AF4Log::logError(tr("tvoid must not be smaller than offset!"));
   case CalibErrorCode::eluTimeTooSmall:
      AF4Log::logError(tr("te must not be smaller than tvoid!"));
   case CalibErrorCode::ParamsNotChecked:
      AF4Log::logError(tr("Parameters were not checked! رجم مبرمج"));
      break;
   default:
      break;
   }
}
