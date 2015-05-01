#!/usr/bin/env python 

import math
import os
import random
import sys

fileName = "trainingData.txt"

if __name__ == '__main__':
    if 5 != len(sys.argv):
        print """Missing arguments.
                 Usage: createData.py topology[0] topology[1] topology[2] samplesize"""
        sys.exit(1)

    params   = sys.argv[1:-1]
    samples  = int(sys.argv[-1])

    topology = 'topology:' 

    for i in params:
        topology = topology + ' ' + str(i) 


    radius = 1.0
    results = [[]]

    i = 0
    for i in range(samples):
        xCurr = random.uniform(-radius, radius)
        yMax  = math.sqrt(radius**2 - xCurr**2)
        yCurr = random.uniform(0.0, yMax)
        
        hyp   = math.sqrt(xCurr**2 + yCurr**2)
#        angle from x-axis
#        angle = math.atan2(yCurr, xCurr)
#        angle between vector and x-axis
#        angle = math.asin(yCurr/hyp)
#        angle from y-axis
        angle = math.asin(yCurr/hyp)# - math.pi/2

        results.append([xCurr, yCurr, hyp, angle])

    # And another factor of randomness
    random.shuffle(results)

    file = open(fileName, 'w+')
    file.write('{}\n'.format(topology))

    for r in results:
        if [] != r:
#            file.write('in: {0} {1}\nout: {2} {3}\n'.format(r[0], r[1], r[2], r[3]))
            file.write('in: {0} {1}\nout: {2}\n'.format(r[0], r[1], r[3]))
    file.close;
