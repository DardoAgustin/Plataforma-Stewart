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

// External libs.
#include <Arduino.h>
#include <ESP32Servo.h>
//internal Files.
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
 * Mueve la plataforma a los ángulos de roll y pitch indicados (en grados)
   
 */
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
    hx_gpio.setupGPIO();
    hx_servo.setupServo();
    delay(500);

    // Prueba: mover a roll = +5°, pitch = -3°
    moverPlataforma(5.0, -3.0);
    delay(1000);

    // Volver a home
    moverPlataforma(0.0, 0.0);
}

void loop()
{
    /*
    Probar servos, definir homes 
    Configurar MPU

    */
}