/**
@file  pyfunctions3.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/defines.hpp>
#include <orflib/products/europeancallput.hpp>
#include <orflib/products/asianbasketcallput.hpp>
#include <orflib/pricers/bsmcpricer.hpp>
#include <orflib/pricers/multiassetbsmcpricer.hpp>
#include <orflib/math/stats/meanvarcalculator.hpp>
#include <orflib/math/random/rng.hpp>


static
PyObject*  pyOrfEuroBSMC(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyMcParams(NULL);
  PyObject* pyNPaths(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyMcParams, &pyNPaths))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);
 
  // read the MC parameters
  orf::McParams mcparams = asMcParams(pyMcParams);
  // read the number of paths
  unsigned long npaths = asInt(pyNPaths);

  // create the product
  orf::SPtrProduct spprod(new orf::EuropeanCallPut(payoffType, strike, timeToExp));
  // create the pricer
  orf::BsMcPricer bsmcpricer(spprod, spyc, divYield, vol, spot, mcparams);
  // create the statistics calculator
  orf::MeanVarCalculator<double *> sc(bsmcpricer.nVariables());
  // run the simulation
  bsmcpricer.simulate(sc, npaths);
  // collect results
  orf::Matrix const& results = sc.results();
  // read out results
  size_t nsamples = sc.nSamples();
  double mean = results(0, 0);
  double stderror = results(1, 0);
  stderror = std::sqrt(stderror / nsamples);

  // write mean and standard error into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(mean));
  PyDict_SetItem(ret, asPyScalar("StdErr"), asPyScalar(stderror));
  return ret;

PY_END;
}

static
PyObject*  pyOrfAsianBasketBSMC(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyFixingTimes(NULL);
  PyObject* pyAssetQuantities(NULL);
  PyObject* pySpots(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYields(NULL);
  PyObject* pyVolatilities(NULL);
  PyObject* pyCorrelMatrix(NULL);
  PyObject* pyMcParams(NULL);
  PyObject* pyNPaths(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOOOO", &pyPayoffType, &pyStrike, &pyFixingTimes, 
    &pyAssetQuantities, &pySpots, &pyDiscountCrv, &pyDivYields, &pyVolatilities, 
    &pyCorrelMatrix, &pyMcParams, &pyNPaths))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double strike = asDouble(pyStrike);
  orf::Vector fixingTimes = asVector(pyFixingTimes);
  orf::Vector assetQuantities = asVector(pyAssetQuantities);
  orf::Vector spots = asVector(pySpots);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  orf::Vector divYields = asVector(pyDivYields);
  orf::Vector vols = asVector(pyVolatilities);
  orf::Matrix correlMat = asMatrix(pyCorrelMatrix);
  // read the MC parameters
  orf::McParams mcparams = asMcParams(pyMcParams);
  // read the number of paths
  unsigned long npaths = asInt(pyNPaths);

  // create the product
  orf::SPtrProduct spprod(new orf::AsianBasketCallPut(payoffType, strike, fixingTimes, assetQuantities));
  // create the pricer
  orf::MultiAssetBsMcPricer bsmcpricer(spprod, spyc, divYields, vols, spots, correlMat, mcparams);
  // create the statistics calculator
  orf::MeanVarCalculator<double *> sc(bsmcpricer.nVariables());
  // run the simulation
  bsmcpricer.simulate(sc, npaths);
  // collect results
  orf::Matrix const & results = sc.results();
  // read out results
  double mean = results(0, 0);
  double stderror = results(1, 0);
  stderror = std::sqrt(stderror / sc.nSamples());

  // write mean and standard error into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(mean));
  PyDict_SetItem(ret, asPyScalar("StdErr"), asPyScalar(stderror));
  return ret;

PY_END;
}
