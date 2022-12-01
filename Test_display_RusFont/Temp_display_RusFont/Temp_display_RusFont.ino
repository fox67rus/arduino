/*
Управление экраном 3.2
Вывод русских имволов
Регулировка яркости (ШИМ)

VGA_BLACK – черный, 
VGA_SILVER – серебряный
VGA_GRAY – серый
VGA_WHITE – белый
VGA_MAROON – красно-коричневый
VGA_RED – красный
VGA_PURPLE – пурпурный
VGA_FUCHSIA – фуксия
VGA_GREEN – зеленый 
VGA_LIME – лайм
VGA_NAVY – темно-синий
VGA_BLUE – синий
VGA_TEAL – сине-зеленый
VGA_AQUA – морская волна
*/

#include <UTFT.h>
#include <UTFT_DLB.h>

UTFT_DLB   myGLCD(TFT01_22SP, 9, 8, 12, 11, 10);  // инициализация дисплея

//#define PIN_BLK  46     // выход включения подсветки  (мах -4 мА) я использую ШИМ для регулировки яркости

//объявление шрифта
extern uint8_t FontRusProp18[];

void setup()
{
/* выключение подсветки экрана
  pinMode(PIN_BLK, OUTPUT);    
  digitalWrite(PIN_BLK, LOW); 
  analogWrite(PIN_BLK,150);  // о 5 до 255 */ 

  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setColor(VGA_WHITE); 

}

void loop()
{
char szTextRus1[60]="АБВГДЕЁЖЗИЙКЛМНОПРСТИУФХ.";
char szTextRus2[60]="ЦЧЬЪШЩЫЭЮЯ  1234567890";
char szTextRus3[60]="абвгдеёжзийклмнопрстуфхцчшщъы";
char szTextRus4[60]="ьэюя  +-/=*_:";
char szTextASCII[60];
unsigned int i,j;

    myGLCD.clrScr();    
    myGLCD.fillScr(VGA_BLACK);   
    myGLCD.setColor(VGA_TEAL);    //myGLCD.setBackColor(VGA_WHITE);      
    myGLCD.setFont(FontRusProp18);
     
     
// вариант 1
     j=ConvertUniToAscii(szTextRus1,szTextASCII,30);     
     myGLCD.print(szTextASCII, 4, 6, 0);  
// вариант 2
     RusPrintStr(myGLCD,szTextRus2,0,24,0);


     RusPrintStr(myGLCD,szTextRus3,0,42,0);


     j=ConvertUniToAscii(szTextRus4,szTextASCII,30);     
     myGLCD.print(szTextASCII, 0, 60, 0); 


     j=ConvertUniToAscii("ТЕМПЕРАТУРА +24.7гр  ",szTextASCII,30);     
     myGLCD.print(szTextASCII, 0, 80, 0);

     j=ConvertUniToAscii("Влажность-85% ",szTextASCII,30);     
     myGLCD.print(szTextASCII, 0, 100, 0);

     j=ConvertUniToAscii("Время 12:30 Дата 05.07.2020г.",szTextASCII,30);     
     myGLCD.print(szTextASCII, 50, 125, 0);

     j=ConvertUniToAscii("ВРЕМЯ 19:45  ДАТА 30.06.2027г.",szTextASCII,30);     
     myGLCD.print(szTextASCII, 0, 185, 0);


//     j=ConvertUniToAscii("ВРЕМЯ кушать ягоду!",szTextASCII,30);     
//     myGLCD.print(szTextASCII, 0, 215, 0);

     RusPrintStr(myGLCD,"ВРЕМЯ кушать ягоду!",0,215,0);



    delay(1000000);



  
}


//=====================================================================
// перекодировка русских символов и вывод на экран
void RusPrintStr(UTFT_DLB tft,char *st, int x, int y, int deg)
{
char szTextASCII[60];  
     ConvertUniToAscii(st,szTextASCII,30);     
     tft.print(szTextASCII, x, y,deg);
}

//=========================================================================
// возвращает длину получившейся строки в приемние или 0 при ошибке
// iDlPri - максимальное длина буфера
int  ConvertUniToAscii(char szIst[],char szPri[], int iDlPri)
{
int iDl = strlen(szIst);  
int iPr=0,iI;
uint8_t ui8C,ui8C2;
    for(iI=0;iI<iDl;iI++)
    { ui8C= szIst[iI]; 
      if(ui8C!=208&&ui8C!=209) {szPri[iPr]=ui8C; iPr++;}
      else
      {  iI++;
         ui8C2=szIst[iI];  
         if(ui8C==208)
         {  
            if(ui8C2>=144&&ui8C2<=191) {ui8C2=ui8C2-16; szPri[iPr]=ui8C2; iPr++;}
            else 
            { if(ui8C2==129) {ui8C2=240; szPri[iPr]=ui8C2; iPr++;}
            }
         }
         if(ui8C==209)
         {  if(ui8C2>=128&&ui8C2<=143||ui8C2==145) {ui8C2=ui8C2+96; szPri[iPr]=ui8C2; iPr++;}
         }
         
      }
      if(iPr>=iDlPri) {szPri[iDlPri-1]=0; return 0;}
   }  
   szPri[iPr]=0;
   return iPr;
}  
