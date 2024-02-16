#include <parameters.h>
#include <function_declarations_and_globals.h>

// Same for both LSM9DS1/BMI270_BMM150
// Accelerometer range is set at [-4, +4]g -/+0.122 mg.
// Gyroscope range is set at [-2000, +2000] dps +/-70 mdps.
// Magnetometer range is set at [-400, +400] uT +/-0.014 uT.
// Accelerometer output data rate is fixed at 104 Hz.
// Gyroscope output data rate is fixed at 104 Hz.
// Magnetometer output data rate is fixed at 20 Hz.

bool imu_active = 1;
bool mag_active = 1;
bool gyr_active = 1;

float status_acc_filt_x, status_acc_filt_y, status_acc_filt_z; // global

//SimpleKalmanFilter acc_kalman_filter_x(ACC_KALMAN_MEASUREMENT_UNCERTAINTY, ACC_KALMAN_ESTIMATION_UNCERTAINTY, ACC_KALMAN_PROCESS_NOISE_UNCERTAINTY);
//SimpleKalmanFilter acc_kalman_filter_y(ACC_KALMAN_MEASUREMENT_UNCERTAINTY, ACC_KALMAN_ESTIMATION_UNCERTAINTY, ACC_KALMAN_PROCESS_NOISE_UNCERTAINTY);
//SimpleKalmanFilter acc_kalman_filter_z(ACC_KALMAN_MEASUREMENT_UNCERTAINTY, ACC_KALMAN_ESTIMATION_UNCERTAINTY, ACC_KALMAN_PROCESS_NOISE_UNCERTAINTY);

bool imu_initialize(void)
{
    if (IMU.begin())
    {
        status_acc_x = 0.0;
        status_acc_y = 0.0;
        status_acc_z = 0.0;
        status_acc_filt_x = 0.0;
        status_acc_filt_y = 0.0;
        status_acc_filt_z = 0.0;
        status_mag_x = 0.0;
        status_mag_y = 0.0;
        status_mag_z = 0.0;
        status_gyr_x = 0.0;
        status_gyr_y = 0.0;
        status_gyr_z = 0.0;

        Serial.print("Acc sample rate = ");
        Serial.print(imu_get_acc_update_rate());
        Serial.println(" Hz");

        Serial.print("Mag sample rate = ");
        Serial.print(imu_get_mag_update_rate());
        Serial.println(" Hz");

        Serial.print("Gyro sample rate = ");
        Serial.print(imu_get_gyro_update_rate());
        Serial.println(" Hz");

        return 1;
    }
    else
    {
        return 0;
    }
}

/*
void imu_update_accel_values_filtered(void)
{
    status_acc_filt_x = acc_kalman_filter_x.updateEstimate(status_acc_x);
    status_acc_filt_y = acc_kalman_filter_y.updateEstimate(status_acc_y);
    status_acc_filt_z = acc_kalman_filter_z.updateEstimate(status_acc_z);

#if SERIAL_OUT_ACC_PLOT == 1
    //    Serial.print("gx:");
    Serial.print(acc_x);
    Serial.print(",");
    //    Serial.print("gy:");
    Serial.print(acc_y);
    Serial.print(",");
    //    Serial.print("gx:");
    Serial.print(status_acc_z);
    Serial.print(",");
    //    Serial.print("g_filt_x:");
    Serial.print(status_acc_filt_x);
    Serial.print(",");
    //   Serial.print("g_filt_y:");
    Serial.print(status_acc_filt_y);
    Serial.print(",");
    //    Serial.print("g_filt_x:");
    Serial.println(status_acc_filt_z);
#endif
}
*/

void imu_update_accel_values(void)
{
    if (IMU.accelerationAvailable())
    {
        IMU.readAcceleration(status_acc_x, status_acc_y, status_acc_z);
        status_cal_acc_x = status_acc_x;
        status_cal_acc_y = status_acc_y;
        status_cal_acc_z = status_acc_z;
        // imu_update_accel_values_filtered();
    }
}

void imu_update_mag_values(void)
{
    if (IMU.magneticFieldAvailable())
    {
        IMU.readMagneticField(status_mag_x, status_mag_y, status_mag_z);

        float status_cal_mag_x_temp = status_mag_x - CAL_MAG_OFFSET_X;
        float status_cal_mag_y_temp = status_mag_y - CAL_MAG_OFFSET_Y;
        float status_cal_mag_z_temp = status_mag_z - CAL_MAG_OFFSET_Z;

        status_cal_mag_x = status_cal_mag_x_temp * CAL_MAG_11 + status_cal_mag_y_temp * CAL_MAG_12 + status_cal_mag_z_temp * CAL_MAG_13;
        status_cal_mag_y = status_cal_mag_x_temp * CAL_MAG_21 + status_cal_mag_y_temp * CAL_MAG_22 + status_cal_mag_z_temp * CAL_MAG_23;
        status_cal_mag_z = status_cal_mag_x_temp * CAL_MAG_31 + status_cal_mag_y_temp * CAL_MAG_32 + status_cal_mag_z_temp * CAL_MAG_33;
    }
}

void imu_update_gyro_values(void)
{
    if (IMU.gyroscopeAvailable())
    {
        IMU.readGyroscope(status_gyr_x, status_gyr_y, status_gyr_z);
        status_cal_gyr_x = status_gyr_x - CAL_GYR_X;
        status_cal_gyr_y = status_gyr_y - CAL_GYR_Y;
        status_cal_gyr_z = status_gyr_z - CAL_GYR_Z;
    }
}

float imu_get_acc_update_rate(void)
{
    return IMU.accelerationSampleRate();
}

float imu_get_mag_update_rate(void)
{
    return IMU.magneticFieldSampleRate();
}

float imu_get_gyro_update_rate(void)
{
    return IMU.gyroscopeSampleRate();
}
