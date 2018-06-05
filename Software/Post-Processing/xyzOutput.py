from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib.pyplot import *
from math import cos, sin, pi, radians
import numpy as np

# open file with scan data
allData = open("testDataHallwaySkateboard.txt").read().split('\n')

# Fixed rotation
frame_rot = -pi/4
piece_rot = -4*pi/3

# Point at which the LIDAR is reflecting on itself and the bounds around which points are removed
reflection_point = pi/9
remove_range = radians(10)

# Minimum distance to pick up scans
min_distance = 20

# Split the data into their own datasets
scanData = []
imuData = []
for line in allData:
    if line.startswith("~S:") and line.endswith("~~"):
        scanData.append(line[3:-2].split(','))

# Transform the data into the correct types
timestamps = []
scanTimestamps = []
angles = []
distances = []
intensities = []
scanNumbers = []
xScanData = []
yScanData = []
zScanData = []
rejections = 0
scanNumber = 0
first_timestamp = int(scanData[0][0])
for i in range(len(scanData)):
    # Reject any invalid data
    if int(scanData[i][2]) < min_distance:
        rejections += 1
        continue

    # Cast the data as their proper types
    scanData[i][0] = (int(scanData[i][0]) - first_timestamp) * 10**-6
    scanTimestamps.append(scanData[i][0])
    scanData[i][1] = float(scanData[i][1])
    angles.append(scanData[i][1] + frame_rot)
    scanData[i][2] = int(scanData[i][2])
    distances.append(scanData[i][2])
    scanData[i][3] = int(scanData[i][3])
    intensities.append(scanData[i][3])

    # Clamps the outputs of the rotations, if the scan is greater than 130 degrees or less than 30 degrees drop the scan
    if reflection_point+remove_range > angles[-1] > reflection_point-remove_range:
        del scanTimestamps[-1]
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
    zScanData.append(distances[-1]*sin(angles[-1]))

    if i == 0:
        scanNumbers.append(scanNumber)
        continue
    if scanData[i][1] < float(scanData[i-1][1]):
        scanNumber += 1
        scanNumbers.append(scanNumber)
    else:
        scanNumbers.append(scanNumber)

xScanData = np.array(xScanData)
# Generate pseudo y timeseries for constant travel
yScanData = np.linspace(0, 500, len(xScanData))
zScanData = np.array(zScanData)


# Create list of lists containing the different scans
# This essentially captures different 'frames' for ICP
scanSegmentedData = []
indvSegmentData = []
for i in range(len(xScanData)):
    if i == 0:
        indvSegmentData.append([xScanData[i], zScanData[i]])
        continue

    if scanNumbers[i] == scanNumbers[i-1]:
        indvSegmentData.append([xScanData[i], zScanData[i]])
    else:
        scanSegmentedData.append(indvSegmentData.copy())
        indvSegmentData.clear()
        indvSegmentData.append([xScanData[i], zScanData[i]])


print("Rejections:\t%d" % rejections)
print("Scans:\t%d" % scanNumber)
print("Average Valid Points Per Scan:\t%f" % ((len(scanData)-rejections)/scanNumber))
print("Scan Time:\t%f seconds" % (scanTimestamps[-1] - scanTimestamps[0]))

# Plot data as polar scatter plot
colors = zScanData
# colors = scanNumbers
# Plot data as cartesian
fig = plt.figure();
ax = Axes3D(fig)
# ax = fig.add_subplot(111, projection='3d')
ax.scatter(xScanData, yScanData, zScanData, c=colors, cmap='jet')
ax.axis('equal')
# for ii in range(0,360,1):
#     ax.view_init(elev=20, azim=ii)
#     savefig("scanSpin\\movie%d.png" % ii)
plt.gca().invert_xaxis()
plt.show()
