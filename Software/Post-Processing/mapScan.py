import numpy as np
from pylab import *
import matplotlib.pyplot as plt
from math import cos, sin, pi, radians
import cv2

# open file with scan data
allData = open("testData.txt").read().split('\n')

# Fixed rotation
frame_rot = -pi/4
piece_rot = -4*pi/3

# Point at which the LIDAR is reflecting on itself and the bounds around which points are removed
reflection_point = pi/9
remove_range = radians(10)

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
    angles.append(scanData[i][1] + frame_rot)
    scanData[i][2] = int(scanData[i][2])
    distances.append(scanData[i][2])
    scanData[i][3] = int(scanData[i][3])
    intensities.append(scanData[i][3])

    # Clamps the outputs of the rotations, if the scan is greater than 130 degrees or less than 30 degrees drop the scan
    # print(pi/6, pi/18)
    # print(reflection_point+remove_range, reflection_point-remove_range)
    if reflection_point+remove_range > angles[-1] > reflection_point-remove_range:
    # if pi/6 > angles[-1] > pi/18:
        del timestamps[-1]
        del distances[-1]
        del intensities[-1]
        del angles[-1]
        rejections += 1
        continue
    else:
        pass

    # If the angles reported are out of range, rotate them back into their proper place
    # Rotate 240 degrees CCW if the angles is lower than 20 degrees
    if angles[-1] < reflection_point:
        angles[-1] = angles[-1] + piece_rot

    # Transform the angles and distances into x-y data
    xScanData.append(distances[-1]*cos(angles[-1]))
    yScanData.append(distances[-1]*sin(angles[-1]))

    if i == 0:
        scanNumbers.append(scanNumber)
        continue
    if scanData[i][1] < float(scanData[i-1][1]):
        scanNumber += 1
        scanNumbers.append(scanNumber)
    else:
        scanNumbers.append(scanNumber)

# Create list of lists containing the different scans
# This essentially captures different 'frames' for ICP
scanSegmentedData = []
indvSegmentData = []
for i in range(len(scanNumbers)):
    if i == 0:
        indvSegmentData.append([scanNumbers[i], timestamps[i], angles[i], distances[i], intensities[i]])
        continue

    if scanNumbers[i] == scanNumbers[i-1]:
        indvSegmentData.append([scanNumbers[i], timestamps[i], angles[i], distances[i], intensities[i]])
    else:
        scanSegmentedData.append(indvSegmentData.copy())
        indvSegmentData.clear()
        indvSegmentData.append([scanNumbers[i], timestamps[i], angles[i], distances[i], intensities[i]])


print("Rejections:\t", rejections)
print("Scans:\t", scanNumber)
print("Average Valid Points Per Scan:\t", (len(scanData)-rejections)/scanNumber)

# Plot data as polar scatter plot
colors = intensities
# colors = scanNumbers
# ax = subplot(111, polar=True)
# p = scatter(angles, distances, c=colors, cmap=inferno(), s=5)
# p.set_alpha(0.75)
# plt.colorbar()
# show()

# Plot data as cartesian
plt.scatter(xScanData, yScanData, c=colors, cmap=inferno(), s=5)
plt.colorbar()
plt.show()
