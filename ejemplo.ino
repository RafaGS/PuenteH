///
/// Ejemplo de control de Puente H
///
/// by RafaG
///

#include "PuenteH.h"

/// Definición de pines
///

// Motor A
const int ENA = 5; // Pin PWM
const int IN1 = 8;
const int IN2 = 13;

// Motor B
const int ENB = 6; // Pin PWM
const int IN3 = 4;
const int IN4 = 7;
///
/// Fin de definición de pines

/// Instancia a clase PuenteH
///
PuenteH puenteH(PuenteH::L298);
///
/// Fin de instancia a clase PuenteH

/// Funciones básicas
///
void setup() {
    Serial.begin(9600);
    Serial.println("Inicio de sistema:");

    puenteH.setMotor1(ENA, IN1, IN2);
    puenteH.setMotor2(ENB, IN3, IN4);
    puenteH.initMotors();
}

void loop() {
    puenteH.goForward(200);
    delay(1000);
    puenteH.goBackward(200);
    delay(1000);
    puenteH.turnLeft(200);
    delay(1000);
    puenteH.turnRight(200);
    delay(1000);
    puenteH.stop();
    delay(1000);
}
///
/// Fin funciones básicas
