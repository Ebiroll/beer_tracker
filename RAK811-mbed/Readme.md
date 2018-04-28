# Instrctions for setting up mbed,

https://nicbkw.com/stmicro-nucleo-disco-l072cz-lrwan1-quickstart-mbed-os/

https://os.mbed.com/questions/81016/LoRaWAN-support-for-MTB_RAK811-target-in/

## Make sure you have the compiler.
which arm-none-eabi-gcc

    git clone https://github.com/ARMmbed/mbed-cli

    cd mbed-cli

    python setup.py install

    which arm-none-eabi-gcc

    mbed config -G ARM_PATH "/usr/local/bin"

## Pull the example code

   git clone https://github.com/ARMmbed/mbed-os-example-lorawan.git

   cd mbed-os-example-lorawan/

   mbed deploy

   mbed new .

## Configure for your node
edit mbedapp.json to configure lora.device-eui, lora.application-eui, lora.application-key

##. compile and send to evaluation board (assumes there is only one ST-Link/Nucleo/DISCO board connected)
mbed compile -m MTB_RAK811 -t GCC_ARM --flash



