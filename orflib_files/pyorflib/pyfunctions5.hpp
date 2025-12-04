/**
@file  pyfunctions5.hpp
@brief Implementation of Python callable functions
*/
#include "pyutils.hpp"

#include <orflib/pricers/ptpricers.hpp>
#include <cmath>

static
PyObject*  pyOrfPtRisk(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPtWghts(NULL);
  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyPtWghts, &pyAssetRets, 
    &pyAssetVols, &pyCorrelMat))
    return NULL;

  orf::Vector ptwghts = asVector(pyPtWghts);
  orf::Vector assetRets = asVector(pyAssetRets);
  orf::Vector assetVols = asVector(pyAssetVols);
  orf::Matrix correlMat = asMatrix(pyCorrelMat);

  std::tuple<double, double> res = orf::ptRisk(ptwghts, assetRets, assetVols, correlMat);

  // write mean and standard deviation into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(get<0>(res)));
  PyDict_SetItem(ret, asPyScalar("StdDev"), asPyScalar(get<1>(res)));
  return ret;

PY_END;
}

static
PyObject* pyOrfMvpWghts(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOO", &pyAssetRets, &pyAssetVols, &pyCorrelMat))
    return NULL;

  orf::Vector assetRets = asVector(pyAssetRets);
  orf::Vector assetVols = asVector(pyAssetVols);
  orf::Matrix correlMat = asMatrix(pyCorrelMat);

  orf::Vector wghts = orf::mvpWeights(assetRets, assetVols, correlMat);

  return asNumpy(wghts);

  PY_END;
}


static
PyObject* pyOrfMktRisk(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);
  PyObject* pyRfreeRate(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyAssetRets, &pyAssetVols, &pyCorrelMat, &pyRfreeRate))
    return NULL;

  orf::Vector assetRets = asVector(pyAssetRets);
  orf::Vector assetVols = asVector(pyAssetVols);
  orf::Matrix correlMat = asMatrix(pyCorrelMat);
  double rfreeRate = asDouble(pyRfreeRate);

  std::tuple<double, double, double> res = orf::mktRisk(assetRets, assetVols, correlMat, rfreeRate);

    // write mean and standard deviation into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(get<0>(res)));
  PyDict_SetItem(ret, asPyScalar("StdDev"), asPyScalar(get<1>(res)));
  PyDict_SetItem(ret, asPyScalar("Lambda"), asPyScalar(get<2>(res)));
  return ret;

  PY_END;
}


static
PyObject* pyOrfMktWghts(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);
  PyObject* pyRfreeRate(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyAssetRets, &pyAssetVols, &pyCorrelMat, &pyRfreeRate))
    return NULL;

  orf::Vector assetRets = asVector(pyAssetRets);
  orf::Vector assetVols = asVector(pyAssetVols);
  orf::Matrix correlMat = asMatrix(pyCorrelMat);
  double rfreeRate = asDouble(pyRfreeRate);

  orf::Vector wghts = orf::mktWeights(assetRets, assetVols, correlMat, rfreeRate);

  return asNumpy(wghts);

  PY_END;
}



static
PyObject* pyOrfMeanVarWghts(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);
  PyObject* pyLambda(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyAssetRets, &pyAssetVols, &pyCorrelMat, &pyLambda))
    return NULL;

  orf::Vector assetRets = asVector(pyAssetRets);
  orf::Vector assetVols = asVector(pyAssetVols);
  orf::Matrix correlMat = asMatrix(pyCorrelMat);
  double lambda = asDouble(pyLambda);

  orf::Vector wghts = orf::meanVarWeights(assetRets, assetVols, correlMat, lambda);

  return asNumpy(wghts);

  PY_END;
}


static
PyObject* pyOrfMeanVarFront(PyObject* pyDummy, PyObject* pyArgs)
{
  PY_BEGIN;

  PyObject* pyAssetRets(NULL);
  PyObject* pyAssetVols(NULL);
  PyObject* pyCorrelMat(NULL);
  PyObject* pyLambdaMax(NULL);
  PyObject* pyNSteps(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyAssetRets, &pyAssetVols, &pyCorrelMat, &pyLambdaMax, &pyNSteps))
    return NULL;

  orf::Vector assetRets = asVector(pyAssetRets);
  orf::Vector assetVols = asVector(pyAssetVols);
  orf::Matrix correlMat = asMatrix(pyCorrelMat);
  double lambdaMax = asDouble(pyLambdaMax);
  int nSteps = asInt(pyNSteps);

  std::tuple<orf::Vector, orf::Vector, orf::Vector> res = 
      orf::meanVarFront(assetRets, assetVols, correlMat, lambdaMax, nSteps);

  orf::Vector means = std::get<0>(res);
  orf::Vector stds = std::get<1>(res);
  orf::Vector lams = std::get<2>(res);

  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asNumpy(means));
  PyDict_SetItem(ret, asPyScalar("StdDev"), asNumpy(stds));
  PyDict_SetItem(ret, asPyScalar("Lambda"), asNumpy(lams));
  
  return ret;

  PY_END;
}