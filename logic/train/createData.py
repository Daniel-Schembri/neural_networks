#!/usr/bin/env python 

import math
import os
import random
import sys

fileName = "trainingData.txt"

if __name__ == '__main__':
    if 4 != len(sys.argv):
        print """Missing topology arguments.
                 Usage: createData.py topology[0] topology[1] topology[2]"""
        sys.exit(1)

    params   = sys.argv[1:]
    topology = 'topology:' 

    for i in params:
        topology = topology + ' ' + str(i) 

    radius = 20.0
    semicircle = {0.0 : 0.0}

    xTmp = 0.0
    yTmp = 0.0

    # define semicircle
    for alpha in range(181):
            rad  = alpha/1.0  * math.pi/180
            xTmp = radius * math.cos(rad)
            yTmp = radius * math.sin(rad)

            semicircle[xTmp] = yTmp

    xList = sorted(semicircle.keys())
    results = [[]]

    for x in xList:
            yCurr = 0.0 
            yMax  = semicircle[x]
            stepSize = yMax / 10
            while(yCurr < yMax):
                    hyp      = math.sqrt(x**2 + yCurr**2)
                    velocity = hyp / radius
                    angle    = math.asin(yCurr/hyp)

                    results.append([x,yCurr,velocity,angle])
                    yCurr += stepSize

    # Avoid data in the same order as it was created
    random.shuffle(results)

    file = open(fileName, 'w+')
    file.write('{}\n'.format(topology))

    for r in results:
        if 0 < len(r):
            file.write('in: {0} {1}\nout: {2} {3}\n'.format(r[0], r[1], r[2], r[3]))
    file.close;
