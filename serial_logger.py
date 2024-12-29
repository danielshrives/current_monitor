import serial
from serial import Serial
import json
import time
import pandas as pd

import matplotlib.pyplot as plt

# Configure the serial port and baud rate
ser = serial.Serial('/dev/cu.usbserial-A1528541E6', 9600)  # Change '/dev/cu.usbserial' to your Arduino's serial port

# Open a file to log the data
log_file = open('arduino_data.log', 'w')

# Initialize a list to store the data
data = []

# Record data for 30 seconds
start_time = time.time()
while time.time() - start_time < 20:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        try:
            json_data = json.loads(line)
            # print(json_data)
            data.append(json_data)
            log_file.write(line + '\n')
        except json.JSONDecodeError:
            print("Received invalid JSON data")

# Close the log file and serial port
log_file.close()
ser.close()

# Convert the data to a DataFrame
df = pd.DataFrame(data)

# Plot the 'current_mA' column
plt.figure(figsize=(10, 5))
plt.plot(df['current_mA'], label='Current (mA)')
plt.xlabel('Time')
plt.ylabel('Current (mA)')
plt.title('Current over Time')
plt.legend()
plt.show()

