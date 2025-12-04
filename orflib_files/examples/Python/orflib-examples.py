# orflib library examples

#%%
import orflib as orf
import numpy as np
import os

ver = orf.version()
print(f'orflib version: {ver}')
pid = os.getpid()
print(f'pid: {pid}')

# Greet
print('----------------')
name = "World"
print(orf.sayHello(name))

# Outer product
print('----------------')
x = [1, 2, 3]
y = [4, 5]
op = orf.outerProd(x, y)
print(f'x: {x}\ny: {y}')
print(f'outerProd:\n{op}')

# Matrix
print('----------------')
m = np.array([[1, 2, 3], [4, 5, 6]])
em = orf.echoMatrix(m)
print(f'orig matrix:\n{m}')
print(f'echo matrix:\n{em}')

#PPolyEval
print('----------------')
xbpt = np.arange(1, 6)
yval =  np.arange(-10, 15, 5)
pord = 1
xval = np.arange(0.5, 6.0, 0.5)
pval = orf.ppolyEval(xbpt, yval, pord, xval, 0)
pder = orf.ppolyEval(xbpt, yval, pord, xval, 1)
pint = orf.ppolyIntegral(xbpt, yval, pord, xval[0], xval)
print('Piecewise polynomial')
print(f'bkpts={xbpt}')
print(f'yvals={yval}')
print(f'pord={pord}')
print(f'xval={xval}')
print(f'pval={pval}')
print(f'pder={pder}')
print(f'pint={pint}')

#Root bracketing and secant method search
print('----------------')
pcoeffs = [0, 2, 1]
lolim = -4
uplim = 3
nsubs = 10

brkts = orf.polyBracket(pcoeffs, lolim, uplim, nsubs)
print(f"Roots of polynomial: {pcoeffs}")
print(f"Root brackets:\n {brkts}")

root = orf.polySecant(pcoeffs, brkts[0, 0], brkts[0, 1], 1e-12)
print(f"root: {root:.4f}")

#ToContCmpd, FromContComp
print('----------------')
anfreq = 1
inrate = 0.10
ccrate = orf.toContCmpd(inrate, anfreq)
outrate = orf.fromContCmpd(ccrate, anfreq)
print('To and from continuous compounding')
print(f'InRate={inrate:.4f}, CCRate={ccrate:.4f}, PerRate={outrate:.4f}')

#CholDcmp
inmat = np.array([[4, 12, -16], [12, 37, -43], [-16, -43, 98]], dtype = 'float')
lmat = orf.cholDcmp(inmat)
outmat = np.dot(lmat, lmat.T)
print('Cholesky decomposition')
print(inmat)
print(outmat)

#EigenSym
inmat = np.array([[4, 12, -16], [12, 37, -43], [-16, -43, 98]], dtype = 'float')
eigen = orf.eigenSym(inmat)
print('Eigenvalues and eigenvectors of symmetric matrix')
print('Eigenvalues = ', eigen['Eigenvals'])
print('Eigenvectors = ')
print(eigen['Eigenvecs'])

#SpecTrunc
incormat = np.array([[1, 0.2, -0.8], [0.2, 1, 0.5], [-0.8, 0.5, 1]])
print('Spectral truncation of correlation matrix')
print(incormat)
print(orf.specTrunc(incormat))

#%%
# function group 1
print('=================')
print('Analytic prices')

fwdpx = orf.fwdPrice(spot = 100, timetoexp = 1.0, intrate = 0.02, divyield = 0.04)
print(f'FwdPrice={fwdpx:.4f}')

digi = orf.digiBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                 intrate = 0.04, divyield = 0.02, volatility = 0.2)[0]
print('Digital call using Black-Scholes analytic solution')
print(f'Price={digi:.4f}')

euro = orf.euroBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                intrate = 0.04, divyield = 0.02, volatility = 0.4)[0]
print('European call using Black-Scholes analytic solution')
print(f'Price={euro:.4f}')

#%%
# function group 2
print('================')
print('Market objects')

#yccreate
yc = orf.ycCreate(ycname = 'USD', 
                 tmats =  [1/12,  1/4,  1/2,   3/4,    1,     2,    3,     4,    5,      10],
                 vals = [0.01,   0.02, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.0575, 0.065],
                 valtype = 0)
print(f'Created yield curve: {yc}')

#discount, spotrate, fwdrate
df = orf.discount(ycname = yc, tmat = 2)
spotrate = orf.spotRate(ycname = yc, tmat = 2)
fwdrate = orf.fwdRate(ycname = yc, tmat1 = 1, tmat2 = 2)
print(f'DF={df:.4f}, SpotRate={spotrate:.4f} FwdRate={fwdrate:.4f}')

print('Market list')
print(orf.mktList())

#%%
# function group 3
print('=================')
print('European option using Black-Scholes Monte Carlo')

#eurobsmc
mcpars0 = {'URNGTYPE': 'MT19937', 'PATHGENTYPE': 'EULER'}
npaths0 = 1000000
euromc0 = orf.euroBSMC(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                       discountcrv =  yc, divyield = 0.02, volatility = 0.4,
                       mcparams = mcpars0, npaths = npaths0)
print(f'URNGTYPE={mcpars0["URNGTYPE"]} PATHGENTYPE={mcpars0["PATHGENTYPE"]} NPATHS={npaths0}')
print(f'Price={euromc0['Mean']:0.4f}  StdErr={euromc0['StdErr']:0.4f}')

#eurobsmc with Sobol
mcpars1 = {'URNGTYPE': 'SOBOL', 'PATHGENTYPE': 'EULER'}
euromc1 = orf.euroBSMC(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                       discountcrv =  yc, divyield = 0.02, volatility = 0.4,
                       mcparams = mcpars1, npaths = npaths0)
print(f'URNGTYPE={mcpars1["URNGTYPE"]} PATHGENTYPE={mcpars1["PATHGENTYPE"]} NPATHS={npaths0}')
print(f'Price={euromc1['Mean']:0.4f}  StdErr={euromc1['StdErr']:0.4f}')

print('=================')
print('Asian basket option using Black-Scholes Monte Carlo')

#asianbasketbsmc
fixtimes = [  0.5,   1.0,   1.5,    2.0]
assqts   = [  0.2,   0.2,   0.2,   0.2,   0.2]
spots    = [100.0, 100.0, 100.0, 100.0, 100.0]
divylds  = [ 0.02,  0.02,  0.02,  0.02,  0.02]
vols     = [ 0.30,  0.30,  0.30,  0.30,  0.30]
correls  = np.array([
     [1.0,  0.5,  0.7, 0.6, 0.6],
     [0.5,  1.0,  0.8, 0.7, 0.5],
     [0.7,  0.8,  1.0, 0.7, 0.5], 
     [0.6,  0.7,  0.7, 1.0, 0.6],
     [0.6,  0.5,  0.5, 0.6, 1.0]])

mcpars1 = {'URNGTYPE': 'MT19937', 'PATHGENTYPE': 'EULER'}
npaths1 = 200000
asianmc = orf.asianBasketBSMC(payofftype = 1, strike = 100, fixtimes = fixtimes, 
                             assetquantities = assqts, spots = spots,
                             discountcrv =  yc, divyields = divylds, 
                             volatilities= vols, correlmat = correls,
                             mcparams = mcpars1, npaths = npaths1)
print(f'URNGTYPE={mcpars1["URNGTYPE"]} PATHGENTYPE={mcpars1["PATHGENTYPE"]} NPATHS={npaths1}')
print(f'Price={asianmc['Mean']:0.4f}  StdErr={asianmc['StdErr']:0.4f}')

#%%
# function group 4
print('=================')
print('European option using Black-Scholes PDE')

pdepars0 = {'NTIMESTEPS': 50, 'NSPOTNODES': 50, 'NSTDDEVS': 4, 'THETA': 0}

europde0 = orf.euroBSPDE(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                        discountcrv =  yc, divyield = 0.02, volatility = 0.4, pdeparams = pdepars0)

print(f'NTIMESTEPS={pdepars0["NTIMESTEPS"]} NSPOTNODES={pdepars0["NSPOTNODES"]} NSTDDEVS={pdepars0["NSTDDEVS"]} THETA={pdepars0["THETA"]}')
print(f'Price={europde0["Price"]:0.4f}')

pdepars1 = {'NTIMESTEPS': 50, 'NSPOTNODES': 50, 'NSTDDEVS': 4, 'THETA': 1}

europde1 = orf.euroBSPDE(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                         discountcrv =  yc, divyield = 0.02, volatility = 0.4, pdeparams = pdepars1)

print(f'NTIMESTEPS={pdepars1["NTIMESTEPS"]} NSPOTNODES={pdepars1["NSPOTNODES"]} NSTDDEVS={pdepars1["NSTDDEVS"]} THETA={pdepars1["THETA"]}')
print(f'Price={europde1["Price"]:0.4f}')

pdepars2 = {'NTIMESTEPS': 50, 'NSPOTNODES': 50, 'NSTDDEVS': 4, 'THETA': 0.5}

europde2 = orf.euroBSPDE(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                         discountcrv =  yc, divyield = 0.02, volatility = 0.4, pdeparams = pdepars2)

print(f'NTIMESTEPS={pdepars2["NTIMESTEPS"]} NSPOTNODES={pdepars1["NSPOTNODES"]} NSTDDEVS={pdepars2["NSTDDEVS"]} THETA={pdepars2["THETA"]}')
print(f'Price={europde2["Price"]:0.4f}')

# exact price
eurobs = orf.euroBS(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                   intrate = orf.spotRate(yc, 1.0), divyield = 0.02, volatility = 0.4)[0]

print(f'BSPrice={eurobs:0.4f}')

print('=================')
print('American option using Black-Scholes PDE')
pdeparams = {'NTIMESTEPS': 800, 'NSPOTNODES': 800, 'NSTDDEVS': 4, 'THETA': 0.5}
paytype = 1
opttype = 'call' if paytype == 1 else 'put'

amerpde = orf.amerBSPDE(payofftype = paytype, strike = 100, timetoexp = 1.0, spot = 100,
                        discountcrv =  yc, divyield = 0.02, volatility = 0.4, pdeparams = pdeparams)

print(f'NTIMESTEPS={pdeparams["NTIMESTEPS"]} NSPOTNODES={pdeparams["NSPOTNODES"]} NSTDDEVS={pdeparams["NSTDDEVS"]} THETA={pdeparams["THETA"]}\n')
print(f'American {opttype} option:')
print(f'Price={amerpde["Price"]:0.4f}')

# compare with corresponding European
europde = orf.euroBSPDE(payofftype = paytype, strike = 100, timetoexp = 1.0, spot = 100,
                        discountcrv =  yc, divyield = 0.02, volatility = 0.4, pdeparams = pdeparams)
print(f'European {opttype} option:')
print(f'Price={europde["Price"]:0.4f}')

#%%
# function group 5
print('=========================')
print('Portfolio return and risk')

assetrets = [0.10, 0.115, 0.08, 0.06]
assetvols = [0.20, 0.30, 0.22, 0.18]
correlmat = np.array([
    [1.0,  0.10, 0.20, 0.30],
    [0.10,  1.0, 0.25, 0.35],
    [0.20, 0.25,  1.0, 0.15],
    [0.30, 0.35, 0.15,  1.0]])

rfreerate = 0.04

mvpwts = orf.mvpWghts(assetrets, assetvols, correlmat)

mvprsk = orf.ptRisk(mvpwts, assetrets, assetvols, correlmat)

mktwts = orf.mktWghts(assetrets, assetvols, correlmat, rfreerate)

mktrsk = orf.mktRisk(assetrets, assetvols, correlmat, rfreerate)

with np.printoptions(precision=4, suppress=True):
     print('MVP Weights =', mvpwts)
print(f'MVP Return = {mvprsk['Mean']:0.4f}')
print(f'MVP Risk = {mvprsk['StdDev']:0.4f}')

with np.printoptions(precision=4, suppress=True):
     print('\nMarket Weights =', mktwts)
print(f'Market Return = {mktrsk['Mean']:0.4f}')
print(f'Market Risk = {mktrsk['StdDev']:0.4f}')
print(f'Market Lambda = {mktrsk['Lambda']:0.4f}')
