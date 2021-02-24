/*
Reads logfiles written by the battery monitor and renders a sequence of images with graphs
Programmed in Processing 3.5.4 by Niklas Roy and published under the beerware license.
*/

String loadFile  = "PWR_0012.LOG"; // name of logfile (must be in data folder of the sketch)
String savePath  = "E:/hoverboard/LOG12/bat-######.png";             // "E:/hoverboard/batteryData/bat-######.png"; // save images in this folder
int    FPS       = 25;             // set FPS in order to sync image sequence to video speed

PFont futura32;  

int   lineCount=0;
float timeMillis=0;
int   dataItem=1;

int[]   millis =new int[20000];
float[] ampere = new float[20000];
float[] volt   = new float[20000];
int[] ye       = new int[20000];
int[] mo       = new int[20000];
int[] da       = new int[20000];
int[] ho       = new int[20000];
int[] mi       = new int[20000];
int[] se       = new int[20000];

float minVolt =1000;
float maxVolt=0;
float minAmpere=1000;
float maxAmpere=0;

float minWatt=5000;
float maxWatt=0;

float Ah=0;

int frame=0;

float maxWh=0;
float maxAh=0;

float Afiltered=0;
float Vfiltered=0;


void setup() {
  fullScreen();
  parseFile();
  futura32 = loadFont("Futura32.vlw");
  textFont(futura32);
  strokeWeight(2);
}

void draw() {

  frame++;
  int videoMillis=frame*1000/FPS;
  
  dataItem=0;
  while (millis[dataItem]<videoMillis) {
    dataItem++; 
    if (dataItem==lineCount-2) {
      println("---EXIT---");
      exit();
    }
  }

  if (dataItem==1) {
    Vfiltered=volt[0];
  }

  background(0);

  // TIME
  stroke(255);
  fill(255);
  text(nf(da[dataItem], 2, 0)+"/"+nf(mo[dataItem], 2, 0)+"/"+nf(ye[dataItem], 2, 0)+"  "+nf(ho[dataItem], 2, 0)+":"+nf(mi[dataItem], 2, 0)+":"+nf(se[dataItem], 2, 0)+"  ms:"+nf(millis[dataItem], 8, 0)+"  Frame:"+nf(frame, 6, 0)+"  Data:"+nf(dataItem, 6, 0)+"  FPS:"+FPS, 50, 1070);
  line(50, 1030, 1870, 1030);
  fill(64);
  rect(50, 980, 1820, 20);
  fill(128);
  rect(50, 980, map(dataItem,0,lineCount,0,1820), 20);

  // VOLT
  Vfiltered=(Vfiltered*100+volt[dataItem])/101;
  if (volt[dataItem]<Vfiltered) {
    Vfiltered=volt[dataItem];
  }
  fill(255, 64);
  stroke(128);
  rect(170, 900, 110, -map(Vfiltered, minVolt, maxVolt, 0, 800));
  fill(255, 32);
  stroke(255);
  rect(170, 900, 110, -map(volt[dataItem], minVolt, maxVolt, 0, 800));
  fill(255);
  text(nf(volt[dataItem], 2, 1)+" V", 170, 950);
  for (int i=int(minVolt+1); i<=int(maxVolt+1); i++) { // draw volt scale
    line(50, 900-map(i, minVolt, maxVolt, 0, 800), 280, 900-map(i, minVolt, maxVolt, 0, 800));
    text(i+" V", 50, 890-map(i, minVolt, maxVolt, 0, 800));
  }

  // AMPERE
  int ax=250;
  Afiltered=(Afiltered*100+ampere[dataItem])/101;
  if (ampere[dataItem]>Afiltered) {
    Afiltered=ampere[dataItem];
  }
  fill(255, 32);
  stroke(128);
  rect(170+ax, 900, 110, -map(Afiltered, minAmpere, maxAmpere, 0, 800));
  fill(255, 64);
  stroke(255);
  rect(170+ax, 900, 110, -map(ampere[dataItem], minAmpere, maxAmpere, 0, 800));
  fill(255);
  text(nf(ampere[dataItem], 2, 1)+" A", 170+ax, 950);
  for (int i=0; i<=int(maxAmpere+1); i+=5) { // draw ampere scale
    line(50+ax, 900-map(i, minAmpere, maxAmpere, 0, 800), 280+ax, 900-map(i, minAmpere, maxAmpere, 0, 800));
    text(i+" A", 50+ax, 890-map(i, minAmpere, maxAmpere, 0, 800));
  }

  // Ah
  float Ah=0;
  for (int i=1; i<=dataItem; i++) {
    float elapsedMillis=millis[i]-millis[i-1];
    float elapsedHours=elapsedMillis/1000/60/60;
    Ah+=elapsedHours*ampere[i];
  }
  int ahx=500;
  fill(255, 128);
  rect(190+ahx, 900, 110, -map(Ah, 0, maxAh, 0, 800));
  fill(255);
  text(nf(Ah, 0, 4)+" Ah", 140+ahx, 950);
  for (float i=0; i<=maxAh; i+=.01) { // draw Ah scale
    line(50+ahx, 900-map(i, 0, maxAh, 0, 800), 300+ahx, 900-map(i, 0, maxAh, 0, 800));
    text(nf(i, 0, 2)+" Ah", 50+ahx, 890-map(i, 0, maxAh, 0, 800));
  }


  // WATT graph
  stroke(255);

  fill(255);
  text(nf(int(ampere[dataItem]*volt[dataItem]), 3, 0)+" W", 950, 950); // W text

  int x=950;
  for (int i=constrain(dataItem-920/5, 1, dataItem); i<dataItem; i++) {    
    stroke(128);
    if (i%2==0) {
      line(x+5, map(volt[i]*ampere[i], 0, maxWatt, 900, 50), x+5, 900);
    }
    stroke(255);
    line(x, map(volt[i-1]*ampere[i-1], 0, maxWatt, 900, 50), x+5, map(volt[i]*ampere[i], 0, maxWatt, 900, 50));
    x+=5;
  }

  fill(255);
  for (int i=0; i<=int(maxWatt+50); i+=100) { // draw Watt scale
    line(820, 900-map(i, minWatt, maxWatt, 0, 800), 1870, 900-map(i, minWatt, maxWatt, 0, 800));
    text(i+" W", 820, 890-map(i, minWatt, maxWatt, 0, 800));
  }
  noFill();
  rect(950, 50, 920, 850); // graph rectangle

  if (savePath!="") {
    saveFrame(savePath); // save image
  }
}

//--------------------------------------------------------------------------------------------------
void parseFile() {

  BufferedReader reader = createReader(loadFile);
  String line = null;
  try {
    while ((line = reader.readLine()) != null) {
      if (lineCount>0) {

        // go through lines
        String[] pieces = split(line, "|");
        millis[lineCount-1]=int(pieces[0]);
        ampere[lineCount-1] = float(pieces[8]);
        volt[lineCount-1]   = float(pieces[7]);
        ye[lineCount-1]   = int(pieces[1]);
        mo[lineCount-1]   = int(pieces[2]);
        da[lineCount-1]   = int(pieces[3]);
        ho[lineCount-1]   = int(pieces[4]);
        mi[lineCount-1]   = int(pieces[5]);
        se[lineCount-1]   = int(pieces[6]);

        if (volt[lineCount-1]<minVolt) {
          minVolt=volt[lineCount-1];
        }
        if (volt[lineCount-1]>maxVolt) {
          maxVolt=volt[lineCount-1];
        }
        if (ampere[lineCount-1]<minAmpere) {
          minAmpere=ampere[lineCount-1];
        }
        if (ampere[lineCount-1]>maxAmpere) {
          maxAmpere=ampere[lineCount-1];
        }
        if (ampere[lineCount-1]*volt[lineCount-1]<minWatt) {
          minWatt=ampere[lineCount-1]*volt[lineCount-1];
        }
        if (ampere[lineCount-1]*volt[lineCount-1]>maxWatt) {
          maxWatt=ampere[lineCount-1]*volt[lineCount-1];
        }
        if (lineCount>1) {
          float elapsedMillis=millis[lineCount-1]-millis[lineCount-2];
          float elapsedHours=elapsedMillis/1000/60/60;
          maxAh+=elapsedHours*ampere[lineCount-1];
          maxWh+=elapsedHours*ampere[lineCount-1]*volt[lineCount-1];
        }
      }
      lineCount++;
    }
    reader.close();

    print("lines: ");
    println(lineCount);
    print("min A: ");
    print(minAmpere);
    print("  max A: ");
    println(maxAmpere);
    print("min V: ");
    print(minVolt);
    print("  max V: ");
    println(maxVolt);
    print("  Ah: ");
    print(maxAh);
    print("  Wh: ");
    print(maxWh);
  } 
  catch (IOException e) {
    e.printStackTrace();
  }
} 
