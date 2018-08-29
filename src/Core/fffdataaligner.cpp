#ifdef __linux__

#include "./fffdataaligner.h"
using std::vector;
using std::complex;
using std::string;
using std::to_string;

void FFFDataAligner::testConvolution()
{
   testBroadening(25.0, 40.0, 1e-7);
}

double FFFDataAligner::testParametrized(double sigma, double noisefactor)
{

   uint size = 3000;
   vecD input(size);

   for(uint i = 0; i < size; ++i)
      input[i] = 0.0001;

   for(uint i = 300; i < 1400; ++i){
      double val =(0.8 * (double(i) - double(size)/2)) * (0.8 * (double(i) - double(size)/2)) / -200000.0 + 0.04;
      if(val > input[i]) input[i] = val;
      }

   for(uint i = 1450; i < 1600; ++i){
      input[i] = 0.05;
   }
   for(uint i = 1600; i < 1800; ++i){
      double val = -0.00008 * (i-1600) + 0.05;
      if(val > input[i]) input[i] = val;
   }
   // add noise
   for(uint i = 0; i < size; ++i){
      double noise = rand() % 1000;
      if(rand() % 2) noise *= -1;
      input[i] += noise * noisefactor;
   }

   vecD signal2(size);
   //double sigma = 10.3354;
   vecD testGauss(size);
   convolveGaussian(input, sigma, signal2, &testGauss);

   // add noise
   for(uint i = 0; i < size; ++i){
      double noise = rand() % 1000;
      if(rand() % 2) noise *= -1;
      signal2[i] += noise * noisefactor;
   }

   // ITERATIVE TEST

   double sigMin = 0.0001;
   double sigMax = 100.0;

   double sigmaDet2 = determineSigma(input, signal2, sigMin, sigMax, 500, 800, 1000,
                                                    0, 1e-10, 1e-20);
   if(sigmaDet2 <= 0.0) {
      qDebug() << "sigma could not be determined" << sigmaDet2;
      return 0.0;
   }



   vecD convTest(size);
   convolveGaussian(input, sigmaDet2, convTest);

// TEST FILE

   std::ofstream outFile;
   outFile.open(string("/home/bluemage/test/graphs_s").append(to_string((double)sigma)).append("_n").append(to_string(0)).append(".csv"));
   outFile << "x" << "," << "signal1" << ","<<"signal2"<< "," << "gauss"<< "," << "broadenedSignal" << "\n";
   for(uint i = 0; i < size; ++i) outFile << i << "," << input[i] << "," << signal2[i] << "," << testGauss[i] << ","<< convTest[i]<< "\n";
   outFile.close();

   return sigmaDet2;
}

void FFFDataAligner::testBroadening(const double sigma, const double tau, const double noiseFactor)
{
   // INITIALIZE FUNCTION
   uint size = 2000;
   vecD input(size);
   for(uint i = 0; i < size; ++i)
      input[i] = 0.001;

   for(uint i = 300; i < 1400; ++i){
      double val =(0.8 * (double(i) - double(size)/2)) * (0.8 * (double(i) - double(size)/2)) / -200000.0 + 0.04;
      if(val > input[i]) input[i] = val;
      }
   for(uint i = 1450; i < 1600; ++i){
      input[i] = 0.05;
   }
   for(uint i = 1600; i < 1800; ++i){
      double val = -0.00008 * (i-1600) + 0.05;
      if(val > input[i]) input[i] = val;
   }
   // add noise
   for(uint i = 0; i < size; ++i){
      double noise = rand() % 1000;
      if(rand() % 2) noise *= -1;
      input[i] += noise * noiseFactor;
   }


   vecD result(size);
   vecD gauss(size);
   vecD broadening(size);
   vecD exponential(size);
   convolveBroadeningModel(input, sigma, tau, result, &broadening, &gauss, &exponential);

   // write graphs
   std::ofstream outFile;
   outFile.open(string("/home/bluemage/test/graphBroadening").append("_sigma").append(to_string(int(sigma))).append("_tau").append(to_string(int(tau))).append(".csv"));
   outFile <<                               "x" << "," << "signal" << "," << "result"   << "," << "gauss"  << "," << "exponential"  << "," << "broadenedSignal" << "\n";
   for(uint i = 0; i < size; ++i) outFile << i  << "," << input[i] << "," <<  result[i] << "," << gauss[i] << "," << exponential[i] << "," << broadening[i]<< "\n";
   outFile.close();
}

double FFFDataAligner::determineSigma(const vecD &signal,
                                      const vecD &reference,
                                      double sigmaMin,
                                      double sigmaMax,
                                      const uint compStart,
                                      const uint compEnd,
                                      const uint maxIterations,
                                      const uint optimizationAlgorithm,
                                      const double maxSigmaInterval,
                                      double maxRMS) const
{
   int errorCode(0);
   double currentSigma = (sigmaMax + sigmaMin) / 2.0;

   const gsl_min_fminimizer_type * fminTypeConst = gsl_min_fminimizer_brent;
   gsl_min_fminimizer * s = gsl_min_fminimizer_alloc(fminTypeConst);
   optFuncParameters params(signal, reference, compStart, compEnd);
   gsl_function func;
   func.function = &FFFDataAligner::funcToOptimize;
   func.params = &params;
   errorCode = gsl_min_fminimizer_set(s, &func, currentSigma, sigmaMin, sigmaMax);
   if(errorCode == GSL_EINVAL){
      FFFLog::logError(string("Invalid arguments supplied."));
      return -1.0;
   }
   else if (errorCode != 0){
      FFFLog::logError(string(" Code ").append(to_string((int) errorCode)));
      return -1.0;
   }

   for(uint it = 0; it < maxIterations; ++it){
      errorCode = gsl_min_fminimizer_iterate(s);
      currentSigma = gsl_min_fminimizer_minimum(s);
      double currentRMS = gsl_min_fminimizer_f_minimum(s);
      double currentSigmaMin = gsl_min_fminimizer_x_lower(s);
      double currentSigmaMax = gsl_min_fminimizer_x_upper(s);

      if(errorCode == GSL_EBADFUNC){
       FFFLog::logError(string("Iteration encountered a point where the optimization function evaluated inf or NaN"));

      }
      else if (errorCode == GSL_FAILURE) {
         string logData(this->logData(currentSigma, currentRMS, it, currentSigmaMax, currentSigmaMin));
         FFFLog::logText(string("No improvement could be achieved.").append(logData));
         break;
      }
      else if (errorCode != 0){
         string logData(this->logData(currentSigma, currentRMS, it, currentSigmaMax, currentSigmaMin));
         FFFLog::logText(string("Error while optimizing. Code ").append(to_string((int) errorCode)));
         break;
      }

      if(maxRMS >= currentRMS){
         string logData(this->logData(currentSigma, currentRMS, it, currentSigmaMax, currentSigmaMin));
         FFFLog::logText(string("Optimized function and reached maximal RMS.").append(logData));
         break;
      }
      if(maxSigmaInterval >= (currentSigmaMax-currentSigmaMin)){
         string logData(this->logData(currentSigma, currentRMS, it, currentSigmaMax, currentSigmaMin));
         FFFLog::logText(string("Optimized function and reached maximal RMS.").append(logData));
         break;
      }
   }

   gsl_min_fminimizer_free(s);

   return currentSigma;
}

void FFFDataAligner::convolveBroadeningModel(const vecD &f,
                                             const double sigma,
                                             const double tau,
                                             vecD &result,
                                             vecD *testBroadeningModel,
                                             vecD *testGaussian,
                                             vecD *testExpDecay)
{
   uint size = f.size();
   // create expDecayFunction
   /*
   vecD expDecay(size);
   for(uint i = 0; i < size; ++i) expDecay[i] = exp(-double(i)/tau);
   const double sumExpDecay = sumOf(expDecay);                       // normalize integral to 1
   for(uint i = 0; i < size; ++i) expDecay[i] /= sumExpDecay;
   vecD broadeningModel(size);
   convolveGaussian(expDecay, sigma, broadeningModel, testGaussian);
*/

   vecD gauss(size);
   uint gaussShift = f.size() / 2;

   for(uint i = 0; i < size; ++i){
      double t = (double(i) - double(gaussShift))  / sigma;
      gauss[i] = (1 / (sigma * sqrt(2 * PI))) * exp(-0.5 * t * t);
   }

   vecD broadeningModel(size);
   convolveExpDecay(gauss, tau, broadeningModel, testExpDecay);

   if(testBroadeningModel) for(uint i = 0; i < size; ++i) (*testBroadeningModel)[i] = broadeningModel[i];
   if(testGaussian) for(uint i = 0; i < size; ++i) (*testGaussian)[i] = gauss[i];

   fourierConvolution(f, gauss, result);

   // shift back
   auto midIt = result.begin();
   std::advance(midIt, gaussShift);
   std::rotate(result.begin(), midIt, result.end());
   result.resize(size);
}



void FFFDataAligner::convolveExpDecay(const vecD &f,
                                      const double tau,
                                      vecD &result,
                                      vecD *testExpDecay)
{
   uint size = f.size();
   vecD expDecay(size);
   //uint gaussShift = f.size() / 2;

   for(uint i = 0; i < size; ++i) expDecay[i] = exp(-double(i)/tau);
   // normalize integral to 1
   const double sumExpDecay = sumOf(expDecay);
   for(uint i = 0; i < size; ++i) expDecay[i] /= sumExpDecay;

   if(testExpDecay) for(uint i=0; i < testExpDecay->size(); ++i) (*testExpDecay)[i] = expDecay[i];
   fourierConvolution(f, expDecay, result);
}

void FFFDataAligner::convolveGaussian(const vecD &f,
                                      const double sigma,
                                      vecD &result,
                                      vecD *testGauss)
{
   uint size = f.size();
   vecD gauss(size);
   uint gaussShift = f.size() / 2;

   for(uint i = 0; i < size; ++i){
      double t = (double(i) - double(gaussShift))  / sigma;
      gauss[i] = (1 / (sigma * sqrt(2 * PI))) * exp(-0.5 * t * t);
   }
   if(testGauss) for(uint i=0; i < testGauss->size(); ++i) (*testGauss)[i] = gauss[i];
   fourierConvolution(f, gauss, result);

   // shift back
   auto midIt = result.begin();
   std::advance(midIt, gaussShift);
   std::rotate(result.begin(), midIt, result.end());
   result.resize(size);
}

void FFFDataAligner::fourierConvolution(const vecD &f,
                                        const vecD &g,
                                        vecD &r)
{
   // find FT-length in order to avoid circular convolution
   uint size = f.size();
   uint sizeFT = nextSuitedPower2(size);

   // Transform to complex vector
   vector<complex<double> > FT_f(sizeFT);
   vector<complex<double> > FT_g(sizeFT);
   for(uint i = 0; i < size; ++i){
      FT_f[i] = complex<double>(f[i], 0.0);
      FT_g[i] = complex<double>(g[i], 0.0);
   }

   // zero padding
   for(uint i = size; i < sizeFT; ++i){
      FT_f[i] = complex<double>(0.0, 0.0);
      FT_g[i] = complex<double>(0.0, 0.0);
   }

   //////////// INCLUDE SMOOTHING FUNCTION for 0-padded ends ///////////

   // calculate FT(f) and FT(g)
   gsl_fft_complex_radix2_forward( (gsl_complex_packed_array) &(FT_f[0]), 1, sizeFT);
   gsl_fft_complex_radix2_forward( (gsl_complex_packed_array) &(FT_g[0]), 1, sizeFT);

   vector<complex<double> > FT_r(sizeFT);
   for(uint i = 0;  i < sizeFT; ++i)
      FT_r[i] = FT_f[i] * FT_g[i];


   gsl_fft_complex_radix2_inverse((gsl_complex_packed_array) &(FT_r[0]), 1, sizeFT);

   r = vecD(sizeFT);
   for(uint i = 0; i < sizeFT; ++i){
      r[i] = FT_r[i].real();
   }

   return;
}

void FFFDataAligner::smoothArea(vecD &v,
                                const uint opStart,
                                const uint opEnd,
                                const uint startLeftArea,
                                const uint endLeftArea,
                                const uint startRightArea,
                                const uint endRightArea)
{
   // TEST INPUTS
   if(opStart < v.size()
         || opStart < v.size()
         || opStart > opEnd
         || startLeftArea > endLeftArea
         || startRightArea > endRightArea
         || startLeftArea > endRightArea)
      return;
   //
   double leftBorder(0.0);
   for(uint i = startLeftArea; i < endLeftArea; ++i) leftBorder += v[i];
   leftBorder /= static_cast<double>(endLeftArea - startLeftArea);
   double rightBorder(0.0);
   for(uint i = startRightArea; i < endRightArea; ++i) rightBorder += v[i];
   rightBorder /= static_cast<double>(endRightArea - startRightArea);
   double slope = (rightBorder -leftBorder) / (opEnd - opStart);
   for(uint i = opStart; i< opEnd; ++i){
      v[i] = leftBorder + slope * (i-opStart);
   }
   return;
}

void FFFDataAligner::derivative(const vecD &signal, vecD &result,
                                uint order, double deltaX)
{
   if(result.size() == 0) result = vecD(signal.size());

   while (order >= 1){
      for(uint i = 0; i < signal.size() - 1; ++i)      // derivative with one shift to left
         result[i] = (signal[i + 1]  - signal [i]) / deltaX;
      result[result.size()-1] = result[result.size() - 2];
      for(uint i = result.size() -1; i >= 1; --i)    // smoothing with a half shift back
         result[i] = (result[i-1] + result[i]) / 2.0;
      --order;
   }
}

double FFFDataAligner::sumOf(const vecD &v)
{
   double sum(0.0);
   for(uint i = 0; i < v.size(); ++i){
      sum += v[i];
   }
   return sum;
}


void FFFDataAligner::avoidZerosOn(std::vector<complex<double> > &v)
{
   for(uint i = 0; i < v.size(); ++i){
      double rT = v[i].real();
      double iT = v[i].imag();
      //if(rT <= 0.0 && rT >= 0.0 && iT <= 0.0 && iT >= 0.0){
      if(rT <= 0.0 && rT >= 0.0){ v[i] = complex<double>(std::numeric_limits<double>::min(), iT);    }
      if(iT <= 0.0 && iT >= 0.0){ v[i] = complex<double>(rT, std::numeric_limits<double>::min());  }
     // }
   }
}

double FFFDataAligner::varianceOf(const vecD &sample,
                                  const vecD &reference,
                                  uint wStart,
                                  uint wEnd)
{
   if(wStart > sample.size()
         || wStart > reference.size()
         || wEnd > reference.size()
         || wStart > sample.size()
         || wStart > wEnd)
      return 0.0;
   double rms(0.0);
   for(uint i = wStart; i < wEnd; ++i){
      double diff = sample[i] - reference[i];
      rms += diff * diff;
   }
   rms /= (wEnd - wStart);
   rms = std::sqrt(rms);
   return rms;
}

const string FFFDataAligner::logData(const double result,
                                     const double rms,
                                     const uint its,
                                     const double upLimit,
                                     const double lowLimit) const
{
   std::stringstream sstr;
   sstr.precision(10);
   sstr << "\nDetermined Sigma: " << result
        <<"\nRMS:         " << rms
       << "\nIterations Performed: " << its
       << "\nLimits: " << lowLimit << "-" << upLimit << "\n";
   return sstr.str();
}

double FFFDataAligner::funcToOptimize(double sigma, void *params)
{
   optFuncParameters *p = reinterpret_cast<optFuncParameters*>(params);
   vecD testResult;
   convolveGaussian(p->signal, sigma, testResult);
   // Derivative Test
   vecD dSignaldx(p->signal.size());
   derivative(p->signal, dSignaldx, 1, 1.0);
   vecD dTestResultdx(p->signal.size());
   derivative(testResult, dTestResultdx, 1, 1.0);
   double var = varianceOf(dTestResultdx, dSignaldx, p->compStart, p->compEnd);
   // end Test
   //double var = varianceOf(testResult, p->reference, p->compStart, p->compEnd);
   if(var > std::numeric_limits<double>::max()) var = std::numeric_limits<double>::max();
   else if(var < - std::numeric_limits<double>::max()) var = - std::numeric_limits<double>::max();
   else if (isnan(var)) var = -1.0;

   return var;
}
#endif // __linux__
