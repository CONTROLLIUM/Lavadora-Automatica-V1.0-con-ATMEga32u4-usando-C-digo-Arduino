// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Proyecto_de_lavadora_mejorado.ino
    Created:	31/08/2023 8:43:28
    Author:     AUTO-LUISMI\LUIS M ECHEVARRIA
*/
#include <Bounce2.h>
#define giroL 9//este pin tiene pwm y analog
#define giroR 10//este pin tiene pwm y analog

#define buzzer 8//este pin tiene pwm y analog

#define SlowWash 0//digital que permite interrupt, este ademas es de rx.medio
#define MediumWash 1 //digital que permite interrupt, este ademas es tx
#define HeavyWash 7//digital que permite interrupt


#define DryLed 14//led indicador de secado
#define WashLed 16//led indicador de lavado
#define RemoveLed 4//enjuage indicator
#define InValve 6 //valvula de entrada de agua
#define Outvalve 5 //valvula de salida de agua

#define SensorDoor 15

unsigned short i = 0;
unsigned short j = 0;

Bounce SlowWashAct = Bounce();
Bounce MediumWashAct = Bounce();
Bounce HeavyWashAct = Bounce();

volatile bool WashActive = false;
unsigned short giros = 0;
unsigned TimeWash = 0;
unsigned short mode = 0;
// The setup() function runs once each time the micro-controller starts
void setup()
{
    Serial.begin(9600);
    pinMode(giroL, OUTPUT);//pin de giro izquierdo
    pinMode(giroR, OUTPUT);//pin de giro derecho
    
    pinMode(buzzer, OUTPUT);//pin del buzzer

    pinMode(DryLed, OUTPUT);//led indicador de secado
    pinMode(WashLed, OUTPUT);//led indicador de lavado

    pinMode(InValve, OUTPUT);
    pinMode(Outvalve, OUTPUT);

    pinMode(SlowWash, INPUT_PULLUP);//pin de lavado suave
    pinMode(MediumWash, INPUT_PULLUP);//pin de lavado medio
    pinMode(HeavyWash, INPUT_PULLUP);//pin de lavado pesado
    pinMode(SensorDoor, INPUT_PULLUP);

    SlowWashAct.attach(SlowWash);
    SlowWashAct.interval(5);
    MediumWashAct.attach(MediumWash);
    SlowWashAct.interval(5);
    HeavyWashAct.attach(HeavyWash);
    HeavyWashAct.interval(5);
    
    attachInterrupt(digitalPinToInterrupt(SlowWash), SlowWashFunction, FALLING);
    attachInterrupt(digitalPinToInterrupt(MediumWash), MediumWashFunction, FALLING);
    attachInterrupt(digitalPinToInterrupt(HeavyWash), HeavyWashFunction, FALLING);
}

// Add the main program code into the continuous loop() function
void loop()
{
    if (SensorDoor == LOW)
    {
        StopWash();
    }

}

void SlowWashFunction()
{
     giros = 4;
     TimeWash = 10000;
     mode = 130;
    StartWash();
}
void MediumWashFunction() 
{
    giros = 8;
    TimeWash = 20000;
    mode = 195;
    StartWash();
}
void HeavyWashFunction()
{
     giros = 12;
     TimeWash = 30000;
     mode = 255;
    StartWash();
}
void StartWash()
{
    if (!WashActive)
    {
        //Sonar buzzer
        digitalWrite(buzzer, HIGH);
        delay(250);
        digitalWrite(buzzer, LOW);

        //Acyivar llenado de tanque
        digitalWrite(InValve, HIGH);
        delay(60000);
        digitalWrite(InValve, LOW);

        //Sonar buzzer
        digitalWrite(buzzer, HIGH);
        delay(250);
        digitalWrite(buzzer, LOW);

        //enciendo led de lavado
        digitalWrite(WashLed, HIGH);

        //empieso a lavar
        for (unsigned short i = 0; i < giros; i++)
        {
            analogWrite(giroL, mode);
            delay(TimeWash);
            analogWrite(giroL, 0);
            analogWrite(giroR, mode);
            delay(TimeWash);
            analogWrite(giroR, 0);
        }
        //apago led de lavado
        digitalWrite(WashLed, LOW);

        //Sonar buzzer
        digitalWrite(buzzer, HIGH);
        delay(250);
        digitalWrite(buzzer, LOW);

        //Activo valvula de salida
        digitalWrite(Outvalve, HIGH);
        delay(60000);
        digitalWrite(Outvalve, LOW);

        //enjuago abriendo valvulas 
        digitalWrite(RemoveLed,HIGH);
        digitalWrite(InValve, HIGH);
        digitalWrite(Outvalve, HIGH);
        analogWrite(giroL, mode);
        delay(60000);
        digitalWrite(InValve, LOW);
        digitalWrite(Outvalve, LOW);
        digitalWrite(RemoveLed, LOW);
        analogWrite(giroL, 0);

        //Sonar buzzer
        digitalWrite(buzzer, HIGH);
        delay(250);
        digitalWrite(buzzer, LOW);

        //empiezo a secar
        digitalWrite(DryLed, HIGH);
        digitalWrite(giroL, HIGH);
        delay(60000);
        digitalWrite(giroL, LOW);
        digitalWrite(DryLed, LOW);

        StopWash();
    }
    

}
void StopWash()
{
    WashActive = false;

    analogWrite(giroL, 0);
    analogWrite(giroR, 0);
    digitalWrite(WashLed, LOW);
    digitalWrite(DryLed, LOW);
    digitalWrite(InValve, LOW);
    digitalWrite(Outvalve, LOW);

    unsigned short Sonido = 10;
    for (unsigned i = 0; i < Sonido; i++)
    {
        digitalWrite(buzzer, HIGH);
        delay(250);
        digitalWrite(buzzer, LOW);
        delay(250);
    }

}
