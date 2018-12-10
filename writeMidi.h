//
//  writeMidi.h
//  
//
//  Created by mesichj on 2018-12-01.
//
//

#ifndef writeMidi_h
#define writeMidi_h

#include "MidiFile.h"
#include <iostream>

using namespace std;

std::vector<std::vector<int> > extendTrack(std::vector<std::vector<int> > sequences,std::vector<std::vector<int> > notes);
int writeMidiFile(std::vector<std::vector<int> > sequences,std::vector<std::vector<int> > notes);

#endif /* writeMidi_h */
