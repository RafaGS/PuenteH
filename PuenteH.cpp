/*
   PuenteH - Library for controlling dual H-Bridge motor drivers
   Created by Rafa Gomez, 2014, 2015
   Licensed under GPL v3
   */

#include "PuenteH.h"

PuenteH::PuenteH(modeloPuenteH modelo, int pinENABLE) // Constructor para puente H
{
    _pinENABLE = pinENABLE;
    _modoActual = MODO_UNDEF; // No están inicializados los motores

    MotorCtrl *_motor1 = NULL;
    MotorCtrl *_motor2 = NULL;

    selectConstants(modelo);
}


PuenteH::~PuenteH()
{
    delete _motor1;
    delete _motor2;
}


void PuenteH::selectConstants(modeloPuenteH modelo) // Selecciona los valores para PWM -máximo, mínimo y de arranque- óptimos para cada puente H
{
        switch(modelo)
        {
            case IRF3205:
                _MAX_PWM = 255;
                _MIN_PWM = 26;
                _START_PWM = 27;
                break;
            case TB6612:
                _MAX_PWM = 255;
                _MIN_PWM = 26;
                _START_PWM = 27;
                break;
            case L298:
                _MAX_PWM = 255;
                _MIN_PWM = 150;
                _START_PWM = 200;
                break;
            case L293:
                _MAX_PWM = 255;
                _MIN_PWM = 90;
                _START_PWM = 100;
                break;
            case L9110:
                _MAX_PWM = 255;
                _MIN_PWM = 90;
                _START_PWM = 100;
                break;
            default:
                // do nothing
                break;
        }
}


void PuenteH::setMotor1(int pinPWM, int pinINA, int pinINB)  // Inicializa motor A
{
    if(!_motor1) {
        _motor1 = new MotorCtrl(pinPWM, pinINA, pinINB);
        _motor1->setConstants(_MAX_PWM, _MIN_PWM, _START_PWM);
    }
}


void PuenteH::setMotor2(int pinPWM, int pinINA, int pinINB)  // Inicializa motor B
{
    if(!_motor2)
        _motor2 = new MotorCtrl(pinPWM, pinINA, pinINB);
        _motor2->setConstants(_MAX_PWM, _MIN_PWM, _START_PWM);
}


void PuenteH::initMotors()  // Inicializa motores
{
    if(!_motor1 || !_motor2) // comprueba si los motores han sido inicializados  
        return;
    _modoActual = MODO_STOP;

    if(_pinENABLE != -1) // si hay definido pin de enable
    {
        pinMode(_pinENABLE, OUTPUT);
        digitalWrite(_pinENABLE, LOW);

        delay(500); // tiempo de espera para que se estabilice el driver

        digitalWrite(_pinENABLE, HIGH);
    }

    _motor1->initMotor();
    _motor2->initMotor();
}


void PuenteH::turnLeft(int speed, bool escala)
{
    if(escala == true) speed = map(speed, 0, 9, _MIN_PWM, _MAX_PWM);
    setDirSpeed(MODO_GIRO_IZQUIERDA, speed);
}


void PuenteH::turnRight(int speed, bool escala)
{
    if(escala == true) speed = map(speed, 0, 9, _MIN_PWM, _MAX_PWM);
    setDirSpeed(MODO_GIRO_DERECHA, speed);
}


void PuenteH::goForward(int speed, bool escala)
{
    if(escala == true) speed = map(speed, 0, 9, _MIN_PWM, _MAX_PWM);
    setDirSpeed(MODO_AVANZA, speed);
}


void PuenteH::goBackward(int speed, bool escala)
{
    if(escala == true) speed = map(speed, 0, 9, _MIN_PWM, _MAX_PWM);
    setDirSpeed(MODO_RETROCEDE, speed);
}


void PuenteH::stop()
{
    if( _modoActual != MODO_STOP )
    {
        _motor1->stop();
        _motor2->stop();
        _modoActual = MODO_STOP;
    }
}


void PuenteH::setDirSpeed(modo modo_candidato, int speed)
{
    if(_modoActual != MODO_UNDEF)
    {
        if( modo_candidato != _modoActual ) // cambia el modo de desplazamiento 
        {
            _motor1->stop();
            _motor2->stop();

            switch(modo_candidato)
            {
                case MODO_AVANZA:
                    _motor1->setDirection(MotorCtrl::AVANZA);
                    _motor2->setDirection(MotorCtrl::AVANZA);
                    break;
                case MODO_RETROCEDE:
                    _motor1->setDirection(MotorCtrl::RETROCEDE);
                    _motor2->setDirection(MotorCtrl::RETROCEDE);
                    break;
                case MODO_GIRO_IZQUIERDA:
                    _motor1->setDirection(MotorCtrl::RETROCEDE);
                    _motor2->setDirection(MotorCtrl::AVANZA);
                    break;
                case MODO_GIRO_DERECHA:
                    _motor1->setDirection(MotorCtrl::AVANZA);
                    _motor2->setDirection(MotorCtrl::RETROCEDE);
                    break;
                default:
                    // do nothing
                    break;
            }

            _modoActual = modo_candidato;
        }

        _motor1->setSpeed(speed);
        _motor2->setSpeed(speed);
    }
    else
    {
        // Motores no inicializados
    }
}


void PuenteH::correctToLeft()
{
    int offsetM1 = _motor1->getOffset();
    int offsetM2 = _motor2->getOffset();

    if( offsetM1 >= 0 && offsetM2 == 0)
    {
        offsetM1++;
    }
    else if( offsetM1 == 0 && offsetM2 > 0)
    {
        offsetM2--;
    }

    _motor1->setOffset(offsetM1);
    _motor2->setOffset(offsetM2);
}


void PuenteH::correctToRight()
{
    int offsetM1 = _motor1->getOffset();
    int offsetM2 = _motor2->getOffset();

    if( offsetM2 >= 0 && offsetM1 == 0)
    {
        offsetM2++;
    }
    else if( offsetM2 == 0 && offsetM1 > 0)
    {
        offsetM1--;
    }

    _motor1->setOffset(offsetM1);
    _motor2->setOffset(offsetM2);
}


void PuenteH::resetCorrectToLeftAndRight()
{
    _motor1->setOffset(0);
    _motor2->setOffset(0);
}


int PuenteH::getMotorDir(lateral id)
{
    int dir = 1;
    MotorCtrl::direccion dire;

    switch(id)
    {
        case IZQUIERDO:
            dire = _motor2->getDirection();
            break;
        case DERECHO:
            dire = _motor1->getDirection();
            break;
        default:
            break;
    }
    
    if(dire == MotorCtrl::RETROCEDE) dir = -1;

    return dir;
}


int PuenteH::getMotorIzq()
{
    return getMotorDir(IZQUIERDO);
}


int PuenteH::getMotorDer()
{
    return getMotorDir(DERECHO);
}
