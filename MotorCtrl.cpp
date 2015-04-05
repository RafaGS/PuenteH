/*
  MotorCtrl - Clase para control de motor 
  Created by Rafa Gomez, 2014, 2015
  Licensed under GPL v3
*/

#include "MotorCtrl.h"

MotorCtrl::MotorCtrl(int pinPWM, int pinDIRECTION1, int pinDIRECTION2)
{
    _pinPWM = pinPWM;
    _pinDIRECTION1 = pinDIRECTION1;
    _pinDIRECTION2 = pinDIRECTION2;

    _maxPWM = 255;
    _minPWM = 0;
    _startPWM = 1;

    _offSetPWM = 0;
    _velPWM = 0;
    _direction = ND;
}


MotorCtrl::~MotorCtrl()
{
    // Nothing to do
}


void MotorCtrl::initMotor() // Inicializa motor
{
    pinMode(_pinPWM, OUTPUT);
    analogWrite(_pinPWM, 0);
    pinMode(_pinDIRECTION1, OUTPUT);
    if(_pinDIRECTION2 != -1)
        pinMode(_pinDIRECTION2, OUTPUT);
    setDirection(AVANZA);
}


void MotorCtrl::setConstants(int max, int min, int start) // Selecciona los valores máximos y mínimos para PWM
{
    _maxPWM = max;
    _minPWM = min;
    _startPWM = start;
}


MotorCtrl::resultados MotorCtrl::setSpeed(int pwm) // Selecciona la velocidad del motor, comprobando que se encuentre dentro de los márgenes adecuados
{
    MotorCtrl::resultados resultado = OK;

    int min = (_velPWM < _minPWM) ? _startPWM : _minPWM;

    if( (pwm - _offSetPWM) < min ) // PWM está por debajo de valor válido
    {
        resultado = TOOSMALL;
    }
    else if( (pwm - _offSetPWM) > _maxPWM ) // PWM está por encima de valor válido
    {
        resultado = TOOBIG;
    }
    else
    {
        if ( _velPWM != (pwm - _offSetPWM) ) // escribe sólo si el valor ha cambiado
        {
            _velPWM = pwm - _offSetPWM;
            analogWrite(_pinPWM, _velPWM);
        }
    }

    return resultado;
}


void MotorCtrl::setDirection(MotorCtrl::direccion dir) // Selecciona el sentido de giro del motor
{
    if (dir != _direction) // escribe sólo si el valor ha cambiado
    {
        _direction = dir;
        switch(_direction)
        {
            case AVANZA:
                digitalWrite(_pinDIRECTION1, LOW);
                if(_pinDIRECTION2 != -1)
                    digitalWrite(_pinDIRECTION2, HIGH);
                break;
            case RETROCEDE:
                digitalWrite(_pinDIRECTION1, HIGH);
                if(_pinDIRECTION2 != -1)
                    digitalWrite(_pinDIRECTION2, LOW);
                break;
            default:
                // do nothing
                break;
        }
    }
}


void MotorCtrl::stop()  // detiene el motor
{
    if ( _velPWM != 0 ) // escribe sólo si el valor ha cambiado
    {
        if(_pinDIRECTION2 != -1) { // Usa combinaciones de pines de dirección para seleccionar dirección/frenar
            digitalWrite(_pinDIRECTION1, LOW);
            digitalWrite(_pinDIRECTION2, HIGH);
        }
        _velPWM = 0;
        analogWrite(_pinPWM, _velPWM);
    }
}


void MotorCtrl::setOffset(int offset)  // selecciona el offset de velocidad del motor
{
    int pwm_raw = _velPWM + _offSetPWM;
    _offSetPWM = offset;
    setSpeed(pwm_raw);
}


int MotorCtrl::getOffset()  // devuelve el offset de velocidad del motor
{
    return _offSetPWM;
}

