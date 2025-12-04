/** 
@file  pyfunctions1.hpp
@brief Implementation of Python callable functions
*/
#include "pycpp.hpp"
#include <orflib/defines.hpp>
#include <orflib/pricers/simplepricers.hpp>
#include <string>

static
PyObject*  pyOrfFwdPrice(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pySpot(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pySpot, &pyTimeToExp, &pyIntRate, &pyDivYield))
    return NULL;

  double spot = asDouble(pySpot);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);

  double fwd = orf::fwdPrice(spot, timeToExp, intRate, divYield);

  return asPyScalar(fwd);
PY_END;
}

static
PyObject*  pyOrfQFwdPrice(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pySpot(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyAssetVol(NULL);
  PyObject* pyFxVol(NULL);
  PyObject* pyCorrel(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO", &pySpot, &pyTimeToExp, &pyIntRate, 
    &pyDivYield, &pyAssetVol, &pyFxVol, &pyCorrel))
    return NULL;

  double spot = asDouble(pySpot);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double assetvol = asDouble(pyAssetVol);
  double fxvol = asDouble(pyFxVol);
  double correl = asDouble(pyCorrel);

  double qfwd = orf::quantoFwdPrice(spot, timeToExp, intRate, divYield,
    assetvol, fxvol, correl);

  return asPyScalar(qfwd);
PY_END;
}

static
PyObject*  pyOrfDigiBS(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO", &pyPayoffType, &pySpot, &pyStrike,
    &pyTimeToExp, &pyIntRate, &pyDivYield, &pyVolatility))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  orf::Vector greeks = orf::digitalOptionBS(payoffType, spot, strike, timeToExp, intRate, divYield, vol);

  return asNumpy(greeks);
PY_END;
}

static
PyObject*  pyOrfEuroBS(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOO", &pyPayoffType, &pySpot, &pyStrike,
    &pyTimeToExp, &pyIntRate, &pyDivYield, &pyVolatility))
    return NULL;


  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  orf::Vector greeks = orf::europeanOptionBS(payoffType, spot, strike, timeToExp, intRate, divYield, vol);

  return asNumpy(greeks);
PY_END;
}

static
PyObject*  pyOrfKOFwd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyKOLevel(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyTimeToKO(NULL);
  PyObject* pyIntRate(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOO", &pySpot, &pyStrike, &pyKOLevel,
      &pyTimeToExp, &pyTimeToKO, &pyIntRate, &pyDivYield, &pyVolatility))
    return NULL;

  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double kolevel = asDouble(pyKOLevel);
  double timeToExp = asDouble(pyTimeToExp);
  double timeToKO = asDouble(pyTimeToKO);
  double intRate = asDouble(pyIntRate);
  double divYield = asDouble(pyDivYield);
  double vol = asDouble(pyVolatility);

  double price = orf::knockoutFwd(spot, strike, kolevel, timeToExp, timeToKO,
    intRate, divYield, vol);

  return asPyScalar(price);
PY_END;
}
