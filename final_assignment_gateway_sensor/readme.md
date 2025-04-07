---

# Gateway Project - Sensor

This project implements a Gateway system to receive, process, and store sensor temperature data. Below is the complete detailed information about the implementation idea for `ConnectionManager`, along with instructions for compilation, execution, and sample output.

## Implementation Idea for ConnectionManager

In the current version (Req 1-4), `ConnectionManager` is responsible for receiving sensor data from an external source (initially via a socket from `SensorNode`, but since Req 6 was removed, we simulate the data). Here is the implementation concept:

### Objectives of ConnectionManager
- Receive sensor data (including sensor ID and temperature) from a source (simulated in this case).
- Parse the data to extract sensor ID and temperature.
- Store the data in `SensorData` for use by other components (`DataManager`, `StorageManager`).

### Simulated Sensor Data
Since `SensorNode` (Req 6) is no longer present, I simulate sensor data directly within `ConnectionManager` to mimic receiving data from a client.

#### Simulation Method
- Generate 5 data messages, each representing a sensor reading.
- Each message follows the real data format: `"ID:<sensor_id>,Temp:<temperature>"` (e.g., `"ID:1,Temp:25.4977"`).
- Sensor ID is fixed at 1 (for simplicity).
- Temperature is randomly generated between 0.0 and 40.0 (consistent with the original `SensorNode` requirement in Req 6).
- Messages are generated 1 second apart to simulate real-time data reception.

#### Reason for Simulation
- Ensure the program has data to process without requiring `SensorNode`.
- Allow testing of other components (`SensorData`, `DataManager`) without a real socket connection.

### Data Processing (Parsing and Storage)

#### Parsing Data
- Simulated data is in string format: `"ID:<sensor_id>,Temp:<temperature>"`.
- The idea is to split this string into two parts: ID and Temp.
- Validate the format of each part:
  - The ID part must start with `"ID:"`, followed by an integer (sensor ID).
  - The Temp part must start with `"Temp:"`, followed by a floating-point number (temperature).
- If the format is incorrect (e.g., not a number), skip the message and print an error.

#### Storing Data
- After successful parsing, extract `sensor_id` (int) and `temperature` (double).
- Call `SensorData::addReading(sensor_id, temperature)` to store the data in `SensorData`.
- `SensorData` automatically adds a timestamp and marks the message as "unprocessed" (`valid = true`).

### Workflow
- `ConnectionManager` runs in a separate thread (initialized by `Gateway`).
- It simulates receiving 5 messages, each spaced 1 second apart.
- For each message:
  1. Generate simulated data.
  2. Parse the data to extract sensor ID and temperature.
  3. Store it in `SensorData`.
- After processing all 5 messages, `ConnectionManager` terminates (prints "Simulation finished").

### Interaction with Other Components
- `ConnectionManager` is solely responsible for receiving and storing data in `SensorData`.
- `SensorData` acts as shared memory for other threads (`DataManager`, `StorageManager`) to access the data.
- `DataManager` reads data from `SensorData` to calculate the average temperature.
- `StorageManager` (currently without full logic implementation) will also read data from `SensorData` in the future.

## Compilation and Execution

### Compilation
```
g++ -o bin/gateway src/Gateway.cpp src/SensorData.cpp src/ConnectionManager.cpp src/DataManager.cpp src/StorageManager.cpp -pthread -lsqlite3
g++ -o bin/sensor_node src/SensorNode.cpp
```

### Running the Program
```
./bin/gateway 1234
```

## Sample Output

```
Starting Main Process (PID: 234534) on port 1234
Main Process continues (PID: 234534), Log Process PID: 234535
Log Process started (PID: 234535)
Log Process running, iteration 1
Connection Manager: Listening on port 1234
Storage Manager: Database initialized
Log Process running, iteration 2
Log Process running, iteration 3
Log Process running, iteration 4
Log Process running, iteration 5
Log Process exiting
Connection Manager: New connection accepted
Connection Manager: Received data: ID:2,Temp:18.3167
SensorData: Added reading - ID: 2, Temp: 18.3167, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:28.1929
SensorData: Added reading - ID: 2, Temp: 28.1929, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:2.18087
SensorData: Added reading - ID: 2, Temp: 2.18087, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:13.7513
SensorData: Added reading - ID: 2, Temp: 13.7513, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:30.4079ID:2,Temp:8.69494
SensorData: Added reading - ID: 2, Temp: 30.4079, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:6.39365ID:2,Temp:4.13213
SensorData: Added reading - ID: 2, Temp: 6.39365, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:15.7241
SensorData: Added reading - ID: 2, Temp: 15.7241, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:33.0302
SensorData: Added reading - ID: 2, Temp: 33.0302, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:6.15113
SensorData: Added reading - ID: 2, Temp: 6.15113, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:15.7259
SensorData: Added reading - ID: 2, Temp: 15.7259, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:34.4925
SensorData: Added reading - ID: 2, Temp: 34.4925, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:31.7128
SensorData: Added reading - ID: 2, Temp: 31.7128, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:8.15725
SensorData: Added reading - ID: 2, Temp: 8.15725, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:14.7525
SensorData: Added reading - ID: 2, Temp: 14.7525, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:31.6718
SensorData: Added reading - ID: 2, Temp: 31.6718, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:18.4073
SensorData: Added reading - ID: 2, Temp: 18.4073, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:26.5209
SensorData: Added reading - ID: 2, Temp: 26.5209, Timestamp: 2025-04-07 16:46:19
Connection Manager: Received data: ID:2,Temp:19.2194
SensorData: Added reading - ID: 2, Temp: 19.2194, Timestamp: 2025-04-07 16:46:19
Connection Manager: Connection closed by client
Data Manager: Sensor ID: 2, Average Temp: 19.7116
Storage Manager: Stored - ID: 2, Temp: 18.3167, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 28.1929, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 2.18087, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 13.7513, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 30.4079, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 6.39365, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 15.7241, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 33.0302, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 6.15113, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 15.7259, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 34.4925, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 31.7128, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 8.15725, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 14.7525, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 31.6718, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 18.4073, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 26.5209, Timestamp: 2025-04-07 16:46:19
Storage Manager: Stored - ID: 2, Temp: 19.2194, Timestamp: 2025-04-07 16:46:19
```

### Checking the Database
```
sqlite3 sensor_data.db
SQLite version 3.45.1 2024-01-30 16:01:20
Enter ".help" for usage hints.
sqlite> SELECT * FROM sensor_readings;
2|18.3167|2025-04-07 16:46:19
2|28.1929|2025-04-07 16:46:19
2|2.18087|2025-04-07 16:46:19
2|13.7513|2025-04-07 16:46:19
2|30.4079|2025-04-07 16:46:19
2|6.39365|2025-04-07 16:46:19
2|15.7241|2025-04-07 16:46:19
2|33.0302|2025-04-07 16:46:19
2|6.15113|2025-04-07 16:46:19
2|15.7259|2025-04-07 16:46:19
2|34.4925|2025-04-07 16:46:19
2|31.7128|2025-04-07 16:46:19
2|8.15725|2025-04-07 16:46:19
2|14.7525|2025-04-07 16:46:19
2|31.6718|2025-04-07 16:46:19
2|18.4073|2025-04-07 16:46:19
2|26.5209|2025-04-07 16:46:19
2|19.2194|2025-04-07 16:46:19
sqlite>
```

---
