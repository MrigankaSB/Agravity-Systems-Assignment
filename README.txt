# Altitude Estimation and Sensor Fault Detection
**Round 2 Submission — Embedded Systems Intern (Drones)**

---

## Objective:
Simulate altitude sensor data, estimate altitude using sensor fusion, detect sensor faults, and apply failsafe mechanisms. Bonus: Implemented a 1D Kalman Filter.

---

## Logic:

### Sensor Fusion:
Estimated altitude is derived using a Kalman Filter:
- Prediction: Uses IMU acceleration integrated over time.
- Correction: Uses barometer data for measurement correction.
- GPS is monitored but not directly fused into the Kalman filter due to unreliability during anomalies.

---

## Sensor Simulation:
- **GPS**: Base + random noise (±1 m), sudden +50 m jump after 15s.
- **Barometer**: Base + noise (±0.5 m), with +0.2 m drift per second after 15s.
- **IMU**: Simulates vertical acceleration with low noise initially; erratic values (±3 m/s²) after 15s.
- IMU is integrated over time to estimate altitude.

---

## Failsafe Handling:
- If the difference between any two sensors exceeds 10 m for 3 consecutive seconds:
  - A warning is printed.
  - A "Failsafe" mode is logged, suggesting which sensors to trust.
  - Kalman filter continues to work primarily using Barometer and IMU.

---

## Bonus: Kalman Filter
Implemented a 1D Kalman filter:
- State = Altitude
- Measurement = Barometer
- Prediction uses IMU-based velocity integration
- This smooths noise and responds robustly to sensor faults.

---

## How to Run:
```bash
g++ altitude_estimation.cpp -o altimeter
./altimeter
```

Runs quickly for screen recording (200ms delay between readings).

---

## Author:
Mriganka Shekhar Barman  
Submission Date: May 17, 2025
