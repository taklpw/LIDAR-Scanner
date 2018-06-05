from pylab import *
import numpy as np
from scipy.signal import lfilter, butter
from skinematics import view
from skinematics.imus import analytical, kalman
from skinematics.sensors.polulu import Polulu
from skinematics.sensors.manual import MyOwnSensor
from skimage.restoration import denoise_tv_chambolle
#import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from math import cos, sin, pi, radians

file_in = "testDataHallway.txt"
# filter parameters
b, a = butter(2, 0.05)

# open file with scan data
allData = open(file_in).read().split('\n')

# Read data in
accelData = []
gyroData = []
magData = []
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
    if line.startswith("~M:") and line.endswith("~~"):
        magData.append(line[3:-2].split(","))
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
magTimestamps = []
magXs = []
magYs = []
magZs = []
magXYZ = []
if len(imuData) == 0:
    for i in range(len(accelData)):
        accelData[i][0] = ((int(accelData[i][0])-first_timestamp)*10**-6)
        accelTimestamps.append(accelData[i][0])
        accelData[i][1] = float(accelData[i][1])
        accelXs.append(accelData[i][1])
        accelData[i][2] = float(accelData[i][2])
        accelYs.append(accelData[i][2])
        accelData[i][3] = -1* float(accelData[i][3])
        accelZs.append(accelData[i][3])
        accelXYZ.append(np.array([accelData[i][1], accelData[i][2], accelData[i][3]]))
else:
    for i in range(len(imuData)):
        imuData[i][0] = ((int(imuData[i][0])-first_timestamp)*10**-6)
        accelTimestamps.append(imuData[i][0])
        imuData[i][1] = float(imuData[i][1])
        accelXs.append(imuData[i][1])
        imuData[i][2] = float(imuData[i][2])
        accelYs.append(imuData[i][2])
        imuData[i][3] = -1 * float(imuData[i][3])
        accelZs.append(imuData[i][3])
        accelXYZ.append(np.array([imuData[i][1], imuData[i][2], imuData[i][3]]))

        imuData[i][4] = float(imuData[i][4])
        gyroXs.append(imuData[i][4])
        imuData[i][5] = float(imuData[i][5])
        gyroYs.append(imuData[i][5])
        imuData[i][6] = float(imuData[i][6])
        gyroZs.append(imuData[i][6])
        gyroXYZ.append(np.array([imuData[i][4], imuData[i][5], imuData[i][6]]))

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

for i in range(len(magData)):
    magData[i][0] = (int(magData[i][0])-first_timestamp)*10**-6
    magTimestamps.append(magData[i][0])
    magData[i][1] = float(magData[i][1])
    magXs.append(gyroData[i][1])
    magData[i][2] = float(magData[i][2])
    magYs.append(gyroData[i][2])
    magData[i][3] = float(magData[i][3])
    magZs.append(magData[i][3])
    magXYZ.append(np.array([magData[i][1], magData[i][2], magData[i][3]]))

print(magXYZ)
# Filter the noise out of the data
accelXs_denoised = lfilter(b, a, accelXs)
accelYs_denoised = lfilter(b, a, accelYs)
accelZs_denoised = lfilter(b, a, accelZs)

# Get imu frequency
cumulative_sum = 0
for i in range(len(gyroTimestamps)):
    if i == 0:
        continue
    cumulative_sum += gyroTimestamps[i] - gyroTimestamps[i-1]
for i in range(len(accelTimestamps)):
    if i == 0:
        continue
    cumulative_sum += accelTimestamps[i] - accelTimestamps[i-1]
for i in range(len(magTimestamps)):
    if i ==0:
        continue
    cumulative_sum += magTimestamps[i] - magTimestamps[i-1]
avg_imu_timestep = cumulative_sum / ((len(gyroTimestamps)-1)+(len(accelTimestamps)-1))
print("IMU Frquency: %f Hz" % (1/avg_imu_timestep))

# Format data for skinematics
in_data = dict()
in_data['rate'] = (1/avg_imu_timestep)
in_data['omega'] = np.array(gyroXYZ)
in_data['acc'] = np.array(accelXYZ)
in_data['mag'] = np.array(magXYZ)

sensor = MyOwnSensor(in_data=in_data, calculate_position=True, q_type='kalman')
# sensor.q_type = 'kalman'
# q_Madgwick = sensor.quat
# sensor.calc_position()
print(sensor.pos)

# print(positions)
# print(sensor.rate)
# print(sensor.acc)
# print(sensor.omega)
# print(sensor.mag)

view.ts(sensor.pos)
# view.orientation(sensor.quat)
#my_imu = MyOwnSensor(in_data=in_data)
color = accelTimestamps
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(sensor.pos[:,0], sensor.pos[:,1], sensor.pos[:,2], c=color)
plt.show()
# Axes3D.plot3D([pos1[:,0], pos1[:,1]], pos1[:,2])

# # Plot data as polar scatter plot
# # Plot data as cartesian
# plt.title('Accelerometer (noisy)')
# plt.subplot(231)
# plt.plot(accelTimestamps, accelXs)
# grid(True)
# plt.xlabel('Time (s)')
# plt.ylabel("X acceleration (m/s^2)")
# plt.subplot(232)
# plt.plot(accelTimestamps, accelYs)
# grid(True)
# plt.xlabel('Time (s)')
# plt.ylabel("Y acceleration (m/s^2)")
# plt.subplot(233)
# plt.plot(accelTimestamps, accelZs)
# grid(True)
# plt.xlabel('Time (s)')
# plt.ylabel("Z acceleration (m/s^2)")
#
# # Plot filtered data
# plt.subplot(234)
# plt.plot(accelTimestamps, accelXs_denoised)
# grid(True)
# plt.xlabel('Time (s)')
# plt.ylabel("X acceleration (m/s^2)")
# plt.subplot(235)
# plt.plot(accelTimestamps, accelYs_denoised)
# grid(True)
# plt.xlabel('Time (s)')
# plt.ylabel("Y acceleration (m/s^2)")
# plt.subplot(236)
# plt.plot(accelTimestamps, accelZs_denoised)
# grid(True)
# plt.xlabel('Time (s)')
# plt.ylabel("Z acceleration (m/s^2)")
# plt.show()


