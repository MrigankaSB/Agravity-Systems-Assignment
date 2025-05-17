#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

// Function to generate random noise
float generate_noise(float amplitude) {
    return ((float)rand() / RAND_MAX - 0.5f) * 2 * amplitude;
}

int main() {
    srand(time(0));

    // Initial states
    float gps = 100.0, baro = 100.0, imu_acc = 0.0;
    float imu_velocity = 0.0, imu_altitude = 100.0;
    float gps_jump = 0.0, baro_drift = 0.0;

    // Kalman Filter variables
    float kalman_alt = 100.0;
    float kalman_error = 1.0;
    float process_noise = 0.5;
    float measurement_noise = 2.0;
    float dt = 1.0;

    // Sensor fault detection counters
    const float threshold = 10.0;
    const int limit = 3;
    int gps_fault_count = 0, baro_fault_count = 0, imu_fault_count = 0;

    for (int t = 1; t <= 30; ++t) {
        // Inject anomalies after 15s
        if (t >= 15) {
            gps_jump = 50.0;
            imu_acc = generate_noise(3.0);
            baro_drift += 0.2;
        } else {
            imu_acc = generate_noise(0.1);
        }

        gps = 100.0 + gps_jump + generate_noise(1.0);
        baro = 100.0 + baro_drift + generate_noise(0.5);
        imu_velocity += imu_acc * dt;
        imu_altitude += imu_velocity * dt;

        // Kalman Filter
        float predicted_alt = kalman_alt + imu_velocity * dt;
        float predicted_error = kalman_error + process_noise;
        float kalman_gain = predicted_error / (predicted_error + measurement_noise);
        kalman_alt = predicted_alt + kalman_gain * (baro - predicted_alt);
        kalman_error = (1 - kalman_gain) * predicted_error;

        // Fault detection
        if (abs(gps - baro) > threshold || abs(gps - imu_altitude) > threshold)
            gps_fault_count++;
        else gps_fault_count = 0;

        if (abs(baro - imu_altitude) > threshold || abs(baro - gps) > threshold)
            baro_fault_count++;
        else baro_fault_count = 0;

        if (abs(imu_altitude - baro) > threshold || abs(imu_altitude - gps) > threshold)
            imu_fault_count++;
        else imu_fault_count = 0;

        // Output
        cout << "Time: " << t << "s" << endl;
        cout << "  GPS Altitude: " << gps << " m" << endl;
        cout << "  Baro Altitude: " << baro << " m" << endl;
        cout << "  IMU Altitude: " << imu_altitude << " m" << endl;
        cout << "  Kalman Filter Altitude: " << kalman_alt << " m" << endl;

        if (gps_fault_count >= limit) {
            cout << "  WARNING: Potential sensor fault in GPS" << endl;
            cout << "  Failsafe: Switching to Baro + IMU only" << endl;
        }

        if (baro_fault_count >= limit) {
            cout << "  WARNING: Potential sensor fault in Barometer" << endl;
            cout << "  Failsafe: Switching to GPS + IMU only" << endl;
        }

        if (imu_fault_count >= limit) {
            cout << "  WARNING: Potential sensor fault in IMU" << endl;
            cout << "  Failsafe: Switching to GPS + Baro only" << endl;
        }

        cout << "-------------------------------------------" << endl;

        this_thread::sleep_for(chrono::milliseconds(200));  // Faster for recording
    }

    return 0;
}
