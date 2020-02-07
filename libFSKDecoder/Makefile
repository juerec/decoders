libname=libFSKDecoder
libversion=1.0
lib_so_name=$(libname).so.$(libversion)
lib_ar_name=$(libname).a

CC=gcc
CCC=g++ -std=c++98

ifeq ($(MAKECMDGOALS),shared)
ODIR=./obj-shared
c_flags=-fPIC -DPIC
lib_name=$(lib_so_name)
endif

ifeq ($(MAKECMDGOALS),clean-shared)
ODIR=./obj-shared
lib_name=$(lib_so_name)
endif

ifeq ($(MAKECMDGOALS),static)
ODIR=./obj-static
lib_name=$(lib_ar_name)
endif

ifeq ($(MAKECMDGOALS),clean-static)
ODIR=./obj-static
lib_name=$(lib_ar_name)
endif

ifeq ($(MAKECMDGOALS),clean-all)
#ODIR=./obj-static ./obj-shared
lib_name=$(lib_ar_name) $(lib_so_name)
endif


cpp_flags=-fno-exceptions -fno-rtti -O0 $(c_flags)
c_include=-I../common

OBJ=$(ODIR)/FSKDecoder.o \
    $(ODIR)/FSKDemod.o \
    $(ODIR)/DecoderBase.o \
    $(ODIR)/BaudotDecoder.o \
    $(ODIR)/NavtexDecoder.o \
    $(ODIR)/Spectrogram.o \
    $(ODIR)/SpectrogramReal.o \
    $(ODIR)/SpectrogramComplex.o \
    $(ODIR)/WindowFunc.o


libs=-lfftw3f
link_flags=-shared 
               
all :
	@echo '  make < shared | static | clean-all | clean-shared | clean-static >'

shared : $(ODIR) $(lib_so_name) ok

static : $(ODIR) $(lib_ar_name) ok

$(ODIR) :
	mkdir $(ODIR)

$(lib_so_name) : $(OBJ)
	$(CCC) -o $(lib_so_name) -Wl,-soname,$(lib_so_name) $(link_flags) $(OBJ) $(lib_path) $(libs)

$(lib_ar_name) : $(OBJ)
	ar r $(lib_ar_name) $(OBJ)

$(ODIR)/FSKDecoder.o : FSKDecoder.cpp FSKDecoder.h FSKDecoderOptions.h Spectrogram.h SpectrogramReal.h SpectrogramComplex.h \
                                                   FSKDemod.h BaudotDecoder.h NavtexDecoder.h DecoderBase.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/FSKDecoder.o FSKDecoder.cpp

$(ODIR)/FSKDemod.o : FSKDemod.cpp FSKDemod.h BaudotDecoder.h NavtexDecoder.h DecoderBase.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/FSKDemod.o FSKDemod.cpp

$(ODIR)/DecoderBase.o : DecoderBase.cpp DecoderBase.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/DecoderBase.o DecoderBase.cpp

$(ODIR)/BaudotDecoder.o : BaudotDecoder.cpp BaudotDecoder.h DecoderBase.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/BaudotDecoder.o BaudotDecoder.cpp

$(ODIR)/NavtexDecoder.o : NavtexDecoder.cpp NavtexDecoder.h DecoderBase.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/NavtexDecoder.o NavtexDecoder.cpp

$(ODIR)/Spectrogram.o : Spectrogram.cpp Spectrogram.h WindowFunc.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/Spectrogram.o Spectrogram.cpp

$(ODIR)/SpectrogramReal.o : SpectrogramReal.cpp SpectrogramReal.h Spectrogram.h WindowFunc.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/SpectrogramReal.o SpectrogramReal.cpp

$(ODIR)/SpectrogramComplex.o : SpectrogramComplex.cpp SpectrogramComplex.h Spectrogram.h WindowFunc.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/SpectrogramComplex.o SpectrogramComplex.cpp

$(ODIR)/WindowFunc.o : WindowFunc.cpp WindowFunc.h
	$(CCC) $(cpp_flags) $(c_include) -c -o $(ODIR)/WindowFunc.o WindowFunc.cpp

clean : 
	@echo ' make < clean-all | clean-shared | clean-static >'

clean-all :
	rm -fr ./obj-static ./obj-shared $(lib_so_name) $(lib_ar_name)

clean-shared : clean-all
clean-statig : clean-all
	
ok : 
	@echo $(lib_name) Ok.
