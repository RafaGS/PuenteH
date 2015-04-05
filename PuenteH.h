/*
   PuenteH.h - Library for controlling dual H-Bridge motor drivers
   Created by Rafa Gomez, 2014, 2015
   Licensed under GPL v3
   */
#ifndef PuenteH_h
#define PuenteH_h

#include "Arduino.h"
#include "MotorCtrl.h"

class PuenteH
{
    public:
        enum modeloPuenteH {IRF3205, TB6612, L298, L293, L9110};

        PuenteH(modeloPuenteH modelo, int pinENABLE = -1);
        ~PuenteH();
        void setMotor1(int pinPWM, int pinINA, int pinINB = -1);
        void setMotor2(int pinPWM, int pinINA, int pinINB = -1);
        void initMotors();
        void turnLeft(int speed);
        void turnRight(int speed);
        void goForward(int speed);
        void goBackward(int speed);
        void stop();
        void correctToLeft();
        void correctToRight();
        void resetCorrectToLeftAndRight();

    private:
        enum modo {MODO_UNDEF, MODO_STOP, MODO_AVANZA, MODO_RETROCEDE, MODO_GIRO_IZQUIERDA, MODO_GIRO_DERECHA};

        MotorCtrl *_motor1;
        MotorCtrl *_motor2;

        void setDirSpeed(modo candidato, int speed);
        void selectConstants(modeloPuenteH modelo);
        
        int _MAX_PWM;
        int _MIN_PWM;
        int _START_PWM;

        int _pinENABLE;
        modo _modoActual;
};

#endif
