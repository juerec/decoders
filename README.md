##  cmdfsk

<h3>A small command line tool for decoding RTTY and NAVTEX signals</h3>

**Works with real signals or IQ signals**<br>
**Works with any sample rates and any shift**<br>
**Works on Linux systems**

As input source can serve a .wav file or a alsa sound device. 
This program is a user interface for the library **libFSKDecoder**.<br>
It reads command line options, set up the decoder, reads input data form file or sound device and writes
 decoded result to stdout.

Cmdfsk can work with one channel (real) signals and IQ signals. If ths signal one channel, 
it must be in a one channel (mono) file. Mono signals in a stereo file does not work.
If you use a SDR receiver, it is a good idea using it's IQ output.

For the sample rate, 8000 is a good value. Higher ones does increase the calculation power 
without any improvments on decoding result.


<h3>If have problems decoding your files:</h3>

* Check:       --center-freq  --shift  --decoder-type
* Play with:   --window-size  --fft-size
* Fine tune:   --window-func
* Inspect your file: Look for correct center freq.
A very good program therefor is sonic-visualiser, audacity does a good job too.
* For IQ signals, test -xchg-i-q. The samples order in the stereo file may be inverse.


<h3>cmdfsk v1.0.4 command line options:</h3>


<table>
<tr><td><b>Parameter</b></td><td><b>Description</b></td><td><b>Default value</b></td></tr>
    <tr><td>-h<br>--help</td><td>                  print help screen and exit</td><td></td></tr>
<tr><td>-v<br>--verbose</td><td>prints any details</td><td>off</td></tr>
<tr><td>-t<br>--decoder-type</td><td>decoder&nbsp;type:<br>rtty&nbsp;|&nbsp;rtty-iq&nbsp;|&nbsp;navtex&nbsp;|&nbsp;navtex-iq</td><td>no default, must be specifyed</td></tr>
<tr><td>-S<br>--shift</td><td>signal shift</td><td>navtex: 170  rtty: 450</td></tr>
<tr><td>-c<br>--center-freq</td><td>center frequency of signal<br>for IQ sources, it may be negative<br>use: --center-freq=nnn for negative values</td><td>1000</td></tr>
<tr><td>-w<br>--window-size</td><td>fft window size</td><td>navtex: samplerate/100<br>rtty: samplerate/50</td></tr>
<tr><td>-W<br>--window-func</td><td>fft window function<br>by number or by name</td><td>navtex: 4 - Blackman<br>rtty: 6 - Welch</td></tr>
<tr><td>-F<br>--fft-size</td><td>fft size >= fft window size</td><td>window size</td></tr>
<tr><td>-f<br>--file</td><td>wave file for input (.wav only)</td><td>must be set<br>omit if -D option is used</td></tr>
<tr><td>-D<br>--audio-device</td><td>alsa audio device for input</td><td>must be set<br>omit if -f option is used</td></tr>
<tr><td>-i<br>--incr-factor</td><td>factor for fading compensation</td><td>0.4</td></tr>
<tr><td>-d<br>--decr-factor</td><td>factor for fading compensation</td><td>0.9988</td></tr>
<tr><td>-B<br>--buffer-width</td><td>internal char buffer 8 ... 128</td><td>navtex: 48   rtty: 24</td></tr>
<tr><td>-R<br>--samplerate</td><td>sample rate, for audio input only</td><td>-D: 8000<br>-f: taken from file</td></tr>
<tr><td>-x<br>--xchg-i-q</td><td>exchange I - Q, for IQ sources only</td><td>unset</td></tr>
</table>



                           the window functions are:
                                0  Rectangular 
                                1  Bartlett
                                2  Hamming
                                3  Hann
                                4  Blackman
                                5  Blackman-Harris
                                6  Welch
                                7  Gaussian(a=2.5)
                                8  Gaussian(a=3.5)
                                9  Gaussian(a=4.5)
                               10  Flat-Top



## libFSKDecoder
 
<h3>A library for decoding RTTY and NAVTEX signals.</h3>

Compilation is tested under Linux only. How long it uses no systemspecific calls, it should work on other systems too.

Usage is simple. You does initialize, then deliver sound samples and receive the decoded text.
For a usage example see the program **cmdfsk**.

You can run multiple instances in parallel, but the init routines must be called one by one.
This is caused by fftw, not the library self.

For the meaning of the init parameters see the decription of cmdfsk