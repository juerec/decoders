# libFSKDecoder

<h3>A small library for decoding RTTY and NAVTEX signals</h3>

**Works with real signals or IQ signals**<br>
**Works with any sample rates and any shift**<br>
**Tested on Linux systems, but should work on other systems too**

This library is developed for decoding signals received by radios. So, a fading compensation ist implemented.
It works with a wide range of signal formes. It accepts real signals and IQ signals. When using real signals, it can be a have two channels, the specified channel will be used.<br>
The sample rate can by any. But consider that high sample rates increases the calculation power massive without any improvment of the result.<br>
The center frequency of the signal content can by any, for IQ signals negative too. But by the real computer math, place the signal content a little bit fare from Nyquist or zero.<br>
You can run multiple instances in parallel, but the init() call must be done one by one. 


#### Building

* install develovment package for fftw3
* type <b>make static</b> or <b>make shared</b>


#### Known problems

If you want to use multiple instances, the initialisation of **fftw library** is not threadsave. Do the initialisation one by one. The claculation can run in threads without problems.
