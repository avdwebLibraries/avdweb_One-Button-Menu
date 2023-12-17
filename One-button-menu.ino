// NOTE: This code is not yet finished, it still needs to be made into a library.

bool menuRunning=0; 

void loop() // with debug 170us !!! Attention: is blocked during menu operations !!!
{ button.poll(); 
  if(longPress()) selectMenu(); // has its own button.poll()
  else dongleLoop(); 
}

void selectMenu()
{ menuRunning = 1; // stop the ledBar updates in the yamahaISR because this disturbes the ledbar
  if(debug) Serial << "\nselectMenu"; 
  ledBar.turnLedsOff();
  delay(2000); 
  int menuNo = countFlashes(5); // there are 5 menus
  switch(menuNo)
  { case 1: setSpecified_mAh(); break;
    case 2: showValueInTenths(specified_mAh/100); break; // show in Ah with one decimal 12500 -> 125 
    case 3: setFuelGaugeFull(); break; 
    case 4: setendOffChargeVoltage(); break;
    case 5: showValueInTenths(endOffChargeVoltage*10); break; // show in Volt with one decimal 41.5V -> 415 
  }
  delay(2000); 
  if(debug) Serial << "\nend";  
  menuRunning = 0;
}

long setValueInTenths() 
{ if(debug) Serial << "\nSet Value";

  ledBar.flash(5); // must be without delay
  int tens = countFlashes(9); // enter tens 12.5Ah, tens=1

  ledBar.flash(5);
  int units = countFlashes(9); // enter units 12.5Ah, units=2
  delay(1000);

  ledBar.flash(5); 
  int tenths = countFlashes(9); // enter decimal 12.5Ah decimal=5
  delay(1000);

  long value = tens*100 + units*10 + tenths; 
  if(debug) Serial << "\nValue = " << value; 
  return value;
}

void showValueInTenths(long value)  
{ if(debug) Serial << "\nShow value: " << value; 

  int tenths = value % 10; // operator % needs int or long

  value = value / 10;     
  int units = value % 10; 

  value = value/ 10;   
  int tens = value % 10; 
  
  delayFlashesDelay(1000, 5, 1000); // start
  for(int i=1; i <= tens; i++) delayFlashesDelay(0, 1, 500); // show tens 

  delayFlashesDelay(1000, 5, 1000); // start
  for(int i=1; i <= units; i++) delayFlashesDelay(0, 1, 500); // show units 

  delayFlashesDelay(1000, 5, 1000); // start
  for(int i=1; i <= tenths; i++) delayFlashesDelay(0, 1, 500); // show decimal 

  delayFlashesDelay(1000, 5, 1000); // start
}

void delayFlashesDelay(unsigned long delay1, int flashes, unsigned long delay2)
{ delay(delay1);
  ledBar.flash(flashes); 
  delay(delay2);
}

int countFlashes(int maxCount)
{ int i=0;
  bool pressed=0;
  if(clicked()) i=0;
  else 
  { for(i=1; i <= maxCount; i++) // first flash = 1
    { ledBar.flash(1); 
      if(clicked()) // stop with counting
      { pressed = 1;
        break;
      } 
    }
    if(!pressed) i=0; // for input zero: do nothing
  }
  if(debug) Serial << "\nno = " << i; 
  return i;
}

bool clicked()
{ bool returnVal=0;
  unsigned long start_ms = millis();
  while(millis() < start_ms + countFlashesInterval_ms)
  { button.poll();
    if(button.pushed()) returnVal = 1;
  }
  return returnVal;
}
