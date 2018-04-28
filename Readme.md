The purpose of this project is to track location of lora equiped beer drinkers

# How this works,

You must set up a GPS enabled Lora-wan device that sends data to the things network.

Data is collected and stored on the things network, then the webpage on firebase will use the
appkey to retreive the data.

https://www.thethingsnetwork.org/docs/applications/storage/api.html



## Setting up your application in the ttn console

#  PayloadFormats tab.

To get the data collected by the things network you must enter the decoder,js in the things console.


# Integrations tab

You must also enable Data Storage in the things network console.
When you do this you get an interface for fetching data direcly from the things network.

https://esp32_heltec.data.thethingsnetwork.org/


The application on firebase uses these url to fetch the data.




# Interesting links



https://os.mbed.com/teams/Semtech/code/LoRaWAN-NAMote72-Application-Demo/

https://www.thethingsnetwork.org/forum/t/rak811-tracker-board/11006/234



# Wifi localisation

https://ico.org.uk/media/1560691/wi-fi-location-analytics-guidance.pdf

https://www.thethingsnetwork.org/forum/t/big-esp32-sx127x-topic-part-2/11973/267

https://www.cbsnews.com/news/uk-bars-trash-cans-from-tracking-people-with-wi-fi/


To develop,
 ```
All files to deploy should be placed in public
 ```

To deploy
 ```
npm -g install firebase-tools

firebase login
firebase deploy
 ```
