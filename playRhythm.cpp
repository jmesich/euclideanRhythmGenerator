#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include <thread>
#include <mutex>

using namespace std;

// Platform-dependent sleep routines.
#if defined(__WINDOWS_MM__)
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif


RtMidiOut *midiout = 0;
std::mutex m;
std::vector<unsigned char> message(3,0);

//These methods were coded by Dr. Gary Scavone.

bool chooseMidiPort( RtMidiOut *rtmidi ){
    string keyHit;
    std::cout << "\nWould you like to open a virtual output port? [y/N] ";
    std::cin >> keyHit;
    if ( keyHit == "y" ) {
        rtmidi->openVirtualPort();
        return true;
    } 

    string portName;
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if ( nPorts == 0 ) {
        std::cout << "No output ports available!" << std::endl;
        return false;
    }
    if ( nPorts == 1 ) {
        std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
    }
    else {
        for ( i=0; i<nPorts; i++ ) {
            portName = rtmidi->getPortName(i);
            std::cout << "  Output port #" << i << ": " << portName << '\n';
        }
        
        do {
            std::cout << "\nChoose a port number: ";
            std::cin >> i;
        } while ( i >= nPorts );
    }
    std::cout << "\n";
    rtmidi->openPort( i );
    return true;
}
void createRtMidi(){
    try {
        midiout = new RtMidiOut();
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }
    
    // Call function to select port.
    try {
        bool test = chooseMidiPort(midiout);
        
        if ( !test ){
            delete midiout;
            return;
        }
        return;
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        delete midiout;
        return;
    }
    
}
//My work starts here
void playSingleTrack(std::vector<int> sequence, std::vector<int> note){
    int length = sequence.size();
    
    //create a new RtMidi object if it hasnt been created yet
    if(midiout==0){
        createRtMidi();
    }
    
    
   
    //program change
    message[0]=192+note[0];
    message[1]=note[1];
    midiout->sendMessage(&message);
    
    
    for(int i =0; i < length; i++){

        
        
        if(sequence[i]==1){
           //play note on
            message[0]= 144+note[0];
            message[1]= note[2]; // note
            message[2]= note[4]; // velocity
            midiout->sendMessage(&message);
           
            
        }
        else{
            //play note off

            message[0]= 128+note[0];
            message[1]= note[2];
            message[2]= note[4];
            
            midiout->sendMessage(&message);

            

        }
        //sleep for duration
        SLEEP(note[3]);
    }
    
    //make sure there is no residual sound
    message[0]= 128+note[0];
    message[1]= note[2];
    message[2]= note[4];
    
    midiout->sendMessage(&message);
    return;
    
}

void playSingleTrackThread(std::vector<int> sequence, std::vector<int> note){
    int length = sequence.size();
    
    //program change with a mutex lock so threads dont access the same thing at the same time
    m.lock();
    message[0]=192+note[0];
    message[1]=note[1];
    midiout->sendMessage(&message);
    m.unlock();
    
    for(int i =0; i < length; i++){
        
        
        
        //plays note on
        if(sequence[i]==1){
            m.lock();
            message[0]= 144+note[0];
            message[1]= note[2]; // note
            message[2]= note[4];
            midiout->sendMessage(&message);
            m.unlock();
            SLEEP(note[3]);
            
        }
        else{
            //play note off
            
            m.lock();
            
            message[0]= 128+note[0];
            message[1]= note[2];
            message[2]= note[4];
            
            midiout->sendMessage(&message);
            
            m.unlock();
            
            SLEEP(note[3]);
            
        }
    }
    //sends an extra note off to make sure there is no residual sound
    m.lock();
    
    message[0]= 128+note[0];
    message[1]= note[2];
    message[2]= note[4];
    
    midiout->sendMessage(&message);
    
    m.unlock();
    
    
}


void playWholeRhythm(std::vector<std::vector<int> > sequences,std::vector<std::vector<int> > notes){
    //initiate vars
    int temp=0;
    int difference=0;
    int longest=sequences[0].size()*notes[0][3];
    int numberOfSequences = sequences.size();
    std::vector<float> multiplier (sequences.size());
    
    //make rt midi object if not already made
    if(midiout==0){
        createRtMidi();
    }
    //1. make all the sequences the same length as the longest one
    //2. for loop nested in a while loop to continuously play the rhythm
    
    
    
    
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
    
    //create an array of threads
    std::thread t[sequences.size()];
    
    
    //creates the threads then joins them to the main thread
        for(int i = 0; i< sequences.size(); i++){
            t[i]=std::thread(playSingleTrackThread,sequences[i],notes[i]);
        }
        for(int i = 0; i< sequences.size(); i++){
            t[i].join();
        }
    
    
}




