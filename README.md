# Signal-Generator-ARM-stm32
display some of  Trigonometric functions withb ARM STM32

here are them modules we use : 

1-DAC module(DAC_8)

2-Oscilloscope

3-ARM STM32 (STM32F401RE)*2

4-Potentiometer(POT-HG)

5-LCD Dispaly (LM044L)

6-KeyPad (KEYPAD-PHONE)

You can See the schematic toward Proteus.

we are using **Proteus** and **KEILI** to run this program

Since we use Two STM32 modules , The whole program consists of Two Part,

**1_ User Interface with Keypad and Sending Data**

**2_ Receiving Data and Generate Signals**

In the first part , we config our modules and get the Frequency and duration Time and name of one Trigonometric function (sine , cos , ... ) from user. And then by using USART , 
Send them as packets to the other STM32 module.

In the Sec part,By receiving the packets we just sent from the first ARM module , we generate the given signal using specefic linear equation with given conditions (frequency , duration time).

(after running and compiling(F7) two specefic KEIL programs and extracting the **.hex** file out of it , we should give the **.hex** files to two STM32 modules in Proteus.)
