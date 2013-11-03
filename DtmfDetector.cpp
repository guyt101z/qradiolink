/** Author:       Plyashkevich Viatcheslav <plyashkevich@yandex.ru> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * All rights reserved. 
 */
// Modified by Adrian Musceac, October 2013

#include "DtmfDetector.hpp"

static inline INT32 MPY48SR(INT16 o16, INT32 o32)
{UINT32   Temp0;
 INT32    Temp1;
	Temp0 = (((UINT16)o32 * o16) + 0x4000) >> 15;
	Temp1 = (INT16)(o32 >> 16) * o16;
	return (Temp1 << 1) + Temp0;
}

static void goertzel_filter(INT16 Koeff0, INT16 Koeff1, const INT16 arraySamples[], INT32 *Magnitude0, INT32 *Magnitude1, UINT32 COUNT)
{INT32 Temp0, Temp1;
 UINT16 ii;
 INT32 Vk1_0 = 0, Vk2_0 = 0, Vk1_1 = 0, Vk2_1 = 0;
 
	for(ii = 0; ii < COUNT; ++ii)
	{
		Temp0 = MPY48SR(Koeff0, Vk1_0 << 1) - Vk2_0 + arraySamples[ii],
		Temp1 = MPY48SR(Koeff1, Vk1_1 << 1) - Vk2_1 + arraySamples[ii];
		Vk2_0 = Vk1_0,
		Vk2_1 = Vk1_1;
		Vk1_0 = Temp0,
		Vk1_1 = Temp1;
	}
	
	Vk1_0 >>= 10,
	Vk1_1 >>= 10,
	Vk2_0 >>= 10,
	Vk2_1 >>= 10;
	Temp0 = MPY48SR(Koeff0, Vk1_0 << 1),
	Temp1 = MPY48SR(Koeff1, Vk1_1 << 1);
	Temp0 = (INT16)Temp0 * (INT16)Vk2_0,
	Temp1 = (INT16)Temp1 * (INT16)Vk2_1;
	Temp0 = (INT16)Vk1_0 * (INT16)Vk1_0 + (INT16)Vk2_0 * (INT16)Vk2_0 - Temp0;
	Temp1 = (INT16)Vk1_1 * (INT16)Vk1_1 + (INT16)Vk2_1 * (INT16)Vk2_1 - Temp1;
	*Magnitude0 = Temp0,
	*Magnitude1 = Temp1;
	return;
}


// This is a GSM function, for concrete processors she may be replaced
// for same processor's optimized function (norm_l)
static inline INT16 norm_l(INT32 L_var1)
  {
   INT16 var_out;

   if (L_var1 == 0)
     {
      var_out = 0;
     }
   else
     {
      if (L_var1 == (INT32)0xffffffff)
 {
  var_out = 31;
 }
      else
 {
  if (L_var1 < 0)
    {
     L_var1 = ~L_var1;
    }

  for(var_out = 0;L_var1 < (INT32)0x40000000;var_out++)
    {
     L_var1 <<= 1;
    }
 }
     }

   return(var_out);
  }

const UINT32 DtmfDetectorInterface::NUMBER_OF_BUTTONS;
const unsigned DtmfDetector::COEFF_NUMBER;
//const INT16 DtmfDetector::CONSTANTS[COEFF_NUMBER] = {27860, 26745, 25529, 24216, 19747, 16384, 12773, 8967, 21319, 29769, 32706, 32210, 31778, 31226, -1009, -12772, -22811, -30555};
// 8000 K sampling, combined
//const INT16 DtmfDetector::CONSTANTS[COEFF_NUMBER] = {27906, 26802, 25597, 24295, 19747, 16529, 12773, 9166, 21319, 29769, 32706, 32210, 31778, 31226, -1009, -12772, -22811, -30555};
// paper way
const INT16 DtmfDetector::CONSTANTS[COEFF_NUMBER] = {27906, 26802, 25597, 24295, 19057, 16529, 12945, 9166, 21319, 29769, 32706, 32210, 31778, 31226, -1009, -12772, -22811, -30555};
//const INT16 DtmfDetector::CONSTANTS[COEFF_NUMBER] = {2799, 26605, 25997, 22295, 20057, 14529, 11945, 7166, 21319, 29769, 32706, 32210, 31778, 31226, -1009, -12772, -22811, -30555};
INT32 DtmfDetector::powerThreshold = 328;
INT32 DtmfDetector::dialTonesToOhersTones = 2;
INT32 DtmfDetector::dialTonesToOhersDialTones = 6;
const INT32 DtmfDetector::SAMPLES = 102;
//--------------------------------------------------------------------
DtmfDetector::DtmfDetector(INT32 frameSize_): frameSize(frameSize_)
{
 pArraySamples = new INT16 [frameSize + SAMPLES];
 internalArray = new INT16 [SAMPLES];
 frameCount = 0;
 prevDialButton = ' ';
 permissionFlag = 0;
}
//---------------------------------------------------------------------
DtmfDetector::~DtmfDetector()
{
 delete [] pArraySamples; 
 delete [] internalArray; 
}

void DtmfDetector::dtmfDetecting(INT16 input_array[])
{UINT32 ii;
 char temp_dial_button;
   
   for(ii=0; ii < frameSize; ii++)
    {
     pArraySamples[ii + frameCount] = input_array[ii];
    }
    
   frameCount += frameSize;

   UINT32 temp_index = 0;
   if(frameCount >= SAMPLES)
    { 
     while(frameCount >= SAMPLES)
      {
        temp_dial_button = DTMF_detection(&pArraySamples[temp_index]); 
         
        if(permissionFlag)
         {
          if(temp_dial_button != ' ')
           {
            dialButtons[indexForDialButtons++] = temp_dial_button;
            dialButtons[indexForDialButtons] = 0;
            if(indexForDialButtons >= 64)
                         indexForDialButtons = 0;
           }
          permissionFlag = 0;
         }
         
        if((temp_dial_button != ' ') && (prevDialButton == ' '))
         {
          permissionFlag = 1;
         }
         
        prevDialButton = temp_dial_button;
	if(temp_dial_button!=' ')
	{
	  std::cerr << temp_dial_button << std::endl;
	}
        temp_index += SAMPLES;
        frameCount -= SAMPLES;
      }
     
     for(ii=0; ii < frameCount; ii++)
      {
       pArraySamples[ii] = pArraySamples[ii + temp_index];
      }        
    }
    
}
//-----------------------------------------------------------------
char DtmfDetector::DTMF_detection(INT16 short_array_samples[])
{                   
   INT32 Dial=32, Sum;
   char return_value=' ';
   unsigned ii;
   Sum = 0;

   for(ii = 0; ii < SAMPLES; ii ++)
    {
     if(short_array_samples[ii] >= 0)
      Sum += short_array_samples[ii];
     else
      Sum -= short_array_samples[ii];
    }
   Sum /= SAMPLES;
   if(Sum < powerThreshold)
   {
     //std::cerr << "Sum smaller than power treshhold" << std::endl;
     return ' '; 
   }
      
   //Normalization
   for(ii = 0; ii < SAMPLES; ii++)
   {
      T[0] = static_cast<INT32>(short_array_samples[ii]);
       if(T[0] != 0)
       {
     	if(Dial > norm_l(T[0]))
      	{
       		Dial = norm_l(T[0]);
      	}
       }
   }
   
   Dial -= 16;
   

   for(ii = 0; ii < SAMPLES; ii++)
    {
     T[0] = short_array_samples[ii];
     internalArray[ii] = static_cast<INT16>(T[0] << Dial);
    }
    
  
   //Frequency detection
   goertzel_filter(CONSTANTS[0], CONSTANTS[1], internalArray, &T[0], &T[1], SAMPLES); 
   goertzel_filter(CONSTANTS[2], CONSTANTS[3], internalArray, &T[2], &T[3], SAMPLES); 
   goertzel_filter(CONSTANTS[4], CONSTANTS[5], internalArray, &T[4], &T[5], SAMPLES); 
   goertzel_filter(CONSTANTS[6], CONSTANTS[7], internalArray, &T[6], &T[7], SAMPLES); 
   goertzel_filter(CONSTANTS[8], CONSTANTS[9], internalArray, &T[8], &T[9], SAMPLES);
   goertzel_filter(CONSTANTS[10], CONSTANTS[11], internalArray, &T[10], &T[11], SAMPLES);
   goertzel_filter(CONSTANTS[12], CONSTANTS[13], internalArray, &T[12], &T[13], SAMPLES);
   goertzel_filter(CONSTANTS[14], CONSTANTS[15], internalArray, &T[14], &T[15], SAMPLES);
   goertzel_filter(CONSTANTS[16], CONSTANTS[17], internalArray, &T[16], &T[17], SAMPLES);

   //qDebug() << T[0] << ":" << T[1] << ":" << T[2] << ":" << T[3] << ":" << T[4];
   
   INT32 Row = 0;
   INT32 Temp = 0;
   //Find max row(low frequences) tones
   for(ii = 0; ii < 4; ii++)
    {
     if(Temp < T[ii]) 
      {
       Row = ii;
       Temp = T[ii];       
      }
    }
    
   INT32 Column = 4;
   Temp = 0;
   //Find max column(high frequences) tones
   for(ii = 4; ii < 8; ii++)
    {
     if(Temp < T[ii])
      {
       Column = ii;
       Temp = T[ii];
      }
    }
    
    Sum=0;
    //Find average value dial tones without max row and max column
    for(ii = 0; ii < 10; ii++)
     {
      Sum += T[ii];
     }     
   Sum -= T[Row];
   Sum -= T[Column];
   Sum >>= 3;
   
   if(!Sum)
     Sum = 1;

   //If relations max row and max column to average value
   //are less then threshold then return
   if(T[Row]/Sum < dialTonesToOhersDialTones) 
   {
     return ' ';
   }
   if(T[Column]/Sum < dialTonesToOhersDialTones) 
   {
     return ' ';
   }

   //If relations max colum to max row is large then 4 then return
   if(T[Row] < (T[Column] >> 2)) return ' ';
   //If relations max colum to max row is large then 4 then return
   if(T[Column] < ((T[Row] >> 1) - (T[Row] >> 3))) return ' ';    
   
   for(ii = 0; ii < COEFF_NUMBER; ii++)
            if(T[ii] == 0)
                T[ii] = 1;

   //If relations max row and max column to all other tones are less then
   //threshold then return

   for(ii = 10; ii < COEFF_NUMBER; ii ++)
    {
     if(T[Row]/T[ii] < dialTonesToOhersTones) 
     {
       std::cerr << "Row smaller than other tones" << std::endl;
       return ' ';
     }
     if(T[Column]/T[ii] < dialTonesToOhersTones) 
     {
       std::cerr << "Column smaller than other tones" << std::endl;
       return ' ';
     }
    }

    
   //If relations max row and max column tones to other dial tones are
   //less then threshold then return  

   for(ii = 0; ii < 10; ii ++)
    {
     if(T[ii] != T[Column])
      {
       if(T[ii] != T[Row])
        {
         if(T[Row]/T[ii] < dialTonesToOhersDialTones) 
         {
               //std::cerr << "Row smaller than other dial tones" << std::endl;
               return ' ';
         }
         if(Column != 4)
          {
              if(T[Column]/T[ii] < dialTonesToOhersDialTones) 
              {
                    //std::cerr << "Column smaller than other dial tones" << std::endl;
                    return ' ';
              }
          }
         else
          {
              if(T[Column]/T[ii] < (dialTonesToOhersDialTones/3)) 
              {
                    std::cerr << "Weird case scenario" << std::endl;
                    return ' ';
              }
          }
        }
      }
    }

   //We are choosed a push button
   switch (Row)
   {
    case 0: switch (Column){case 4: return_value='1'; break; // no worky
                            case 5: return_value='2'; break; // no worky
                            case 6: return_value='3'; break; // no worky
                            case 7: return_value='A'; break;}; break; // no worky -> 8th coeff
    case 1: switch (Column){case 4: return_value='4'; break; //worky
                            case 5: return_value='5'; break; // no worky -> worky 5th coeff
                            case 6: return_value='6'; break; // no worky -> 7th coeff
                            case 7: return_value='B'; break;}; break; //no worky -> 8th coeff
    case 2: switch (Column){case 4: return_value='7'; break; //worky
                            case 5: return_value='8'; break; // worky
                            case 6: return_value='9'; break; // sometimes worky -> worky 5th coeff
                            case 7: return_value='C'; break;}; break; // worky -> 3rd coeff
    case 3: switch (Column){case 4: return_value='*'; break; // worky
                            case 5: return_value='0'; break; // worky
                            case 6: return_value='#'; break; // worky
                            case 7: return_value='D'; break;} //worky
   }
   
  return return_value;
}







