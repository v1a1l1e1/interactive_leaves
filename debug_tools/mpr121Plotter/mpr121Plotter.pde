int stato [];
int r0 = 240;
int r1 = 0x08;

void setup(){
   size(800,300);
   smooth();
   stato = new int [12];
   for (int i = 0; i < 8; i++){
     if ((r0 & (1 << i)) != 0)
       stato[i] = 1;
     else 
       stato[i] = 0;
   }
     for (int i = 0; i < 4; i++){
     if ((r1 & (1 << i)) != 0)
       stato[i+8] = 1;
     else 
       stato[i+8] = 0;
   }
   for(int i = 0; i < 12; i++)
     println("stato [" + i + "] = " + stato[i]);
}

void draw(){
  background(50);
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
