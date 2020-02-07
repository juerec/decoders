/*******************************************/

  cmdfsk
 
/*******************************************/


cmdfsk is small program which can decode RTTY and NAVTEX signals. 

It works as command line tool on Linux systems. 
As input source can serve a .wav file or a alsa sound device. 
This program is a user interface for libFSKDecoder. It does reading
command line options, set up the decoder, reading input data and writing 
decodet results to stdout. The Decoding work is done by libFSKDecoder.

Cmdfsk can work with one channel (real) signals and IQ signals. If ths signal one channel, 
it must be in a one channel (mono) file. Mono signals in a stereo file does not work.
If you use a SDR receiver, it is a good idea using it's IQ output.

For the sample rate, 8000 is a good value. Higher ones does increase the calculation power 
without any improvments on decoding result.


If have problems decoding your files:

Check:       --center-freq  --shift  --decoder-type
Play with:   --window-size  --fft-size
Fine tune:   --window-func
Inspect your file: Look for correct center freq.
A very good program therefor is sonic-visualiser, audacity does a good job too.
For IQ signals, test -xchg-i-q. The samples order in the stereo file can be inverse.


cmdfsk v1.0.4 command line options:
                                                                      default values
-h --help                  print this screen
-v --verbose               prints any details                         off
-t --decoder-type          decoder type;                              no default, must be specifyed
                           rtty | rtty-iq | navtex | navtex-iq
-S --shift                 signal shift                               navtex: 170  rtty: 450
-c --center-freq           center frequency                           1000
                           --center-freq=nnn for negative values      for IQ sources, if may be negative
-w --window-size           fft window size                            navtex: samplerate/100  rtty: samplerate/50
-W --window-func           fft window function                        navtex: 4               rtty: 6
                           by number or by name                               Blackman              Welch
-F --fft-size              fft size >= fft window size                window size
-f --file                  wave file for input (.wav only)            must be set  or omit if -D option is used
-D --audio-device          alsa audio device for input                must be set  or omit if -f option is used
-i --incr-factor           factor for fading compensation             0.4
-d --decr-factor           factor for fading compensation             0.9988
-B --buffer-width          internal char buffer 8 ... 128             navtex: 48   rtty: 24
-R --samplerate            sample rate, for audio input only          -D: 8000     -f: taken from file
-x --xchg-i-q              exchange I - Q, for IQ sources only        unset

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


/*******************************************/

  libFSKDecoder
 
/*******************************************/


The library libFSKDecoder is a library which can decode RTTY and NAVTEX signals.

Usage is simple. You does initialize, then deliver sound samples and receive the decoded text.
For a usage example see the program cmdfsk.

You can run multiple instances in parallel, but the init routines must be called one by one.
This is caused by fftw, not the library self.

For the meaning of the init parameters see the decription of cmdfsk.
