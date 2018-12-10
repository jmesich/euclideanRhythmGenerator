//
//  writeMidi.cpp
//  
//
//  Created by mesichj on 2018-12-01.
//
//

#include<iostream>
#include "MidiFile.h"


/*
 writeMidiFile takes input the sequences and the notes from the userInterface file and writes them to a MIDI file.
 This file uses the repo from craigsapp's github. Thank you Craig.
 */

std::vector<std::vector<int> > extendTrack(std::vector<std::vector<int> > sequences,std::vector<std::vector<int> > notes){

    //initiate vars
    int temp=0;
    int difference=0;
    int longest=sequences[0].size()*notes[0][3];
    int numberOfSequences = sequences.size();
    std::vector<float> multiplier (sequences.size());
    
    
    //find longest track length
    for(int i=1;i<numberOfSequences;i++){
        temp =sequences[i].size()*notes[i][3];
        if(temp>longest){
            
            longest=temp;
            
        }
        
    }
    
    //take longest and divide by other lengths to find multiplier
    for(int i=0;i<numberOfSequences;i++){
        multiplier[i]=(longest/(sequences[i].size()*notes[i][3]))-1;
    }
    
    //make each sequence the same length
    for(int i=0;i<numberOfSequences;i++){
        
        
        difference = multiplier[i] * sequences[i].size();
        for(int j=0; j<difference; j++){
            
            sequences[i].push_back(sequences[i][j]);
            
        }
        
    }

    return sequences;
}
int writeMidiFile(std::vector<std::vector<int> > sequences,std::vector<std::vector<int> > notes){
    //initializes the midiFile object
    MidiFile midiFile;
    midiFile.addTracks(sequences.size());
    midiFile.setTicksPerQuarterNote(250);
    //initialize variables needed
    int actiontick = 0;
    int track = 0;
    int velocity = 0;
    int duration = 0;
    int note =0;
    int channel = 0;
    int program = 0;
    std::string title;
    
    
    //extend the tracks to all the same size
    sequences=extendTrack(sequences,notes);
    //user inputs the title of the midi file
    std::cout << "Please enter the title of your file:"<< std::endl;
    std::cin >> title;
    //track 0 info
    midiFile.addTrackName(track, actiontick, title);
    midiFile.addTempo(track, actiontick, 250);
    
    track++;
    //fill up the rest of the tracks
    for(int i =0; i<sequences.size(); i++){
        
        //set variables for clarity
        velocity = notes[i][4];
        duration = notes[i][3];
        note = notes[i][2];
        channel = notes[i][0];
        program = notes[i][1];
        
        
        //only needed if not a drum so
        if(channel!=9){
            midiFile.addPatchChange(track, actiontick, channel, program);
        }
        //reset actiontick for each track
        actiontick=0;
        //translates the two vectors into midifile note on and note off messages
        for(int j =0; j< sequences[i].size();j++){
            
            if(sequences[i][j]==1){
                midiFile.addNoteOn(track, actiontick, channel, note, velocity);
                actiontick+=duration;
                midiFile.addNoteOff(track, actiontick, channel, note, velocity);
            }
            
            else{
                actiontick+=duration;
            }
            
        
        }
        track++;
    
    }
    
    midiFile.sortTracks();
    midiFile.write(title+".mid");
    
    return 0;


}
