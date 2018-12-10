### Example Makefile for MUMT-306 Homework assignments on OS-X systems

PROGRAMS = userInterface
RM = /bin/rm
CC       = g++
DEFS     = -D__MACOSX_CORE__ -D__LITTLE_ENDIAN__ # use __WINDOWS_MM__ for Windoze
CFLAGS   = -O3 -Wall -std=c++11 -lpthread
LIBRARY = -lm -framework CoreMIDI -framework CoreFoundation -framework CoreAudio # use winmm.lib for Windoze

all : $(PROGRAMS)

clean : 
	-rm -f *~ *.o
	-rm $(PROGRAMS)

strip : 
	strip $(PROGRAMS)

## Include all source files necessary for a program in the dependencies and compiler statements below

MidiMessage.o: MidiMessage.h MidiMessage.cpp 
	$(CC) $(CFLAGS) $(DEFS) -c  MidiMessage.cpp 

Binasc.o: Binasc.h Binasc.cpp 
	$(CC) $(CFLAGS) $(DEFS) -c Binasc.cpp
 
MidiEvent.o: MidiEvent.cpp MidiMessage.h 
	$(CC) $(CFLAGS) $(DEFS) -c  MidiEvent.cpp 

MidiEventList.o: MidiEventList.cpp MidiEvent.h 
	$(CC) $(CFLAGS) $(DEFS) -c  MidiEventList.cpp

MidiFile.o: MidiFile.cpp MidiEventList.h Binasc.h 
	$(CC) $(CFLAGS) $(DEFS) -c  MidiFile.cpp

writeMidi.o: writeMidi.cpp MidiFile.h 
	$(CC) $(CFLAGS) $(DEFS) -c writeMidi.cpp

RtMidi.o: RtMidi.h RtMidi.cpp 
	$(CC) $(CFLAGS) $(DEFS) -c RtMidi.cpp

playRhythm.o: playRhythm.cpp RtMidi.h
	$(CC) $(CFLAGS) $(DEFS) -c playRhythm.cpp $(LIBRARY)

userInterface: userInterface.cpp playRhythm.h writeMidi.h writeMidi.o playRhythm.o MidiMessage.o Binasc.o MidiEvent.o MidiEventList.o MidiFile.o RtMidi.o
	$(CC) $(CFLAGS) $(DEFS) -o userInterface userInterface.cpp $(LIBRARY) writeMidi.o playRhythm.o MidiMessage.o Binasc.o MidiEvent.o MidiEventList.o MidiFile.o RtMidi.o




















