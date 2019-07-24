import processing.serial.*;
Serial s_port;

int stato [];
byte inBuffer [];

void setup(){
   size(800,300);
   smooth();
   stato = new int [12];
   s_port = new Serial(this, "/dev/ttyUSB0", 9600);
   s_port.buffer(2);
   inBuffer = new byte[2];
   //r1=3; //ele8 ele9
}

void draw() {
    background(50);
    PFont font;
    font = loadFont("BABELUnicode-24.vlw");
    textFont(font);
    text("MPR121 - cap sens -", 10, 50);
    show_status();
    
} 

void show_status(){
  pushStyle();
  stroke(207,0,255);
  strokeWeight(1);
  for(int ele = 11; ele>=0; ele--){
    fill(180);
    text("e" + ele, width/12*ele+width/24, height/2-10);
    if(stato[ele] == 1)
      fill(250,3,69);
     else
       fill(172,222,197);
    rect(width/12*ele, height/2, width/12, width/12);
  }  
      popStyle();

}

void update_status(int b){
     for (int i = 0; i < 12; i++){
     if ((b & (1 << i)) != 0)
       stato[i] = 1;
     else 
       stato[i] = 0;
   }
}

void serialEvent(Serial s){
    s_port.readBytes(inBuffer);  
    int n = inBuffer[0] + (inBuffer[1] << 4);
    update_status(n);
}
