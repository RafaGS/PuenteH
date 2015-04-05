/*
   MotorCtrl.h - Clase para control de motor
   Created by Rafa Gomez, 2014, 2015
   Licensed under GPL v3
   */
#ifndef MotorCtrl_h
#define MotorCtrl_h

#include "Arduino.h"

class MotorCtrl
{
    public:
        enum resultados {OK, TOOBIG, TOOSMALL};
        enum direccion {AVANZA, RETROCEDE, ND};

        MotorCtrl(int pinPWM, int pinDIRECTION1, int pinDIRECTION2 = -1);
        ~MotorCtrl();
        resultados setSpeed(int pwm);
        void stop();
        void setDirection(direccion);
        void initMotor();
        void setOffset(int offset);
        int getOffset();
        void setConstants(int max, int min, int start);

    private:
        int MAX_PWM;
        int MIN_PWM;
        int START_PWM;

        int _pinPWM; // pin de PWM
        int _pinDIRECTION1; // primer pin de avance/retroceso
        int _pinDIRECTION2; // segundo pin de avance/retroceso
        int _maxPWM; // valor máximo permitido de PWM
        int _minPWM; // valor mínimo permitido de PWM (debajo de este valor no tiene efecto sobre el motor)
        int _startPWM; // valor necesario para movimiento de motor desde reposo
        int _offSetPWM; // corrección de velocidad
        int _velPWM; // velocidad del motor
        direccion _direction;
};

#endif
