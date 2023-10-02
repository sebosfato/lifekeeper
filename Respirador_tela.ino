
//alteracoes lib nextionhardware //dbserial e mudar delay para delayMicroseconds
#include "Arduino.h"
#include <TaskScheduler.h>

#include "EEPROM.h"
#define RXD2 16
#define TXD2 17
 #include "Nextion.h"
#define   CH0_OFFSET   160
#define   CH1_OFFSET    119
#define   CH2_OFFSET    20
#define   CH3_OFFSET    240

//#define EEPROM_SIZE 32



//EEPROM.put(0, 9);

uint8_t ch0_data =0x00;
uint8_t ch1_data =0x00;
uint8_t ch2_data =0x00;
uint8_t ch3_data =0x00;

float medidaanterior=0;
float medidaatual=0;
float soma=0;
int primeira=0;

int Pause=0;
//RTC_DATA_ATTR 
int tempo=0;
//RTC_DATA_ATTR 
bool ciclo=false;
int tempoinspiracao=1000;
int tempoexpiracao=2000;
int pausaexp=100;
int pausains=100;
int cont=0; 


#include <Servo.h>


Servo servo1;
Servo servo2;
int pos = 0;
static const int servoPin = 4;
static const int servoPin2 = 5;

unsigned long lastmillis=0;

int sense_state=1;
int mode_state=1;
uint32_t dual_state;

int pag=0;

int change=330;

uint32_t inspexpauto=33;
uint32_t autofio2=0;
uint32_t sexo;

 char crazy[1];

//RTC_NOINIT_ATTR 
uint32_t volume;
uint32_t RPM;
uint32_t pressao;
uint32_t fluxs;
uint32_t PEEP;
uint32_t FIO2;
uint32_t SENS;
uint32_t TINS;
uint32_t TSUB;
uint32_t INSPEXP;
uint32_t altura;
uint32_t idade;
uint32_t volcalc;
uint32_t pausa;


  uint32_t pressaomax; 
  uint32_t pressaomin;
  uint32_t volumecorrentemax;
  uint32_t volumecorrentemin;
  uint32_t peepmax;
  uint32_t peepmin;
  uint32_t fluxomax;
  uint32_t fluxomin;
  uint32_t platomax;
  uint32_t platomin;
  uint32_t rpmmax;
  uint32_t rpmmin;
  uint32_t maxapnea;
  uint32_t minapnea;
  uint32_t sao2min;
  uint32_t co2min;
  

class FLASHvariables 
 {                          //Class to hold various persistent variables we want to save to non-volatile SRAM
 
 public:
    //VARIABLES

uint32_t volume;
uint32_t RPM;
uint32_t pressao;
uint32_t fluxs;
uint32_t PEEP;
uint32_t FIO2;
uint32_t SENS;
uint32_t TINS;
uint32_t TSUB;
uint32_t INSPEXP;
uint32_t altura;
uint32_t idade;
uint32_t volcalc;
uint32_t pausa;

//
//uint32_t pressaomax; 
//uint32_t pressaomin;
//uint32_t volumecorrentemax;
//uint32_t volumecorrentemin;
//uint32_t peepmax;
//uint32_t peepmin;
//uint32_t fluxomax;
//uint32_t fluxomin;
//uint32_t platomax;
//uint32_t platomin;
//uint32_t rpmmax;
//uint32_t rpmmin;
//uint32_t maxapnea;
//uint32_t minapnea;
//uint32_t sao2min;
//uint32_t co2min;








 uint8_t varsGood;

 //METHODS
    void save();
    void get();
    void initialize();
   
} myVars;

void FLASHvariables::save()   
{
  EEPROM.put(0,myVars);
  EEPROM.commit();
}

void FLASHvariables::get()
{
  EEPROM.begin(sizeof(myVars)); 
  EEPROM.get(0,myVars);  
}

void FLASHvariables::initialize()    
{
  //If no data can be found in flash RAM, initialize the variables held in Class Structure here.

uint32_t volume=200;
uint32_t RPM=10;
uint32_t pressao=40;
uint32_t fluxs=60;
uint32_t PEEP=55;
uint32_t FIO2=20;
uint32_t SENS=20;
uint32_t TINS=10;
uint32_t TSUB=1;
uint32_t INSPEXP=33;
uint32_t altura=175;
uint32_t idade=65;
uint32_t volcalc=300;
uint32_t pausa=10;


//uint32_t pressaomax=45; 
//uint32_t pressaomin=30;
//uint32_t volumecorrentemax=422;
//uint32_t volumecorrentemin=222;
//uint32_t peepmax=7;
//uint32_t peepmin=4;
//uint32_t fluxomax=38;
//uint32_t fluxomin=20;
//uint32_t platomax=35;
//uint32_t platomin=25;
//uint32_t rpmmax=18;
//uint32_t rpmmin=12;
//uint32_t maxapnea=5;
//uint32_t minapnea=18;
//uint32_t sao2min=85;
//uint32_t co2min=25;




 
  varsGood=190;
  
  EEPROM.put(0,myVars);
  EEPROM.commit();
}

 //============END FLASHVARIABLES CLASS DEFINITION =======================

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } 
esp_chip_info_t chip_info; //Instantiate object chip_info of class esp_chip_info_t to access data on chip hardware
 
uint32_t pressaoinstantanea=40;
uint32_t volumecorrente=322;
uint32_t lastpeep=4;
uint32_t fluxoinstantaneo=33;
uint32_t fio2fracao=40;
uint32_t sao2=85;
uint32_t pressaopk=40;
uint32_t plato=30;
uint32_t rpmpaciente=18;
uint32_t iefracao=2;



void t1Callback();
void t2Callback();
void t3Callback();
void t4Callback();

  TaskHandle_t Task1;
  TaskHandle_t Task2;

Task t1(10, TASK_FOREVER, &t1Callback);
Task t2(100, TASK_FOREVER, &t2Callback);
Task t3(10, TASK_FOREVER, &t3Callback);
Task t4(100, TASK_FOREVER, &t4Callback);

Scheduler runner;


void save(){
//EEPROM.put(0, volume);
//    EEPROM.commit();
//EEPROM.put(8, RPM);
//    EEPROM.commit();
//    EEPROM.put(4, pressao);
//    EEPROM.commit();
//    EEPROM.put(6, PEEP);
//    EEPROM.commit();
//    EEPROM.put(8, fluxs);
//    EEPROM.commit();
//    EEPROM.put(10, altura);
//    EEPROM.commit();
//EEPROM.put(12, INSPEXP);
//    EEPROM.commit();
//    EEPROM.put(14, mode_state);
//    EEPROM.commit();
//
//    EEPROM.put(16, sense_state);
//    EEPROM.commit();
//
//    EEPROM.put(18, sexo);
//    EEPROM.commit();
//    EEPROM.put(20, SENS);
//    EEPROM.commit();
//    EEPROM.put(22, TINS);
//    EEPROM.commit();
//    EEPROM.put(24, TSUB);
//    EEPROM.commit();
//    EEPROM.put(26, pausa);
//    EEPROM.commit();
//    EEPROM.put(28, FIO2);
//    EEPROM.commit();
//   
}

    
    void savealarm(){
  
}


//Declare a number object [page id:0,component id:3, component name: "n0"]. 
NexWaveform s0 = NexWaveform(0, 1, "s0");
//page 2
NexNumber vca = NexNumber(2, 12, "vca");
NexNumber vci = NexNumber(2, 11, "vci");
NexNumber fla = NexNumber(2, 14, "fla");
NexNumber fli = NexNumber(2, 13, "fli");
NexNumber pra = NexNumber(2, 16, "pra");
NexNumber pri = NexNumber(2, 15, "pri");
NexNumber rpa = NexNumber(2, 22, "rpa");
NexNumber rpi = NexNumber(2, 21, "rpi");
NexNumber pta = NexNumber(2, 20, "pta");
NexNumber pti = NexNumber(2, 19, "pti");
NexNumber pea = NexNumber(2, 18, "pea");
NexNumber pei = NexNumber(2, 17, "pei");
NexNumber apa = NexNumber(2, 33, "apa");
NexNumber api = NexNumber(2, 32, "api");
//NexNumber soa = NexNumber(2, 22, "soa");
NexNumber soi = NexNumber(2, 34, "soi");
NexNumber coi = NexNumber(2, 71, "coi");


//NexButton b1 = NexButton(0, 15, "b1"); 
NexButton a2 = NexButton(2, 23, "b2");
NexButton a3 = NexButton(2, 24, "b3"); 
NexButton a4 = NexButton(2, 25, "b4");
NexButton a5 = NexButton(2, 26, "b5"); 
NexButton a6 = NexButton(2, 27, "b6");
NexButton a7 = NexButton(2, 28, "b7"); 
NexButton a8 = NexButton(2, 67, "b8");
NexButton a9 = NexButton(2, 36, "a9"); 
NexButton a10 = NexButton(2, 37, "b10");
NexButton a11 = NexButton(2, 38, "b11"); 
NexButton a12 = NexButton(2, 39, "b12");
NexButton a13 = NexButton(2, 35, "b13"); 
NexButton a14 = NexButton(2, 68, "b14");
NexButton a15 = NexButton(2, 69, "b15"); 
NexButton a16 = NexButton(2, 40, "b16");
NexButton a17 = NexButton(2, 41, "b17"); 
NexButton a18 = NexButton(2, 70, "b18");
NexButton a19 = NexButton(2, 42, "b19"); 
NexButton a20 = NexButton(2, 43, "b20");
//NexButton a21 = NexButton(2, 15, "b21"); 
NexButton a22 = NexButton(2, 44, "b22");
NexButton a23 = NexButton(2, 45, "b23"); 
NexButton a24 = NexButton(2, 46, "b24");
NexButton a25 = NexButton(2, 47, "b25"); 
NexButton a26 = NexButton(2, 48, "b26");
NexButton a27 = NexButton(2, 49, "b27"); 
//NexButton a28 = NexButton(2, 16, "b28");
NexButton a29 = NexButton(2, 50, "b29"); 
NexButton a30 = NexButton(2, 51, "b30");
//NexButton a31 = NexButton(2, 15, "b31"); 
NexButton a32 = NexButton(2, 52, "b32");
NexButton a33 = NexButton(2, 53, "b33");
NexButton a34 = NexButton(2, 54, "b34"); 
NexButton a35 = NexButton(2, 55, "b35");
NexButton a36 = NexButton(2, 56, "b36");
NexButton a37 = NexButton(2, 57, "b37"); 
//NexButton a38 = NexButton(2, 58, "b38");
NexButton a39 = NexButton(2, 58, "b39");
//NexButton a40 = NexButton(2, 15, "b40"); 
//NexButton a41 = NexButton(2, 52, "b41");
NexButton a42 = NexButton(2, 59, "b42");
NexButton a43 = NexButton(2, 60, "b43"); 
NexButton a44 = NexButton(2, 61, "b44");
NexButton a45 = NexButton(2, 62, "b45");
NexButton a46 = NexButton(2, 63, "b46");
NexButton a47 = NexButton(2, 64, "b47"); 
//NexButton a48 = NexButton(2, 64, "b48");
NexButton a49 = NexButton(2, 65, "b49");


//page0
NexNumber vce = NexNumber(0, 2, "vce");
NexNumber flx = NexNumber(0, 3, "flx");
NexNumber prs = NexNumber(0, 4, "prs");
NexNumber pee = NexNumber(0, 8, "pee");
NexNumber sao = NexNumber(0, 20, "sao");
NexNumber pla = NexNumber(0, 23, "pla");
NexNumber rpm = NexNumber(0, 25, "rpm");
NexNumber ief = NexNumber(0, 31, "ief");
NexNumber ppk = NexNumber(0, 34, "ppk");
NexNumber fio = NexNumber(0, 13, "fio");


//NexVariable va0=NexVariable(0,35,"va0");

//page 1
NexCheckbox c0 = NexCheckbox(1, 67, "c0"); //insp/exp
NexCheckbox c1 = NexCheckbox(1, 28, "c1");//auto fi02
NexCheckbox c2 = NexCheckbox(1, 69, "c2");//sexo m f


NexDSButton bt0 = NexDSButton(1, 3, "bt0"); //modo 
NexDSButton bt1 = NexDSButton(1, 4, "bt1");
NexDSButton bt2 = NexDSButton(1, 11, "bt2");

NexDSButton bt3 = NexDSButton(1, 59, "bt3"); //modo sens
NexDSButton bt4 = NexDSButton(1, 60, "bt4");
//NexText t0 = NexText(1, 5, "t0");

//VOLUME 
NexNumber n0 = NexNumber(1, 14, "n0");
NexButton b3 = NexButton(1, 12, "b3");
NexButton b4 = NexButton(1, 13, "b4");
//RPM
NexNumber n1 = NexNumber(1, 17, "n1");
NexNumber n22 = NexNumber(1, 20, "n22");

NexButton b5 = NexButton(1, 15, "b5"); 
NexButton b6 = NexButton(1, 16, "b6");
//Pressao

NexButton b7 = NexButton(1, 18, "b7");
NexButton b8 = NexButton(1, 19, "b8");
//Fluxo
NexNumber fl = NexNumber(1, 39, "fl");
NexButton b18 = NexButton(1, 37, "b18");
NexButton b19 = NexButton(1, 38, "b19");
//PEEP
NexNumber pe = NexNumber(1, 35, "pe");
NexButton b9 = NexButton(1, 21, "b9");
NexButton b10 = NexButton(1, 22, "b10");
///FIO2
NexNumber n4 = NexNumber(1, 25, "n4");
NexButton b11 = NexButton(1, 23, "b11");
NexButton b12 = NexButton(1, 24, "b12");
//Sens
NexNumber x2 = NexNumber(1, 40, "x2");
NexButton b13 = NexButton(1, 26, "b13");
NexButton b14 = NexButton(1, 27, "b14");
///Tins
NexNumber x3 = NexNumber(1, 44, "x3");
NexButton b20 = NexButton(1, 42, "b20");
NexButton b21 = NexButton(1, 43, "b21");

///tsub

NexNumber x4 = NexNumber(1, 48, "x4");
NexButton b22 = NexButton(1, 46, "b22");
NexButton b23 = NexButton(1, 47, "b23");

//ins/ex
NexNumber n7 = NexNumber(1, 64, "n7");
NexButton b28 = NexButton(1, 62, "b28");
NexButton b29 = NexButton(1, 63, "b29");

//pausa
NexNumber x0 = NexNumber(1, 34, "x0");
NexButton b1 = NexButton(1, 31, "b1");
NexButton b15 = NexButton(1, 32, "b15");

///altura
NexNumber x5 = NexNumber(1, 52, "x5");
NexButton b24 = NexButton(1, 50, "b24");
NexButton b25 = NexButton(1, 51, "b25");
//idade
NexNumber n5 = NexNumber(1, 56, "n5");
NexButton b26 = NexButton(1, 54, "b26");
NexButton b27 = NexButton(1, 55, "b27");

//vol calc
NexNumber n6 = NexNumber(1, 57, "n6");
//aplicar
NexButton b16 = NexButton(1, 30, "b16");
//pausa insp
NexButton b17 = NexButton(1, 33, "b17");

//page
NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");
NexPage page2    = NexPage(2, 0, "page2");
NexPage page3    = NexPage(3, 0, "page3");


char buffer[100];

NexTouch *nex_listen_list2[] = 
{
&page0,
&page1,
&page2,
&a2,
&a3,
&a4,
&a5,
&a6,
&a7,
&a8,
&a9,
&a10,
&a11,
&a12,
&a13,
&a14,
&a15,
&a16,
&a17,
&a18,
&a19,
&a20,
&a22,
&a23,
&a24,
&a25,
&a26,
&a27,
&a29,
&a30,
&a32,
&a33,
&a34,
&a35,
&a36,
&a37,
&a39,
&a42,
&a43,
&a44,
&a45,
&a46,
&a47,
&a49,
NULL 
};

// Register object n0, b0, b1, to the touch event list.  
  NexTouch *nex_listen_list0[] = 
{
  &vce,
&page0,
&page1,
&page2,
 NULL 
};
 NexTouch *nex_listen_list1[] = 
{
&page0,
&page1,
&page2,
&b1,
&b3,
&b4,
&b5,
&b6,
&b7,
&b8,
&b9,
&b10,
&b11,
&b12,
&b13,
&b14,
&b15,
&b16,
&b17,
&b18,
&b19,
&b20,
&b21,
&b22,
&b23,
&b24,
&b25,
&b26,
&b27,
&b28,
&b29,
&bt0,
&bt1,
&bt2,
&bt3,
&bt4,
&c0,
&c1,
&c2,
NULL
};


// funcoes alarmes
void a2PopCallback(void *ptr)
{
  a2.Set_background_color_bco(61248);
}
void a3PopCallback(void *ptr)
{
  a3.Set_background_color_bco(61248);
}
void a4PopCallback(void *ptr)
{
  a4.Set_background_color_bco(61248);
}
void a5PopCallback(void *ptr)
{
  a5.Set_background_color_bco(61248);
}
void a6PopCallback(void *ptr)
{
  a6.Set_background_color_bco(61248);
}
void a7PopCallback(void *ptr)
{
  a7.Set_background_color_bco(61248);
}
void a8PopCallback(void *ptr)
{
  a8.Set_background_color_bco(61248);
}
void a13PopCallback(void *ptr)
{
  a13.Set_background_color_bco(61248);
}
void a14PopCallback(void *ptr)
{
  a14.Set_background_color_bco(61248);
}



void a22PopCallback(void *ptr)
{
    vci.getValue(&volumecorrentemin);
    volumecorrentemin += 1;
    vci.setValue(volumecorrentemin);
}
void a9PopCallback(void *ptr)
{
    vci.getValue(&volumecorrentemin);
    volumecorrentemin -= 1;
    vci.setValue(volumecorrentemin);
}
void a42PopCallback(void *ptr)
{
    vca.getValue(&volumecorrentemax);
    volumecorrentemax += 1;
    vca.setValue(volumecorrentemax);
}
void a32PopCallback(void *ptr)
{
    vca.getValue(&volumecorrentemax);
    volumecorrentemax -= 1;
    vca.setValue(volumecorrentemax);
}

//flux
void a23PopCallback(void *ptr)
{
    fli.getValue(&fluxomin);
    fluxomin += 1;
    fli.setValue(fluxomin);
}
void a10PopCallback(void *ptr)
{
    fli.getValue(&fluxomin);
    fluxomin -= 1;
    fli.setValue(fluxomin);
}
void a43PopCallback(void *ptr)
{
    fla.getValue(&fluxomax);
    fluxomax += 1;
    fla.setValue(fluxomax);
}
void a33PopCallback(void *ptr)
{
    fla.getValue(&fluxomax);
    fluxomax -= 1;
    fla.setValue(fluxomax);
}


//press

void a24PopCallback(void *ptr)
{
    pri.getValue(&pressaomin);
    pressaomin += 1;
    pri.setValue(pressaomin);
}
void a11PopCallback(void *ptr)
{
    pri.getValue(&pressaomin);
    pressaomin -= 1;
    pri.setValue(pressaomin);
}
void a44PopCallback(void *ptr)
{
    pra.getValue(&pressaomax);
    pressaomax += 1;
    pra.setValue(pressaomax);
}
void a34PopCallback(void *ptr)
{
    pra.getValue(&pressaomax);
    pressaomax -= 1;
    pra.setValue(pressaomax);
}


//peep

void a25PopCallback(void *ptr)
{
    pei.getValue(&peepmin);
    peepmin += 1;
    pei.setValue(peepmin);
}
void a12PopCallback(void *ptr)
{
    pei.getValue(&peepmin);
    peepmin -= 1;
    pei.setValue(peepmin);
}
void a45PopCallback(void *ptr)
{
    pea.getValue(&peepmax);
    peepmax += 1;
    pea.setValue(peepmax);
}
void a35PopCallback(void *ptr)
{
    pea.getValue(&peepmax);
    peepmax -= 1;
    pea.setValue(peepmax);
}


//plat

void a26PopCallback(void *ptr)
{
    pti.getValue(&platomin);
    platomin += 1;
    pti.setValue(platomin);
}
void a16PopCallback(void *ptr)
{
    pti.getValue(&platomin);
    platomin -= 1;
    pti.setValue(platomin);
}
void a46PopCallback(void *ptr)
{
    pta.getValue(&platomax);
    platomax += 1;
    pta.setValue(platomax);
}
void a36PopCallback(void *ptr)
{
    pta.getValue(&platomax);
    platomax -= 1;
    pta.setValue(platomax);
}


//rpm


void a27PopCallback(void *ptr)
{
    rpi.getValue(&rpmmin);
    rpmmin += 1;
    rpi.setValue(rpmmin);
}
void a17PopCallback(void *ptr)
{
    rpi.getValue(&rpmmin);
    rpmmin -= 1;
    rpi.setValue(rpmmin);
}
void a47PopCallback(void *ptr)
{
    rpa.getValue(&rpmmax);
    rpmmax += 1;
    rpa.setValue(rpmmax);
}
void a37PopCallback(void *ptr)
{
    rpa.getValue(&rpmmax);
    rpmmax -= 1;
    rpa.setValue(rpmmax);
}


//apnea

void a29PopCallback(void *ptr)
{
    api.getValue(&minapnea);
    minapnea += 1;
    api.setValue(minapnea);
}
void a19PopCallback(void *ptr)
{
    api.getValue(&minapnea);
    minapnea -= 1;
    api.setValue(minapnea);
}
void a49PopCallback(void *ptr)
{
    apa.getValue(&maxapnea);
    maxapnea += 1;
    apa.setValue(maxapnea);
}
void a39PopCallback(void *ptr)
{
    apa.getValue(&maxapnea);
    maxapnea -= 1;
    apa.setValue(maxapnea);
}


//sao2
//
//void a10PopCallback(void *ptr)
//{
//    vci.getValue(&volumecorrentemin);
//    volumecorrentemin += 1;
//    vci.setValue(volumecorrentemin);
//}
//void a9PopCallback(void *ptr)
//{
//    vci.getValue(&volumecorrentemin);
//    volumecorrentemin -= 1;
//    vci.setValue(volumecorrentemin);
//}
void a30PopCallback(void *ptr)
{
    soi.getValue(&sao2min);
    sao2min += 1;
    soi.setValue(sao2min);
}
void a20PopCallback(void *ptr)
{
    soi.getValue(&sao2min);
    sao2min -= 1;
    soi.setValue(sao2min);
}

//co2

//void a10PopCallback(void *ptr)
//{
//    vci.getValue(&volumecorrentemin);
//    volumecorrentemin += 1;
//    vci.setValue(volumecorrentemin);
//}
//void a9PopCallback(void *ptr)
//{
//    vci.getValue(&volumecorrentemin);
//    volumecorrentemin -= 1;
//    vci.setValue(volumecorrentemin);
//}
void a15PopCallback(void *ptr)
{
   coi.getValue(&co2min);
    co2min += 1;
    coi.setValue(co2min);
}
void a18PopCallback(void *ptr)
{
    coi.getValue(&co2min);
    co2min -= 1;
    coi.setValue(co2min);
}

void c0PopCallback(void *ptr){
NexCheckbox *cn = (NexCheckbox *)ptr;
memset(buffer, 200, sizeof(buffer));
c0.getValue(&dual_state);

if(dual_state==1){
  c0.setValue(1);
}
  else{
    c0.setValue(0);
  }
}
void c1PopCallback(void *ptr){
NexCheckbox *cn = (NexCheckbox *)ptr;
memset(buffer, 200, sizeof(buffer));
c1.getValue(&dual_state);

if(dual_state==1){
  c1.setValue(1);
}
  else{
    c1.setValue(0);
  }
}
///sexo 
void c2PopCallback(void *ptr){ 
NexCheckbox *cn = (NexCheckbox *)ptr;
memset(buffer, 200, sizeof(buffer));
c2.getValue(&dual_state);

if(dual_state==1){
  sexo=true;
  c2.setValue(1);
  if (sexo==true)volcalc= (52 + (0.75 * (altura - 152.4)))*6;
if (sexo==false)volcalc= (49 + (0.67 * (altura - 152.4)))*6;
n6.setValue(volcalc);
}
  else{
    c2.setValue(0);
    sexo=false;
    if (sexo==true)volcalc= (52 + (0.75 * (altura - 152.4)))*6;
if (sexo==false)volcalc= (49 + (0.67 * (altura - 152.4)))*6;
n6.setValue(volcalc);
  }
}



//vcv
void bt0PopCallback(void *ptr){
    NexDSButton *btn = (NexDSButton *)ptr;
    memset(buffer, 200, sizeof(buffer));
    bt0.getValue(&dual_state);/* Get the state value of dual state button component . */
if (mode_state != 1){
  mode_state=1;
tempo=0;
}
    if(mode_state==1) {
        bt0.setValue(1);
        bt1.setValue(0);
        bt2.setValue(0);
     
    }
}

//pcv
void bt1PopCallback(void *ptr){
    NexDSButton *btn = (NexDSButton *)ptr;
    memset(buffer, 200, sizeof(buffer));
    bt1.getValue(&dual_state);
   if (mode_state != 2) {
    mode_state=2;
   tempo=0;
   }
if(mode_state==2){
      bt1.setValue(1);
      bt0.setValue(0);
      bt2.setValue(0);
    }
}

//psv
void bt2PopCallback(void *ptr){
    NexDSButton *btn = (NexDSButton *)ptr;
    memset(buffer, 200, sizeof(buffer));
 bt2.getValue(&dual_state);
if (mode_state != 3){
mode_state=3;
tempo=0;
}
if(mode_state==3){
      bt2.setValue(1);
      bt0.setValue(0);
      bt1.setValue(0);
    }
}

//sens cmh20
void bt3PopCallback(void *ptr){
    NexDSButton *btn = (NexDSButton *)ptr;
    memset(buffer, 200, sizeof(buffer));
 bt3.getValue(&dual_state);
if (sense_state != 1){
  sense_state=1;
}
if(sense_state==1)   {
      bt3.setValue(1);
      bt4.setValue(0);
    }
}
//sens fluxo
void bt4PopCallback(void *ptr){
     NexDSButton *btn = (NexDSButton *)ptr;
    memset(buffer, 200, sizeof(buffer));
 bt4.getValue(&dual_state);
if (sense_state != 2){
  sense_state=2;
}
if(sense_state==2){
      bt4.setValue(1);
      bt3.setValue(0);
    }
}


void b3PopCallback(void *ptr)
{
    n0.getValue(&volume);
    volume += 1;
    n0.setValue(volume);
}
void b4PopCallback(void *ptr)
{
    n0.getValue(&volume);
    volume -= 1;
    n0.setValue(volume);
}

void b5PopCallback(void *ptr)
{
    n1.getValue(&RPM);
    RPM += 1;
    n1.setValue(RPM);
}
void b6PopCallback(void *ptr)
{
    n1.getValue(&RPM);
    RPM -= 1;
    n1.setValue(RPM);
}

void b7PopCallback(void *ptr)
{
    n22.getValue(&pressao);
    pressao += 1;
    n22.setValue(pressao);
}
void b8PopCallback(void *ptr)
{
    n22.getValue(&pressao);
    pressao -= 1;
    n22.setValue(pressao);
}

//fluxo
void flPopCallback(void *ptr)
{
    fl.setValue(50);
}
void b18PopCallback(void *ptr)
{
    fl.getValue(&fluxs);
    fluxs += 1;
    fl.setValue(fluxs);
}
void b19PopCallback(void *ptr)
{
    fl.getValue(&fluxs);
    fluxs -= 1;
  fl.setValue(fluxs);
}


//PEEP
void pePopCallback(void *ptr)
{
    pe.setValue(50);
}
void b9PopCallback(void *ptr)
{
    pe.getValue(&PEEP);
    PEEP += 1;
    pe.setValue(PEEP);
}
void b10PopCallback(void *ptr)
{
    pe.getValue(&PEEP);
    PEEP -= 1;
    pe.setValue(PEEP);
}

void b11PopCallback(void *ptr)
{
    n4.getValue(&FIO2);
    FIO2 += 1;
    n4.setValue(FIO2);
}
void b12PopCallback(void *ptr)
{
    n4.getValue(&FIO2);
    FIO2 -= 1;
    n4.setValue(FIO2);
}

void b13PopCallback(void *ptr)
{
    x2.getValue(&SENS);
    SENS += 1;
    x2.setValue(SENS);
}
void b14PopCallback(void *ptr)
{
    x2.getValue(&SENS);
    SENS -= 1;
    x2.setValue(SENS);
}

void b20PopCallback(void *ptr)
{
    x3.getValue(&TINS);
    TINS += 1;
    x3.setValue(TINS);
}
void b21PopCallback(void *ptr)
{
    x3.getValue(&TINS);
    TINS -= 1;
    x3.setValue(TINS);
}

void b22PopCallback(void *ptr)
{
    x4.getValue(&TSUB);
    TSUB += 1;
    x4.setValue(TSUB);
}
void b23PopCallback(void *ptr)
{
    x4.getValue(&TSUB);
    TSUB -= 1;
    x4.setValue(TSUB);
}

void b28PopCallback(void *ptr)
{
    n7.getValue(&INSPEXP);
    INSPEXP += 1;
    n7.setValue(INSPEXP);
}
void b29PopCallback(void *ptr)
{
    n7.getValue(&INSPEXP);
    INSPEXP -= 1;
    n7.setValue(INSPEXP);
}

void b1PopCallback(void *ptr)
{
    x0.getValue(&pausa);
    pausa += 1;
    x0.setValue(pausa);
}
void b15PopCallback(void *ptr)
{
    x0.getValue(&pausa);
    pausa -= 1;
    x0.setValue(pausa);
}

void b24PopCallback(void *ptr)
{
    x5.getValue(&altura);
    altura += 1;
    x5.setValue(altura);

if (sexo==true)volcalc= (52 + (0.75 * (altura - 152.4)))*6;
if (sexo==false)volcalc= (49 + (0.67 * (altura - 152.4)))*6;
n6.setValue(volcalc);
}
void b25PopCallback(void *ptr)
{
    x5.getValue(&altura);
    altura -= 1;
    x5.setValue(altura);

if (sexo==true)volcalc= (52 + (0.75 * (altura - 152.4)))*6;
if (sexo==false)volcalc= (49 + (0.67 * (altura - 152.4)))*6;
n6.setValue(volcalc);

}

void b26PopCallback(void *ptr)
{
    n5.getValue(&idade);
    idade += 1;
    n5.setValue(idade);
}
void b27PopCallback(void *ptr)
{
    n5.getValue(&idade);
    idade -= 1;
    n5.setValue(idade);
}



void b16PopCallback(void *ptr)
{

//save();

 myVars.save();
}


void page0PopCallback(void *ptr)
{
pag=0;
    page0.show();
 }
//va0.getValue(&change);
void page1PopCallback(void *ptr)
{
page1.show();
pag=1;
if(mode_state==1){
        bt0.setValue(1);
        bt1.setValue(0);
        bt2.setValue(0);
    }
if(mode_state==2){
      bt1.setValue(1);
      bt0.setValue(0);
      bt2.setValue(0);
    }
if(mode_state==3){
      bt2.setValue(1);
      bt0.setValue(0);
      bt1.setValue(0);
    }

if(sense_state==1){
        bt3.setValue(1);
        bt4.setValue(0);
    }
if(sense_state==2)   {
      bt4.setValue(1);
      bt3.setValue(0);
 }

n0.setValue(volume);
n1.setValue(RPM);
n22.setValue(pressao);
fl.setValue(fluxs); 
n4.setValue(FIO2);
n5.setValue(idade);
n6.setValue(volcalc);
n7.setValue(INSPEXP); 
x0.setValue(pausa);
pe.setValue(PEEP);
x2.setValue(SENS);
x3.setValue(TINS); 
x4.setValue(TSUB);
x5.setValue(altura);
 
}

void page2PopCallback(void *ptr)
{
  page2.show();

  pag=2;
    n0.setValue(volume);
    vca.setValue(volumecorrentemax);
    vci.setValue(volumecorrentemin);
    fla.setValue(fluxomax); 
    fli.setValue(fluxomin);
    pra.setValue(pressaomax);
    pri.setValue(pressaomin);
    rpa.setValue(rpmmax); 
    rpi.setValue(rpmmin);
    pta.setValue(platomax);
    pti.setValue(platomin); 
    pea.setValue(peepmax);
    pei.setValue(peepmin);
    apa.setValue(maxapnea);
    api.setValue(INSPEXP); 
    //soa.setValue(RPM);
    soi.setValue(sao2min);
    coi.setValue(co2min); 

 
  Serial2.write(0xff);
 Serial2.write(0xff);
 Serial2.write(0xff);
Serial2.print("\"");

}

void page3PopCallback(void *ptr)
{
    pag=3;
    //page3.show();
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void t1Callback() {
//

if(primeira <= 0){
      medidaanterior=(analogRead(36) - 144)/10;
      soma = medidaanterior*0.010;
      primeira=1;
} else { 
     medidaatual= (analogRead(36) - 144)/10;
     soma = (soma + (medidaatual*0.010)) - (((medidaanterior-medidaatual)/2)*0.010) ; // ponto nao virgula
     medidaanterior=medidaatual;
     if (soma<=0)soma=0;
     volumecorrente=soma;
     primeira++;
}


//if (primeira > 2400/RPM ){
//  // Serial.println(soma);
//  primeira=0;
//  soma=0;
//}

ch0_data = volumecorrente/10 ;// verde 
 ch1_data = map(analogRead(36),120 ,5000 ,0 ,60);
    ch2_data = map(analogRead(36),120 ,4100 ,0 ,140 );
    ch3_data = map(analogRead(36),120 ,5000 ,254 ,200);
   
    pressaoinstantanea=ch2_data/1.4;
    fluxoinstantaneo=ch1_data;
}

void t2Callback(){
   cont++; 


//Serial.println("testtime");
   
if (pag == 0){
    s0.addValue(0, 160 + ch0_data);
    s0.addValue(1, 119 + ch1_data);
    s0.addValue(2, 20 + ch2_data);

if (cont==5||cont==10||cont==15){
  vce.setValue(volumecorrente);
  prs.setValue(pressaoinstantanea); 
  flx.setValue(fluxoinstantaneo);
    Serial2.write(0xff);
 Serial2.write(0xff);
 Serial2.write(0xff);
}
  if(cont>=16){
  s0.addValue( 3, 240 + ch3_data);
  pee.setValue(lastpeep);
  ppk.setValue(pressaopk);  
  fio.setValue(fio2fracao);  
  ief.setValue(iefracao);  
  rpm.setValue(rpmpaciente);    
  pla.setValue(plato);  
  sao.setValue(sao2); 
  Serial2.write(0xff);
 Serial2.write(0xff);
 Serial2.write(0xff);
  cont=0;
}
}
 
if(pag==2&&cont>=20){
 Serial2.write(0xff);
 Serial2.write(0xff);
 Serial2.write(0xff);
if(volumecorrente>=volumecorrentemax || volumecorrente<=volumecorrentemin)a2.Set_background_color_bco(63488);

if(fluxoinstantaneo>fluxomax || fluxoinstantaneo<fluxomin) a3.Set_background_color_bco(63488); 

if(pressaoinstantanea>pressaomax || pressaoinstantanea<pressaomin) a4.Set_background_color_bco(63488); 

if(pressaopk>pressaomax)a4.Set_background_color_bco(63488); 

if(lastpeep>peepmax || lastpeep<peepmin)  a5.Set_background_color_bco(63488); 

if (plato > platomax || plato < platomin)  a6.Set_background_color_bco(63488); 

if(rpmpaciente>rpmmax || rpmpaciente< rpmmin) a7.Set_background_color_bco(63488); 
  
  cont=0;
}


if(cont>100)cont=0;

}

void t3Callback() {

  tempoexpiracao = 600/RPM*(100-INSPEXP);
tempoinspiracao = 600/RPM*INSPEXP;
//Serial.println("start");
  if (mode_state == 1){//vcv
    if (ciclo == true){  // true = inspiracao
      tempo=tempo+10;
  if (tempo<=15){
      soma=0;
      primeira=0;
  }
    if(volumecorrente<=volume){
      servo1.write(10 );//servos ins abre max 
     // Pause=Pause+20;
    }
     //if (Pause >= pausaexp){
  if ((volumecorrente>=volume)||(tempo >= tempoinspiracao)){ //|| tempo >= tempoinspiracao){
    servo2.write(45);// servo exp abre 
    tempo=0;
    soma=0;
    ciclo=false;
}}

if (ciclo == false){          // false = expiracao 
 tempo=tempo+10;
 servo1.write(100);//servo ins fecha 
 //Pause=Pause+20;
if (pressaoinstantanea <= PEEP){
  servo2.write(0); // valvula exp fecha 
}
 if (tempo >= tempoexpiracao){
  ciclo=true;
   tempo=0;
   soma=0;
}
}
//Serial.println("end");
}


if (mode_state == 2){//pcv

if (ciclo == true && pressaoinstantanea <= pressao){  // true = inspiracao
  tempo =tempo+10;
  servo1.write(10 );
  //Pause=Pause+20;
if (pressaoinstantanea >= pressao){ 
    servo2.write(45);
    if (tempo >= tempoinspiracao){
    tempo=0;
    ciclo=false;
}}}

if (ciclo ==false){          // false = expiracao 
 tempo=tempo+10;
 servo1.write(100);   // fecha 
if (pressaoinstantanea <= PEEP){
 servo2.write(0);   //fecha
if (tempo >= tempoexpiracao){
  tempo=0;
  ciclo=true;
}
}}}

if (mode_state == 3){ //psv  
  if (ciclo == true){  // true = inspiracao
    tempo =tempo+10;
    servo1.write(10 );
    if (tempo >= tempoinspiracao){
    servo2.write(45);
    tempo=0;
    ciclo=false;
}}

if (ciclo ==false){          // false = expiracao 
 tempo=tempo+10;
 servo1.write(100);
 if (tempo >= tempoexpiracao){
  servo2.write(0);
  tempo=0;
  ciclo=true;
}}}

}

 void printmyVars()
{
//   Serial<<"   "<<myVars.volume<<"\n";
//   Serial<<"   "<<myVars.RPM<<"\n";
//   Serial<<"   "<<myVars.fluxs<<"\n";
//   Serial<<"   "<<myVars.PEEP<<"\n";
//   Serial<<"   "<<myVars.FIO2<<"\n";
//   Serial<<"   "<<myVars.SENS<<"\n";
//   Serial<<"   "<<myVars.TINS<<"\n";
//   Serial<<"   "<<myVars.TSUB<<"\n";
//   Serial<<"   "<<myVars.INSPEXP<<"\n";
//   Serial<<"   "<<myVars.altura<<"\n";
//   Serial<<"   "<<myVars.idade<<"\n";
//   Serial<<"   "<<myVars.volcalc<<"\n";
//   Serial<<"   "<<myVars.pausa<<"\n";
//   Serial<<"   "<<myVars.pressaomax<<"\n";
//   Serial<<"   "<<myVars.pressaomin<<"\n";
//   Serial<<"   "<<myVars.volumecorrentemax<<"\n";
//   Serial<<"   "<<myVars.volumecorrentemin<<"\n";
//   Serial<<"   "<<myVars.peepmax<<"\n";
//   Serial<<"   "<<myVars.peepmin<<"\n";
//   Serial<<"   "<<myVars.fluxomax<<"\n";
//   Serial<<"   "<<myVars.fluxomin<<"\n";
//   Serial<<"   "<<myVars.platomax<<"\n";
//   Serial<<"   "<<myVars.platomin<<"\n";
//   Serial<<"   "<<myVars.rpmmax<<"\n";
//   Serial<<"   "<<myVars.rpmmin<<"\n";
//   Serial<<"   "<<myVars.maxapnea<<"\n";
//   Serial<<"   "<<myVars.minapnea<<"\n";
//   Serial<<"   "<<myVars.sao2min<<"\n";
//   Serial<<"   "<<myVars.co2min<<"\n";
//  
//
//
//   
//   Serial<<"   "<<myVars.varsGood<<"\n";
}
   
void t4Callback() { 
//Serial.print("volume ");
// Serial.println(volume);
//Serial.print("tempo ");
// Serial.println(tempo);
// Serial.print("modo de ventilacao ");
//  Serial.println(mode_state);
//  Serial.print("vce ");
//   Serial.println(volumecorrente);
//   Serial.print("ciclo ");
//Serial.println(ciclo);
//   Serial.print("tempoexpiracao");
//Serial.println(tempoexpiracao);
//   Serial.print("");
//
//   Serial.print("");
     printmyVars();

}
   
  
//////////////////////////////////////////////////////////////////////////////////////////////


void setup(void)  
{


pinMode(36, INPUT);
nexInit();

// dbSerialPrintln("setup done");

servo1.attach(servoPin);
servo2.attach(servoPin2);
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); 

     myVars.get();
  if(myVars.varsGood!=190)
    {
      Serial<<"Failed to load persistent variables...initializing to defaults.! "<<micros();
      myVars.initialize();
    }
    else Serial<<"Successfully retrieved persistent variables....! "<<micros();
 
  Serial<<"\nStartup complete...\n";
    
   /* Set the baudrate which is for debug and communicate with Nextion screen. */
xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1,  0); 
xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 2, &Task2,  1); 

//vce.attachPop(vcePopCallback);
//popcalback alarm page
//a1.attachPop(a1PopCallback);
a2.attachPop(a2PopCallback);
a3.attachPop(a3PopCallback);
a4.attachPop(a4PopCallback);
a5.attachPop(a5PopCallback);
a6.attachPop(a6PopCallback);
a7.attachPop(a7PopCallback);
a8.attachPop(a8PopCallback);
a9.attachPop(a9PopCallback);
a10.attachPop(a10PopCallback);
a11.attachPop(a11PopCallback);
a12.attachPop(a12PopCallback);
a13.attachPop(a13PopCallback);
a14.attachPop(a14PopCallback);
a15.attachPop(a15PopCallback);
a16.attachPop(a16PopCallback);
a17.attachPop(a17PopCallback);
a18.attachPop(a18PopCallback);
a19.attachPop(a19PopCallback);
a20.attachPop(a20PopCallback);
a22.attachPop(a22PopCallback);
a23.attachPop(a23PopCallback);
a24.attachPop(a24PopCallback);
a25.attachPop(a25PopCallback);
a26.attachPop(a26PopCallback);
a27.attachPop(a27PopCallback);
a29.attachPop(a29PopCallback);
a30.attachPop(a30PopCallback);
a32.attachPop(a32PopCallback);
a33.attachPop(a33PopCallback);
a34.attachPop(a34PopCallback);
a35.attachPop(a35PopCallback);
a36.attachPop(a36PopCallback);
a37.attachPop(a37PopCallback);
a39.attachPop(a39PopCallback);
a42.attachPop(a42PopCallback);
a43.attachPop(a43PopCallback);
a44.attachPop(a44PopCallback);
a45.attachPop(a45PopCallback);
a46.attachPop(a46PopCallback);
a47.attachPop(a47PopCallback);
a49.attachPop(a49PopCallback);

//popcalbacks config page
//volume
      b3.attachPop(b3PopCallback);/* Register the pop event callback function of the current button0 component. */
    b4.attachPop(b4PopCallback); /* Register the pop event callback function of the current button1 component. */
//RPM
    b5.attachPop(b5PopCallback);/* Register the pop event callback function of the current button0 component. */
    b6.attachPop(b6PopCallback); /* Register the pop event callback function of the current button1 component. */
//pressao
    b7.attachPop(b7PopCallback);
    b8.attachPop(b8PopCallback);
//fluxo 
    b18.attachPop(b18PopCallback);/* Register the pop event callback function of the current button0 component. */
    b19.attachPop(b19PopCallback);
    //peep
    b9.attachPop(b9PopCallback);/* Register the pop event callback function of the current button0 component. */
    b10.attachPop(b10PopCallback);
   // fio2
    b11.attachPop(b11PopCallback);/* Register the pop event callback function of the current button0 component. */
    b12.attachPop(b12PopCallback);
    //sens
    b13.attachPop(b13PopCallback);/* Register the pop event callback function of the current button0 component. */
    b14.attachPop(b14PopCallback);
    //tins
    b20.attachPop(b20PopCallback);/* Register the pop event callback function of the current button0 component. */
    b21.attachPop(b21PopCallback);
    //tsub
    b22.attachPop(b22PopCallback);/* Register the pop event callback function of the current button0 component. */
    b23.attachPop(b23PopCallback);
  //inspexp
    b28.attachPop(b28PopCallback);/* Register the pop event callback function of the current button0 component. */
    b29.attachPop(b29PopCallback);
//pausa
    b1.attachPop(b1PopCallback);/* Register the pop event callback function of the current button0 component. */
    b15.attachPop(b15PopCallback);
   //altura
    b24.attachPop(b24PopCallback);/* Register the pop event callback function of the current button0 component. */
    b25.attachPop(b25PopCallback);
    //idade
    b26.attachPop(b26PopCallback);/* Register the pop event callback function of the current button0 component. */
    b27.attachPop(b27PopCallback);
  
//page popcalbacks
    page0.attachPop(page0PopCallback);
    page1.attachPop(page1PopCallback);
    page2.attachPop(page2PopCallback);
    page3.attachPop(page3PopCallback);

//dual state pop
  bt0.attachPop(bt0PopCallback, &bt0);
  bt1.attachPop(bt1PopCallback, &bt1);
  bt2.attachPop(bt2PopCallback, &bt2);
  bt3.attachPop(bt3PopCallback, &bt3);
  bt4.attachPop(bt4PopCallback, &bt4);

 //checkbox pop
  c0.attachPop(c0PopCallback, &c0);
  c1.attachPop(c1PopCallback, &c1);
  c2.attachPop(c2PopCallback, &c2);

  runner.init();  
  runner.addTask(t1);
  runner.addTask(t2);
  runner.addTask(t3);
  runner.addTask(t4);

  t1.enable();
  t2.enable();
  t3.enable();
  t4.enable();
//EEPROM.begin(EEPROM_SIZE);
//volume=crazy[0];
//        volume=EEPROM.get(0 );
//        
//        RPM=EEPROM.get(2);
//        
//        pressao=EEPROM.read(4);
//        
//        PEEP=EEPROM.read(6);
//        
//        fluxs=EEPROM.read(8);
//        
//        altura=EEPROM.read(10);
//        
//        INSPEXP=EEPROM.read(12);
//        
//        mode_state=EEPROM.read(14);
//        
//        
//        sense_state=EEPROM.read(16);
//        
//        
//        sexo=EEPROM.read(18);
//        
//        SENS=EEPROM.read(20);
//        
//        TINS=EEPROM.read(22);
//        
//        TSUB=EEPROM.read(24);
//        
//        pausa=EEPROM.read(26);
//        
//        FIO2=EEPROM.read(28);
//        

//EEPROM.get(0, volume);
//    
//EEPROM.get(8, RPM);
    
//    EEPROM.get(4, pressao);
//    
//    EEPROM.get(6, PEEP);
//    
//    EEPROM.get(8, fluxs);
//    
//    EEPROM.get(10, altura);
//    
//EEPROM.get(12, INSPEXP);
//    
//    EEPROM.get(14, mode_state);
//    
//
//    EEPROM.get(16, sense_state);
//    
//
//    EEPROM.get(18, sexo);
//    
//    EEPROM.get(20, SENS);
//    
//    EEPROM.get(22, TINS);
//    
//    EEPROM.get(24, TSUB);
//    
//    EEPROM.get(26, pausa);
//    
//    EEPROM.get(28, FIO2);

}





void Task1code( void * pvParameters ){
    for (;;){
    runner.execute();
    vTaskDelay(10);
  }}
//  
  void Task2code( void * pvParameters ){
    for (;;){
        if (pag==0){
nexLoop(nex_listen_list0);
  }
if (pag==1){
nexLoop(nex_listen_list1);
  }
  if (pag==2){
nexLoop(nex_listen_list2);
  }
  
 vTaskDelay(10);
}
}

// if (millis()>=lastmillis+200){
//
//Serial.println (fluxs);
//lastmillis=millis();
// }

   
void loop(void){
   }

  
