/** 
@file  pyfunctions0.hpp
@brief Implementation of Python callable functions
*/
#include "pyutils.hpp"
#include <orflib/defines.hpp>
#include <orflib/math/stats/errorfunction.hpp>
#include <orflib/math/stats/normaldistribution.hpp>
#include <orflib/math/interpol/piecewisepolynomial.hpp>
#include <orflib/math/optim/polyfunc.hpp>
#include <orflib/math/optim/roots.hpp>
#include <orflib/math/linalg/linalg.hpp>
#include <orflib/utils.hpp>
#include <string>

static 
PyObject*  pyOrfVersion(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  return asPyScalar(std::string(ORF_VERSION_STRING));
PY_END;
}

static 
PyObject* pyOrfSayHello(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyArg1))
    return NULL;
  std::string name = asString(pyArg1);        // read in the name
  std::string greet = "Hello " + name + "!";  // compose the greeting  
  return asPyScalar(greet);
PY_END;
}


static 
PyObject* pyOrfOuterProd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  PyObject* pyArg2(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyArg1, &pyArg2))
    return NULL;

  std::vector<double> vec1 = asDblVec(pyArg1);
  std::vector<double> vec2 = asDblVec(pyArg2);
  size_t nrow = vec1.size();
  size_t ncol = vec2.size();
  
  // allocate and compute the answer
  std::vector<std::vector<double>> outprod(nrow);
  for (size_t i = 0; i < nrow; ++i) {
	  outprod[i].resize(ncol);
	  for (size_t j = 0; j < ncol; ++j) {
		  outprod[i][j] = vec1[i] * vec2[j];
	  };
  };
	
  return asPyArray(outprod);

PY_END;
}

static 
PyObject* pyOrfPolyProd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  PyObject* pyArg2(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyArg1, &pyArg2))
    return NULL;

  std::vector<double> pvec = asDblVec(pyArg1);
  std::vector<double> qvec = asDblVec(pyArg2);
  size_t m = pvec.size();
  if (m == 0) 
    throw std::invalid_argument("p vector is empty");
  size_t n = qvec.size();
  if (n == 0) 
    throw std::invalid_argument("q vector is empty");

  size_t p = m + n - 1;  // size of the product vector
  std::vector<double> prod(p, 0.0);
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      prod[i + j] += pvec[i] * qvec[j];
    }
  }
  // find the index of the last non-zero element of the prod vector
  size_t k = p - 1;
  while (k > 0 && prod[k] == 0)
    --k;

  // allocate and compute the answer
  std::vector<double> polyprod(k + 1);
  for (size_t i = 0; i <= k; ++i) {
    polyprod[i] = prod[i];
  };

  return asPyArray(polyprod);

PY_END;
}

static
PyObject*  pyOrfErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(orf::ErrorFunction::erf(x));
PY_END;
}


static
PyObject*  pyOrfInvErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(orf::ErrorFunction::inverf(x));
PY_END;
}


static
PyObject*  pyOrfNormalCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  orf::NormalDistribution f;

  return asPyScalar(f.cdf(x));
PY_END;
}

static
PyObject*  pyOrfNormalInvCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  orf::NormalDistribution f;

  return asPyScalar(f.invcdf(x));
PY_END;
}

static 
PyObject*  pyOrfEchoMatrix(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;

  orf::Matrix mat = asMatrix(pyMat);
  return asNumpy(mat);
PY_END;
}

static 
PyObject*  pyOrfPPolyEval(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXVec(NULL);
  PyObject* pyDerivOrder(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXVec, &pyDerivOrder))
    return NULL;

  orf::Vector bkpts = asVector(pyBkPoints);
  orf::Vector vals = asVector(pyValues);
  int degree = asInt(pyPolyOrder);
  orf::Vector xx = asVector(pyXVec);
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int derivOrder = asInt(pyDerivOrder);

  // create the curve
  orf::PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);

  orf::Vector yy(xx.size());
  pp.eval(xx.begin(), xx.end(), yy.begin(), derivOrder);

  return asNumpy(yy);
PY_END;
}

static 
PyObject*  pyOrfPPolyIntegral(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXStart(NULL);
  PyObject* pyXVecEnd(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXStart, &pyXVecEnd))
    return NULL;

  orf::Vector bkpts = asVector(pyBkPoints);
  orf::Vector vals = asVector(pyValues);
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int degree = asInt(pyPolyOrder);
  double x0 = asDouble(pyXStart);
  orf::Vector xx = asVector(pyXVecEnd);
  // create the curve
  orf::PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);
  // integrate
  orf::Vector yy(xx.size());
  pp.integral(x0, xx.begin(), xx.end(), yy.begin());

  return asNumpy(yy);
PY_END;
}

static 
PyObject*  pyOrfPPolySum(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints1(NULL);
  PyObject* pyValues1(NULL);
  PyObject* pyBkPoints2(NULL);
  PyObject* pyValues2(NULL);
  PyObject* pyPolyOrder(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints1, &pyValues1, &pyBkPoints2, &pyValues2, &pyPolyOrder))
    return NULL;

  orf::Vector bkpts1 = asVector(pyBkPoints1);
  orf::Vector vals1 = asVector(pyValues1);
  ORF_ASSERT(bkpts1.size() == vals1.size(), "unequal number of breakpoints and vals");
  orf::Vector bkpts2 = asVector(pyBkPoints2);
  orf::Vector vals2 = asVector(pyValues2);
  ORF_ASSERT(bkpts2.size() == vals2.size(), "unequal number of breakpoints and vals");
  int order = asInt(pyPolyOrder);
  ORF_ASSERT(order < 2, "only polynomials of order 0 or 1 can be added with this function");

  // create the curves
  orf::PiecewisePolynomial pp1(bkpts1.begin(), bkpts1.end(), vals1.begin(), order);
  orf::PiecewisePolynomial pp2(bkpts2.begin(), bkpts2.end(), vals2.begin(), order);
  // add them
  orf::PiecewisePolynomial psum = pp1 + pp2;
  orf::Vector const& bkpts = psum.breakPoints();
  orf::Vector vals(bkpts.size());
  psum.eval(bkpts.begin(), bkpts.end(), vals.begin(), 0);
  
  // collect breakpoints and values of the sum in a Matrix and return it
  orf::Matrix ret(bkpts.size(), 2);
  ret.col(0) = bkpts;
  ret.col(1) = vals;
  return asNumpy(ret);
PY_END;
}

static
PyObject*  pyOrfPolyBracket(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyCoeffs(NULL);
  PyObject* pyLowerLimit(NULL);
  PyObject* pyUpperLimit(NULL);
  PyObject* pyNSubs(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyCoeffs, &pyLowerLimit, &pyUpperLimit, &pyNSubs))
    return NULL;

  orf::Vector coeffs = asVector(pyCoeffs);
  double xlo = asDouble(pyLowerLimit);
  double xhi = asDouble(pyUpperLimit);
  int nsubs = asInt(pyNSubs);

  orf::Polynomial p(coeffs);
  orf::Vector xb1, xb2;
  int nroot;
  orf::zbrak(p, xlo, xhi, nsubs, xb1, xb2, nroot);

  orf::Matrix bkts(nroot, 2);
  for (int i = 0; i < nroot; ++i) {
    bkts(i, 0) = xb1[i];
    bkts(i, 1) = xb2[i];
  };
  return asNumpy(bkts);
PY_END;
}

static
PyObject*  pyOrfPolySecant(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyCoeffs(NULL);
  PyObject* pyLowerLimit(NULL);
  PyObject* pyUpperLimit(NULL);
  PyObject* pyTol(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOO", &pyCoeffs, &pyLowerLimit, &pyUpperLimit, &pyTol))
    return NULL;

  orf::Vector coeffs = asVector(pyCoeffs);
  double xlo = asDouble(pyLowerLimit);
  double xhi = asDouble(pyUpperLimit);
  double tol = asDouble(pyTol);

  orf::Polynomial p(coeffs);
  double root = orf::rtsec(p, xlo, xhi, tol);
  return asPyScalar(root);
PY_END;
}

static
PyObject*  pyOrfToContCmpd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyRate(NULL);
  PyObject* pyAnnFreq(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyRate, &pyAnnFreq))
    return NULL;

  double ret = orf::toContCmpd(asDouble(pyRate), asLong(pyAnnFreq));
  return asPyScalar(ret);
PY_END;
}

static
PyObject*  pyOrfFromContCmpd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyRate(NULL);
  PyObject* pyAnnFreq(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyRate, &pyAnnFreq))
    return NULL;

  double ret = orf::fromContCmpd(asDouble(pyRate), asLong(pyAnnFreq));
  return asPyScalar(ret);
PY_END;
}

static
PyObject*  pyOrfCholDcmp(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;
  orf::Matrix inmat = asMatrix(pyMat);
  orf::Matrix outmat;
  orf::choldcmp(inmat, outmat);
  return asNumpy(outmat);
PY_END;
}

static 
PyObject*  pyOrfEigenSym(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;
  orf::Matrix mat = asMatrix(pyMat);
  orf::Vector eigenvals;
  orf::Matrix eigenvecs;
  orf::eigensym(mat, eigenvals, eigenvecs);

  // write eigenvalues and eigenvectors into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Eigenvals"), asNumpy(eigenvals));
  ok = PyDict_SetItem(ret, asPyScalar("Eigenvecs"), asNumpy(eigenvecs));
  return ret;

PY_END;
}

static 
PyObject*  pyOrfSpecTrunc(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;

  orf::Matrix mat = asMatrix(pyMat);
  orf::spectrunc(mat);

  return asNumpy(mat);
PY_END;
}
