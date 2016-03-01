/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2015-2016  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

//include files
#include"picsimlab1.h"
#include"board_5.h"


/* ids of inputs of input map*/
#define I_ICSP	1  //ICSP connector
#define I_PWR	2  //Power button
#define I_RST	3  //Reset button
#define I_S1	4  //S1 push button
#define I_JMP	5  //JMP


/* ids of outputs of output map*/
#define O_JMP	1  //JMP
#define O_D1	2  //LED D1
#define O_D2	3  //LED D2 
#define O_D4	4  //LED D4 
#define O_D5	5  //LED D5 
#define O_D6	6  //LED D6 
#define O_D7	7  //LED D7 

//return the input ids numbers of names used in input map
unsigned short 
cboard_5::get_in_id(char * name)
{
  if(strcmp(name,"I_ICSP")==0)return I_ICSP;
  if(strcmp(name,"I_PWR")==0)return I_PWR;  
  if(strcmp(name,"I_RST")==0)return I_RST; 
  if(strcmp(name,"I_S1")==0)return I_S1;
  if(strcmp(name,"I_JMP")==0)return I_JMP;
  
  printf("Erro input '%s' don't have a valid id! \n",name);
  return -1;
};

//return the output ids numbers of names used in output map
unsigned short 
cboard_5::get_out_id(char * name)
{ 
    
  if(strcmp(name,"O_JMP")==0)return O_JMP;
  if(strcmp(name,"O_D1")==0)return O_D1;
  if(strcmp(name,"O_D2")==0)return O_D2;
  if(strcmp(name,"O_D4")==0)return O_D4;
  if(strcmp(name,"O_D5")==0)return O_D5;
  if(strcmp(name,"O_D6")==0)return O_D6;
  if(strcmp(name,"O_D7")==0)return O_D7;
  
  printf("Erro output '%s' don't have a valid id! \n",name);
  return 1;
};

//Constructor called once on board creation 
cboard_5::cboard_5(void)
{
  proc=P16F1619;  //default microcontroller if none defined in preferences
  ReadMaps();     //Read input and output board maps
  jmp[0]=0; 
      
  //controls propierties and creation
  //scroll1
  scroll1=new CScroll();   
  scroll1->SetFOwner(&Window1);
  scroll1->SetName(wxT("scroll1_p5"));
  scroll1->SetX(48);
  scroll1->SetY(200);
  scroll1->SetWidth(110);
  scroll1->SetHeight(22);
  scroll1->SetEnable(1);
  scroll1->SetVisible(1);
  scroll1->SetRange(100);
  scroll1->SetPosition(50);
  scroll1->SetType(4);
  Window1.CreateChild(scroll1);
  //gauge1
  gauge1=new CGauge();
  gauge1->SetFOwner(&Window1);
  gauge1->SetName(wxT("gauge1_p5"));
  gauge1->SetX(48);
  gauge1->SetY(230);
  gauge1->SetWidth(110);
  gauge1->SetHeight(20);
  gauge1->SetEnable(1);
  gauge1->SetVisible(1);
  gauge1->SetRange(100);
  gauge1->SetValue(0);
  gauge1->SetType(4);
  Window1.CreateChild(gauge1);
  //gauge2
  gauge2=new CGauge();
  gauge2->SetFOwner(&Window1);
  gauge2->SetName(wxT("gauge2_p5"));
  gauge2->SetX(48);
  gauge2->SetY(255);
  gauge2->SetWidth(110);
  gauge2->SetHeight(20);
  gauge2->SetEnable(1);
  gauge2->SetVisible(1);
  gauge2->SetRange(100);
  gauge2->SetValue(0);
  gauge2->SetType(4);
  Window1.CreateChild(gauge2);   
  //gauge3
  gauge3=new CGauge();
  gauge3->SetFOwner(&Window1);
  gauge3->SetName(wxT("gauge3_p5"));
  gauge3->SetX(48);
  gauge3->SetY(280);
  gauge3->SetWidth(110);
  gauge3->SetHeight(20);
  gauge3->SetEnable(1);
  gauge3->SetVisible(1);
  gauge3->SetRange(100);
  gauge3->SetValue(0);
  gauge3->SetType(4);
  Window1.CreateChild(gauge3);   
  //gauge4
  gauge4=new CGauge();
  gauge4->SetFOwner(&Window1);
  gauge4->SetName(wxT("gauge4_p5"));
  gauge4->SetX(48);
  gauge4->SetY(305);
  gauge4->SetWidth(110);
  gauge4->SetHeight(20);
  gauge4->SetEnable(1);
  gauge4->SetVisible(1);
  gauge4->SetRange(100);
  gauge4->SetValue(0);
  gauge4->SetType(4);
  Window1.CreateChild(gauge4);   
  //label1
  label1=new CLabel();
  label1->SetFOwner(&Window1);
  label1->SetName(wxT("label1_p5"));
  label1->SetX(12);
  label1->SetY(200);
  label1->SetWidth(60);
  label1->SetHeight(20);
  label1->SetEnable(1);
  label1->SetVisible(1);
  label1->SetText(wxT("AN4"));
  label1->SetAlign(1);
  Window1.CreateChild(label1); 
  //label2
  label2=new CLabel();
  label2->SetFOwner(&Window1);
  label2->SetName(wxT("label2_p5"));
  label2->SetX(12);
  label2->SetY(230);
  label2->SetWidth(60);
  label2->SetHeight(20);
  label2->SetEnable(1);
  label2->SetVisible(1);
  label2->SetText(wxT("RA5"));
  label2->SetAlign(1);
  Window1.CreateChild(label2);    
  //label3
  label3=new CLabel();
  label3->SetFOwner(&Window1);
  label3->SetName(wxT("label3_p5"));
  label3->SetX(13);
  label3->SetY(255);
  label3->SetWidth(60);
  label3->SetHeight(20);
  label3->SetEnable(1);
  label3->SetVisible(1);
  label3->SetText(wxT("RA1"));
  label3->SetAlign(1);
  Window1.CreateChild(label3);    
  //label4
  label4=new CLabel();
  label4->SetFOwner(&Window1);
  label4->SetName(wxT("label4_p5"));
  label4->SetX(13);
  label4->SetY(280);
  label4->SetWidth(60);
  label4->SetHeight(20);
  label4->SetEnable(1);
  label4->SetVisible(1);
  label4->SetText(wxT("RA2"));
  label4->SetAlign(1);
  Window1.CreateChild(label4);  
  //label5
  label5=new CLabel();
  label5->SetFOwner(&Window1);
  label5->SetName(wxT("label5_p5"));
  label5->SetX(13);
  label5->SetY(305);
  label5->SetWidth(60);
  label5->SetHeight(20);
  label5->SetEnable(1);
  label5->SetVisible(1);
  label5->SetText(wxT("RC5"));
  label5->SetAlign(1);
  Window1.CreateChild(label5);  
};

//Destructor called once on board destruction 
cboard_5::~cboard_5(void)
{
    //controls destruction 
    Window1.DestroyChild(scroll1);
    Window1.DestroyChild(gauge1);
    Window1.DestroyChild(gauge2);
    Window1.DestroyChild(gauge3);
    Window1.DestroyChild(gauge4);
    Window1.DestroyChild(label1);
    Window1.DestroyChild(label2);
    Window1.DestroyChild(label3);
    Window1.DestroyChild(label4);
    Window1.DestroyChild(label5);
}

//Reset board status
void 
cboard_5::Reset(_pic *pic)
{
   
    p_BT1=1;//set push button  in default state (high) 
       
    //write button state to pic pin 6 (RC4)
    pic_set_pin(pic,6,p_BT1); 

        
  //verify serial port state and refresh status bar  
#ifndef _WIN_
    if(pic->serialfd > 0)
#else
    if(pic->serialfd != INVALID_HANDLE_VALUE)
#endif
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(":")+itoa(pic->serialbaud)+wxT("(")+
        String().Format("%4.1f",fabs((100.0*pic->serialexbaud-100.0*
        pic->serialbaud)/pic->serialexbaud))+wxT("%)"));
    else  
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(" (ERROR)"));
        

};

//Called ever 1s to refresh status
void 
cboard_5::RefreshStatus(_pic *pic)
{
   //verify serial port state and refresh status bar   
#ifndef _WIN_
    if(pic->serialfd > 0)
#else
    if(pic->serialfd != INVALID_HANDLE_VALUE)
#endif
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(":")+itoa(pic->serialbaud)+wxT("(")+
        String().Format("%4.1f",fabs((100.0*pic->serialexbaud-100.0*
        pic->serialbaud)/pic->serialexbaud))+wxT("%)"));
    else  
      Window1.statusbar1.SetField(2,wxT("Serial Port: ")+
        String::FromAscii(SERIALDEVICE)+wxT(" (ERROR)"));
    
};

//Called to save board preferences in configuration file
void 
cboard_5::WritePreferences(void)
{
    char line[100];
    //write selected microcontroller of board_5 to preferences
    Window1.saveprefs(wxT("p5_proc"),getnamebyproc(proc,line));
    Window1.saveprefs(wxT("p5_jmp"),String::Format("%i",jmp[0]));
};

//Called whe configuration file load  preferences 
void 
cboard_5::ReadPreferences(char *name,char *value)
{
    //read microcontroller of preferences
    if(!strcmp(name,"p5_proc"))
    {
      proc=getprocbyname(value); 
    }
    
    if(!strcmp(name,"p5_jmp"))
    {
      int i;  
      for(i=0;i<1;i++)
      {
        if(value[i] == '0')      
          jmp[i]=0;
        else
          jmp[i]=1;
      } 
    }
};


//Event on the board
void 
cboard_5::KeyPress(_pic *pic, uint key, uint x, uint y,uint mask)
{
  //if keyboard key 1 is pressed then activate button (state=0)   
  if(key == '1')
  {
    p_BT1=0; 
  }
  
  
};

//Event on the board
void
cboard_5::KeyRelease(_pic *pic, uint key, uint x, uint y,uint mask)
{
  //if keyboard key 1 is pressed then deactivate button (state=1)     
  if(key == '1')
  {
    p_BT1=1; 
  }  
  
};

//Event on the board
void 
cboard_5::MouseButtonPress(_pic *pic, uint button, uint x, uint y,uint state)
{
 
  int i;
  
  //search for the input area which owner the event    
  for(i=0;i<inputc;i++)
  {
    if(((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&
       (input[i].y2 >= y)))
    {
   
      switch(input[i].id) 
      { 
        //if event is over I_ISCP area then load hex file  
        case I_ICSP:
          Window1.menu1_File_LoadHex_EvMenuActive(NULL);
          break;  
        //if event is over I_PWR area then toggle board on/off
        case I_PWR:
          if(Window1.Get_picpwr()) //if on turn off
          { 
            Window1.Set_picrun(0); 
            Window1.Set_picpwr(0); 
            pic_reset(pic,1);
            Reset(pic);
            p_BT1=1; 
            Window1.statusbar1.SetField(0,wxT("Stoped"));
          }
          else //if off turn on
          {
            Window1.Set_picpwr(1);
            Window1.Set_picrun(1);
            pic_reset(pic,1);          
            Reset(pic);
            Window1.statusbar1.SetField(0,wxT("Running..."));
          } 
          break;
        //if event is over I_RST area then turn off and reset
        case I_RST:
          if(Window1.Get_picpwr())//if powered
          {
            Window1.Set_picpwr(0);
            Window1.Set_picrst(1);
          } 
          break;
        //if event is over I_S1 area then activate button (state=0) 
        case I_S1:
          p_BT1=0;
          break;
        case I_JMP:
          jmp[0]^=0x01;
          break;  

      }
    }
  }
    
};

//Event on the board
void 
cboard_5::MouseButtonRelease(_pic *pic, uint button, uint x, uint y,uint state)
{
  int i;

  //search for the input area which owner the event  
  for(i=0;i<inputc;i++)
  {
    if(((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&
       (input[i].y2 >= y)))
    {
      switch(input[i].id)
      {
        //if event is over I_RST area then turn on  
        case I_RST:
          if(Window1.Get_picrst())//if powered
          {
            Window1.Set_picpwr(1);
            Window1.Set_picrst(0);

            if(pic_reset(pic,-1))
            {  
              Reset(pic);
            }
          } 
          break;
        //if event is over I_S1 area then deactivate button (state=1) 
        case I_S1:
          p_BT1=1; 
          break;
      }
    } 
  }

};


//Called ever 100ms to draw board
//This is the critical code for simulator running speed
void cboard_5::Draw(_pic *pic, CDraw *draw,double scale)
{
  int i;
  int j;
  unsigned char pi;
  const picpin * pins;
  
  int JUMPSTEPS = Window1.GetJUMPSTEPS(); //number of steps skipped
  long int NSTEPJ=Window1.GetNSTEPJ();  //number of steps in 100ms
  
  
  draw->Canvas.Init(scale,scale); //initialize draw context
  
//board_5 draw 
  for(i=0;i<outputc;i++) //run over all outputs
  {
    if(!output[i].r)//if output shape is a rectangle
    {
        draw->Canvas.SetFgColor (0, 0, 0);//black
 
        switch(output[i].id)//search for color of output
        {
          case O_D1: //green using picpwr value
            draw->Canvas.SetColor (0 , 225*Window1.Get_picpwr()+30, 0 );
            break;
          case O_D2: //green using picpwr value
            draw->Canvas.SetColor (0 , 225*Window1.Get_picpwr()+30, 0);
            break;
          case O_D4: //Red using pin 2 mean  value (RA5) 
            draw->Canvas.SetColor (lm[1], 0, 0);
            break;
          case O_D5: //Red using pin 18 mean value (RA1)
            draw->Canvas.SetColor (lm[17], 0, 0);
            break;
          case O_D6: //Red using pin 17 mean value (RA2)
            draw->Canvas.SetColor (lm[16],0 , 0);
            break;
          case O_D7: //Red using pin 5 mean value (RC5)
            draw->Canvas.SetColor (lm[4],0 , 0);
            break;   
          case O_JMP:
            draw->Canvas.SetColor (150, 150, 150);  
            break;
        }
        
        //draw a rectangle
        draw->Canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2-output[i].x1,output[i].y2-output[i].y1 );       

        if(output[i].id == O_JMP)
        {
         if(!jmp[0])
         {
           draw->Canvas.SetColor (70, 70, 70);
           draw->Canvas.Rectangle (1, output[i].x1, output[i].y1, (int)((output[i].x2-output[i].x1)*0.65),output[i].y2-output[i].y1 );
           draw->Canvas.SetColor (220, 220, 0);
           draw->Canvas.Circle (1,output[i].x1+(int)((output[i].x2-output[i].x1)*0.80), output[i].y1+((output[i].y2-output[i].y1)/2),3);
         }
         else
         {
           draw->Canvas.SetColor (70, 70, 70);
           draw->Canvas.Rectangle (1, output[i].x1+((int)((output[i].x2-output[i].x1)*0.35)), output[i].y1, (int)((output[i].x2-output[i].x1)*0.65),output[i].y2-output[i].y1 );
           draw->Canvas.SetColor (220, 220, 0);
           draw->Canvas.Circle (1,output[i].x1+(int)((output[i].x2-output[i].x1)*0.20), output[i].y1+((output[i].y2-output[i].y1)/2),3);
         };
        } 
      
    }
    else //if output shape is a circle
    {
      
    };    

  };

  //end draw
  draw->Canvas.End();
  draw->Update ();
  
  
  //reset mean value
  for(pi=0;pi < pic->PINCOUNT;pi++)
  {
    lm[pi]=0;
  };

 //read pic.pins to a local variable to speed up 
 pins = pic->pins;

  
 j=JUMPSTEPS+1;//step counter
 if(Window1.Get_picpwr()) //if powered
   for(i=0;i<Window1.GetNSTEP();i++) //repeat for number of steps in 100ms
      {
 
        if(j > JUMPSTEPS)//if number of step is bigger than steps to skip 
        {  
          pic_set_pin(pic,6,p_BT1);//Set pin 6 (RC4) with button state 
        } 
        
        //verify if a breakpoint is reached if not run one instruction 
        if(!mplabxd_testbp(pic))pic_step(pic,0);

        if(j > JUMPSTEPS)//if number of step is bigger than steps to skip 
        {  
          //increment mean value counter if pin is high  
          for(pi=0;pi < pic->PINCOUNT;pi++)
          {
           lm[pi]+=pins[pi].value;
          }
          
          //set analog pin 16 (RC0 AN4) with value from scroll  
          pic_set_apin(pic,16,((5.0*(scroll1->GetPosition()))/
            (scroll1->GetRange()-1)));
          
          j=0;//reset counter
        }
        
        j++;//counter increment
     }


     //RA5 mean value to gauge1
     gauge1->SetValue((100.0*lm[1])/NSTEPJ); 
     //RA1 mean value to gauge2
     gauge2->SetValue((100.0*lm[17])/NSTEPJ); 
     //RA2 mean value to gauge3
     gauge3->SetValue((100.0*lm[16])/NSTEPJ); 
     //RC5 mean value to gauge4
     gauge4->SetValue((100.0*lm[4])/NSTEPJ); 
   

     //calculate mean value
     for(pi=0;pi < pic->PINCOUNT;pi++)
     { 
      lm[pi]= (int)(((225.0*lm[pi])/NSTEPJ)+30);
     }

};
