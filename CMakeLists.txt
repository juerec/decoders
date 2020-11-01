cmake_minimum_required(VERSION 3.17...3.18 )

project("decoders" CXX C)

find_package (ALSA)

include(FetchContent)

FetchContent_Declare(
  libfftw
  URL http://fftw.org/fftw-3.3.8.tar.gz
  URL_HASH MD5=8aac833c943d8e90d51b697b27d4384d
)

FetchContent_Declare(
	libkissfft
	GIT_REPOSITORY https://github.com/mborgerding/kissfft.git
	GIT_TAG 0a70dfd0cf2cd620f993c280661372da808c941a
	)

FetchContent_GetProperties(libfftw)
if(NOT libfftw_POPULATED)
  FetchContent_Populate(libfftw)
  add_subdirectory(${libfftw_SOURCE_DIR} ${libfftw_BINARY_DIR})
endif()

FetchContent_GetProperties(libkissfft)
if(NOT kissfft_POPULATED)
	FetchContent_Populate(libkissfft)
	add_subdirectory(${libkissfft_SOURCE_DIR} ${libkissfft_BINARY_DIR})
endif()

#----------------------

add_library(FSKDecoder
	libFSKDecoder/BaudotDecoder.cpp
	libFSKDecoder/DecoderBase.cpp
	libFSKDecoder/FSKDecoder.cpp
	libFSKDecoder/FSKDemod.cpp
	libFSKDecoder/NavtexDecoder.cpp
	libFSKDecoder/SpectrogramComplex.cpp
	libFSKDecoder/Spectrogram.cpp
	libFSKDecoder/SpectrogramReal.cpp
	libFSKDecoder/WindowFunc.cpp
	)

set_property(TARGET FSKDecoder PROPERTY CXX_STANDARD 98)

target_include_directories(FSKDecoder
	PUBLIC libFSKDecoder
)

target_link_libraries(FSKDecoder
	PUBLIC fftw3f
)

#-------cmdfsk---------------
add_executable(cmdfsk
	cmdfsk/main.cpp
	cmdfsk/DecoderIO.cpp
	cmdfsk/WaveFile.cpp
	cmdfsk/ASoundCapture.cpp
	cmdfsk/Logger.cpp)

target_include_directories(cmdfsk
	PUBLIC cmdfsk
)
target_compile_definitions(cmdfsk
	PRIVATE NO_THREADS)

target_compile_options(cmdfsk
	PRIVATE	-fno-rtti
	PRIVATE -fno-exceptions
)

target_link_libraries(cmdfsk
	${ALSA_LIBRARIES}
	fftw3
	FSKDecoder
)

#---------kissfftr

add_library(kissfftr
	${libkissfft_SOURCE_DIR}/kiss_fft.c
	${libkissfft_SOURCE_DIR}/tools/kiss_fastfir.c
	${libkissfft_SOURCE_DIR}/tools/kiss_fftr.c
	)
target_include_directories(kissfftr
	PUBLIC ${libkissfft_SOURCE_DIR}
	PUBLIC ${libkissfft_SOURCE_DIR}/tools
	)
target_compile_definitions(kissfftr
	PRIVATE	REAL_FASTFIR
	PRIVATE FAST_FILT_UTIL
)

#-------cmdfax---------------
add_executable(cmdfax
	cmdfax/BMP.cpp
	cmdfax/CmdFax.cpp
	cmdfax/FaxDemodulator.cpp
	cmdfax/FaxImage.cpp
	cmdfax/FIRFilter.cpp
	cmdfax/Options.cpp
	cmdfax/Oscillator.cpp
	cmdfax/PhaseTestComplex.cpp
	cmdfax/PhaseTest.cpp
	cmdfax/PhaseTestReal.cpp
	cmdfax/ReSampler.cpp
	cmdfax/WaveFile.cpp
	cmdfax/main.cpp
)

target_compile_definitions(cmdfax
	PRIVATE kiss_fft_scalar=float
)

target_link_libraries(cmdfax
	samplerate
	kissfftr
	)

