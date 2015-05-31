import matplotlib.pyplot as plt
import math
import numpy as np

def sigf(x):
    result = []
    for item in x:
        result.append(1.0/(1.0 + math.exp(-item)))
    return result 

def sigsh(x, sh):
    result = []
    for item in x:
        new_x = item + sh*1.0
        result.append((1.0/(1.0 + math.exp(-new_x))))
    return result 

def sigsc(x, sc):
    result = []
    for item in x:
        new_x = sc*item
        result.append((1.0/(1.0 + math.exp(-new_x))))
    return result 

def tanhf(x):
    result = []
    for item in x:
        result.append(math.tanh(item))
    return result 

plt.rc('text', usetex=True)
plt.rc('font', family='serif')
plt.rcParams['text.latex.unicode']=True

x    = np.arange(-10, 10, 0.1)
sigsc1  = sigsc(x, 0.5)
sigsc2  = sigsc(x, 1.0)
sigsc3  = sigsc(x, 2.0)
#sigsh1  = sigsh(x, -5)
#sigsh2  = sigsh(x,  0)
#sigsh3  = sigsh(x,  5)
#tanh = tanhf(x)
#step = stepf(x)

#plt.plot(x, sigsh1,  label=r'$sig(1 \cdot x + -5 \cdot 1.0)$')
#plt.plot(x, sigsh2,  label=r'$sig(1 \cdot x +  0 \cdot 1.0)$')
#plt.plot(x, sigsh3,  label=r'$sig(1 \cdot x +  5 \cdot 1.0)$')
plt.plot(x, sigsc1,  label=r'$sig(0.5 \cdot x)$')
plt.plot(x, sigsc2,  label=r'$sig(1 \cdot x)$')
plt.plot(x, sigsc3,  label=r'$sig(2.0 \cdot x)$')
#plt.plot(x, tanh,  label=r'$tanh(x)$')
#x = range(-1,2)
#y = 2 * [0] + 1 * [1]
#plt.step(x, y, label=r'$step(x)$')

x0, xy, yo, y1 = plt.axis()

plot_margin = 0.075

x0, x1, y0, y1 = plt.axis()
plt.axis((x0,
          x1,
          y0 - plot_margin,
          y1 + plot_margin))

plt.xlabel(r'\textit{x}', fontsize=15)
plt.ylabel(r'\textit{f(x)}', fontsize=15)
plt.legend(loc=2)
plt.grid(True)
plt.xticks(np.arange(-10, 10, 2.5))
plt.savefig('sigsc.pdf')
