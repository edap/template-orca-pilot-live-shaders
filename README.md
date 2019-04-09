## Description

This is an openFrameworks app to live conding shaders receiving OSC signals from Orca.

## Instruction

In order to get this app running:

- Download [Orca](https://github.com/hundredrabbits/Orca/) and [Pilot](https://github.com/hundredrabbits/Pilot).
- Open Orca and Pilot. Pilot is a UDP synthetiser that receives UDP signals from ORCA and plays the sounds. From Orca, open the file `bin/data/pilot-example.orca`. You should ear the track as soon as you open it, if not, something is wrong with your installation.
- Download [ofxAutoReloadedShader](https://github.com/andreasmuller/ofxAutoReloadedShader) and put it in the `addons` folder of your openFrameworks installation. Open this project with the project generator. Be sure that orca is sending OSC messages on the default port (49162). As soon as the app run, the torus and the sphere should move accordingly to the beat.

## Live coding
- Once the app is running, you can edit the file `frag.glsl` with any editor and it will be automatically reloaded.

