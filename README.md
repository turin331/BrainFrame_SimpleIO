=============================
BrainFrame_SimpleIO
=============================

![Erasmus Brain Project](http://erasmusbrainproject.com/images/lampmonosmall.png)

This is the source code and binaries of the Maxeler-based Demo implementation of the Inferior Olive Model develpoed  within
The Neuroscience department of the Erasmus Medical Center. 

## Description

The BrainFrame demo demonstrates the simulation of an Inferior Olive network model. A crucial component of the Olivorcerebellar circuit imperative for the control and synchronization of motor functions. The model is based on a highly complex, bio-physically meaningful and computationally demanding neuron model (Hodgkin-Huxley) and also supports Gap Junction connectivity between neurons. It supports maximally a netwrok of 7680 neurons and has a real-time capability (simulating the network with real-time speed) of about 400 neurons. 

## Content

The repo root directory contains the following items:

- APP
- DOCS
- LICENCE.txt

### APP

Directory containing project sources.

### DOCS

Documentation of the project.
  
### LICENSE.txt

License of the project.

## Information to compile

Ensure the environment variables below are correctly set:
  * `MAXELEROSDIR`
  * `MAXCOMPILERDIR`

To compile the application, run within the CpuCode folder:

    make RUNRULE="DFE"

Currently the demo is run and tested on a Maia DFE card.

To excute the application run :

    make run RUNRULE="DFE"


BrainFrame Demo on [AppGallery](http://appgallery.maxeler.com/) 
# BrainFrame_SimpleIO
# BrainFrame_SimpleIO
# BrainFrame_SimpleIO