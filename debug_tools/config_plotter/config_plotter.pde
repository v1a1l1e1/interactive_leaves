/******************************************************************************/
/* Configurazione e debug del sistema di filtraggio dei dati e della baseline */
/******************************************************************************/

import processing.serial.*;
Serial s_port;
boolean bSerialListen = false;

final int ADDR_BASE0 = 20;
final int ADDR_FILT0 = 21;
final int ADDR_TOUCH0 = 22;

boolean filtered_lsb;

int N = 100; // numero letture mappate
int baseline0[];
int filtered0[];
int baseline1[];
int filtered1[];
int current;

int USL = 626; //156B
int LSL = 405; //101B
int TAR = 562; //140B

int vBase0 = -1;
int vFilt0 = -1;
int vDelta0= -1;

int bestDelta0 = 0;

int W=15;
int hGraph;
int offset = 0;
PFont font;

color base  = color(0, 168, 255);
color filt  = color(240, 0, 0);
color touch = color(255, 0, 0);
color lines = color(220, 220, 220, 120);
color bckgr = color(30, 30, 30);

boolean bTouch0 = false;

void setup()
{
  size(1000, 512);  
  W = width/N;
  hGraph = height / 2;
  frameRate(60);
  smooth();
  
  s_port = new Serial(this, "/dev/ttyUSB0", 9600);
  s_port.buffer( 2 );
  
  baseline0 = new int [N];
  filtered0 = new int [N];
  filtered_lsb = true;
  
  for (int i = 0; i < N; i++)  {
    baseline0[i]=0;
    filtered0[i]=0;
  }
  current = 0;
  
  USL = (int)map(USL, 0, 1023, hGraph, 0);
  LSL = (int)map(LSL, 0, 1023, hGraph, 0);
  TAR = (int)map(TAR, 0, 1023, hGraph, 0);
  
  font = loadFont("Monospaced-32.vlw");
  textFont(font, 14);
}

void draw()
{
  background(bckgr);
  
  if( bSerialListen) 
    draw_graph(0);
  
  draw_info();
}
   
void update() {
    vDelta0 = vBase0 - vFilt0;
    if( abs(vDelta0) > bestDelta0 ) 
      bestDelta0 = abs(vDelta0);
      
    int v =  (int)map(vBase0, 0, 1023, hGraph, 0);
    baseline0[current]= v;
    v = (int)map(vFilt0, 0, 1023, hGraph, 0);
    filtered0[current]= v;
  current++;
  current = current%N;
}

void serialEvent(Serial s)
{
  int addr = s.read();// - 128;
  switch( addr ) {
    case ADDR_BASE0:
      vBase0 = s.read();
      break;
    case ADDR_FILT0:
      if (filtered_lsb)
        vFilt0 = s.read();
      else
        vFilt0 += ( s.read() << 8);
      filtered_lsb = !filtered_lsb;
      break;
    case ADDR_TOUCH0:
      if( (s.read() & (0x01))== 1){
        bTouch0 = true;
      }
      else
        bTouch0 = false;
      update();
     default:  
      int v = s.read();
      break;
  
  }
}

void keyPressed()
{
  if (key == 'o' || key == 'O')
  {
    //s_port.write('o');
    bSerialListen = true;
  }
  else if( key == 'c' || key == 'C')
  {
    //s_port.write('c');
    bSerialListen = false;
  }
  else if (key == 'r' || key == 'R')
  {
    //s_port.write('r');
    bestDelta0 = 0;
    //bestDelta1 = 0;
  }
}

void draw_graph(int id)
{
  draw_deltas( id );
  draw_baseline( id );
  draw_filtered( id );
  draw_touches( id );
  draw_axes_and_texts( id );
}

void draw_touches( int id )
{
  int r = 30;
  pushStyle();
  stroke( touch );
  
  
  fill( touch );
  if( id == 0 && bTouch0 )
  {
    offset = 0;
    ellipse(width - r - 5, 45 + offset, r, r);
  }
  popStyle();  
}

void draw_deltas(int id)
{
  pushStyle();
  noStroke();
  fill( base, 120 );
  if( id == 0 ){
    offset = 0;
    for(int i = 0; i < N; i++){
      int delta = baseline0[i] - filtered0[i];
      rect(i*W, filtered0[i] + offset, W, delta);
    }
  }
  popStyle();
}


void draw_baseline(int id)
{
  pushStyle();
  stroke(base);
  strokeWeight(2);
  if( id == 0 )
  {
    offset = 0;
    for(int i = 0; i < N; i++)
      line(i*W, baseline0[i] + offset, (i+1)*W, baseline0[i] + offset);
  }
  popStyle();
}

void draw_filtered(int id)
{
  pushStyle();
  stroke(filt);
  strokeWeight(2);
  fill(filt);
  if( id == 0)
  {
    offset = 0;
    for(int i = 0; i < N; i++)
      ellipse( (i*W)+(W/2), filtered0[i]+offset, 2, 2);
  }
  popStyle();
}

void draw_axes_and_texts(int id)
{
  pushStyle();
  stroke(lines);
  strokeWeight(1);
  
  if( id == 0)
  {
    offset = 0;
    line(0, USL+offset, width, USL+offset);
    line(0, LSL+offset, width, LSL+offset);
    line(0, TAR+offset, width, TAR+offset);
    
    textAlign(LEFT);
    text("USL", 10, USL+offset-2);
    text("LSL", 10, LSL+offset-2);
    text("TAR", 10, TAR+offset-2);
    
    text("Base: "+vBase0, 10, 20+offset);
    text("Filt: "+vFilt0, 10, 40+offset);
    text("Dlta: "+vDelta0, 10, 60+offset);
    text("best: "+bestDelta0, 10, 80+offset);
    
    textAlign(RIGHT);
    text("Graph 0", width, offset+20);
  }
  popStyle();
}

void draw_info()
{
  pushStyle();
  textAlign(CENTER);
  if( bSerialListen )
    text("Serial: ON", width/2, 20);
  else
    text("Serial: OFF", width/2, 20);
  popStyle();
}
