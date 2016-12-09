unsigned int nHits = 0;
float rotationRate = 0.0f;
char flip;
char input;
unsigned char b_direction=0;
unsigned char b_commandTarget=0;

#define BOTHACTUATOR     0b00000011
#define INNERACTUATOR    0b00000001
#define OUTERACTUATOR    0b00000010



void setup()
{
  pinMode(23,INPUT); //INDEX
  pinMode(22,INPUT); //ENCODER
  //attachInterrupt(digitalPinToInterrupt(18), encoder, CHANGE); 
  Serial.begin(115200);
  Serial.setTimeout(200);
  Serial.println("Wellcome!");
  Serial.print('>');

  init_SinWAVE();
  setAmplitudeSinWAVE(0.25,BOTHACTUATOR);
  setFrequencySinWAVE(100,BOTHACTUATOR);


}

void encoder()
{
  nHits++;
}
void loop()
{

  fire100();
  //delay(1000);
  //Serial.println(nHits / 2500.0f);
  //nHits = 0;

}


void fire100()
{
  //wait for human input.
  if(Serial.available()){
    char input = Serial.read();
    Serial.println(input);
    switch(input){

    case 'i':
      {
        b_commandTarget = INNERACTUATOR;
        break;
      }
    case 'o':
      {
        b_commandTarget = OUTERACTUATOR;
        break;
      }
    case 'b':
      {
        b_commandTarget = BOTHACTUATOR;
        break;
      }
    default:
      Serial.println("Please start the command with i or e");
      return;
      break;
    }
  }
  
  if(Serial.available()){
    char input = Serial.read();
    Serial.println(input);
    switch(input){
    case 'a': //change amplitude
      {
        Serial.println("enter new amp (0to1):");
        Serial.flush();
        while(!Serial.available());
        float newAmp = Serial.parseFloat();
        if(newAmp > 1.0f || newAmp <= 0.0f){
          Serial.println("Amp out of range.");
          break;
        }
        setAmplitudeSinWAVE(newAmp,b_commandTarget);
        Serial.print("Amp set to ");
        Serial.print(newAmp * 100);
        Serial.println("%");
      }
      break;

    case 'w': 

      for(int i = 0; i < 256; ++i){
        //Serial.print(sinon[i]);
        Serial.print(' ');
      }

      Serial.println();

      break;

    case 'f': //change frequency
      {
        Serial.println("enter new frequency:");
        Serial.flush();
        while(!Serial.available());
        float freq = Serial.parseFloat();
        setFrequencySinWAVE(freq,b_commandTarget);
        Serial.print("frequency set to ");
        Serial.print(freq);
        Serial.println("Hz");
      }
      break;

    case 't': //encoder values
      {
        Serial.println("waiting for encoder");
        Serial.flush();
        while(!digitalRead(22));
        Serial.println("found encoder");
        int count=0;
        long t1=millis();
        startSinWAVE(b_commandTarget);
        while(count<431)
        {
          while(digitalRead(23));
          count+=1;
          while(!digitalRead(23));

        }
        stopSinWAVE(b_commandTarget);
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(5,LOW);

        t1=millis()-t1;

        Serial.print(count);
        Serial.print(" encoder pulses in ");
        Serial.println(t1);


      }
      Serial.println("done");
      break;

    case 'n':
      //get time
      Serial.println("enter Time (ms)");
      {
        Serial.flush();
        while(!Serial.available());
        int runtime = Serial.parseInt();
        Serial.print("Running for ");
        Serial.print(runtime);
        Serial.println("ms");


        //pulse
        startSinWAVE(b_commandTarget);
        delay(runtime);
        stopSinWAVE(b_commandTarget);
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(5,LOW);
      }
      Serial.println("done");
      break;


    case 'z':
      //get time
      Serial.println("enter Time (ms)");
      {
        Serial.flush();
        while(!Serial.available());
        int runtime = Serial.parseInt();
        Serial.print("Running for ");
        Serial.print(runtime);
        Serial.println("ms");


        //pulse
        //wait for index pulse
        Serial.println("waiting");
        while(!digitalRead(22));
        Serial.println("found");
        startSinWAVE(b_commandTarget);
        delay(runtime);
        stopSinWAVE(b_commandTarget);
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(5,LOW);
      }
      Serial.println("done");
      break;

    case 'm':
      //get time
      {
        Serial.flush();
        Serial.println("enterAtocontinue");

        //pulse
        startSinWAVE(b_commandTarget);

        while(!Serial.available());

        stopSinWAVE(b_commandTarget);
        flip=Serial.read();
        Serial.flush();

        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(5,LOW);
      }

      Serial.println("done");
      break;

    case 'p': //encoder values
      {
        plug(b_commandTarget);
        Serial.print("direction swapped to ");
        if(b_direction){
          Serial.println("forward");
        }
        else{
          Serial.println("backward");
        }
        b_direction = !b_direction;

      }
      Serial.println("done");
      break;

    default:
      Serial.println("?");
    }
    Serial.print('>');
  }


}



