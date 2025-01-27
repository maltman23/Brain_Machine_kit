# Brain Machine Kit  
  
A hackable Sound &amp; Light Machine kit that is easy to make,  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;and comes pre-programmed with a really nice 14-minute meditation brainwave sequence.  
Not only that, but you get to hallucinate beautiful colors and patterns along the way...  
<img src="https://github.com/user-attachments/assets/61bc42e6-737f-4c7c-96b8-14911c37bd04" width="200" />  
Great for **learning to solder** -- even for total beginners.  
Great for **hacking on** to play with brainwave sequences.  
Great for **Meditating, Hallucinating, and Tripping Out!**  
<img src="https://github.com/user-attachments/assets/41ef8c39-80f0-4eb3-aa94-ef013615c867" width="300" />  
  
## Brief History of the Project  
  
The Brain Machine was originally a project I created by hacking the Adafruit MiniPOV3 kit, 
which was a really cool kit, but is no longer available.  
https://www.adafruit.com/product/20  
<img src="https://github.com/user-attachments/assets/b4c762a0-92d3-4f20-85f0-3dc0a3c2b484" width="200" />  
  
This was in 2007, before Arduino was popular, and I used the MiniPOV3 kit as a platform for many interesting projects.  
I used these intriguing projects to make it fun and easy to teach total beginners of all ages how to solder at my workshops all over the world.  

The Brain Machine was one of these projects -- and it was popular.  
  
I wrote up the project for MAKE Magazine, issue #10, in 2007:  
https://makezine.com/article/home/fun-games/the-brain-machine/  
<img src="https://github.com/user-attachments/assets/b38d4aa1-ff5a-47ad-9cc3-6d382dc82f20" width="200" />  
  
After giving countless well-attended soldering workshops where Brain Machines, with the MiniPOV3 hack, 
were very popular amongst participants, 
in 2010 I got together with **Limor Fried ("Ladyada")** (who founded Adafruit Industries), to create a Brain Machine kit.  
This kit was popular for quite a number of years.  Although it is no longer available, the cool webpage for it is still up:  
https://www.adafruit.com/product/287  
<img src="https://github.com/user-attachments/assets/f2614ee6-6099-4382-8f70-b328661d667c" width="200" />  
  
In 2011, after Arduino became quite popular for making it much easier to make just about anything with microcontrollers, 
**Chris Sparnicht ("Laughter On Water")**, created an Arduino version of the Brain Machine, based on my original project.  
https://github.com/LaughterOnWater/Arduino-Brain-Machine  
Many thanks, Chris!  
  
**This year (2025) I decided to create a new, much easier to make version of the kit I made with Limor.**  
**The result is the project in this GitHub repository, with all documentation given.**  
  
## More Background of the Brain Machine  
  
It has been known since people started recording brainwaves (with an EEG) in the 1920s, that if you blink lights at brainwave frequencies, when a person watches the blinking lights (with their eyes closed), for most people, their brain will synchronize to the external brainwave frequencies.  (This is called "entrainment".)  And people subjectively say that they feel similarly as if their brain were generating those brainwave frequency on their own (for instance: a state of meditation).  One interesting (and popular) side effect is that people hallucinate beautiful colors and patterns while enjoying the state they are in.  
  
People started creating devices to generate brainwave frequencies as early as the 1950s.  Here is one example -- Brion Gysin's Dreammachine:  
https://en.wikipedia.org/wiki/Dreamachine  
<img src="https://github.com/user-attachments/assets/5fa44f32-06ca-499b-8071-f1d35c588714" width="200" />  
  
Later people added sound, in the form of **"binaural beats"**.  
When there are two different pitches generated in a physical space, the difference in frequency between two pitches causes a physical "beat frequency" which everyone can hear.  
If instead, the two different pitches are presented separately to each ear, 
the difference in the two pitches is (oddly) perceived inside most peoples' brains 
as an imaginary beat frequency, which is called a "binaural beat".  
  
In the 1970s, when microcontrollers started to become affordable, people started making commercial devices that created a sequence of brainwave frequencies, presented with blinking lights (in front of glasses) and binaural beats (with headphones). These devices were called **"Sound & Light Machines".**  They were somewhat popular for some people, but also kind of expensive -- several hundred US dollars.  
  
In the early 1990s I created my own Sound & Light Machine, using an inexpensive microcontroller that was available. 
I have been meditating since I was 13 years old, and it became an important part of my well-being and my life. 
I was at a university where I volunteered as a subject for brainwave studies. 
While there I learned that you can record brainwaves and watch as people go into and out of a meditative state (and other states, such as sleep). 
I thought it would be cool to make a device that played those frequencies back 
through LEDs and binaural beats through headphones -- 
using a sequence of brainwave frequencies that were recorded from me going into and out of a nice 15-minute meditation.  The total cost was about US $5. 
I thought that others would like to be able to easily meditate by wearing and using my Sound & Light Machine 
-- especially if I promised them that they would hallucinate beautiful colors and patterns along the way. 
I considered manufacturing and selling them.  But, I was too busy at the time.  
  
Then, over 10 years later, after the first Maker Faire in 2006, after meeting Limor Fried, and seeing her MiniPOV3 kit, 
I thought it would be fun to use her kit as a platform to make it easy for total beginners to learn soldering, 
and to make several projects that I developed using the MiniPOV3 kit -- including what I now called the "Brain Machine".
  
As stated earlier, this turned into the Brain Machine kit that I created with Limor.  It also inspired Chris "Laughter On Water" to make his Arduino version.  And, many years later, I'm now making a new, Arduino-based Brain Machine kit.  
  
## The New Brain Machine Kit  
  
This new version of the Brain Machine kit functions exactly like the other versions.  
It comes pre-programmed with the same really nice meditation sequence.  
The main differences between the new Brain Machine kit and the old one are -- the new one:  
* Has a new PC Board that is much easier to solder together.  
* Uses a small Arduino Nano board, so it is much easier to re-program and hack on.  
* Can be easily re-programmed with other available Brainwave sequences.  
* The Sketch has even more comments in it than the old firmware, making it easy to modify, and great to learn from, if you like.  
* Has the ability to easily add Gama waves to a Brainwave sequence.  
* Has the ability to add alternate blinking LEDs (left/right), instead of both always blinking On/Off simultaneously.
* Runs on one AAA battery, so it is lightweight and more comfortable to wear and use.  
  
## How the Binaural Beats are Generated  

To create the binaural beat frequencies that we desire, 
we choose a central pitch, and generate pitches for each ear, as needed (using the Arduino Tone library), 
which are half of the desired difference higher and lower from the central pitch. 
If we choose a central pitch of 200 Hz, 
we can generate the following pitches for each ear 
to create the desired binaural beat for our brains to synch up with (entrain to):  
|Brainwave type   | R Ear  | L Ear  | Beat     |  
| :-------------: | ------ | ------ | -------: |
| Gama:           | 280.00 | 220.00 | 40.0 Hz  |
| Beta:           | 192.80 | 207.20 | 14.4 Hz  |
| Alpha:          | 194.45 | 205.55 | 11.1 Hz  |
| Theta:          | 197.00 | 203.00 |  6.0 Hz  |
| Delta:          | 198.90 | 201.10 |  2.2 Hz  |
  
## The Arduino Nano SuperMicro  
  
<img src="https://github.com/user-attachments/assets/f04cb68f-99c4-4ba4-b45e-f3d4c473631b" width="200" />  
  
For this new version of the Brain Machine kit, I use the Arduino Nano SuperMicro as the controller.  
It is a little bigger than a postage stamp, and is a complete Arduino Nano.  
It also comes with 3 NeoPixel LEDs connected to Arduino Pin 2 (so to use Arduino Pin 2 you would need to unsolder the first NeoPixel).  
It's a nice little board, available on Aliexpress:  
https://www.aliexpress.com/item/1005006468917096.html  
For people interested in playing with this nice little board, I created a Blink_Test program to blink all of the LEDs on this board, as well as document how to easily use it with the free Arduino software:  
https://github.com/maltman23/ArduinoNanoSuperMini  
  
## DISCLAIMER  
  
Light and Sound Machines, such as this one, can be fun for many of us, 
but may be seriously dangerous for those prone to siezures or who are photosensitive. 
When in doubt, seek medical attention. 
You assume all liability for any damage 
done to your equipment, or to your own health, or to those whom you expose to this 
technology. Any and all notes, sketches or associated files within this repository are presented as is, with no promise or gurarantee of any particular result(s).  
  
USE AT YOUR OWN RISK.  
  
## License  
  
Creative Commons Attribution-Sharealike 4.0 International  
CC BY-SA 4.0  
<img src="https://github.com/user-attachments/assets/f5e50525-b9bc-4b47-a2aa-e7795c1449eb" width="88" />  
https://creativecommons.org/licenses/by-sa/4.0/  
  
