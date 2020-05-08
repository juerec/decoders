progname=cmdfax


CCC=g++

ODIR=obj

OBJ= $(ODIR)/main.o \
     $(ODIR)/WaveFile.o \
     $(ODIR)/kiss_fft.o \
     $(ODIR)/kiss_fftr.o \
     $(ODIR)/PhaseTest.o \
     $(ODIR)/PhaseTestComplex.o \
     $(ODIR)/PhaseTestReal.o \
     $(ODIR)/Options.o \
     $(ODIR)/CmdFax.o \
     $(ODIR)/FaxDemodulator.o \
     $(ODIR)/FIRFilter.o \
     $(ODIR)/FaxImage.o \
     $(ODIR)/BMP.o \
     $(ODIR)/ReSampler.o \
     $(ODIR)/Oscillator.o \
     


lib_path= $(LDFLAGS)

libs=-lsamplerate

link_flags=-ffunction-sections -fdata-sections -Wl,--gc-sections

c_flags=-fno-exceptions -fno-rtti 

ARCH=$(shell uname -m)

ifeq "$(ARCH)" "armv6l"
c_flags+=-march=armv6 -mfpu=vfp -mfloat-abi=hard
endif

ifeq "$(ARCH)" "armv7l"
c_flags+=-march=armv7 -mfpu=vfp -mfloat-abi=hard
endif

ifeq "$(ARCH)" "x86_64"
c_flags+=-ffast-math -fomit-frame-pointer
endif

ifeq "$(MAKECMDGOALS)" "debug"
c_flags+=-g -Wall -O0
c_defines=-DDEBUG
else
c_flags+= -Wall -O2
endif


c_defines+= -Dkiss_fft_scalar=float
c_include=-I. 

all : $(ODIR) ok

$(ODIR) :
	mkdir $(ODIR)

$(ODIR)/main.o : main.cpp
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/main.o main.cpp

$(ODIR)/WaveFile.o : WaveFile.cpp WaveFile.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/WaveFile.o WaveFile.cpp

$(ODIR)/kiss_fft.o : kiss_fft.c kiss_fft.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/kiss_fft.o kiss_fft.c

$(ODIR)/kiss_fftr.o : kiss_fftr.c kiss_fftr.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/kiss_fftr.o kiss_fftr.c

$(ODIR)/PhaseTest.o : PhaseTest.cpp PhaseTest.h PhaseTestComplex.h PhaseTestReal.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/PhaseTest.o PhaseTest.cpp
	
$(ODIR)/PhaseTestComplex.o : PhaseTestComplex.cpp PhaseTestComplex.h PhaseTest.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/PhaseTestComplex.o PhaseTestComplex.cpp
	
$(ODIR)/PhaseTestReal.o : PhaseTestReal.cpp PhaseTestReal.h PhaseTest.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/PhaseTestReal.o PhaseTestReal.cpp
	
$(ODIR)/Options.o : Options.cpp Options.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/Options.o Options.cpp

$(ODIR)/CmdFax.o : CmdFax.cpp CmdFax.h Options.h FaxDemodulator.h PhaseTest.h PhaseTestComplex.h PhaseTestReal.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/CmdFax.o CmdFax.cpp

$(ODIR)/FaxDemodulator.o : FaxDemodulator.cpp FaxDemodulator.h FIRFilter.h CmdFaxLookUpTable.h ReSampler.h Oscillator.h Real.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/FaxDemodulator.o FaxDemodulator.cpp
	
$(ODIR)/FIRFilter.o : FIRFilter.cpp FIRFilter.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/FIRFilter.o FIRFilter.cpp
	
$(ODIR)/BMP.o : BMP.cpp BMP.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/BMP.o BMP.cpp

$(ODIR)/FaxImage.o : FaxImage.cpp FaxImage.h BMP.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/FaxImage.o FaxImage.cpp
	
$(ODIR)/ReSampler.o : ReSampler.cpp ReSampler.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/ReSampler.o ReSampler.cpp
	
$(ODIR)/Oscillator.o : Oscillator.cpp Oscillator.h Real.h
	$(CXX) $(c_flags) $(c_defines) $(c_include) -c -o $(ODIR)/Oscillator.o Oscillator.cpp
	
	
$(progname) : $(OBJ)
	$(CXX) -o $(progname) $(link_flags) $(OBJ) $(lib_path) $(libs)

ok : $(ODIR) $(progname)
	@echo $(progname) Ok.

run : ok
	./$(progname) -f 5faxe.wav -c 1500 

debug : ok
	kdbg $(progname)

clean :
	rm -fr $(progname) $(ODIR)
