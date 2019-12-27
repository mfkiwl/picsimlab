/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2020  Luis Claudio Gambôa Lopes

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

#include"../picsimlab1.h"
#include"../picsimlab4.h"
#include"../picsimlab5.h"
#include"part_TempSys.h"

/* outputs */
enum
{
 O_HT, O_CO, O_TE, O_TA, O_F1, O_F2, O_OTA, O_OTE, O_VT
};

/* inputs */
enum
{
 I_PO1, I_PO2, I_PO3, I_PO4
};

cpart_tempsys::cpart_tempsys(unsigned x, unsigned y)
{
 X = x;
 Y = y;
 ReadMaps ();

 lxImage image;
 image.LoadFile (Window1.GetSharePath () + lxT ("parts/") + GetPictureFileName ());

 Bitmap = new lxBitmap (image, &Window5);
 image.Destroy ();

 canvas.Create (Window5.GetWWidget (), Bitmap);

 vtc = 0;
 vt = 0;

 image.LoadFile (Window1.GetSharePath () + lxT ("boards/VT1.png"));
 vent[0] = new lxBitmap (image, &Window1);
 image.Destroy ();

 image.LoadFile (Window1.GetSharePath () + lxT ("boards/VT2.png"));
 vent[1] = new lxBitmap (image, &Window1);
 image.Destroy ();

 input_pins[0] = 0;
 input_pins[1] = 0;
 input_pins[2] = 0;
 input_pins[3] = 0;

 temp[0] = 27.5;
 temp[1] = 27.5;
 ref = 0;
 rpmstp = 0; //(NSTEP*2)/100;
 rpmc = 0;
 
 refresh=0;
}

cpart_tempsys::~cpart_tempsys(void)
{
 delete Bitmap;

 delete vent[0];
 delete vent[1];
 vent[0] = NULL;
 vent[1] = NULL;

}

void
cpart_tempsys::Draw(void)
{
 int i;
 String str;
 board *pboard = Window1.GetBoard ();

 const picpin * ppins = pboard->MGetPinsValues ();

 canvas.Init ();

 lxFont font (9, lxFONTFAMILY_TELETYPE, lxFONTSTYLE_NORMAL, lxFONTWEIGHT_BOLD);
 canvas.SetFont (font);

 for (i = 0; i < outputc; i++)
  {

   switch (output[i].id)
    {
    case O_HT:
    case O_CO:
    case O_TA:
    case O_TE:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     if (input_pins[output[i].id - O_HT] == 0)
      canvas.Text ("NC", output[i].x1, output[i].y1);
     else
      canvas.Text (pboard->MGetPinName (input_pins[output[i].id - O_HT]), output[i].x1, output[i].y1);
     break;
    case O_F1:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (155, 155, 155);
     canvas.Text ("12V", output[i].x1, output[i].y1);
     break;
    case O_F2:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (155, 155, 155);
     canvas.Text ("GND", output[i].x1, output[i].y1);
     break;
    case O_OTA:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     canvas.Text ("Ambient=27.5C", output[i].x1, output[i].y1);
     break;
    case O_OTE:
     str.Printf (lxT ("Temp.=%5.2fC"), temp[0]);
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     canvas.Text (str, output[i].x1, output[i].y1);
     break;
    case O_VT:
     if(input_pins[1] == 0)break;
     if (ppins[input_pins[1] - 1].oavalue > 30) vtc++;

     if (vtc > (4 - 0.04 * ppins[input_pins[1] - 1].oavalue))
      {
       vtc = 0;
       canvas.PutBitmap (vent[vt], output[i].x1, output[i].y1);
       vt ^= 1;
      }
     break;
    }
  }

 //sensor ventilador
 if(input_pins[1] > 0)
   rpmstp = ((float) Window1.GetNSTEPJ ()) / (0.64 * (ppins[input_pins[1] - 1].oavalue - 29));
 //temperatura 
 if((input_pins[0] > 0)&&(input_pins[1] > 0))
   ref = ((0.2222 * (ppins[input_pins[0] - 1].oavalue - 30)))-(0.2222 * (ppins[input_pins[1] - 1].oavalue - 30));

 if (ref < 0)
  ref = 0;

 temp[1] = temp[0];
 temp[0] = ((27.5 + ref)*0.003) + temp[1]*(0.997);

 pboard->MSetAPin (input_pins[2], temp[0] / 100.0);

 canvas.End ();

}

void
cpart_tempsys::Process(void)
{


 if (!refresh)
  {
   refresh = Window1.GetJUMPSTEPS ();

   board *pboard = Window1.GetBoard ();

   const picpin * ppins = pboard->MGetPinsValues ();
   
   if((input_pins[1]>0)&&(input_pins[3]>0))
   {
   if (ppins[input_pins[1] - 1].oavalue > 30)
    {
     rpmc++;
     if (rpmc > rpmstp)
      {
       rpmc = 0;
       pboard->MSetPin (input_pins[3], !ppins[input_pins[3] - 1].value);
      }
    }
   else
    pboard->MSetPin (input_pins[3], 0);
   }
  }
 refresh--;

}

void
cpart_tempsys::EvMouseButtonPress(uint button, uint x, uint y, uint state) {
 }

void
cpart_tempsys::EvMouseButtonRelease(uint button, uint x, uint y, uint state) {
 }

void
cpart_tempsys::EvMouseMove(uint button, uint x, uint y, uint state) {
 }

unsigned short
cpart_tempsys::get_in_id(char * name)
{

 printf ("Erro input '%s' don't have a valid id! \n", name);
 return -1;
};

unsigned short
cpart_tempsys::get_out_id(char * name)
{

 if (strcmp (name, "VT") == 0)return O_VT;
 if (strcmp (name, "HT") == 0)return O_HT;
 if (strcmp (name, "CO") == 0)return O_CO;
 if (strcmp (name, "TE") == 0)return O_TE;
 if (strcmp (name, "TA") == 0)return O_TA;
 if (strcmp (name, "F1") == 0)return O_F1;
 if (strcmp (name, "F2") == 0)return O_F2;
 if (strcmp (name, "OTA") == 0)return O_OTA;
 if (strcmp (name, "OTE") == 0)return O_OTE;

 printf ("Erro output '%s' don't have a valid id! \n", name);
 return 1;
};

String
cpart_tempsys::WritePreferences(void)
{
 char prefs[256];

 sprintf (prefs, "%hhu,%hhu,%hhu,%hhu", input_pins[0], input_pins[1], input_pins[2], input_pins[3]);

 return prefs;
};

void
cpart_tempsys::ReadPreferences(String value)
{
 sscanf (value.c_str (), "%hhu,%hhu,%hhu,%hhu", &input_pins[0], &input_pins[1], &input_pins[2], &input_pins[3]);
};

CPWindow * WProp_tempsys;

void
cpart_tempsys::ConfigurePropertiesWindow(CPWindow * wprop)
{
 String Items = "0  NC,";
 String spin;
 WProp_tempsys = wprop;
 board *pboard = Window1.GetBoard ();

 for (int i = 1; i <= pboard->MGetPinCount (); i++)
  {
   spin = pboard->MGetPinName (i);

   if (spin.Cmp (lxT ("error")))
    {
     Items = Items + itoa (i) + "  " + spin + ",";
    }
  }

 ((CCombo*) WProp_tempsys->GetChildByName ("combo1"))->SetItems (Items);
 if (input_pins[0] == 0)
  ((CCombo*) WProp_tempsys->GetChildByName ("combo1"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (input_pins[0]);
   ((CCombo*) WProp_tempsys->GetChildByName ("combo1"))->SetText (itoa (input_pins[0]) + "  " + spin);
  }

 ((CCombo*) WProp_tempsys->GetChildByName ("combo2"))->SetItems (Items);
 if (input_pins[1] == 0)
  ((CCombo*) WProp_tempsys->GetChildByName ("combo2"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (input_pins[1]);
   ((CCombo*) WProp_tempsys->GetChildByName ("combo2"))->SetText (itoa (input_pins[1]) + "  " + spin);
  }

 ((CCombo*) WProp_tempsys->GetChildByName ("combo3"))->SetItems (Items);
 if (input_pins[2] == 0)
  ((CCombo*) WProp_tempsys->GetChildByName ("combo3"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (input_pins[2]);
   ((CCombo*) WProp_tempsys->GetChildByName ("combo3"))->SetText (itoa (input_pins[2]) + "  " + spin);
  }

 ((CCombo*) WProp_tempsys->GetChildByName ("combo4"))->SetItems (Items);
 if (input_pins[3] == 0)
  ((CCombo*) WProp_tempsys->GetChildByName ("combo4"))->SetText ("0  NC");
 else
  {
   spin = pboard->MGetPinName (input_pins[3]);
   ((CCombo*) WProp_tempsys->GetChildByName ("combo4"))->SetText (itoa (input_pins[3]) + "  " + spin);
  }


 ((CButton*) WProp_tempsys->GetChildByName ("button1"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CPWindow5::PropButtonRelease;
 ((CButton*) WProp_tempsys->GetChildByName ("button1"))->SetTag (1);

 ((CButton*) WProp_tempsys->GetChildByName ("button2"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CPWindow5::PropButtonRelease;
}

void
cpart_tempsys::ReadPropertiesWindow(void)
{
 input_pins[0] = atoi (((CCombo*) WProp_tempsys->GetChildByName ("combo1"))->GetText ());
 input_pins[1] = atoi (((CCombo*) WProp_tempsys->GetChildByName ("combo2"))->GetText ());
 input_pins[2] = atoi (((CCombo*) WProp_tempsys->GetChildByName ("combo3"))->GetText ());
 input_pins[3] = atoi (((CCombo*) WProp_tempsys->GetChildByName ("combo4"))->GetText ());
}

