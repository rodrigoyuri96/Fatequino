int trigPin = 11; // Trigger frontal
int echoPin = 12; // Echo frontal

int trigPinEsq = 25; //Trigger esquerda
int echoPinEsq = 24; //Echo esquerda

int trigPinDir = 13; //Trigger direita
int echoPinDir = 10; //Echo direita

long cm, inches;
int IN1 = 4;
int IN2 = 5;
int IN3 = 6;
int IN4 = 7;
int VEL1 = 8;
int VEL2 = 9;
int vel = 0;

void setup()
{
    //Serial Port begin
    Serial.begin(9600);
    //Define inputs and outputs
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    pinMode(trigPinEsq, OUTPUT);
    pinMode(echoPinEsq, INPUT);

    pinMode(trigPinDir, OUTPUT);
    pinMode(echoPinDir, INPUT);

    //Define os pinos como saida
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(VEL1, OUTPUT);
    pinMode(VEL2, OUTPUT);

    analogWrite(VEL1, vel);
    analogWrite(VEL2, vel);
}

void loop()
{

    cm = calculaDistanciaFrontal();

    //define a velocidade que os motores irão girar
    vel = 255;
    analogWrite(VEL1, vel);
    analogWrite(VEL2, vel);

    //Caso a distancia for maior que 50 cm os motores são acionados para ir em frente
    if (cm >= 50)
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        Serial.print("SIGA!");
    }
    else
    {
        /*caso a distancia seja menor que 10 cm o motor é acionado para dar ré
     para evitar problemas na hora do robo girar*/
        if (cm <= 10)
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }
        else
        {

            /*se a distancia estiver entre 10cm e 50cm é verificado qual dos lados esta disponivel para continuar andando */
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN1, HIGH);

            if (verificaLadoDisponivel() == 1)
            {
                //gira para o lado direito
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                Serial.print("Gire para direiita");
            }
            else
            {
                //gira para o lado esquerdo
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                Serial.print("Gire para esquerda");
            }
            delay(1000);
            //apos 1 segundo o robo para de se mover e tenta novamente andar para frente
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            Serial.print("PARE!");
            delay(500);
        }
    }
}

long calculaDistanciaFrontal()
{
    long calculoCm;
    long duration;
    // O sensor é acionado por um pulso HIGH de 10 ou mais microssegundos.
    // Antes é dado um pulso LOW para garantir um HIGH mais limpo

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    /* 
    Leia o sinal do sensor: um pulso HIGH cuja duração 
    é o tempo (em microssegundos) desde o envio do ping 
    até a recepção do eco de um objeto.
     */
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);

    //Converte o tempo em distancia
    calculoCm = (duration / 2) / 29.1; // Divide by 29.1 or multiply by 0.0343
    inches = (duration / 2) / 74;      // Divide by 74 or multiply by 0.0135

    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();

    return calculoCm;
}

long verificaLadoDisponivel()
{
    long cmDir;
    long cmEsq;
    long durationDir;
    long durationEsq;
    // O sensor direito é acionado por um pulso HIGH de 10 ou mais microssegundos.
    // Antes é dado um pulso LOW para garantir um HIGH mais limpo

    digitalWrite(trigPinDir, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPinDir, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinDir, LOW);

    /*
    Leia o sinal do sensor: um pulso HIGH cuja duração 
    é o tempo (em microssegundos) desde o envio do ping 
    até a recepção do eco de um objeto.
    */

    pinMode(echoPinDir, INPUT);
    durationDir = pulseIn(echoPinDir, HIGH);

    // O sensor esquerdo é acionado por um pulso HIGH de 10 ou mais microssegundos.
    // Antes é dado um pulso LOW para garantir um HIGH mais limpo
    digitalWrite(trigPinEsq, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPinEsq, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    /*Leia o sinal do sensor: um pulso HIGH cuja duração 
   é o tempo (em microssegundos) desde o envio do ping 
   até a recepção do eco de um objeto.
  */
    pinMode(echoPinEsq, INPUT);
    durationEsq = pulseIn(echoPinEsq, HIGH);

    //Converte o tempo em distancia
    cmDir = (durationDir / 2) / 29.1; // Divide by 29.1 or multiply by 0.0343
    cmEsq = (durationEsq / 2) / 29.1;

    Serial.print(cmDir);
    Serial.print("cmDir");

    Serial.print(cmEsq);
    Serial.print("cmEsq");
    Serial.println();

    //caso lado direito esteja disponivel retorna o valor 1, caso lado esquerdo retorna o 0
    if (cmDir > cmEsq && cmDir > 50)
    {
        return 1;
    }

    return 0;
}