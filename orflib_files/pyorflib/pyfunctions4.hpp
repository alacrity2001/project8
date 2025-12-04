/**
@file  pyfunctions4.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/market/market.hpp>
#include <orflib/products/europeancallput.hpp>
#include <orflib/products/americancallput.hpp>
#include <orflib/methods/pde/pde1dsolver.hpp>
#include <orflib/products/convertiblebond.hpp> 

using namespace std;

static
PyObject*  pyOrfEuroBSPDE(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyPdeParams(NULL);
  PyObject* pyAllResults(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyPdeParams, &pyAllResults))
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

  // read the PDE parameters
  orf::PdeParams pdeparams = asPdeParams(pyPdeParams);
  // read the allresults flag
  bool allresults = asBool(pyAllResults);

  // create the product
  orf::SPtrProduct spprod(new orf::EuropeanCallPut(payoffType, strike, timeToExp));
  // create the PDE solver
  orf::Pde1DResults results;
  orf::Pde1DSolver solver(spprod, spyc, spot, divYield, vol, results);
  solver.solve(pdeparams);

  // write results
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(results.prices[0]));

  if (allresults) {
    orf::Vector spots;
    results.getSpotAxis(0, spots);
    orf::Matrix values(results.times.size(), results.values.front().size());
    for (size_t i = 0; i < results.times.size(); ++i)
      for (size_t j = 0; j < results.values.front().size(); ++j)
        values(i, j) = results.values[i](j, 0);

    PyDict_SetItem(ret, asPyScalar("Times"), asNumpy(results.times));
    PyDict_SetItem(ret, asPyScalar("Spots"), asNumpy(spots));
    PyDict_SetItem(ret, asPyScalar("Values"), asNumpy(values));
  }
  return ret;

PY_END;
}

static
PyObject*  pyOrfAmerBSPDE(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyPdeParams(NULL);
  PyObject* pyAllResults(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyPdeParams, &pyAllResults))
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

  // read the PDE parameters
  orf::PdeParams pdeparams = asPdeParams(pyPdeParams);
  // read the allresults flag
  bool allresults = asBool(pyAllResults);

  // create the product
  orf::SPtrProduct spprod(new orf::AmericanCallPut(payoffType, strike, timeToExp));
  // create the PDE solver
  orf::Pde1DResults results;
  bool storeAllResults = true;
  orf::Pde1DSolver solver(spprod, spyc, spot, divYield, vol, results, storeAllResults);
  solver.solve(pdeparams);

  // write results
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(results.prices[0]));

  if (allresults) {
    orf::Vector spots;
    results.getSpotAxis(0, spots);
    orf::Matrix values(results.times.size(), results.values.front().size());
    for (size_t i = 0; i < results.times.size(); ++i)
      for (size_t j = 0; j < results.values.front().size(); ++j)
        values(i, j) = results.values[i](j, 0);

    PyDict_SetItem(ret, asPyScalar("Times"), asNumpy(results.times));
    PyDict_SetItem(ret, asPyScalar("Spots"), asNumpy(spots));
    PyDict_SetItem(ret, asPyScalar("Values"), asNumpy(values));
  }
  return ret;

PY_END;
}

static
PyObject* pyOrfCbBSPDE(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyFaceValue(NULL);
  PyObject* pyMaturity(NULL);
  PyObject* pyConvRatio(NULL);
  PyObject* pyConvStart(NULL);
  PyObject* pyConvEnd(NULL);
  PyObject* pyCallStrike(NULL);
  PyObject* pyCallStart(NULL);
  PyObject* pyCallEnd(NULL);
  
  PyObject* pySpot(NULL);
  PyObject* pyDiscountCrv(NULL); 
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  
  PyObject* pyPdeParams(NULL);
  PyObject* pyAllResults(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOOOOOOO", 
      &pyFaceValue, &pyMaturity, &pyConvRatio, &pyConvStart, &pyConvEnd,
      &pyCallStrike, &pyCallStart, &pyCallEnd,
      &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility,
      &pyPdeParams, &pyAllResults))
    return NULL;

  double faceValue = asDouble(pyFaceValue);
  double maturity = asDouble(pyMaturity);
  double convRatio = asDouble(pyConvRatio);
  double convStart = asDouble(pyConvStart);
  double convEnd = asDouble(pyConvEnd);
  double callStrike = asDouble(pyCallStrike);
  double callStart = asDouble(pyCallStart);
  double callEnd = asDouble(pyCallEnd);

  double spot = asDouble(pySpot);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  orf::PdeParams pdeparams = asPdeParams(pyPdeParams);
  bool allresults = asBool(pyAllResults);

  orf::SPtrProduct spprod(new orf::ConvertibleBond(
      faceValue, maturity, convRatio, convStart, convEnd, 
      callStrike, callStart, callEnd));

  orf::Pde1DResults results;
  orf::Pde1DSolver solver(spprod, spyc, spot, divYield, vol, results, allresults);

  solver.solve(pdeparams);

  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(results.prices[0]));

  if (allresults) {
    orf::Vector spots;
    results.getSpotAxis(0, spots);
    orf::Matrix values(results.times.size(), results.values.front().size());
    
    for (size_t i = 0; i < results.times.size(); ++i)
      for (size_t j = 0; j < results.values.front().size(); ++j)
        values(i, j) = results.values[i](j, 0);

    PyDict_SetItem(ret, asPyScalar("Times"), asNumpy(results.times));
    PyDict_SetItem(ret, asPyScalar("Spots"), asNumpy(spots));
    PyDict_SetItem(ret, asPyScalar("Values"), asNumpy(values));
  }
  return ret;

  PY_END;
}