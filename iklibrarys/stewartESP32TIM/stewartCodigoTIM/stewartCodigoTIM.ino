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
    
    // Diagnóstico
    Serial.print("Estado Mov (0 = OK): ");
    Serial.println(movOK); 
    
    hx_servo.updateServos(movOK);
}

/**
 * Imprime los ángulos actuales de los 6 servos.
 */
void imprimirServos(const char* etiqueta)
{
    Serial.print(etiqueta);
    Serial.print(" (grados) -> ");
    for (int i = 0; i < NB_SERVOS; i++) {
        Serial.print(servo_angles[i].deg, 1);
        Serial.print("  ");
    }
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    delay(1000); // Espera para abrir el Monitor Serie

    Serial.println("\n=== PRUEBA DE CINEMATICA CON GRADOS ===");

    // Inicializar GPIO y servos
    hx_gpio.setupGPIO();
    hx_servo.setupServo();
    delay(1000);

    // --- HOME (Z = 0) ---
    Serial.println("\n>>> Moviendo a HOME (Z = 0)");
    moverPlataforma(0, 0, 0, 0, 0, 0);
    imprimirServos("HOME");

    delay(3000); // 3 segundos para observar

    // --- SUBIR A Z = +10 mm ---
    Serial.println("\n>>> Subiendo a Z = +10 mm");
    moverPlataforma(0, 0, 10, 0, 0, 0);
    imprimirServos("Z=+10");

    delay(3000);

    // --- VOLVER A HOME (Z = 0) ---
    Serial.println("\n>>> Volviendo a HOME (Z = 0)");
    moverPlataforma(0, 0, 0, 0, 0, 0);
    imprimirServos("HOME");

    Serial.println("\n=== FIN DE LA PRUEBA ===");
}

void loop()
{
    // Vacío
}