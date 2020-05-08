# cmdfax

<h3>A small command line tool for decoding hf weather faxes</h3>

**Works with real signals or IQ signals**<br>
**Works with any sample rates and any shift**<br>
**Runs on Linux systems, PC or SBC's like Raspberry Pi or ODROID**

**cmdfax** does decode hf weather faxes how German DWD it broadcasts.<br>
Internal, the processing is done on a IQ signal with a sample rate of 8000. All other input format is converted in this format. It means, if you provide a real/mono signal, it will transformed in a IQ signal. If your signal in a other sample rate, it will resampled to 8000.

The decoding result is written to a simple .bmp file. If the input file contains more than one fax, all faxes are appended to the same file.

**cmdfax** was started with ideas and parts from **hamfax** project. 

#### Command line options

<table>
<tr><td><b>Parameter</b></td><td><b>Description</b></td><td><b>Default value</b></td></tr>
<tr><td>-h<br>--help</td><td>prints a help</td><td></td></tr>
<tr><td>-f<br>--file</td><td>input file</td><td>fax.wav</td></tr>
<tr><td>-o<br>--outfile</td><td>output file</td><td>&lt;input&gt;.bmp</td></tr>
<tr><td>-l<br>--turn-left</td><td>turn left image</td><td>
<tr><td>-r<br>--turn-right</td><td>turn right image</td><td>
<tr><td>-O<br>--format</td><td>1|8|32 bit image format</td><td>8</td></tr>
<tr><td>-c<br>--center-freq</td><td>Center frequency</td><td>1500</td></tr>
<tr><td>-s<br>--shift</td><td>Shift (fwhite - fblack)</td><td>850</td></tr>
<tr><td>-F<br>--filter</td><td>l0|l1|l2|l3|l4</td><td>l2</td></tr>
<tr><td>-t<br>--start-threshold</td><td>1|2|3</td><td>2</td></tr>
<tr><td>-u<br>--stop-thershold</td><td>1|2|3</td><td>2</td></tr>
<tr><td>-S<br>--slant-corr</td><td>-100...100</td><td>0</td></tr>
<tr><td>-L<br>--limits</td><td>comma sep. max length of fax in sec.<br>0 for unlimited</td><td>0</td></tr>
<tr><td>-v<br>--verbose</td><td>print more informations 0|1|2|3</td><td>1</td></tr>
</table>

#### Known problems

Faxes from GB's Northwood uses for the end mark a series of increasing tons, not a fix ton. The end detection does not work for it. So you have to use the time limit.


### Building

**cmdfax** uses **libsamplerate** and **kissfft**
* Install the development package for **libsamplerate**
* From **kissfft** project (https://github.com/mborgerding/kissfft), copy the files kiss_fft.c, kiss_fft.h, kiss_fftr.c and kiss_fftr.h in your build directory
* type **make**

