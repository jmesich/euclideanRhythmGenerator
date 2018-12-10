//
//  playRhythm.h
//  
//
//  Created by mesichj on 2018-11-27.
//
//

#ifndef playRhythm_h
#define playRhythm_h

#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <thread>
#include <vector>
#include <mutex>

bool chooseMidiPort( RtMidiOut *rtmidi );
void createRtMidi();
void playSingleTrack(std::vector<int> sequence, std::vector<int> note);
void programChange(std::vector<int > sequence,std::vector<int > note);
void playWholeRhythm(std::vector<std::vector<int> > sequences,std::vector<std::vector<int> > notes);
void playSingleTrackThread(std::vector<int> sequence, std::vector<int> note);
void programChangeThread(std::vector<int > sequence,std::vector<int > note);

#endif /* playRhythm_h */
