import processing.serial.*;
Serial s_port;

int stato [];
String r;

void setup(){
   size(800,300);
   smooth();
   stato = new int [12];
   s_port = new Serial(this, "/dev/ttyUSB0", 9600);
   r= ""; //ele3
   //r1=3; //ele8 ele9
}

void draw(){
  background(50);
  // Expand array size to the number of bytes you expect
  byte[] inBuffer = new byte[8];
  int tmp = 0;
  while (s_port.available() > 0) {
    inBuffer = s_port.readBytes();
    s_port.readBytes(inBuffer);
    if (inBuffer != null) {
      tmp = Integer.parseInt(new String(inBuffer));
      update_status(tmp);
    }
  }

  show_status();
  }

void show_status(){
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
}

void update_status(int b){
     for (int i = 0; i < 12; i++){
     if ((b & (1 << i)) != 0)
       stato[i] = 1;
     else 
       stato[i] = 0;
   }
   for(int i = 0; i < 12; i++){
     //print(stato[i] + " ");
     if(stato[i] == 1)
       println("ele [" + i + "] = T");
   }
   println();
}

/*void serialEvent(Serial s){
  r = (s.read()-128) | ((s.read()-128) << 8);
  update_status();
}*/
