import numpy as np
import matplotlib.pyplot as plt
import math
import scipy.io as sio
from scipy.optimize import curve_fit

def fit_poly_func(X, Y, d):
    A = np.ones((len(X), 2*d+1))
    for i in range(len(X)):
        A[i,:] = X[i]
    for i in range(-d, d+1):
        A[:,i] = A[:,i]**i
    Ainv = np.linalg.pinv(A)
    return np.dot(Ainv, Y)

def apply_poly_func(X, coef):
    Y = 0
    mid = len(coef) / 2
    for i in range(-mid, mid+1):
        Y += coef[i]*X**i
    return Y

data = sio.loadmat('measurements.mat')

X = data['readings']
Y = data['distValues']

X = np.array([float(x[0]) for x in X])
Y = np.array([float(y[0]) for y in Y])

d = 3
b = fit_poly_func(X, Y, d)
Y_fit = apply_poly_func(X, b)
print b

X_ = np.arange(0, 1, 0.01)
Y_ = apply_poly_func(X_, b)

plt.figure()

plt.plot(X, Y, 'co-', label="Original Data")

plt.plot(X, Y_fit, 'ro-', label="Inverse deg=" + str(d) + " Fit of Data")
plt.plot(X_, Y_, 'y-', label="Inverse deg=" + str(d) + " Function")

plt.legend()
plt.show()
