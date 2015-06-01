import matplotlib.pyplot as plt
import math
import numpy as np

plt.rc('text', usetex=True)
plt.rc('font', family='serif')
plt.rcParams['text.latex.unicode']=True

x = range(1,20001)
for i in ['t', 'f']:
    filename   = 'b' + i + 'a231.txt'
    y = [line.strip() for line in open(filename, 'r')]
    mlabel = ''
    if ('t' == i):
        mlabel = 'true'
    else:
        mlabel = 'false'

    plt.plot(x, y,  label= r'Bias: ' + mlabel)

plt.title(r'Comparison of network $net_{\beta}$ with and without bias neurons.', fontsize=15)
plt.xlabel(r'Training iterations', fontsize=15)
plt.ylabel(r'Error', fontsize=15)
plt.legend()
plt.grid(True)
plt.show()

#plot_margin = 0.075
#
#x0, x1, y0, y1 = plt.axis()
#plt.axis((x0,
#          x1,
#          y0 - plot_margin,
#          y1 + plot_margin))
#
#plt.xlabel(r'\textit{x}', fontsize=15)
#plt.ylabel(r'\textit{f(x)}', fontsize=15)
#plt.legend(loc=2)
#plt.grid(True)
#plt.xticks(np.arange(-10, 10, 2.5))
#plt.savefig('sigsc.pdf')
