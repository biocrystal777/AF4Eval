#ifdef __linux__

#ifndef FFFDATAALIGNER_H
#define FFFDATAALIGNER_H

#include "./fffconstants.h"
#include "./fffcalculator.h"
#include <string>
#include <QDebug>
#include <vector>
#include <limits>
#include <ostream>
#include <fstream>
#include <sstream>
#include <complex>
#include <utility>
#include <cmath>

#ifdef __linux__

#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>

#endif

class FFFDataAligner : public FFFCalculator
{
public:
   FFFDataAligner(){}
   FFFDataAligner(const FFFDataAligner& src) = delete;
   FFFDataAligner& operator= (FFFDataAligner& src) = delete;
   FFFDataAligner(FFFDataAligner&& src) = delete;
   FFFDataAligner& operator= (FFFDataAligner&& src) = delete;

   int alignData();

   void testConvolution();

private:

   double testParametrized(double sigma, double noisefactor);

   void testBroadening(const double sigma, const double tau, const double noiseFactor = 0.0);

   static uint nextSuitedPower2(const uint n){
      uint r = 32; // minimum
      uint a = 2 * n; // double the minimum size to shift circular effects
      uint lim = 0x100000;
      while( a > r && r < lim) r *= 2;
      return r;
   }



   // optimization Algoritm:
   // 0: goldensection
   // 1: quad_golden
   // 2: brent



   double determineSigma(const vecD &signal,
                         const vecD &reference,
                         double sigmaMin,
                         double sigmaMax,
                         const uint compStart,
                         const uint compEnd,
                         const uint maxIterations,
                         const uint optimizationAlgorithm = 0,
                         const double maxSigmaInterval = std::numeric_limits<double>::max(),
                         double maxRMS = -1.0) const;

   // "C-ish" Interface for gsl_optimization of sigma;
   // Has to provide a function f  with prototype "double (double x, void *params)"
   // This will be the callback function for optimization and is passed as pointer
   // to the gsl optimization

   struct optFuncParameters{ // these parameters should be accessed via the required void pointer of the target function
      const vecD &signal;
      const vecD &reference;
      const uint compStart;
      const uint compEnd;
      optFuncParameters(const vecD &s,
                        const vecD &r,
                        const uint cS,
                        const uint cE) : signal(s), reference(r), compStart(cS), compEnd(cE){}
   };

   static double funcToOptimize(double sigma, void *params);


   static void convolveBroadeningModel(const vecD &f,
                                       const double sigma,
                                       const double tau,
                                       vecD &result,
                                       vecD *testBroadeningModel = nullptr,
                                       vecD *testGaussian = nullptr,
                                       vecD *testExpDecay = nullptr);

   static void convolveExpDecay(const vecD &f,
                                const double tau,
                                vecD &result,
                                vecD *testExpDecay = nullptr);

   static void convolveGaussian(const vecD &f,
                                const double sigma,
                                vecD &result,
                                vecD *testGauss = nullptr);

   static void fourierConvolution(const vecD &f,
                           const vecD &g,
                           vecD &r) ;

   static void smoothArea(vecD &v,
                          const uint opStart,
                          const uint opEnd,
                          const uint startLeftArea,
                          const uint endLeftArea,
                          const uint startRightArea,
                          const uint endRightArea
                          );

   static void derivative(const vecD &signal, vecD &result, uint order = 1, double deltaX = 1.0);

   static double sumOf(const vecD &v);


   static void avoidZerosOn(std::vector<std::complex<double> >&v) ;

   static double varianceOf(const vecD &sample,
                            const vecD &reference,
                            uint wStart, uint wEnd);

   inline const std::string logData(const double result,
                                    const double rms,
                                    const uint its,
                                    const double upLimit,
                                    const double lowLimit) const;
};

#endif // FFFDATAALIGNER_H
#endif // __linux__
