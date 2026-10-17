/*
  Estabilizador Stewart: MPU6050 (en la base) + ESP32 + posicion(roll, pitch, yaw)

  Idea: el sensor mide cuanto se inclina la BASE. La plataforma superior
  tiene que inclinarse lo MISMO pero en sentido contrario para quedar nivelada.
  Por eso se le pasa a posicion() el angulo medido con signo invertido.
*/

#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

// ---------- Parametros ajustables ----------
const float LIMITE_GRADOS = 10.0;    // maximo angulo que se intenta compensar (roll/pitch)
const float SIGNO_ROLL    = -1.0;    // -1 = compensar. Si la plataforma se mueve "para el mismo lado", poner +1
const float SIGNO_PITCH   = -1.0;    // idem para pitch
const float GANANCIA      = 1.0;     // 1.0 = compensacion total. Bajar si oscila
const float SUAVIZADO     = 0.3;     // 0..1 (1 = sin suavizado, menor = mas suave pero mas lento)
const unsigned long PERIODO_MS = 10; // ciclo de control: 10 ms = 100 Hz

// ---------- Estado ----------
float rollSuave  = 0;
float pitchSuave = 0;
unsigned long ultimoCiclo = 0;

// ---------- Funcion del companero (cinematica inversa) ----------
// Recibe angulos en grados y acomoda la plataforma.
// Por ahora es un stub que solo imprime, reemplazar por la real.
void posicion(float e1_roll, float e2_pitch, float e3_yaw) {
  Serial.print("posicion -> roll: ");
  Serial.print(e1_roll);
  Serial.print("  pitch: ");
  Serial.print(e2_pitch);
  Serial.print("  yaw: ");
  Serial.println(e3_yaw);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  byte status = mpu.begin(0, 0);   // maxima sensibilidad
  while (status != 0) {
    Serial.println("Error al conectar con el MPU6050, revisar cableado");
    delay(1000);
  }

  Serial.println("No mover la base, calibrando...");
  delay(1000);
  mpu.calcOffsets(true, true);     // la base tiene que estar quieta y nivelada
  Serial.println("Calibrado.");

  posicion(0, 0, 0);               // arrancar en posicion neutra
}

void loop() {
  mpu.update();                    // llamar lo mas seguido posible

  if (millis() - ultimoCiclo < PERIODO_MS) return;
  ultimoCiclo = millis();

  // 1) Inclinacion actual de la base (grados)
  float rollBase  = mpu.getAngleX();
  float pitchBase = mpu.getAngleY();



  // 3) Compensacion: angulo opuesto, con ganancia y limite de seguridad
  float rollObjetivo  = constrain(SIGNO_ROLL  * GANANCIA * rollSuave,  -LIMITE_GRADOS, LIMITE_GRADOS);
  float pitchObjetivo = constrain(SIGNO_PITCH * GANANCIA * pitchSuave, -LIMITE_GRADOS, LIMITE_GRADOS);

  // 4) Mandar a la cinematica inversa (yaw siempre 0: no se controla)
  posicion(rollObjetivo, pitchObjetivo, 0);
}
