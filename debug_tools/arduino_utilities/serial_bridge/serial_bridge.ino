void setup(){
  Serial.begin(9600);
  while(!Serial)
    ;

void loop(){}

void SerialEvent(){
  while(Serial.available()){
    Serial.readBytes(buff, 2);
    Serial.write(buff[0]);
    Serial.write(buff[1]);
  }
}

