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
            status_flag_3 = false;
        }
        /*else if (!strcmp(receivedCommand, "X"))
        {
          //
        }
        */
        else
        {
            // Handle unknown command or report an error
            status_flag_3 = true;
        }
    }
}

void sendBool(bool value)
{
    // Send a single byte for the bool (1 for true, 0 for false)
    Serial.write(value);
}

void sendInt(int8_t value)
{
    // Cast the address of the int to a byte pointer
    byte *byteArray = (byte *)(void *)&value;

    // Send the bytes of the int
    for (uint i = 0; i < sizeof(int8_t); i++)
    {
        Serial.write(byteArray[i]);
    }
}

void sendUnsignedInt(uint8_t value)
{
    // Cast the address of the unsigned int to a byte pointer
    byte *byteArray = (byte *)(void *)&value;

    // Send the bytes of the unsigned int
    for (uint i = 0; i < sizeof(uint8_t); i++)
    {
        Serial.write(byteArray[i]);
    }
}

void sendFloat(float value)
{
    // Cast the address of the float to a byte pointer
    byte *byteArray = (byte *)(void *)&value;

    // Send the bytes of the float
    for (uint i = 0; i < sizeof(float); i++)
    {
        Serial.write(byteArray[i]);
    }
}

// Message:                 XXXXX
// Padded message:          AAXBXBXBXB
// Possible occurrences:    AAABXBXBXB
//                          AABBXBXBXB
//                          AAABABXBXB

void sendSerialData()
{
    // Create a buffer to hold the data
    byte messageBuffer[SERIAL_MESSAGE_BUFFER_SIZE]; // Adjust the buffer size as needed based on your data

    // Index to keep track of the current position in the buffer
    int index = 0;

    // Pack the boolean values into a single byte
    byte boolsPacked = (status_motor1_ina ? 1 : 0) |
                       (status_motor1_inb ? 2 : 0) |
                       (status_motor1_endiag ? 4 : 0) |
                       (status_motor2_ina ? 8 : 0) |
                       (status_motor2_inb ? 16 : 0) |
                       (status_motor2_endiag ? 32 : 0) |
                       (status_flag_1 ? 64 : 0) |
                       (status_flag_2 ? 128 : 0);
    // Store the packed boolean value in the buffer
    messageBuffer[index++] = boolsPacked;

    // Pack the boolean values into a single byte
    boolsPacked = (status_txu_oe ? 1 : 0) |
                  (status_d3 ? 2 : 0) |
                  (status_d11 ? 4 : 0) |
                  (status_d13 ? 8 : 0) |
                  (status_rgb_led_r ? 16 : 0) |
                  (status_rgb_led_g ? 32 : 0) |
                  (status_rgb_led_b ? 64 : 0) |
                  (status_flag_3 ? 128 : 0);
    // Store the packed boolean value in the buffer
    messageBuffer[index++] = boolsPacked;

    memcpy(messageBuffer + index, &status_motor1_pwm, sizeof(uint8_t));
    index += sizeof(uint8_t);
    memcpy(messageBuffer + index, &status_motor1_cs, sizeof(uint8_t));
    index += sizeof(uint8_t);

    memcpy(messageBuffer + index, &status_motor2_pwm, sizeof(uint8_t));
    index += sizeof(uint8_t);
    memcpy(messageBuffer + index, &status_motor2_cs, sizeof(uint8_t));
    index += sizeof(uint8_t);

    memcpy(messageBuffer + index, &status_m1_enc_count, sizeof(uint8_t));
    index += sizeof(uint8_t);
    memcpy(messageBuffer + index, &status_m1_rot_rate, sizeof(uint8_t));
    index += sizeof(uint8_t);
    memcpy(messageBuffer + index, &status_m2_enc_count, sizeof(uint8_t));
    index += sizeof(uint8_t);
    memcpy(messageBuffer + index, &status_m2_rot_rate, sizeof(uint8_t));
    index += sizeof(uint8_t);

    memcpy(messageBuffer + index, &status_acc_x, sizeof(int8_t));
    index += sizeof(int8_t);
    memcpy(messageBuffer + index, &status_acc_y, sizeof(int8_t));
    index += sizeof(int8_t);
    memcpy(messageBuffer + index, &status_acc_z, sizeof(int8_t));
    index += sizeof(int8_t);

    memcpy(messageBuffer + index, &status_ang_rate_x, sizeof(int8_t));
    index += sizeof(int8_t);
    memcpy(messageBuffer + index, &status_ang_rate_y, sizeof(int8_t));
    index += sizeof(int8_t);
    memcpy(messageBuffer + index, &status_ang_rate_z, sizeof(int8_t));
    index += sizeof(int8_t);

    // messageBuffer[index++] = '\n';

    byte sendBuffer[SERIAL_SEND_BUFFER_SIZE]; // Adjust the buffer size as needed based on your data

    sendBuffer[0] = (byte)SERIAL_HEADER;
    sendBuffer[1] = (byte)SERIAL_HEADER;

    for (int i = 0; i < SERIAL_MESSAGE_BUFFER_SIZE; i++)
    {
        sendBuffer[2 + i * 2] = messageBuffer[i];
        sendBuffer[3 + i * 2] = (byte)SERIAL_PAD;
    }

    // Send the entire buffer with a single call to Serial.write()
    Serial.write(sendBuffer, SERIAL_SEND_BUFFER_SIZE);
    /*   Serial.print("\tSERIAL_SEND_BUFFER_SIZE ");
       Serial.print(SERIAL_SEND_BUFFER_SIZE);
       Serial.print("\tSERIAL_MESSAGE_BUFFER_SIZE ");
       Serial.print(SERIAL_MESSAGE_BUFFER_SIZE);
           Serial.print("\tindex ");
       Serial.println(index);
       */
}
