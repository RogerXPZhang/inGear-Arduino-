unsigned int nHits = 0;
float rotationRate = 0.0f;
char flip;
char input;
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
  setAmplitudeSinWAVE(0.25);
  setFrequencySinWAVE(100);
  //startSinWAVE();


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
        setAmplitudeSinWAVE(newAmp);
        Serial.print("Amp set to ");
        Serial.print(newAmp * 100);
        Serial.println("%");
      }
      break;

      case 'w': 

      for(int i = 0; i < 256; ++i){
        Serial.print(sinon[i]);
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
        setFrequencySinWAVE(freq);
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
        startSinWAVE();
        while(count<431)
        {
          while(digitalRead(23));
          count+=1;
          while(!digitalRead(23));
        
        }
        stopSinWAVE();
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
          startSinWAVE();
          delay(runtime);
          stopSinWAVE();
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
          startSinWAVE();
          delay(runtime);
          stopSinWAVE();
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
          startSinWAVE();
          
          while(!Serial.available());
          
          stopSinWAVE();
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
        plug();
        Serial.println("direction swapped");
        
      }
      Serial.println("done");
      break;
  
      default:
      Serial.println("?");
    }
    Serial.print('>');
  }

  
}
