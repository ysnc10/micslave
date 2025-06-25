# Call Center Comunication System Design: Slave microcontroller
- PICPIC16F628A microcontrollers managing multiple slave microphones programmed using MPLAP IDE.
- In this system: We have 4 radio channels, each channel can be used by at most one of the slave microphones at the same time, and the master directs the used microphone to the relevant radio channel.
- According to these restrictions, decision algorithms were developed in C slave microcontrollers by defining state variables for each channel and using the USART system of the PIC16F628A microcontroller.

