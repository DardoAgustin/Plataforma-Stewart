/**
 * S T E W A R T    P L A T F O R M    O N    E S P 3 2
 *
 * Copyright (C) 2019  Nicolas Jeanmonod, ouilogique.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


#include <Arduino.h>
#include <ESP32Servo.h>

#include <hexapod_Arduino.h>
#include <Hexapod_GPIO.h>
#include <Hexapod_Kinematics.h>
#include <Hexapod_Servo.h>
#include <Hexapod_Config_1.h>

// Variables globales
angle_t servo_angles[NB_SERVOS];
Hexapod_Servo hx_servo;
Hexapod_GPIO hx_gpio;

/**
 * Mueve la plataforma a coordenadas (X, Y, Z, roll, pitch, yaw).
 * X, Y, Z en mm; roll, pitch, yaw en grados.
 */
void moverPlataforma(float X, float Y, float Z, float roll, float pitch, float yaw)
{
    platform_t coord;
    coord.hx_x = X;
    coord.hx_y = Y;
    coord.hx_z = Z;
    coord.hx_a = roll  * DEG_TO_RAD;
    coord.hx_b = pitch * DEG_TO_RAD;
    coord.hx_c = yaw   * DEG_TO_RAD;

    int8_t movOK = hx_servo.calcServoAngles(coord, servo_angles);
    hx_servo.updateServos(movOK);
}

/**
 * Imprime los ángulos actuales de los 6 servos.
 */
void imprimirServos(const char* etiqueta)
{
    Serial.print(etiqueta);
    Serial.print(" -> ");
    for (int i = 0; i < NB_SERVOS; i++) {
        Serial.print(servo_angles[i].deg, 1);
        Serial.print("  ");
    }
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n=== PRUEBA DE GIRO CONJUNTO ===");

    // Inicializar GPIO y servos
    hx_gpio.setupGPIO();
    hx_servo.setupServo();
    delay(1000);

    // --- HOME (Z = 0) ---
    Serial.println("\n>>> HOME (Z = 0)");
    moverPlataforma(0, 0, 0, 0, 0, 0);
    imprimirServos("HOME");

    delay(2000);

    // --- SUBIR A Z = +10 mm ---
    Serial.println("\n>>> SUBIENDO a Z = +10 mm");
        moverPlataforma(0, 0, 10, 0, 0, 0);
    imprimirServos("Z=+10");

    delay(2000);

    // --- VOLVER A HOME (Z = 0) ---
    Serial.println("\n>>> VOLVIENDO a HOME (Z = 0)");
    moverPlataforma(0, 0, 0, 0, 0, 0);
    imprimirServos("HOME");

    Serial.println("\n=== FIN ===");
}

void loop()
{
    // Vacío
}


//---------------------------------------------------------------------------


/*
// External libs.
#include <Arduino.h>
#include <ESP32Servo.h>
//internal Files.
#include <hexapod_Arduino.h> 
#include <Hexapod_GPIO.h>
#include <Hexapod_Kinematics.h> //interno de la lib
#include <Hexapod_Servo.h> 
#include <Hexapod_Config_1.h> //parametros de la plataforma

#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
unsigned long ultimoPrint = 0;
const unsigned long intervaloPrint = 100;


// Variables globales
angle_t servo_angles[NB_SERVOS];
Hexapod_Servo hx_servo;
Hexapod_GPIO hx_gpio;

//Mueve la plataforma a los ángulos de roll y pitch indicados (en grados)
   
 
void moverPlataforma(float roll_grados, float pitch_grados)
{
    platform_t coord;
    coord.hx_x = 0.0;
    coord.hx_y = 0.0;
    coord.hx_z = 0.0;
    coord.hx_a = roll_grados  * DEG_TO_RAD;
    coord.hx_b = pitch_grados * DEG_TO_RAD;
    coord.hx_c = 0.0;

    int8_t movOK = hx_servo.calcServoAngles(coord, servo_angles); //los int8_t son variables definidas especificamente de 8 bits.
    hx_servo.updateServos(movOK);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) { delay(10); } 
    Wire.begin(21, 22); // SDA, SCL 
    Serial.println("iniciando MPU");
    byte status = mpu.begin(0, 0); //giroscopio, acelerometro, 0=max, 3=min, donde el max es +-250grados/s y +-2g
    Serial.print("estado del MPU -deberia ser 0-  ");
    Serial.println(status);
    while (status != 0) { 
    // corrobora que este realmente conectado.
    delay(500);
    }
    Serial.println("calibrando :");
    mpu.calcOffsets(true, true); // calibra giroscopio y acelerómetro en reposo
    Serial.println("CALIBRACIONES COMPLETAS");
    Serial.println("-----------------------------------------");
    Serial.println("-----------------------------------------");
    Serial.println("-----------------------------------------");


//servoes.
    hx_gpio.setupGPIO();
    hx_servo.setupServo();
    delay(500);

    // Prueba: mover a roll = +5°, pitch = -3°
    moverPlataforma(5.0, -3.0);
    delay(1000);
    for (int u=0; u<6; u++)
            {Serial.print(servo_angles[u].deg);}
        Serial.println("--");

    // Volver a home
    moverPlataforma(0.0, 0.0);
    for (int u=0; u<6; u++)
            {Serial.println(servo_angles[u].deg);}
        Serial.println("--");
}

void loop()
{

    mpu.update();
    if (millis() - ultimoPrint > intervaloPrint) { //delay sin interrupciones.
        ultimoPrint = millis();

        Serial.print("Roll (X): ");
        Serial.print(mpu.getAngleX());
        Serial.print("\tPitch (Y): ");
        Serial.print(mpu.getAngleY());
        Serial.print("\tYaw (z): ");
        Serial.println(mpu.getAngleZ());
        
        Serial.println("--");
        for (int u=0; u<6; u++)
            {Serial.println(servo_angles[u].deg);}
        Serial.println("--");
    }

    
    {
        
    }
    
   
}

*/