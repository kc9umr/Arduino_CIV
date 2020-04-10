# Arduino_CIV
Icom CI-V library / header creation for Arduino work

This is intended to simplify development of accessories for Icom radios, utilizing the CI-V command protocol.  

The library stems from a need identified during development of a specific accessory, which will be furhter developed upon completion of this library

SW_3 added quite a bit of functionality added, I got read and write functionality working, and to be somewhat generic

SW_4 was a big jump - I got radio auto-detect working.  This assumes the radio's DEFAULT ID is used - if you change the radio's ID, we'd need to change that after the autodetect routine works.  That's a future effort - unless you're one of those SO2R crazies who has identical radios, there's not much reason to change the radio's default ID for THIS function.

