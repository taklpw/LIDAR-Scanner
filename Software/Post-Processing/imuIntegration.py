import numpy as np
from scipy.signal import lfilter, butter
from scipy.integrate import simps, cumtrapz
from pylab import *
import matplotlib.pyplot as plt
from scipy.constants import g

file_in = "testDataHallwaySkateboardStationary.txt"
# open file with scan data
allData = open(file_in).read().split('\n')
b, a = butter(2, 0.05)


accelData = []
gyroData = []
scanData = []
imuData = []
line_count = 0
first_timestamp = 0
for line in allData:
    # Get the first timestamp
    if line_count == 0:
        if line.startswith("~") and line.endswith("~~"):
            first_timestamp = int(line[3:-2].split(",")[0])
        else:
            continue
    # Serparate different data inputs
    if line.startswith("~S:") and line.endswith("~~"):
        scanData.append(line[3:-2].split(","))
    if line.startswith("~A:") and line.endswith("~~"):
        accelData.append(line[3:-2].split(','))
    if line.startswith("~G:") and line.endswith("~~"):
        gyroData.append(line[3:-2].split(","))
    if line.startswith("~I:") and line.endswith("~~"):
        imuData.append(line[3:-2].split(","))
    line_count += 1

# Cast accelerometer data to correct type
accelTimestamps = []
accelXs = []
accelYs = []
accelZs = []
accelXYZ = []
gyroTimestamps = []
gyroXs = []
gyroYs = []
gyroZs = []
gyroXYZ = []
for i in range(len(accelData)):
    accelData[i][0] = ((int(accelData[i][0])-first_timestamp)*10**-6)
    accelTimestamps.append(accelData[i][0])
    accelData[i][1] = float(accelData[i][1])
    accelXs.append(accelData[i][1])
    accelData[i][2] = float(accelData[i][2])
    accelYs.append(accelData[i][2])
    accelData[i][3] = -1 * float(accelData[i][3]) + g
    accelZs.append(accelData[i][3])
    accelXYZ.append(np.array([accelData[i][1], accelData[i][2], accelData[i][3]]))

# Cast gyro data to correct type
for i in range(len(gyroData)):
    gyroData[i][0] = (int(gyroData[i][0])-first_timestamp)*10**-6
    gyroTimestamps.append(gyroData[i][0])
    gyroData[i][1] = float(gyroData[i][1])
    gyroXs.append(gyroData[i][1])
    gyroData[i][2] = float(gyroData[i][2])
    gyroYs.append(gyroData[i][2])
    gyroData[i][3] = float(gyroData[i][3])
    gyroZs.append(gyroData[i][3])
    gyroXYZ.append(np.array([gyroData[i][1], gyroData[i][2], gyroData[i][3]]))

# accelXs = lfilter(b, a, accelXs)
# accelYs = lfilter(b, a, accelYs)
# accelZs = lfilter(b, a, accelZs)

# Integrate Once to get velocity
velXs = []
velYs = []
velZs = []
# for i in range(len(accelData)-1):
#     dt = accelTimestamps[i+1] - accelTimestamps[i]
#     print(dt)
velXs = cumtrapz(accelXs, accelTimestamps)
velYs = cumtrapz(accelYs, accelTimestamps)
velZs = cumtrapz(accelZs, accelTimestamps)

# Integrate Twice to get displacement
dispXs = cumtrapz(velXs, accelTimestamps[:-1])
dispYs = cumtrapz(velYs, accelTimestamps[:-1])
dispZs = cumtrapz(velZs, accelTimestamps[:-1])

# Plot data as polar scatter plot
# Plot data as cartesian
fig = plt.figure()
st = fig.suptitle("Accelerometer Drift", fontsize=20)

# plt.title('Accelerometer Drift')

plt.subplot(331)
plt.plot(accelTimestamps, accelXs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("X acceleration (m/s^2)")
plt.subplot(332)
plt.plot(accelTimestamps, accelYs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("Y acceleration (m/s^2)")
plt.subplot(333)
plt.plot(accelTimestamps, accelZs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("Z acceleration (m/s^2)")

# Plot velocity data
plt.subplot(334)
plt.plot(accelTimestamps[:-1], velXs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("X velocity (m/s)")
plt.subplot(335)
plt.plot(accelTimestamps[:-1], velYs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("Y velocity (m/s)")
plt.subplot(336)
plt.plot(accelTimestamps[:-1], velZs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("Z velocity (m/s)")

# Plot displacement data
plt.subplot(337)
plt.plot(accelTimestamps[:-2], dispXs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("X displacement (m)")
plt.subplot(338)
plt.plot(accelTimestamps[:-2], dispYs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("Y displacement (m)")
plt.subplot(339)
plt.plot(accelTimestamps[:-2], dispZs)
grid(True)
plt.xlabel('Time (s)')
plt.ylabel("Z displacement (m)")
plt.show()
