import numpy as np
from pylab import *
import matplotlib.pyplot as plt
from math import cos, sin, pi
import cv2

# open file with scan data
allData = open("testDataBright90.txt").read().split('\n')

# Fixed rotation
rot = -pi/4

# Split the data into their own datasets
scanData = []
imuData = []
for line in allData:
    if line.startswith("~S:") and line.endswith("~~"):
        scanData.append(line[3:-2].split(','))

# Transform the data into the correct types
timestamps = []
angles = []
distances = []
intensities = []
scanNumbers = []
xScanData = []
yScanData = []
rejections = 0
scanNumber = 0
for i in range(len(scanData)):
    # Reject any invalid data
    if int(scanData[i][2]) < 30:
        rejections += 1
        continue

    # Cast the data as their proper types
    scanData[i][0] = int(scanData[i][0])
    timestamps.append(scanData[i][0])
    scanData[i][1] = float(scanData[i][1])
    angles.append(scanData[i][1]+rot)
    scanData[i][2] = int(scanData[i][2])
    distances.append(scanData[i][2])
    scanData[i][3] = int(scanData[i][3])
    intensities.append(scanData[i][3])

    # Euler angle the angles

    # Transform the angles and distances into x-y data
    xScanData.append(distances[i-rejections]*cos(angles[i-rejections]))
    yScanData.append(distances[i-rejections]*sin(angles[i-rejections]))

    if i == 0:
        scanNumbers.append(scanNumber)
        continue
    if scanData[i][1] < float(scanData[i-1][1]):
        scanNumber += 1
        scanNumbers.append(scanNumber)
    else:
        scanNumbers.append(scanNumber)

print("Rejections:\t", rejections)
print("Scans:\t", scanNumber)
print("Points Per Scan:\t", len(scanData)/scanNumber)

# Plot data as polar scatter plot
colors = intensities
ax = subplot(111, polar=True)
p = scatter(angles, distances, c=colors, cmap=inferno())
p.set_alpha(0.75)
plt.colorbar()
show()

# Plot data as cartesian
# plt.scatter(xScanData, yScanData, c=colors)
# plt.colorbar()
# plt.show()
