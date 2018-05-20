import numpy as np
from pylab import *
import matplotlib.pyplot as plt
from sklearn.neighbors import NearestNeighbors
from math import cos, sin, pi, radians
import cv2
from Icp2d import *
import vtk
from vtk import *


# ICP Function
# https://stackoverflow.com/questions/20120384/iterative-closest-point-icp-implementation-on-python
# def icp(a, b, init_pose=(0,0,0), no_iterations = 13):
#     src = np.array([a.T], copy=True).astype(np.float32)
#     dst = np.array([b.T], copy=True).astype(np.float32)
#
#     Tr = np.array([[np.cos(init_pose[2]),-np.sin(init_pose[2]),init_pose[0]],
#                    [np.sin(init_pose[2]), np.cos(init_pose[2]),init_pose[1]],
#                    [0,                    0,                   1          ]])
#
#     src = cv2.transform(src, Tr[0:2])
#     for i in range(no_iterations):
#         nbrs = NearestNeighbors(n_neighbors=1, algorithm='auto',
#                                 warn_on_equidistant=False).fit(dst[0])
#         distances, indices = nbrs.kneighbors(src[0])
#
#         T = cv2.estimateRigidTransform(src, dst[0, indices.T], False)
#
#         src = cv2.transform(src, T)
#
#         Tr = np.dot(Tr, np.vstack((T, [0,0,1])))
#
#     return Tr[0:2]


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
for i in range(len(xScanData)):
    if i == 0:
        indvSegmentData.append([xScanData[i], yScanData[i]])
        continue

    if scanNumbers[i] == scanNumbers[i-1]:
        indvSegmentData.append([xScanData[i], yScanData[i]])
    else:
        scanSegmentedData.append(indvSegmentData.copy())
        indvSegmentData.clear()
        indvSegmentData.append([xScanData[i], yScanData[i]])

# Convert data into numpy array
npScanSegmentData = np.array([np.array(xi) for xi in scanSegmentedData])

print("Rejections:\t%d" % rejections)
print("Scans:\t%d" % scanNumber)
print("Average Valid Points Per Scan:\t%f" % ((len(scanData)-rejections)/scanNumber))
print("Scan Time:\t%f seconds" % ((timestamps[-1] - timestamps[0])/(10**6)))

# Plot data as polar scatter plot
colors = intensities
# colors = scanNumbers
# ax = subplot(111, polar=True)
# p = scatter(angles, distances, c=colors, cmap=inferno(), s=5)
# p.set_alpha(0.75)
# plt.colorbar()
# show()

# Plot data as cartesian
# plt.scatter(xScanData, yScanData, c=colors, cmap=inferno(), s=5)
# plt.axis('equal')
# grid(True)
# plt.title('Line Scans')
# plt.xlabel('X (cm)')
# plt.ylabel("Y (cm)")
# plt.colorbar().set_label('Intensity')
# plt.show()

# Do ICP Here
# for i in range(1, len(npScanSegmentData)):
ret = icp(npScanSegmentData[1], npScanSegmentData[2])
# M2 = icp(npScanSegmentData[1], npScanSegmentData[2], [0, 0, 0])
# src = np.array([npScanSegmentData[1].T]).astype(np.float32)
# res = cv2.transform(src, M2)

# print(npScanSegmentData[3])
# x = npScanSegmentData[3][:,0]
# y = npScanSegmentData[3][:,1]
# print(x)
# print(" ")
# print(y)
# plt.scatter(x, y)
# plt.show()