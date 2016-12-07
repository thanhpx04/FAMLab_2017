	
#COMPILER: GCC FOR C PROGRAM, G++ FOR c++
CC = g++

#COMPILER FLAG
CFLAGS 		= -O
#CPPFLAGS 	= -I/usr/include/freetypes -I/usr/include/libxml2
#CXX			= g++
#CXXFLAGS 	= -pthread
#DEFS		= -DHAVE_CONFIG_h
#LDFLAGS		= -L/usr/lib/x86_64-linux-gnu
#LIBS		= -ljbig -lwebp -llcms2 -ltiff -lfreetype -ljasper -ljpeg -lpng12 -lwmflite -lXext -lSM -lICE -lX11 -llzma -lbz2 -lxml2 -lz -lm -lgomp -lpthread

SYSDEPMEM= jmemnobs.o

# source files: JPEG library proper
SOURCES= main.cpp io/*.cpp io/LibJpeg/*.c imageModel/*.cpp segmentation/*.cpp histograms/*.cpp pht/*.cpp pointInterest/*.cpp correlation/*.cpp Analysis.cpp
# files included by source files
INCLUDES= io/*.h io/LibJpeg/*.h imageModel/*.h segmentation/*.h histograms/*.h pht/*.h pointInterest/*.h correlation/*.h Analysis.h
# library object files common to compression and decompression
COMOBJECTS= jaricom.o jcomapi.o jutils.o jerror.o jmemmgr.o $(SYSDEPMEM)
# compression library object files
CLIBOBJECTS= jcapimin.o jcapistd.o jcarith.o jctrans.o jcparam.o \
        jdatadst.o jcinit.o jcmaster.o jcmarker.o jcmainct.o jcprepct.o \
        jccoefct.o jccolor.o jcsample.o jchuff.o jcdctmgr.o jfdctfst.o \
        jfdctflt.o jfdctint.o
# decompression library object files
DLIBOBJECTS= jdapimin.o jdapistd.o jdarith.o jdtrans.o jdatasrc.o \
        jdmaster.o jdinput.o jdmarker.o jdhuff.o jdmainct.o \
        jdcoefct.o jdpostct.o jddctmgr.o jidctfst.o jidctflt.o \
        jidctint.o jdsample.o jdcolor.o jquant1.o jquant2.o jdmerge.o
USEOBJECTS= Point.o Line.o Edge.o Matrix.o Thresholds.o Image.o \
			Reader.o TPSReader.o JPEGReader.o \
			Canny.o Suzuki.o ShapeHistogram.o PHTEntry.o PHoughTransform.o \
			Treatments.o Segmentation.o GeometricHistgoram.o ProHoughTransform.o \
			CrossCorrelation.o LandmarkDetection.o Analysis.o
# These objectfiles are included in libjpeg.a
LIBOBJECTS= $(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS) $(USEOBJECTS)

maelab: main.o			 
	$(CC) $(CFLAGS) -std=c++11 -o maelab main.o $(LIBOBJECTS)

main.o: main.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $(SOURCES)




clean:
	rm -f maelab
	rm -f *.o
	rm -f imageModel/*.o
	rm -f io/*.o
	rm -f context/*.o
	rm -f segmentation/*.o
	rm -f histograms/*.o