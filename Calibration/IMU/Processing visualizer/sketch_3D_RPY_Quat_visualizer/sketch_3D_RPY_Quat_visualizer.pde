import processing.serial.*;
Serial myPort;
String inputString = "";

float yaw = 0.0;
float pitch = 0.0;
float roll = 0.0;

float accErr = 0.0;
float accIgnored = 0;
float accRecTrig = 0.0;
float magErr = 0.0;
float magIgnored = 0;
float magRecTrig = 0.0;

float initializing = 0;
float angRec = 0;
float accRec = 0;
float magRec = 0;

float gain = 0.0;
int convention = 0;
float accelerationRejection = 0.0;
float magneticRejection = 0.0;
int recoveryTriggerPeriod = 0;

float time_loop_interval_ms = 0;

PFont f;

void setup()
{
  size(600, 920, P3D);

  f = createFont("Arial", 20, true); // STEP 2 Create Font

  ortho(-width/4, width/4, -height/4, height/4); // Same as ortho()

  // if you have only ONE serial port active
  myPort = new Serial(this, Serial.list()[0], 9600); // if you have only ONE serial port active

  // if you know the serial port name
  //myPort = new Serial(this, "COM5:", 9600);                    // Windows
  //myPort = new Serial(this, "/dev/ttyACM0", 9600);             // Linux
  //myPort = new Serial(this, "/dev/cu.usbmodem1217321", 9600);  // Mac

  textSize(8); // set text size
  textMode(MODEL); // set text mode to shape
}

void draw()
{


  serialEvent();  // read and parse incoming serial message
  background(255); // set background to white
  lights();

  translate(width/2, height/2); // set position to centre

  pushMatrix(); // begin object

  float c1 = cos(radians(roll));
  float s1 = sin(radians(roll));
  float c2 = cos(radians(pitch));
  float s2 = sin(radians(pitch));
  float c3 = cos(radians(yaw));
  float s3 = sin(radians(yaw));
  applyMatrix( c2*c3, s1*s3+c1*c3*s2, c3*s1*s2-c1*s3, 0,
    -s2, c1*c2, c2*s1, 0,
    c2*s3, c1*s2*s3-c3*s1, c1*c3+s1*s2*s3, 0,
    0, 0, 0, 1);

  drawArduino();



  popMatrix(); // end of object

  textFont(f, 8);
  fill(0);

  float float_width = 35.0;
  float float_height = 10.0;

  float text_start_left = -130.0;
  float text_start_top = 100.0;

  // RPY Block
  text("R", -150, text_start_top);
  text(String.format("%.2f", roll), text_start_left, text_start_top);
  text("P", -150, text_start_top + float_height);
  text(String.format("%.2f", pitch), text_start_left, text_start_top + float_height);
  text("Y", -150, text_start_top + 2 * float_height);
  text(String.format("%.2f", yaw), text_start_left, text_start_top + 2 * float_height);


  text_start_left = -100.0;
  text_start_top = text_start_top + 4 * float_height;

  text("acc error:", -150, text_start_top);
  text(String.format("%.2f", accErr), text_start_left, text_start_top);

  text("acc ingored:", -150, text_start_top + float_height);
  text(String.format("%.2f", accIgnored), text_start_left, text_start_top + float_height);

  text("acc rec. Trig.:", -150, text_start_top + 2 * float_height);
  text(String.format("%.2f", accRecTrig), text_start_left, text_start_top + 2 * float_height);

  text_start_left = text_start_left + float_width;

  text("mag error:", text_start_left, text_start_top);
  text(String.format("%.2f", magErr), text_start_left + 2 * float_width, text_start_top);

  text("mag ingored:", text_start_left, text_start_top + float_height);
  text(String.format("%.2f", magIgnored), text_start_left + 2 * float_width, text_start_top + float_height);

  text("mag rec. Trig.:", text_start_left, text_start_top + 2 * float_height);
  text(String.format("%.2f", magRecTrig), text_start_left + 2 * float_width, text_start_top + 2 * float_height);


  text_start_left = text_start_left + 2*float_width;

  text("ang recovery:", text_start_left + float_width, text_start_top);
  text(String.format("%.2f", angRec), text_start_left + 3 * float_width, text_start_top);

  text("acc recovery:", text_start_left + float_width, text_start_top + float_height);
  text(String.format("%.2f", accRec), text_start_left + 3 * float_width, text_start_top + float_height);

  text("mag recovery", text_start_left + float_width, text_start_top + 2 * float_height);
  text(String.format("%.2f", magRec), text_start_left + 3 * float_width, text_start_top + 2 * float_height);

  text("initializing:", text_start_left + float_width, text_start_top + 3 * float_height);
  text(String.format("%.2f", initializing), text_start_left + 3 * float_width, text_start_top + 3 * float_height);

  text("Loop Time:", text_start_left + float_width, text_start_top + 7 * float_height);
  text(String.format("%.2f", time_loop_interval_ms), text_start_left + 3 * float_width, text_start_top + 7 * float_height);

  text_start_top =  text_start_top + 4 * float_height;
  text_start_left = -100.0;

  text("Gain:", -150, text_start_top);
  text(String.format("%.2f", gain), text_start_left, text_start_top);
  text_start_top = text_start_top + float_height;
  text("Convention:", -150, text_start_top);
  text(String.format("%.0f", float(convention)), text_start_left, text_start_top );
  text_start_top = text_start_top + float_height;
  text("Acc Rej:", -150, text_start_top);
  text(String.format("%.2f", accelerationRejection), text_start_left, text_start_top );
  text_start_top = text_start_top + float_height;
  text("Mag Rej:", -150, text_start_top);
  text(String.format("%.2f", magneticRejection), text_start_left, text_start_top );
  text_start_top = text_start_top + float_height;
  text("Rec Trig Prd:", -150, text_start_top);
  text(String.format("%.0f", float(recoveryTriggerPeriod)), text_start_left, text_start_top );
  /*

   float gain = float(gain_str);
   int convention = int(convention_str);
   float accelerationRejection = float(accelerationRejection_str);
   float magneticRejection = float(magneticRejection_str);
   int recoveryTriggerPeriod = int(recoveryTriggerPeriod_str);*/





  text("Command:", -80, 110);
  text(inputString, -80, 120);

  // Print values to console
  /*  print(roll);
   print("\t");
   print(pitch);
   print("\t");
   print(yaw);
   println();*/
}

void serialEvent()
{
  int newLine = 13; // new line character in ASCII
  String message;
  do {
    message = myPort.readStringUntil(newLine); // read from port until new line
    if (message != null) {

      String[] list = split(trim(message), " ");


      if (list.length >= 4 && list[0].equals("Orientation:"))
      {

        String yawstring = list[1].substring(0, list[1].length() - 1);
        String pitchstring = list[2].substring(0, list[2].length() - 1);
        String rollstring = list[3].substring(0, list[3].length() - 1);
        yaw = float(yawstring); // convert to float yaw
        pitch = float(pitchstring); // convert to float pitch
        roll = float(rollstring); // convert to float roll
      }
      if (list.length >= 12 && list[0].equals("States&Flags:"))
      {

        String accErr_str = list[1].substring(0, list[1].length() - 1);
        String accIgnored_str = list[2].substring(0, list[2].length() - 1);
        String accRecTrig_str = list[3].substring(0, list[3].length() - 1);
        String magErr_str = list[4].substring(0, list[4].length() - 1);
        String magIgnored_str = list[5].substring(0, list[5].length() - 1);
        String magRecTrig_str = list[6].substring(0, list[6].length() - 1);

        String initializing_str = list[7].substring(0, list[7].length() - 1);
        String angRec_str = list[8].substring(0, list[8].length() - 1);
        String accRec_str = list[9].substring(0, list[9].length() - 1);
        String magRec_str = list[10].substring(0, list[10].length() - 1);
        String time_loop_interval_ms_str = list[11];

        print("Acc Err: ");
        print(accErr_str);
        print("\tAcc Ignored: ");
        print(accIgnored_str);
        print("\tAcc Rec Trig: ");
        print(accRecTrig_str);
        print("\tMag Err: ");
        print(magErr_str);
        print("\tMag Ignored: ");
        print(magIgnored_str);
        print("\tMag Rec Trig: ");
        print(magRecTrig_str);
        print("\tAng Rec: ");
        print(angRec_str);
        print("\tAcc Rec: ");
        print(accRec_str);
        print("\tMag Rec: ");
        print(magRec_str);
        print("\tInitializing: ");
        print(initializing_str);
        print("\tLoop Time ms: ");
        println(time_loop_interval_ms_str);



        accErr = float(accErr_str);
        accIgnored = float(accIgnored_str);
        accRecTrig = float(accRecTrig_str);
        magErr = float(magErr_str);
        magIgnored = float(magIgnored_str);
        magRecTrig = float(magRecTrig_str);

        initializing = float(initializing_str);
        angRec = float(angRec_str);
        accRec = float(accRec_str);
        magRec = float(magRec_str);
        time_loop_interval_ms =  float(time_loop_interval_ms_str);
      }
      if (list.length >= 6 && list[0].equals("Settings:"))
      {
        String gain_str = list[1].substring(0, list[1].length() - 1);
        String convention_str = list[2].substring(0, list[2].length() - 1);
        String accelerationRejection_str = list[3].substring(0, list[3].length() - 1);
        String magneticRejection_str = list[4].substring(0, list[4].length() - 1);
        String recoveryTriggerPeriod_str = list[5];

        gain = float(gain_str);
        convention = int(convention_str);
        accelerationRejection = float(accelerationRejection_str);
        magneticRejection = float(magneticRejection_str);
        recoveryTriggerPeriod = int(recoveryTriggerPeriod_str);

        print("Gain: ");
        print(gain);
        print("\tConv: ");
        print(convention);
        print("\tAccRej: ");
        print(nf(accelerationRejection, 0, 2));
        print("\tMagRej: ");
        print(nf(magneticRejection, 0, 2));
        print("\tRecTrig Period: ");
        println(recoveryTriggerPeriod);
      }
    }
  } while (message != null);
}

void drawArduino()
{
  /* function contains shape(s) that are rotated with the IMU */
  stroke(0, 90, 90); // set outline colour to darker teal
  fill(0, 130, 130); // set fill colour to lighter teal
  box(120, 5, 80); // draw Arduino board base shape
  translate(20, -10, 0); // set position to edge of Arduino box
  box(65, 5, 65);

  translate(0, 0, -100); // set position to edge of Arduino box
  stroke(255, 0, 0); // set outline colour to darker teal
  fill(255, 0, 0); // set fill colour to lighter teal
  box(3, 3, 200);
  translate(0, 0, 100); // set position to edge of Arduino box

  translate(-100, 0, 0);
  stroke(0, 255, 0); // set outline colour to darker teal
  fill(0, 255, 0); // set fill colour to lighter teal
  box(200, 3, 3);
  translate(100, 0, 0);

  translate(0, -100, 0);
  stroke(0, 0, 255); // set outline colour to darker teal
  fill(0, 0, 255); // set fill colour to lighter teal
  box(3, 200, 3);
  translate(0, 100, 0);
}

void keyPressed() {
  if (key == '\n') {
    // Enter was pressed, send the string over serial
    myPort.write(inputString );
    inputString = "";
  } else if (key == BACKSPACE) {
    if (inputString.length() > 0) {
      inputString = inputString.substring(0, inputString.length() - 1);
    }
  } else {
    // Add the character to the input string
    inputString += key;
  }
}
