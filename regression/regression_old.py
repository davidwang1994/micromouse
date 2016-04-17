import numpy as np
import matplotlib.pyplot as plt
import math
import scipy.io as sio

from scipy.optimize import curve_fit

data = sio.loadmat('measurements.mat')

X = data['readings']
Y = data['distValues']

X = np.array([float(x[0]) for x in X])
Y = np.array([float(y[0]) for y in Y])

def fit_poly_func(X, Y, d):
    A = np.ones((len(X), 2*d+1))
    for i in range(len(X)):
        A[i,:] = X[i]
    for i in range(-d, d+1):
        A[:,i] = A[:,i]**i
    Ainv = np.linalg.pinv(A)
    return np.dot(Ainv, Y)

def fit_log_func(X, d):
    A = np.ones((len(X), d))
    for i in range(len(X)):
        A[i,:] = X[i]
    for i in range(2):
        A[:,i] = A[:,i]**i
    Ainv = np.linalg.pinv(A)
    return np.dot(Ainv, Y)
#    return a * np.exp(-b * x) + c

def apply_poly_func(X, coef):
    Y = 0
    mid = len(coef) / 2
    for i in range(-mid, mid+1):
        Y += coef[i]*X**i
    return Y

def apply_log_func(X, a, b):
    return math.log(a) + b * X

def split_data(X, Y, thresh):
    index1 = np.where(X<thresh)
    index2 = np.where(X>=thresh)

    X1 = X[index1]
    X2 = X[index2]

    Y1 = Y[index1]
    Y2 = Y[index2]

    return X1, X2, Y1, Y2

def fit_poly_func_split(X, b1, b2, thresh):
    Y1 = 0
    Y2 = 0
    
    x1 = np.where(X<thresh)
    x2 = np.where(X>=thresh)

    X1 = X[x1]
    X2 = X[x2]

    for i in range(len(coef1)):
        Y1 += b[i]*X1**i
    for i in range(len(coef2)):
        Y2 += b[i]*X2**i

    size1 = X1.shape[0]
    size2 = X2.shape[0]

    Y = np.zeros((size1+size2))
    Y[:size1] = Y1
    Y[size1:] = Y2

    return Y 

def func(x, a, b, c):
    return a * np.exp(-b * x) + c

def log_func(X, a, b):
    return math.log(a) + b*X 

Y_log = np.log(X)

#popt, pcov = curve_fit(log_func, X, Y_log)

#Y_log_fit = apply_log_func(X, **popt)
#Y_log_fit = fit_log_func(X)

#d = 4
#Y_inv = 1 / Y
#b = fit_poly_func(X, Y_inv, d)
#Y_inv_fit = 1 / apply_poly_func(X, b)

d = 3
Y_inv = Y
b = fit_poly_func(X, Y_inv, d)
Y_inv_fit = apply_poly_func(X, b)
print b

#X_ = np.arange(0, 1, 0.01)
#Y_ = 1 / apply_poly_func(X_, b)

X_ = np.arange(0, 1, 0.01)
Y_ = apply_poly_func(X_, b)

d1 = 3
d2 = 2
thresh = 0.4
X1, X2, Y1, Y2 = split_data(X, Y_inv, thresh)
b1 = fit_poly_func(X1, Y1, d1)
b2 = fit_poly_func(X2, Y2, d2)
Y1_fit = 1 / apply_poly_func(X1, b1)
Y2_fit = 1 / apply_poly_func(X2, b2)

X1_, X2_, Y1_, Y2_ = split_data(X_, Y_, thresh)
Y1_fit_ = 1 / apply_poly_func(X1_, b1)
Y2_fit_ = 1 / apply_poly_func(X2_, b2)

#plt.yticks(range(0, 60, 1))

plt.figure()

plt.plot(X, Y, 'co-', label="Original Data")

plt.plot(X, Y_inv_fit, 'ro-', label="Inverse deg=" + str(d) + " Fit of Data")
plt.plot(X_, Y_, 'y-', label="Inverse deg=" + str(d) + " Function")

#plt.plot(X1, Y1_fit, 'b^-', label="Inverse deg=" + str(d1) + " [0, " + str(thresh) + "] Split Fit of Data")
#plt.plot(X2, Y2_fit, 'g^-', label="Inverse deg=" + str(d2) + " [" + str(thresh) + ", 1] Split Fit of Data")

#plt.plot(X1_, Y1_fit_, 'm^--', label="Inverse deg=" + str(d1) + " [0, " + str(thresh) + "] Split Fit of Data")
#plt.plot(X2_, Y2_fit_, 'r^--', label="Inverse deg=" + str(d2) + " [" + str(thresh) + ", 1] Split Fit of Data")

plt.legend()
plt.show()
