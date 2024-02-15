#include <parameters.h>
#include <function_declarations_and_globals.h>

const byte numChars = 128;
char receivedChars[numChars];
char tempChars[numChars]; // temporary array for use when parsing

// variables to hold the parsed data
char serial_receive_message[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;

boolean serial_receive_newData = false;

void readSerialData()
{
    static boolean serial_receive_recvInProgress = false;
    static byte serial_receive_ndx = 0;
    char serial_receive_startMarker = '<';
    char serial_receive_endMarker = '>';
    char serial_receive_char;

    while (Serial.available() > 0 && serial_receive_newData == false)
    {
        serial_receive_char = Serial.read();

        if (serial_receive_recvInProgress == true)
        {
            if (serial_receive_char != serial_receive_endMarker)
            {
                receivedChars[serial_receive_ndx] = serial_receive_char;
                serial_receive_ndx++;
                if (serial_receive_ndx >= numChars)
                {
                    serial_receive_ndx = numChars - 1;
                }
            }
            else
            {
                receivedChars[serial_receive_ndx] = '\0'; // terminate the string
                serial_receive_recvInProgress = false;
                serial_receive_ndx = 0;
                serial_receive_newData = true;
            }
        }

        else if (serial_receive_char == serial_receive_startMarker)
        {
            serial_receive_recvInProgress = true;
        }
    }

    if (serial_receive_newData == true)
    {
        strcpy(tempChars, receivedChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() used in parseData() replaces the commas with \0
        char *serial_receive_strtokIndx; // this is used by strtok() as an index

        serial_receive_strtokIndx = strtok(tempChars, ",");        // get the first part - the string
        strcpy(serial_receive_message, serial_receive_strtokIndx); // copy it to serial_receive_message

        serial_receive_strtokIndx = strtok(NULL, ",");   // this continues where the previous call left off
        integerFromPC = atoi(serial_receive_strtokIndx); // convert this part to an integer

        serial_receive_strtokIndx = strtok(NULL, ",");
        floatFromPC = atof(serial_receive_strtokIndx); // convert this part to a float

        /*  Serial.print("Message ");
          Serial.println(serial_receive_message);
          Serial.print("Integer ");
          Serial.println(integerFromPC);
          Serial.print("Float ");
          Serial.println(floatFromPC);
      */
        serial_receive_newData = false;

        char receivedCommand[20]; // Adjust the size as needed
        strcpy(receivedCommand, serial_receive_message);
        strupr(receivedCommand); // Convert to lowercase (use strupr for uppercase)

        if (!strcmp(receivedCommand, "M1A")) // Motor 1 INA only
        {
            status_motor1_ina = (bool)integerFromPC;
            motor_driver_setM1_INA(status_motor1_ina);
        }
        else if (!strcmp(receivedCommand, "M1B")) // Motor 1 INB only
        {
            status_motor1_inb = (bool)integerFromPC;
            motor_driver_setM1_INB(status_motor1_inb);
        }
        else if (!strcmp(receivedCommand, "M2A")) // Motor 2 INA only
        {
            status_motor2_ina = (bool)integerFromPC;
            motor_driver_setM2_INA(status_motor2_ina);
        }
        else if (!strcmp(receivedCommand, "M2B")) // Motor 2 INB only
        {
            status_motor2_inb = (bool)integerFromPC;
            motor_driver_setM2_INB(status_motor2_inb);
        }
        else if (!strcmp(receivedCommand, "M1P")) // Motor 1 PWM only
        {
            status_motor1_pwm = integerFromPC;
            motor_driver_setM1_PWM(status_motor1_pwm);
        }
        else if (!strcmp(receivedCommand, "M2P")) // Motor 2 PWM only
        {
            status_motor2_pwm = integerFromPC;
            motor_driver_setM2_PWM(status_motor2_pwm);
        }
        else if (!strcmp(receivedCommand, "M1S")) // Motor 1 speed (PWM and INX direction)
        {
            motor_driver_setM1Speed(integerFromPC);
        }
        else if (!strcmp(receivedCommand, "M2S")) // Motor 2 speed (PWM and INX direction)
        {
            motor_driver_setM2Speed(integerFromPC);
        }
        else if (!strcmp(receivedCommand, "M1K")) // Motor 1 brake (PWM and INX direction)
        {
            motor_driver_setM1Brake(integerFromPC);
        }
        else if (!strcmp(receivedCommand, "M2K")) // Motor 2 brake (PWM and INX direction)
        {
            motor_driver_setM2Brake(integerFromPC);
        }
        else if (!strcmp(receivedCommand, "TXU")) // TXU0104 enable/disable
        {
            status_txu_oe = (bool)integerFromPC;
            if (status_txu_oe == true)
            {
                digitalWrite(TXU0104_ENABLE, HIGH);
            }
            if (status_txu_oe == false)
            {
                digitalWrite(TXU0104_ENABLE, LOW);
            }
        }
        else if (!strcmp(receivedCommand, "LDR")) // Red LED
        {
            // red led
            status_rgb_led_r = (bool)integerFromPC;
            if (status_rgb_led_r == true)
            {
                digitalWrite(LED_RED, LOW);
            }
            if (status_rgb_led_r == false)
            {
                digitalWrite(LED_RED, HIGH);
            }
        }
        else if (!strcmp(receivedCommand, "LDG")) // Green LED
        {
            // red led
            status_rgb_led_g = (bool)integerFromPC;
            if (status_rgb_led_g == true)
            {
                digitalWrite(LED_GREEN, LOW);
            }
            if (status_rgb_led_g == false)
            {
                digitalWrite(LED_GREEN, HIGH);
            }
        }
        else if (!strcmp(receivedCommand, "LDB")) // Blue LED
        {
            // blue led
            status_rgb_led_b = (bool)integerFromPC;
            if (status_rgb_led_b == true)
            {
                digitalWrite(LED_BLUE, LOW);
            }
            if (status_rgb_led_b == false)
            {
                digitalWrite(LED_BLUE, HIGH);
            }
        }
        else if (!strcmp(receivedCommand, "CE")) // Clear error flag (flag 3)
        {
            status_flag_unknown_message = false;
        }
        /*else if (!strcmp(receivedCommand, "X"))
        {
          //
        }
        */
        else
        {
            // Handle unknown command or report an error
            status_flag_unknown_message = true;
        }
    }
}

// Message:                 XXXXX
// Padded message:          AAXBXBXBXB
// Possible occurrences:    AAABXBXBXB
//                          AABBXBXBXB
//                          AAABABXBXB

// Message:                 XXXXX
// Padded message:          AANXXBXBXBXB
// N is packet index

void sendSerialData(byte message_type)
{
    uint send_buffer_size = 0;
    // Create a buffer to hold the data
    if (message_type == 0)
    {
        send_buffer_size = SERIAL_SEND_BUFFER_SIZE_0;
    }
    if (message_type == 1)
    {
        send_buffer_size = SERIAL_SEND_BUFFER_SIZE_1;
    }
    if (message_type == 2)
    {
        send_buffer_size = SERIAL_SEND_BUFFER_SIZE_2;
    }

    // Index to keep track of the current position in the buffer
    int index = 0;

    if (message_type == 0)
    {
        index = 0;

        // 1 byte
        messageBuffer[index] = (byte)message_type;
        index++;
        // Total = 1

        // 1 byte
        byte boolsPacked = (status_motor1_ina ? 1 : 0) |
                           (status_motor1_inb ? 2 : 0) |
                           (status_motor1_endiag ? 4 : 0) |
                           (status_motor2_ina ? 8 : 0) |
                           (status_motor2_inb ? 16 : 0) |
                           (status_motor2_endiag ? 32 : 0) |
                           (status_m1_rot_dir ? 64 : 0) |
                           (status_m2_rot_dir ? 128 : 0);
        // Store the packed boolean value in the buffer
        messageBuffer[index++] = boolsPacked;
        // Total = 2

        // 1 byte
        boolsPacked = (status_txu_oe ? 1 : 0) |
                      (status_d3 ? 2 : 0) |
                      (status_d11 ? 4 : 0) |
                      (status_d13 ? 8 : 0) |
                      (status_rgb_led_r ? 16 : 0) |
                      (status_rgb_led_g ? 32 : 0) |
                      (status_rgb_led_b ? 64 : 0) |
                      (status_flag_unknown_message ? 128 : 0);
        // Store the packed boolean value in the buffer
        messageBuffer[index++] = boolsPacked;
        // Total = 3

        // 1 byte
        boolsPacked = (status_flag_1 ? 1 : 0) |
                      (status_flag_2 ? 2 : 0) |
                      (status_flag_3 ? 4 : 0) |
                      (status_flag_4 ? 8 : 0) |
                      (status_flag_5 ? 16 : 0) |
                      (status_flag_6 ? 32 : 0) |
                      (status_flag_7 ? 64 : 0) |
                      (status_flag_8 ? 128 : 0);
        // Store the packed boolean value in the buffer
        messageBuffer[index++] = boolsPacked;
        // Total = 4

        memcpy(messageBuffer + index, &status_motor1_pwm, sizeof(uint16_t));
        index += sizeof(uint16_t);
        // Total = 6

        memcpy(messageBuffer + index, &status_motor1_cs, sizeof(uint16_t));
        index += sizeof(uint16_t);
        // Total = 8

        memcpy(messageBuffer + index, &status_motor2_pwm, sizeof(uint16_t));
        index += sizeof(uint16_t);
        // Total = 10

        memcpy(messageBuffer + index, &status_motor2_cs, sizeof(uint16_t));
        index += sizeof(uint16_t);
        // Total = 12

        memcpy(messageBuffer + index, &status_m1_enc_count, sizeof(int32_t));
        index += sizeof(int32_t);
        // Total = 16

        memcpy(messageBuffer + index, &status_m1_rot_rate, sizeof(int32_t));
        index += sizeof(int32_t);
        // Total = 20

        memcpy(messageBuffer + index, &status_m2_enc_count, sizeof(int32_t));
        index += sizeof(int32_t);
        // Total = 24

        memcpy(messageBuffer + index, &status_m2_rot_rate, sizeof(int32_t));
        index += sizeof(int32_t);
        // Total = 28
    }
    else if (message_type == 1)
    {
        index = 0;
        messageBuffer[index] = (byte)message_type;
        index++;
        // Total = 1

        memcpy(messageBuffer + index, &status_acc_x, sizeof(float));
        index += sizeof(float);
        // Total = 5

        memcpy(messageBuffer + index, &status_acc_y, sizeof(float));
        index += sizeof(float);
        // Total = 9

        memcpy(messageBuffer + index, &status_acc_z, sizeof(float));
        index += sizeof(float);
        // Total = 13

        memcpy(messageBuffer + index, &status_angle_x, sizeof(float));
        index += sizeof(float);
        // Total = 17

        memcpy(messageBuffer + index, &status_angle_y, sizeof(float));
        index += sizeof(float);
        // Total = 21

        memcpy(messageBuffer + index, &status_angle_z, sizeof(float));
        index += sizeof(float);
        // Total = 25
    }
    else if (message_type == 2)
    {
        index = 0;
        messageBuffer[index] = (byte)message_type;
        index++;
        // Total = 1

        memcpy(messageBuffer + index, &status_gyr_x, sizeof(float));
        index += sizeof(float);
        // Total = 5

        memcpy(messageBuffer + index, &status_gyr_y, sizeof(float));
        index += sizeof(float);
        // Total = 9

        memcpy(messageBuffer + index, &status_gyr_z, sizeof(float));
        index += sizeof(float);
        // Total = 13

        memcpy(messageBuffer + index, &status_mag_x, sizeof(float));
        index += sizeof(float);
        // Total = 17

        memcpy(messageBuffer + index, &status_mag_y, sizeof(float));
        index += sizeof(float);
        // Total = 21

        memcpy(messageBuffer + index, &status_mag_z, sizeof(float));
        index += sizeof(float);
        // Total = 25*/
    }

    sendBuffer[0] = (byte)SERIAL_HEADER;
    sendBuffer[1] = (byte)SERIAL_HEADER;

    for (uint i = 0; i < send_buffer_size; i++)
    {
        sendBuffer[2 + i * 2] = messageBuffer[i];
        sendBuffer[3 + i * 2] = (byte)SERIAL_PAD;
    }

    // Send the entire buffer with a single call to Serial.write()
    Serial.write(sendBuffer, send_buffer_size);
}

#if SERIAL_IMU_MAG_CAL == 1

void sendSerial_imu_mag_cal()
{
    // For use with PJRC Motioncal

    // From Arduino BMI270 BMM150 library
    //
    // default range is +-4G, so conversion factor is (((1 << 15)/4.0f))
    // #define INT16_to_G   (8192.0f)
    // Used as
    //   #ifdef TARGET_ARDUINO_NANO33BLE
    //   x = -sensor_data.acc.y / INT16_to_G;

    // default range is +-2000dps, so conversion factor is (((1 << 15)/4.0f))
    // #define INT16_to_DPS   (16.384f)
    // Used as
    //   #ifdef TARGET_ARDUINO_NANO33BLE
    //   x = -sensor_data.gyr.y / INT16_to_DPS;

    /*
    Accelerometer range is set at [-4, +4]g -/+0.122 mg.
    Gyroscope range is set at [-2000, +2000] dps +/-70 mdps.
    Magnetometer range is set at [-400, +400] uT +/-0.014 uT.
    */
    // From
    Serial.print("Raw:");
    Serial.print(int(status_acc_x * 8192.0)); // Adafruit:  Serial.print(int(status_acc_x * 8192 / 9.8));
    Serial.print(",");
    Serial.print(int(status_acc_y * 8192.0));
    Serial.print(",");
    Serial.print(int(status_acc_z * 8192.0));
    Serial.print(",");
    Serial.print(int(status_gyr_x * 16.384)); //  Adafruit:  Serial.print(int(status_gyr_x * DEGREES_PER_RADIAN * 16));
    Serial.print(",");
    Serial.print(int(status_gyr_y * 16.384));
    Serial.print(",");
    Serial.print(int(status_gyr_z * 16.384));
    Serial.print(",");
    Serial.print(int(status_mag_x * 10));
    Serial.print(",");
    Serial.print(int(status_mag_y * 10));
    Serial.print(",");
    Serial.print(int(status_mag_z * 10));
    Serial.println("");
}
#endif

#if SERIAL_IMU_RAW == 1

void sendSerial_imu_raw()
{
    if (first_run == true)
    {
        first_run = false;
        Serial.println("ax,ay,az,mx,my,mz,gx,gy,gz");
    }

    Serial.print(status_acc_x);
    Serial.print(",");
    Serial.print(status_acc_y);
    Serial.print(",");
    Serial.print(status_acc_z);
    Serial.print(",");
    Serial.print(status_mag_x);
    Serial.print(",");
    Serial.print(status_mag_y);
    Serial.print(",");
    Serial.print(status_mag_z);
    Serial.print(",");
    Serial.print(status_gyr_x);
    Serial.print(",");
    Serial.print(status_gyr_y);
    Serial.print(",");
    Serial.println(status_gyr_z);
}

#endif

#if SERIAL_IMU_CALIBRATED == 1

void sendSerial_imu_calibrated()
{
    Serial.print("a: ");
    Serial.print(status_cal_acc_x);
    Serial.print(" ");
    Serial.print(status_cal_acc_y);
    Serial.print(" ");
    Serial.print(status_cal_acc_z);
    Serial.print("\tm: ");
    Serial.print(status_cal_mag_x);
    Serial.print(" ");
    Serial.print(status_cal_mag_y);
    Serial.print(" ");
    Serial.print(status_cal_mag_z);
    Serial.print("\tr: ");
    Serial.print(status_cal_gyr_x);
    Serial.print(" ");
    Serial.print(status_cal_gyr_y);
    Serial.print(" ");
    Serial.println(status_cal_gyr_z);
}
#endif

#if SERIAL_IMU_RAW_VS_CAL == 1

void sendSerial_imu_raw_vs_cal()
{
    Serial.print(status_acc_x);
    Serial.print(",");
    Serial.print(status_cal_acc_x);
    Serial.print(",");

    Serial.print(status_acc_y);
    Serial.print(",");
    Serial.print(status_cal_acc_y);
    Serial.print(",");

    Serial.print(status_acc_z);
    Serial.print(",");
    Serial.print(status_cal_acc_z);
    Serial.print(",");

    Serial.print(status_mag_x);
    Serial.print(",");
    Serial.print(status_cal_mag_x);
    Serial.print(",");

    Serial.print(status_mag_y);
    Serial.print(",");
    Serial.print(status_cal_mag_y);
    Serial.print(",");

    Serial.print(status_mag_z);
    Serial.print(",");
    Serial.print(status_cal_mag_z);
    Serial.print(",");

    Serial.print(status_gyr_x);
    Serial.print(",");
    Serial.print(status_cal_gyr_x);
    Serial.print(",");

    Serial.print(status_gyr_y);
    Serial.print(",");
    Serial.print(status_cal_gyr_y);
    Serial.print(",");

    Serial.print(status_gyr_z);
    Serial.print(",");
    Serial.println(status_cal_gyr_z);
}
#endif

#if SERIAL_IMU_RPY == 1

void sendSerial_imu_rpy()
{
    float roll = filter.getRoll();
    float pitch = filter.getPitch();
    float yaw = filter.getYaw();
    Serial.print("Roll:");
    Serial.print(roll);
    Serial.print(",Pitch:");
    Serial.print(pitch);
    Serial.print(",Yaw:");
    Serial.println(yaw);
}
#endif

#if SERIAL_IMU_CALIBRATED_SERIALPLOT == 1

void sendSerial_imu_calibrated_serialplot()
{
    // Serial.print("a: ");
    Serial.print(status_cal_acc_x);
    Serial.print(",");
    Serial.print(status_cal_acc_y);
    Serial.print(",");
    Serial.print(status_cal_acc_z);
    Serial.print(",");
    Serial.print(status_cal_mag_x);
    Serial.print(",");
    Serial.print(status_cal_mag_y);
    Serial.print(",");
    Serial.print(status_cal_mag_z);
    Serial.print(",");
    Serial.print(status_cal_gyr_x);
    Serial.print(",");
    Serial.print(status_cal_gyr_y);
    Serial.print(",");
    Serial.println(status_cal_gyr_z);
}
#endif

#if SERIAL_IMU_RPY_SERIALPLOT == 1

void sendSerial_imu_rpy_serialplot()
{

    Serial.print(status_imu_roll);
    Serial.print(",");
    Serial.print(status_imu_pitch);
    Serial.print(",");
    Serial.println(status_imu_yaw);
}
#endif

#if SERIAL_IMU_RPY_ADAFRUIT_WEBSERIAL == 1

void sendSerial_imu_rpy_adafruit_webserial()
{
    Serial.print("Orientation: ");
    Serial.print(status_imu_yaw);
    Serial.print(", ");
    Serial.print(status_imu_pitch);
    Serial.print(", ");
    Serial.println(status_imu_roll);

    Serial.print("Quaternion: ");
    Serial.print(status_imu_quat_w, 4);
    Serial.print(", ");
    Serial.print(status_imu_quat_x, 4);
    Serial.print(", ");
    Serial.print(status_imu_quat_y, 4);
    Serial.print(", ");
    Serial.println(status_imu_quat_z, 4);
}
#endif
