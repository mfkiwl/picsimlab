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
#include"part_VCD_Dump.h"

#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif

const char markers[] = "!$%&[()]";


#ifdef DATA_DEBUG
const char markersd[] = "*#+-<>?^";
char data_debug_name[8][20] = {"d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8"};
unsigned char data_debug[8] = {0, 0, 0, 0, 0, 0, 0, 0};
#endif  

/* outputs */
enum
{
 O_P1, O_P2, O_P3, O_P4, O_P5, O_P6, O_P7, O_P8, O_L1, O_L2, O_L3, O_L4, O_L5, O_L6, O_L7, O_L8, O_NAME, O_REC
};

/*inputs*/
enum
{
 I_START, I_VIEW
};

cpart_VCD_Dump::cpart_VCD_Dump(unsigned x, unsigned y)
{
 X = x;
 Y = y;
 ReadMaps ();

 lxImage image;
 image.LoadFile (Window1.GetSharePath () + lxT ("parts/") + GetPictureFileName ());

 Bitmap = new lxBitmap (image, &Window5);
 image.Destroy ();
 canvas.Create (Window5.GetWWidget (), Bitmap);

 input_pins[0] = 0;
 input_pins[1] = 0;
 input_pins[2] = 0;
 input_pins[3] = 0;
 input_pins[4] = 0;
 input_pins[5] = 0;
 input_pins[6] = 0;
 input_pins[7] = 0;

 old_value_pins[0] = 2;
 old_value_pins[1] = 2;
 old_value_pins[2] = 2;
 old_value_pins[3] = 2;
 old_value_pins[4] = 2;
 old_value_pins[5] = 2;
 old_value_pins[6] = 2;
 old_value_pins[7] = 2;

#ifdef DUMP_DIR
 old_value_dir[0] = 2;
 old_value_dir[1] = 2;
 old_value_dir[2] = 2;
 old_value_dir[3] = 2;
 old_value_dir[4] = 2;
 old_value_dir[5] = 2;
 old_value_dir[6] = 2;
 old_value_dir[7] = 2;
#endif
#ifdef DATA_DEBUG 
 old_data_debug[0] = 0xFF;
 old_data_debug[1] = 0xFF;
 old_data_debug[2] = 0xFF;
 old_data_debug[3] = 0xFF;
 old_data_debug[4] = 0xFF;
 old_data_debug[5] = 0xFF;
 old_data_debug[6] = 0xFF;
 old_data_debug[7] = 0xFF;
#endif

 snprintf (f_vcd_name, 200, "%s/picsimlab-XXXXXX", (const char *) lxGetTempDir ("PICSimLab").c_str ());
 close (mkstemp (f_vcd_name));
 unlink (f_vcd_name);

 strncat (f_vcd_name, ".vcd", 200);

 f_vcd = fopen (f_vcd_name, "w");
 fclose (f_vcd);
 f_vcd = NULL;

 rec = 0;
 vcd_count = 0;

}

cpart_VCD_Dump::~cpart_VCD_Dump(void)
{
 delete Bitmap;
 canvas.Destroy ();

 if (f_vcd)
  {
   fclose (f_vcd);
   f_vcd = NULL;
  }
 unlink (f_vcd_name);
}

void
cpart_VCD_Dump::Draw(void)
{

 int i;

 const picpin * ppins = Window5.GetPinsValues ();

 canvas.Init ();

 lxFont font (9, lxFONTFAMILY_TELETYPE, lxFONTSTYLE_NORMAL, lxFONTWEIGHT_BOLD);
 canvas.SetFont (font);

 for (i = 0; i < outputc; i++)
  {

   switch (output[i].id)
    {
    case O_P1:
    case O_P2:
    case O_P3:
    case O_P4:
    case O_P5:
    case O_P6:
    case O_P7:
    case O_P8:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     if (input_pins[output[i].id - O_P1] == 0)
      canvas.Text ("NC", output[i].x1, output[i].y1);
     else
      canvas.Text (Window5.GetPinName (input_pins[output[i].id - O_P1]), output[i].x1, output[i].y1);
     break;
    case O_NAME:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     canvas.Text (f_vcd_name, output[i].x1, output[i].y1);
     break;
    case O_L1:
    case O_L2:
    case O_L3:
    case O_L4:
    case O_L5:
    case O_L6:
    case O_L7:
    case O_L8:
     if (input_pins[output[i].id - O_L1] > 0)
      {
       canvas.SetColor (ppins[input_pins[output[i].id - O_L1] - 1].oavalue, 0, 0);
      }
     else
      {
       canvas.SetColor (30, 0, 0);
      }
     canvas.Circle (1, output[i].x1, output[i].y1, output[i].r);
     break;
    case O_REC:
     if (rec > 0)
      {
       canvas.SetColor (0, 255, 0);
      }
     else
      {
       canvas.SetColor (255, 0, 0);
      }
     canvas.Circle (1, output[i].x1, output[i].y1, output[i].r);
     break;
    }


  };


 canvas.End ();

}

unsigned short
cpart_VCD_Dump::get_in_id(char * name)
{
 if (strcmp (name, "START") == 0)return I_START;
 if (strcmp (name, "VIEW") == 0)return I_VIEW;

 printf ("Erro input '%s' don't have a valid id! \n", name);
 return -1;
};

unsigned short
cpart_VCD_Dump::get_out_id(char * name)
{

 if (strcmp (name, "P1") == 0)return O_P1;
 if (strcmp (name, "P2") == 0)return O_P2;
 if (strcmp (name, "P3") == 0)return O_P3;
 if (strcmp (name, "P4") == 0)return O_P4;
 if (strcmp (name, "P5") == 0)return O_P5;
 if (strcmp (name, "P6") == 0)return O_P6;
 if (strcmp (name, "P7") == 0)return O_P7;
 if (strcmp (name, "P8") == 0)return O_P8;

 if (strcmp (name, "L1") == 0)return O_L1;
 if (strcmp (name, "L2") == 0)return O_L2;
 if (strcmp (name, "L3") == 0)return O_L3;
 if (strcmp (name, "L4") == 0)return O_L4;
 if (strcmp (name, "L5") == 0)return O_L5;
 if (strcmp (name, "L6") == 0)return O_L6;
 if (strcmp (name, "L7") == 0)return O_L7;
 if (strcmp (name, "L8") == 0)return O_L8;

 if (strcmp (name, "NAME") == 0)return O_NAME;
 if (strcmp (name, "REC") == 0)return O_REC;

 printf ("Erro output '%s' don't have a valid id! \n", name);
 return 1;
};

lxString
cpart_VCD_Dump::WritePreferences(void)
{
 char prefs[256];

 sprintf (prefs, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", input_pins[0], input_pins[1], input_pins[2], input_pins[3], input_pins[4], input_pins[5], input_pins[6], input_pins[7]);

 return prefs;
}

void
cpart_VCD_Dump::ReadPreferences(lxString value)
{
 sscanf (value.c_str (), "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", &input_pins[0], &input_pins[1], &input_pins[2], &input_pins[3], &input_pins[4], &input_pins[5], &input_pins[6], &input_pins[7]);
}


void
cpart_VCD_Dump::ConfigurePropertiesWindow(CPWindow * WProp)
{
 lxString Items = Window5.GetPinsNames ();
 lxString spin;

 ((CCombo*) WProp->GetChildByName ("combo1"))->SetItems (Items);
 if (input_pins[0] == 0)
  ((CCombo*) WProp->GetChildByName ("combo1"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[0]);
   ((CCombo*) WProp->GetChildByName ("combo1"))->SetText (itoa (input_pins[0]) + "  " + spin);
  }

 ((CCombo*) WProp->GetChildByName ("combo2"))->SetItems (Items);
 if (input_pins[1] == 0)
  ((CCombo*) WProp->GetChildByName ("combo2"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[1]);
   ((CCombo*) WProp->GetChildByName ("combo2"))->SetText (itoa (input_pins[1]) + "  " + spin);
  }

 ((CCombo*) WProp->GetChildByName ("combo3"))->SetItems (Items);
 if (input_pins[2] == 0)
  ((CCombo*) WProp->GetChildByName ("combo3"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[2]);
   ((CCombo*) WProp->GetChildByName ("combo3"))->SetText (itoa (input_pins[2]) + "  " + spin);
  }

 ((CCombo*) WProp->GetChildByName ("combo4"))->SetItems (Items);
 if (input_pins[3] == 0)
  ((CCombo*) WProp->GetChildByName ("combo4"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[3]);
   ((CCombo*) WProp->GetChildByName ("combo4"))->SetText (itoa (input_pins[3]) + "  " + spin);
  }

 ((CCombo*) WProp->GetChildByName ("combo5"))->SetItems (Items);
 if (input_pins[4] == 0)
  ((CCombo*) WProp->GetChildByName ("combo5"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[4]);
   ((CCombo*) WProp->GetChildByName ("combo5"))->SetText (itoa (input_pins[4]) + "  " + spin);
  }

 ((CCombo*) WProp->GetChildByName ("combo6"))->SetItems (Items);
 if (input_pins[5] == 0)
  ((CCombo*) WProp->GetChildByName ("combo6"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[5]);
   ((CCombo*) WProp->GetChildByName ("combo6"))->SetText (itoa (input_pins[5]) + "  " + spin);
  }

 ((CCombo*) WProp->GetChildByName ("combo7"))->SetItems (Items);
 if (input_pins[6] == 0)
  ((CCombo*) WProp->GetChildByName ("combo7"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[6]);
   ((CCombo*) WProp->GetChildByName ("combo7"))->SetText (itoa (input_pins[6]) + "  " + spin);
  }

 ((CCombo*) WProp->GetChildByName ("combo8"))->SetItems (Items);
 if (input_pins[7] == 0)
  ((CCombo*) WProp->GetChildByName ("combo8"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[7]);
   ((CCombo*) WProp->GetChildByName ("combo8"))->SetText (itoa (input_pins[7]) + "  " + spin);
  }


 ((CButton*) WProp->GetChildByName ("button1"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CPWindow5::PropButtonRelease;
 ((CButton*) WProp->GetChildByName ("button1"))->SetTag (1);

 ((CButton*) WProp->GetChildByName ("button2"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CPWindow5::PropButtonRelease;
}

void
cpart_VCD_Dump::ReadPropertiesWindow(CPWindow * WProp)
{
 input_pins[0] = atoi (((CCombo*) WProp->GetChildByName ("combo1"))->GetText ());
 input_pins[1] = atoi (((CCombo*) WProp->GetChildByName ("combo2"))->GetText ());
 input_pins[2] = atoi (((CCombo*) WProp->GetChildByName ("combo3"))->GetText ());
 input_pins[3] = atoi (((CCombo*) WProp->GetChildByName ("combo4"))->GetText ());
 input_pins[4] = atoi (((CCombo*) WProp->GetChildByName ("combo5"))->GetText ());
 input_pins[5] = atoi (((CCombo*) WProp->GetChildByName ("combo6"))->GetText ());
 input_pins[6] = atoi (((CCombo*) WProp->GetChildByName ("combo7"))->GetText ());
 input_pins[7] = atoi (((CCombo*) WProp->GetChildByName ("combo8"))->GetText ());
}

void
cpart_VCD_Dump::Process(void)
{


 if (rec)
  {
   const picpin * ppins = Window5.GetPinsValues ();


   vcd_count++;
   int tprint = 0;

   for (int i = 0; i < 8; i++)
    {
     if (input_pins[i] != 0)
      {
#ifndef DUMP_DIR       
       if (ppins[input_pins[i] - 1].value != old_value_pins[i])
        {
         if (!tprint)
          {
           tprint = 1;
           fprintf (f_vcd, "#%li\n", vcd_count);
          }
         old_value_pins[i] = ppins[input_pins[i] - 1].value;
         fprintf (f_vcd, "%i%c\n", old_value_pins[i], markers[i]);
         fflush (f_vcd);
        }
#else
       if ((ppins[input_pins[i] - 1].value != old_value_pins[i]) ||
           (ppins[input_pins[i] - 1].dir != old_value_dir[i]))
        {
         if (!tprint)
          {
           tprint = 1;
           fprintf (f_vcd, "#%li\n", vcd_count);
          }
         old_value_pins[i] = ppins[input_pins[i] - 1].value;
         old_value_dir[i] = ppins[input_pins[i] - 1].dir;
         fprintf (f_vcd, "b%i%i %c\n", old_value_pins[i], old_value_dir[i], markers[i]);
         fflush (f_vcd);
        }
#endif       

      }
#ifdef DATA_DEBUG       
     if (data_debug[i] != old_data_debug[i])
      {
       if (!tprint)
        {
         tprint = 1;
         fprintf (f_vcd, "#%li\n", vcd_count);
        }
       old_data_debug[i] = data_debug[i];
       fprintf (f_vcd, "b%i%i%i%i%i%i%i%i %c\n",
                (data_debug[i]&0x80) > 0,
                (data_debug[i]&0x40) > 0,
                (data_debug[i]&0x20) > 0,
                (data_debug[i]&0x10) > 0,
                (data_debug[i]&0x08) > 0,
                (data_debug[i]&0x04) > 0,
                (data_debug[i]&0x02) > 0,
                (data_debug[i]&0x01) > 0,
                markersd[i]);
       fflush (f_vcd);
      }

#endif       
    }
  }
}

void
cpart_VCD_Dump::EvMouseButtonPress(uint button, uint x, uint y, uint state)
{
 int i;

 for (i = 0; i < inputc; i++)
  {
   if (((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&(input[i].y2 >= y)))
    {

     switch (input[i].id)
      {
      case I_START:
       if (!rec)
        {
         unsigned int tscale= 1.0e9/Window1.GetBoard ()->MGetInstClock (); //ns step
         
         f_vcd = fopen (f_vcd_name, "w");
         vcd_count = 0;
         
         

#ifndef DUMP_DIR         
         fprintf (f_vcd,
                  "$version Generated by PICSimLab $end\n"
                  "$timescale %ins $end\n"
                  "$scope module logic $end\n"
                  "$var wire 1 !  1-%s $end\n"
                  "$var wire 1 $  2-%s $end\n"
                  "$var wire 1 %% 3-%s $end\n"
                  "$var wire 1 &  4-%s $end\n"
                  "$var wire 1 [  5-%s $end\n"
                  "$var wire 1 (  6-%s $end\n"
                  "$var wire 1 )  7-%s $end\n"
                  "$var wire 1 ]  8-%s $end\n"
#ifdef DATA_DEBUG  
                  "$var wire 8 *  %s $end\n"
                  "$var wire 8 #  %s $end\n"
                  "$var wire 8 +  %s $end\n"
                  "$var wire 8 -  %s $end\n"
                  "$var wire 8 <  %s $end\n"
                  "$var wire 8 >  %s $end\n"
                  "$var wire 8 ?  %s $end\n"
                  "$var wire 8 ^  %s $end\n"
#endif             
                  "$upscope $end\n"
                  "$enddefinitions $end\n"
                  "$dumpvars\n"
                  "x!\n"
                  "x$\n"
                  "x%%\n"
                  "x&\n"
                  "x[\n"
                  "x(\n"
                  "x)\n"
                  "x[\n"
#ifdef DATA_DEBUG  
                  "bxxxxxxxx *\n"
                  "bxxxxxxxx #\n"
                  "bxxxxxxxx +\n"
                  "bxxxxxxxx -\n"
                  "bxxxxxxxx <\n"
                  "bxxxxxxxx >\n"
                  "bxxxxxxxx ?\n"
                  "bxxxxxxxx ^\n"
#endif             
                  "$end\n",tscale,
                  (const char *) Window5.GetPinName (input_pins[0]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[1]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[2]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[3]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[4]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[5]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[6]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[7]).c_str ()
#ifdef DATA_DEBUG  
                  ,data_debug_name[0],
                  data_debug_name[1],
                  data_debug_name[2],
                  data_debug_name[3],
                  data_debug_name[4],
                  data_debug_name[5],
                  data_debug_name[6],
                  data_debug_name[7]
#endif                      
                      );
#else
         fprintf (f_vcd,
                  "$version Generated by PICSimLab $end\n"
                  "$timescale 10ns $end\n"
                  "$scope module logic $end\n"
                  "$var wire 2 !  1-%s $end\n"
                  "$var wire 2 $  2-%s $end\n"
                  "$var wire 2 %% 3-%s $end\n"
                  "$var wire 2 &  4-%s $end\n"
                  "$var wire 2 [  5-%s $end\n"
                  "$var wire 2 (  6-%s $end\n"
                  "$var wire 2 )  7-%s $end\n"
                  "$var wire 2 ]  8-%s $end\n"
#ifdef DATA_DEBUG  
                  "$var wire 8 *  %s $end\n"
                  "$var wire 8 #  %s $end\n"
                  "$var wire 8 +  %s $end\n"
                  "$var wire 8 -  %s $end\n"
                  "$var wire 8 <  %s $end\n"
                  "$var wire 8 >  %s $end\n"
                  "$var wire 8 ?  %s $end\n"
                  "$var wire 8 ^  %s $end\n"
#endif                    
                  "$upscope $end\n"
                  "$enddefinitions $end\n"
                  "$dumpvars\n"
                  "bxx !\n"
                  "bxx $\n"
                  "bxx %%\n"
                  "bxx &\n"
                  "bxx [\n"
                  "bxx (\n"
                  "bxx )\n"
                  "bxx [\n"
#ifdef DATA_DEBUG  
                  "bxxxxxxxx *\n"
                  "bxxxxxxxx #\n"
                  "bxxxxxxxx +\n"
                  "bxxxxxxxx -\n"
                  "bxxxxxxxx <\n"
                  "bxxxxxxxx >\n"
                  "bxxxxxxxx ?\n"
                  "bxxxxxxxx ^\n"
#endif                 
                  "$end\n",
                  (const char *) Window5.GetPinName (input_pins[0]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[1]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[2]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[3]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[4]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[5]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[6]).c_str (),
                  (const char *) Window5.GetPinName (input_pins[7]).c_str ()
             #ifdef DATA_DEBUG  
                  ,data_debug_name[0],
                  data_debug_name[1],
                  data_debug_name[2],
                  data_debug_name[3],
                  data_debug_name[4],
                  data_debug_name[5],
                  data_debug_name[6],
                  data_debug_name[7]
#endif 
             );
#endif         
         rec = 1;
        }
       else
        {
         rec = 0;
         if (f_vcd)
          {
           fclose (f_vcd);
           f_vcd = NULL;
          }
        }
       break;
      case I_VIEW:
#ifdef __EMSCRIPTEN__
   EM_ASM_({
	   var filename=UTF8TolxString($0);
           var buf = FS.readFile(filename);
           var blob = new Blob([buf],  {"type" : "application/octet-stream" });
           var text = URL.createObjectURL(blob);

	   var element = document.createElement('a');
           element.setAttribute('href', text);
           element.setAttribute('download', filename);

           element.style.display = 'none';
           document.body.appendChild(element);

           element.click();

           document.body.removeChild(element);
           URL.revokeObjectURL(text);
	  },f_vcd_name);
#else
#ifdef _WIN_
       lxExecute (Window1.GetSharePath()+ lxT ("/../tools/gtkwave/bin/gtkwave.exe ") + f_vcd_name);
#else

       lxExecute (lxString ("gtkwave ") + f_vcd_name, lxEXEC_MAKE_GROUP_LEADER);
#endif
#endif
       break;
      }
    }
  }
}

part_init("VCD Dump", cpart_VCD_Dump);

