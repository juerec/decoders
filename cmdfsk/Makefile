progname=cmdfsk


CCC=g++
c_flags=
cpp_flags=-fno-exceptions -fno-rtti -O0 $(c_flags) -DNO_THREADS

c_include=-I. -I../libFSKDecoder

ODIR=./obj

OBJ=$(ODIR)/main.o \
    $(ODIR)/DecoderIO.o \
    $(ODIR)/WaveFile.o \
    $(ODIR)/ASoundCapture.o \
    $(ODIR)/Logger.o


libs=-lasound -lfftw3f
lib_path=-L$(HOME)/lib

libs_static=../libFSKDecoder/libFSKDecoder.a

all : $(ODIR) $(progname) ok

$(ODIR) :
	mkdir $(ODIR)

$(progname) : $(OBJ) $(libs_static)
	$(CCC) -o $(progname) $(link_flags) $(OBJ) $(lib_path) $(libs) $(libs_static)

$(ODIR)/main.o : main.cpp
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/main.o main.cpp

$(ODIR)/DecoderIO.o : DecoderIO.cpp DecoderIO.h WaveFile.h ASoundCapture.h Logger.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/DecoderIO.o DecoderIO.cpp

$(ODIR)/WaveFile.o : WaveFile.cpp WaveFile.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/WaveFile.o WaveFile.cpp

$(ODIR)/ASoundCapture.o : ASoundCapture.cpp ASoundCapture.h Logger.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/ASoundCapture.o ASoundCapture.cpp

$(ODIR)/Logger.o : Logger.cpp Logger.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/Logger.o Logger.cpp

ok :
	@echo "$(progname) : Ok."

clean :
	rm -fr $(ODIR) $(progname)

run : all
	./$(progname) --help

debug : all
	kdbg $(progname)
