#!/usr/bin/env python 

import math
import os

fileName = "trainingData.txt"

if __name__ == '__main__':
	radius = 20.0
	halbKreis = {0.0 : 0.0}

	xTmp = 0.0
	yTmp = 0.0

	# define halbKreis
	for alpha in range(181):
		rad  = alpha * math.pi/180
		xTmp = radius * math.cos(rad)
		yTmp = radius * math.sin(rad)

		halbKreis[xTmp] = yTmp

	xList    = sorted(halbKreis.keys())
	file = open(fileName, 'w+')

	for x in xList:
		yCurr = 0.0 
		yMax  = halbKreis[x]
		stepSize = yMax /20
		while(yCurr < yMax):
			hyp      = math.sqrt(x**2 + yCurr**2)
			velocity = hyp / radius
			angle    = math.asin(yCurr/hyp)

			file.write('{0};{1};{2};{3}\n'.format(x,yCurr,velocity,angle))
			yCurr += stepSize

	file.close;
