#include <parameters.h>
#include <function_declarations_and_globals.h>

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

    sendBuffer[0] = (byte) SERIAL_HEADER;
    sendBuffer[1] = (byte) SERIAL_HEADER;

    for (int i = 0; i < SERIAL_MESSAGE_BUFFER_SIZE; i++)
    {
        sendBuffer[2 + i * 2] = messageBuffer[i];
        sendBuffer[3 + i * 2] = (byte) SERIAL_PAD;
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

/*
void sendSerialData()
{
    // Pack the booleans into a single byte
    byte boolsPacked = (status_motor1_ina ? 1 : 0) | (status_motor1_inb ? 2 : 0) | (status_motor1_endiag ? 4 : 0) | (status_motor2_ina ? 8 : 0) | (status_motor2_inb ? 16 : 0) | (status_motor2_endiag ? 32 : 0) | (status_flag_1 ? 64 : 0) | (status_flag_2 ? 128 : 0);
    Serial.write(boolsPacked);
    boolsPacked = (status_txu_oe ? 1 : 0) | (status_d3 ? 2 : 0) | (status_d11 ? 4 : 0) | (status_d13 ? 8 : 0) | (status_rgb_led_r ? 16 : 0) | (status_rgb_led_g ? 32 : 0) | (status_rgb_led_b ? 64 : 0) | (status_flag_3 ? 128 : 0);
    Serial.write(boolsPacked);

    // Send the ints
    sendUnsignedInt(status_motor1_pwm);
    sendUnsignedInt(status_motor1_cs);
    sendUnsignedInt(status_motor2_pwm);
    sendUnsignedInt(status_motor2_cs);

    sendUnsignedInt(status_m1_enc_count);
    sendUnsignedInt(status_m1_rot_rate);
    sendUnsignedInt(status_m2_enc_count);
    sendUnsignedInt(status_m2_rot_rate);

    sendInt(status_acc_x);
    sendInt(status_acc_y);
    sendInt(status_acc_z);

    // Send the termination character
    Serial.write('\n');
}*/