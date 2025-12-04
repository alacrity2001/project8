ORFLIB Release Notes
====================

VERSION 0.10.0
-------------

### Additions

1. New files `orflib/pricers/ptpricers.hpp` and `ptpricers.cpp`.  
	They contain portfolio related functions.
  
2. New file `pyqflib/pyfunctions5.hpp` and additions to`pyqflib/qflib/__init__.py`.  
	implementation and registration of the following Python callable functions:  
   orf.ptRisk             (portfolio expected return and risk)  
   orf.mvpWghts           (mvp weights)  
   orf.mktWghts           (CAPM market portfolio weights)  
   orf.mktRisk            (CAPM market portfolio mean return and standard deviation)  


VERSION 0.9.0
-------------

### Additions

1. New file `orflib/products/americancallput.hpp`.  
	Definition of the class AmericanCallPut.

### Modifications

1. In files `pyorflib/pyfunctions4.hpp` and `pyorflib/orflib/__init__.py`.  
	Definition and registration of the Python function orf.amerBSPDE.


VERSION 0.8.0
-------------

### Additions

1. New folder `orflib/methods/pde`. It contains PDE related files.

2. New file `orflib/methods/pde/tridiagonalops1d.hpp`.  
	Definition of the class TridiagonalOperator and derived classes.

3. New file `orflib/methods/pde/pdegrid.hpp`.  
	Definition of the classes CoordinateChange and GridAxis.
	
4. New file `orflib/methods/pde/pdeparams.hpp`.  
	Definition of the class PdeParams.

5. New file `orflib/methods/pde/pderesults.hpp`.  
	Definition of the class PdeResults.

6. New file `orflib/math/interpol/interpolation1d.hpp`.  
	Definition of the class LinearInterpolation1D.

7. New files `orflib/methods/pde/pdebase.hpp` and `pdebase.cpp`.  
	Definition of the abstract base class PdeBase.
	
8. New files `orflib/methods/pde/pde1dsolver.hpp` and `pde1dsolver.cpp`.  
	Definition of the one spot dimension PDE solver.

9. In file `pyorflib/pyutils.hpp` added utility function asPdeParams.

10. New file `pyorflib/pyfunctions4.hpp`.  
	Definition of the Python wrapper function pyOrfEuroBSPDE, which is registered as orf.euroBSPDE



### Modifications

1. In file `orflib/products/product.hpp`.  
   Added method Product::timeSteps. It sets up the time steps to be used in the PDE solver.  

2. In file `orflib/products/product.hpp`.  
   Added method overload Product::eval. It evaluates the product at a specific point in time.  


VERSION 0.7.0
-------------

### Additions

1. New file `orflib/math/random/primitivepolynomials.hpp`  
	It contains the encoding of the first 3,666 primitive polynomials mod 2
  
2. New files `orflib/math/random/sobolurng.hpp` and `.cpp`  
	Implementation of the Sobol sequence generator.

3. New folder `orflib/math/linalg`  
	It will contain linear algebra related routines.

4. New files `choldcmp.cpp`, `eigensym.cpp`, `spectrunc.cpp` in folder `orflib/math/linalg`  
	They contain implementations for Cholesky decomposition, eigenvalues/eigenvectors of 
	a real symmetric matrix and spectral truncation of a non-positive definite correlation matrix.

5.  Added function declarations for above algorithms in `orflib/math/linalg/linalg.hpp`

6. New Python functions orf.cholDcmp(), orf.eigenSym() and orf.specTrunc() in `pyfunctions0.hpp`  
	They allow testing the Cholesky decomposition, diagonalization and spectral truncation of a correlation matrix.

7. New file `orflib/products/asianbasketcallput.hpp`  
	It defines the payoff of an Asian call or put option on a basket of assets.
  
8. New files `orflib/pricers/multiassetbsmcpricer.hpp` and `.cpp`  
	They define the multi-asset Black-Scholes Monte Carlo pricer class.
  
9. New Python function orf.asianBasketBSMC() implemented in the file `pyorflib/pyfunctions3.hpp`	
	Provides the Python interface to pricing Asian baskets in Black-Scholes MC.

### Modifications

1. Added typedef for Sobol generator in file `orflib/math/random/rng.hpp`

2. Added Cholesky decomposition and correlation spectral truncation to class PathGenerator

3. Widened the ctor of EulerPathGenerator to accept an optional correlation matrix

4. Added method nAssets() to class Product in file `orflib/products/product.hpp` and to class EuropeanCallPut

5. Extended the utility function asMcParams() to recognize more values for the URNGType and PathGenType settings

6. Added linking to f2c.lib, blas.lib and lapack.lib third party libraries in the `armadillo-14.6.0/lib` folder.  
   This affects only the pyorflib project.


VERSION 0.6.0
-------------

### Additions

1. New folder `orflib/math/random`  
	It will contain files for random number generation.
  
2. New file `orflib/math/random/normalrng.hpp`  
	It defines the NormalRng class template that generates random normal deviates.
  
3. New file `orflib/math/random/rng.hpp`  
	It contains typedefs for common combinations of distributions and underlying generators.  
	This file is to be included by any other file that requires random samples.
  
4. New folder `orflib/methods/montecarlo`  
	It will contain files specific to MC path generation.
  
5. New files `orflib/methods/montecarlo/pathgenerator.hpp` and `eulerpathgenerator.hpp`  
	The first file defines the abstract base PathGenerator and the second file defines a class template 
	that generates paths using the Euler method.
  
6. New file `orflib/methods/montecarlo/mcparams.hpp`  
	It defines a struct to be used for collecting MC parameters.

7. New folder `orflib/products`  
	It will contain files specific to product payoffs.

8. New files `orflib/products/product.hpp` and `orflib/products/europeancallput.hpp`  
	The abstact product base class and the European option product class.

9. New file `orflib/math/stats/statisticscalculator.hpp`  
	It defines the base class template for classes that compute statistics of samples.
  
10. New file `orflib/math/stats/meanvarcalculator.hpp`  
	It defines the mean-variance calculator.

11. New files  `orflib/pricers/bsmcpricer.hpp` and `bsmcpricer.cpp`  
	They define the Black-Scholes Monte Carlo pricer class.
    
12. New Python file `pyqflib/pyfunctions3.cpp`  
	It implements the Python callable C++ function pyOrfEuroBSMC.

13. New Python callable function orf.euroBSMC.

14. Added solutions to homeworks 1-5.


VERSION 0.5.0
-------------

### Additions

1. New utility functions toContCmpd and fromContCpd in file `orflib/utils.hpp`  
   They convert an interest rate to/from  continuous compounding from/to compounding with a given annual frequency.

2. New folder `orflib/math/optim`  
  It contains root finding and optimization routines.
	
3. New file `orflib/math/optim/roots.hpp`  
  It contains the root finding routines zbrak and rtsec from NR C++ 3ed.

4. New file `orflib/math/optim/polyfunc.hpp`  
  It defines a class that models a polynomial.

5. New Python callable functions orf.polyBracket, orf.polySecant, orf.toContCmpd, orf.fromContCmpd


VERSION 0.4.0
-------------

### Additions

1. New file `orflib/sptr.hpp`
	Type definions for shared pointers.

2. New file `orflib/sptrmap.hpp`
	It defines the SPtrMap class template.

3. New folder `orflib/market` with two new files `yieldcurve.hpp` and `yieldcurve.cpp`.  
	They define and implement the YieldCurve class.

4. Two new files `orflib/market/market.hpp` and `market.cpp`  
	They define and implement the Market singleton class.

5. New file `pyorflib/pyfunctions2.cpp` and additions to `pyorflib/module.cpp`
    and `pyorflib/qflib/__init__.py`. 
    They implement and register the following Python callable functions  
	orf.mktList,  orf.mktClear, orf.ycCreate, orf.discount, orf.fwdDiscount, orf.spotRate, orf.fwdRate  


VERSION 0.3.0
-------------

### Additions

1. New file `orflib/math/matrix.hpp`.   
  It defines aliases for the Vector and Matrix classes to be used in numerical work.

2. New folder `orflib/math/interpol` with two new files `piecewisepolynomial.hpp` and `piecewisepolynomial.cpp`.  
	They implement the class PiecewisePolynomial.

3. New file `pyorflib/pyutils.hpp`.  
	It implements the utility functions asVector(), asMatrix(), and asNumpy().

4. In file `pyorflib/pyfunctions0.hpp` added functions:  
    pyOrfEchoMatrix(), pyOrfPPolyEval(), pyOrfPPolyIntegral().

5. In file `pyorflib/orflib/__init__.py` added Python callable functions:   
	orf.echoMatrix(), orf.ppolyEval(), orf.ppolyIntegral()

### Modifications

1. Orflib now has dependency on the armadillo C++ matrix library. 



VERSION 0.2.0
-------------

### Additions

1. New file `orflib/exception.hpp`.  
	Definition of orf::Exception class and the ORF_ASSERT macro.

2. New folder `orflib/math/stats` to contain statistics related source files.

3. New files `orflib/math/stats/errorfunction.hpp` and `errorfunction.cpp`.  
	Definition of class ErrorFunction.

4. New files `orflib/math/stats/univariatedistribution.hpp` and `normaldistribution.hpp`.  
	Definition of `UnivariateDistribution` and `NormalDistribution` classes.

5. New folder `orflib/math/pricers` to contain pricing related source files.

6. New files `orflib/math/pricers/simplepricers.hpp` and `simplepricers.cpp`.  
	Definition of the functions fwdPrice(), digitalOptionBS() and europeanOptionBS().

7. In file `pyorflib/pyfunctions0.hpp` added functions:  
    pyOrfErf(), pyOrfInvErf(), pyOrfNormalCdf(), pyOrfNormalInvCdf()
 
8. New file `pyorflib/pyfunctions1.hpp`.  
	Added definition of the following functions:  
	pyOrfFwdPrice(), pyOrfDigiBS() and pyOrfEuroBS().

9. In file `pyorflib/orflib/__init__.py` added Python callable functions:   
	orf.normalCdf(), orf.normalInvCdf(), orf.fwdPrice(), orf.digiBS(), orf.euroBS().


### Modifications

1. Removed unnecessary file `orflib/empty.cpp`


VERSION 0.1.0
-------------

### Additions

1. Top level CMakeLists.txt file with projet-wide settings

2. New folder `orflib` with CMakeLists.txt and start-up code for the core library.

3. New folder `pyorflib` with CMakeLists.txt and start-up code for the Python interface.

4. This release notes file.

5. `.gitignore` file for managing which files to keep under revision control.
