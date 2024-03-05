### 1. Line Follower Robot Introduction:

Line following robots are robots designed to follow a specific line on the ground, usually using one or more line tracking sensors. These sensors detect color differences on the surface and control the robot's movement using this information.

### 2. PID Control:

PID (Proportional-Integral-Derivative) control is a control strategy to correct system behavior by analyzing the error signal in a system and generating a control signal accordingly. PID control algorithms correct the system using the difference between a target value and the actual value.

### 3. Line Tracing Algorithm:

Line following robots generally follow the line by measuring the light reflected from the ground with sensors. Using the information from these sensors, the PID control algorithm is used to make the robot move on the line.

### 4. Basic Mathematical Calculations:

#### 4.1. Error Calculation:
Error (`error`) is a value that indicates how much the sensor value deviates from the expected value. This represents the difference between the reading from the line tracking sensors and the median value (for example, 1500).

```cpp
error = qtra.readLine(sensorValues, 1, ground) - 1500;
```

#### 4.2. PID Control:
PID control is based on the error value and the time derivative of the error change. The parameters Kp, Kd and Kp determine how we will use these values.

```cpp
int correctionrate = Kp * error + Kd * (error - posterror);
```

#### 4.3. Engine Speeds:
The resulting corrected error (`correction speed`) is used with the base speed value to calculate the right and left engine speeds.

```cpp
solmotorpwm = basespeed + correctionspeed;
sagmotorpwm = basespeed - correctionspeed;
```

### 5. Engine Control Function:

The motor control function appropriately adjusts the PWM signals of the right and left motors and also determines the direction of movement.

```cpp
void motorkontrol(int sagmotorpwm, int solmotorpwm) {
   // Speed and direction control of right and left motors
   // Direction information is printed to Serial Monitor
}
```

### 6. PID Control Algorithm:

```cpp
int correctionrate = Kp * error + Kd * (error - posterror);
posterror = error;
solmotorpwm = basespeed + correctionspeed;
sagmotorpwm = basespeed - correctionspeed;
solmotorpwm = constrain(solmotorpwm, -255, maxhiz);
sagmotorpwm = constrain(sagmotorpwm, -255, maxhiz);
```

- `Kp` (Proportional Gain): Provides a correction proportional to the error. So, if the robot deviates from the line, this proportional correction amount increases.
  
- `Kd` (Derivative Gain): Controls the change of the error over time. This helps in quick fixing of the robot.

- `correctionrate`: It is the output of the PID control algorithm. It is obtained by multiplying the error by Kp and Kd.

- `lasterror`: Stores the previous measured error value.

- `solmotorpwm` and `sagmotorpwm`: Speed values corrected by PID control.

- `constrain()`: This function provides a value in the specified range. Engine speeds should be between -255 and +255.

### 7. Engine Control Function:

```cpp
void motorkontrol(int sagmotorpwm, int solmotorpwm) {
   if (sagmotorpwm <= 0) {
     analogWrite(sagmotor1, 0);
     analogWrite(sagmotor2, abs(sagmotorpwm));
   } else {
     analogWrite(sagmotor1, sagmotorpwm);
     analogWrite(sagmotor2, 0);
   }

   if (solmotorpwm <= 0) {
     analogWrite(solmotor1, abs(solmotorpwm));
     analogWrite(solmotor2, 0);
   } else {
     analogWrite(solmotor1, 0);
     analogWrite(solmotor2, solmotorpwm);
   }

   // Direction information is printed to Serial Monitor
   if (sagmotorpwm > 0 && solmotorpwm > 0) {
     Serial.println("Forward");
   } else if (sagmotorpwm < 0 && solmotorpwm < 0) {
     Serial.println("Backward");
   } else if (sagmotorpwm > 0 && solmotorpwm == 0) {
     Serial.println("Right");
   } else if (sagmotorpwm == 0 && solmotorpwm > 0) {
     Serial.println("Left");
   }
}
```

This function allows the right and left motors to be controlled according to their PWM values. Additionally, the movement direction information of the motors is printed to the Serial Monitor.

### 8. Main `loop()` Function:

```cpp
void loop() {
   error = qtra.readLine(sensorValues, 1, ground) - 1500;
   int correctionrate = Kp * error + Kd * (error - posterror);
   posterror = error;
   solmotorpwm = basespeed + correctionspeed;
   sagmotorpwm = basespeed - correctionspeed;
   solmotorpwm = constrain(solmotorpwm, -255, maxhiz);
   sagmotorpwm = constrain(sagmotorpwm, -255, maxhiz);
   motorkontrol(sagmotorpwm, leftmotorpwm);
}
```

### 9. Line Tracing Algorithm:

Line tracking sensors usually detect the line on the ground using an array of photocells or IR sensors. Each of the sensors produces different values depending on whether they are on or off the line.

### 10. PID Control Algorithm:

The PID control algorithm uses information from the robot's line tracking sensors to stay on the line. This algorithm calculates motor speeds using the terms error (difference between the desired position and sensor output), error variation (change of error value over time) and integral (sum of error values) settings.

```cpp
int error = (qtra.readLine(sensorValues, 1, ground) - 1500);
int correctionrate = Kp * error + Kd * (error - posterror);
posterror = error;
solmotorpwm = basespeed + correctionspeed;
sagmotorpwm = basespeed - correctionspeed;
solmotorpwm = constrain(solmotorpwm, -255, maxhiz);
sagmotorpwm = constrain(sagmotorpwm, -255, maxhiz);
```

- `error`: Represents the difference between the reading value received from line tracking sensors and the expected value.

- `correctionrate`: It is the output of the PID control algorithm. It is calculated in terms of Kp and Kd and the error value.

- `lasterror`: Stores the previous measured error value.

- `solmotorpwm` and `sagmotorpwm`: Speed values corrected by PID control.

### 11. Engine Control Function:

The engine control function controls the engines according to calculated engine speeds.

```cpp
void motorkontrol(int sagmotorpwm, int solmotorpwm) {
   if (sagmotorpwm <= 0) {
     analogWrite(sagmotor1, 0);
     analogWrite(sagmotor2, abs(sagmotorpwm));
   } else {
     analogWrite(sagmotor1, sagmotorpwm);
     analogWrite(sagmotor2, 0);
   }

   if (solmotorpwm <= 0) {
     analogWrite(solmotor1, abs(solmotorpwm));
     analogWrite(solmotor2, 0);
   } else {
     analogWrite(solmotor1, 0);
     analogWrite(solmotor2, solmotorpwm);
   }

   // Direction information is printed to Serial Monitor
   if (sagmotorpwm > 0 && solmotorpwm > 0) {
     Serial.println("Forward");
   } else if (sagmotorpwm < 0 && solmotorpwm < 0) {
     Serial.println("Backward");
   } else if (sagmotorpwm > 0 && solmotorpwm == 0) {
     Serial.println("Right");
   } else if (sagmotorpwm == 0 && solmotorpwm > 0) {
     Serial.println("Left");
   }
}
```

This function controls the right and left motors according to the calculated motor speeds. Additionally, the movement direction information of the motors is printed to the Serial Monitor.

### 12. Main `setup()` Function:

```cpp
void setup() {
   pinMode(solmotor1, OUTPUT);
   pinMode(solmotor2, OUTPUT);
   pinMode(sagmotor1, OUTPUT);
   pinMode(sagmotor2, OUTPUT);
   tone(led_buzzer, 500, 100);
   delay(50);
   noTone(led_buzzer);
   Serial.begin(9600);

   for (i = 0; i < 80; i++) {
     //Robot moves in certain directions
     // Sensor calibration is done
     qtra.calibrate();
     delay(3);
   }

   motorcontrol(0, 0);
   tone(led_buzzer, 3000, 100);
   delay(50);
   noTone(led_buzzer);
}
```

Thank you for reviewing the detailed explanations on the code of your line follower robot. If you need more help or want to focus on other topics, please let me know. I wish you success and happy coding!

---

Mr.red

Embedded System Developer & Android Front End Developer
