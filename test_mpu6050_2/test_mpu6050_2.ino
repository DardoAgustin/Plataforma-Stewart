/*
  Prueba básica de MPU6050 con la librería MPU6050_light en ESP32.

  Antes de subir este código:
  1. Instalá la librería "MPU6050_light" desde el Gestor de Librerías
     del Arduino IDE (Herramientas -> Administrar bibliotecas -> buscar "MPU6050_light" de rfetick).
  2. Conexiones típicas ESP32 <-> MPU6050:
       VCC  -> 3.3V
       GND  -> GND
       SDA  -> GPIO21
       SCL  -> GPIO22
  3. Dejá el sensor quieto y nivelado durante el arranque (calibración de offsets).
*/

#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

unsigned long ultimoPrint = 0;
const unsigned long intervaloPrint = 100; // ms entre impresiones (10 Hz, suficiente para ver en el monitor)

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  Wire.begin(21, 22); // SDA, SCL (ajustá si usaste otros pines)

  Serial.println("Inicializando MPU6050...");
  // gyroConfig=0 -> rango +-250 grados/s (maxima sensibilidad de giro)
  // accConfig=0  -> rango +-2g (maxima sensibilidad de acelerometro)
  byte status = mpu.begin(0, 0);
  Serial.print("Estado de conexion MPU6050 (0 = OK): ");
  Serial.println(status);

  while (status != 0) { 
    // Si falla, revisá cableado y dirección I2C (0x68 por defecto)
    delay(500);
  }

  Serial.println("No muevas el sensor, calculando offsets...");
  delay(1000);
  mpu.calcOffsets(true, true); // calibra giroscopio y acelerómetro en reposo
  Serial.println("Listo! Offsets calculados.");
}

void loop() {
  mpu.update();

  if (millis() - ultimoPrint > intervaloPrint) { //delay sin interrupciones
    ultimoPrint = millis(); 

    Serial.print("Roll (X): ");
    Serial.print(mpu.getAngleX());
    Serial.print("\tPitch (Y): ");
    Serial.print(mpu.getAngleY());
    Serial.print("\tYaw (Z, no confiable): ");
    Serial.print(mpu.getAngleZ());
    Serial.print ("\tVelocidad Angular : " );
    Serial.println(mpu.getGyroZ());
  }
}
