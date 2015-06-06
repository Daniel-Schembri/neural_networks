import matplotlib.pyplot as plt
import math
import numpy as np

plt.rc('text', usetex=True)
plt.rc('font', family='serif')
plt.rcParams['text.latex.unicode']=True


labels = ['Hill climbing', 'Simulated Annealing', 'Crossover', 'Backpropagation']
label_index = 0

x = range(1,501)

a_avg = 0.0
b_avg = 0.0

for i in ['hc', 'sa', 'co', 'sv']:
    filename   = i
    a = []
    b = []

    f = open(filename, 'r')
    for line in f:
        line = line.strip()

        if ('A' == line[0]):
            a.append(line[3:])
            a_avg = float(a_avg) + float(a[-1])
        else:
            b.append(line[3:])
            b_avg = float(b_avg) + float(b[-1])

    a_avg = float(a_avg) / float(len(x))
    b_avg = float(b_avg) / float(len(x))

    a_avg = format(a_avg, '.2f')
    b_avg = format(b_avg, '.2f')

    if ('sv' == i):
        plt.plot(x, b, label = labels[label_index] + '. Average: ' + str(b_avg))
    else:
        plt.plot(x, b, label = labels[label_index] + '. Average: ' + str(b_avg))
#    plt.plot(x, b,  label = labels[label_index] + 'Average: ' + str(b_avg))

    label_index += 1

plt.title(r'Comparison of supervised and unsupervised learning.', fontsize=15)
plt.xlabel(r'Generations', fontsize=15)
plt.ylabel(r'Best fitness of a generation', fontsize=15)
plt.legend()
plt.grid(True)
plt.savefig('usub.pdf')
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
