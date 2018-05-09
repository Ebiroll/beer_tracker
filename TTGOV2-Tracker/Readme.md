
# Not sure what do do with this yet.
===========

Double check pinouts with this.
https://github.com/LilyGO/TTGO-LORA32-V2.0/blob/master/images/image1.jpg


This code is base on the PAX counter and reports number of persons in the area.

https://github.com/cyberman54/ESP32-Paxcounter




https://github.com/LilyGO/TTGO-LORA32-V2.0

# Initialisation of SD-card

void setup(){
  Serial.begin(115200);
  SPI.begin(14, 2, 15);
  if(!SD.begin(13)){
    Serial.println("Card Mount Failed");
  }
//
// other setup stuff
//
}


http://elm-chan.org/docs/mmc/mmc_e.html

… from my reading here SD cards can be used in 3 modes, one of which is SPI and the other two are SD specific. on AVRs the SPI mode is normally used because there is hardware built in for it, so it is faster that software emulating either of the other two modes. on the ESP it looks like we have the MMC hardware to use the other two modes natively.
I believe those other two modes (particularly the 4 bit mode) should be faster than SPI, however for you it seems that this didn’t work.
when using the SD_MMC lib the 4 bit is the default unless the board definition says only 1 bit mode is available, so the default didn’t work, my suggestion of using begin("/sdcard", true); should have activated 1 bit mode, which also didn’t work. so my last suggestion using the normal SD lib is using SPI, but configuring the pins of the SPI interface onto the pins normally used by MMC.

