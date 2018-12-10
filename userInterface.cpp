
//USER INTERFACE
#include <iostream>
#include <cstdlib>
#include <vector>
#include "playRhythm.h"
#include "writeMidi.h"


using namespace std;

//this file is just for sending text to user and accepting input


int main(void);
int mainMenu();
int buildRhythmMenu(std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes);

//buildRhythmMenu leads to
void addTrack(std::vector<std::vector<int> >sequences,std::vector<std::vector<int> >notes);
void printCurrentTracks( std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes);
int deleteTrack(std::vector< std::vector<int> >sequences, std::vector< std::vector<int> > notes);
void editTrack(std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes);

//addTrack leads to
std::vector<int> addNote(std::vector<std::vector<int> > sequence, std::vector<std::vector<int> > notes);
std::vector<int> euclideanRhythmGenerator(int steps, int hits, int rotation);
int buildBeatMenu(std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes);

//addNote leads to
std::vector<int> selectInstrument();

//selectNote leads to
int selectNote();

//euclideanRhythmGenerator leads to
std::vector<int> rotateRhythm(std::vector<int> rhy, int rotate);


//helper methods
std::vector<int> selectRhythm();
int selectDrum();




int main(void){
    //main menu greeting
    std::cout << "Welcome to the Euclidean Rhythm Generator!"<< std::endl;
    mainMenu();
    
}
//main menu of the generator
int mainMenu(){
    std::vector< std::vector<int> > sequences;
    std::vector< std::vector<int> > notes;
    int type=0;
    //loops until a correct choice is made
    while(true){
        
        std::cout << "\nType:\n (1) to begin to create your rhythm\n (2) for an explaination on how the generator works!"<< std::endl;
        std::cin >>type;
        if(std::cin.fail()){
            
            std::cout<< "\nThat wasn't a valid choice. Please select an option labelled 1 or 2." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            type=0;
        }
        //begin
        else if(type==1){
            std::cout << "\n\nLet's begin!" << std::endl;
            buildRhythmMenu(sequences,notes);
        }
        
        //explaination of generator
        else if(type==2){
            
            std::cout << "\n\nThis generator has the ability to make a rhythm with up to 16 channels of beats.\n" <<
            "Each beat in each channel will need three inputs from you; the number of beats"<<
            "in the sequence, the number of sounds per sequence and the offset. \n(The number of sounds per sequence must be less than or equal to the number of beats in the sequence)\nAn example "<<
            "of input would be (5,2,3) where 5 is the number of beats in the sequence, 2 is "<<
            "the number of sounds produced and 3 will shift the start. \nIn this case, the algorithm will produce the following beat [.x..x] "<<
            "where the x denotes a hit and the . denotes silence." << std::endl;
            
        }
        
        else{
            
            std::cout<< "\nThat wasn't a valid choice. Please select an option labelled 1 - 2." << std::endl;
            type=0;
        }
    }
}

//build rhythm menu
int buildRhythmMenu(std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes){
    int selection=0;
    //loops infinitly until you go back to main menu
    while(true){
        std::cout << "\n\nPlease select one of the following options\n 1) Add a beat to your track \n 2) Play current rhythm(All beats together) \n 3) " <<
        "Print current beats in rhythm\n 4) Edit a beat from your rhythm\n 5) Delete a beat from your rhythm\n 6) Save Rhythm\n 7) Go back to the main menu (If you pick this option without saving your progress will be lost!)"<< std::endl;
        std::cin >> selection;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            selection=0;
        }
        //add a new beat to track
        else if(selection==1){
            addTrack(sequences,notes);
        }
        
        //play rhythm
        else if(selection==2){
            if(sequences.size()!=0){
                playWholeRhythm(sequences, notes);
                buildRhythmMenu(sequences,notes);
            }
            else{
                std::cout<< "\n\nYou currently have no tracks in your rhythm.\n" << std::endl;
            }
            
            
        }
        
        //print what you have
        else if(selection==3){
            if(sequences.size()!=0){
                printCurrentTracks(sequences,notes);
            }
            else{
                std::cout<< "\n\nYou currently have no tracks in your rhythm.\n" << std::endl;
            }
            
            
        }
        
        //edit your current rhythm
        else if(selection==4){
            
            if(sequences.size()!=0){
                printCurrentTracks(sequences,notes);
                editTrack(sequences,notes);
            }
            else{
                std::cout<< "\n\nYou currently have no tracks in your rhythm.\n" << std::endl;
            }
            
            
        }
        
        //delete a track
        else if(selection==5){
            if(sequences.size()!=0){
                printCurrentTracks(sequences,notes);
                deleteTrack(sequences,notes);
            }
            else{
                std::cout<< "\n\nYou currently have no tracks in your rhythm.\n" << std::endl;
            }
            
        }
        
        //write to file
        else if(selection==6){
            
            writeMidiFile(sequences,notes);
            
        }
        
        //quit
        else if(selection==7){
            mainMenu();
        }
        
        else{
            std::cout<< "\n\nInvalid input. Try again!" << std::endl;
        }
    }
    
}

void addTrack(std::vector<std::vector<int> >sequences,std::vector<std::vector<int> >notes){
    
    std::vector<int> note;
    std::vector <int> rhythm=selectRhythm();
    
    rhythm = euclideanRhythmGenerator(rhythm[0],rhythm[1],rhythm[2]);
    note=addNote(sequences, notes);
    sequences.push_back(rhythm);
    notes.push_back(note);
    
    buildBeatMenu(sequences,notes);
}
void editTrack(std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes){
    bool hold =true;
    int number=0;
    int choice=0;
    int volume = -1;
    int length=-1;
    int note=0;
    
    
    std::vector<int> sound;
    while(hold){
        std::cout<< "\n\nEnter the number of the track you wish to make changes to." << std::endl;
        std::cin >> number;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            number=0;
        }
        else if(number>0&&number<sequences.size()+1){
            hold=false;
        }
        else{
            std::cout << "\n\nPlease enter a valid input" << std::endl;
        }
    }
    hold=true;
    number--;
    while(hold){
        std::cout << "\n\nWhat do you want to edit: \n 1) Instrument and Note \n 2) Note \n 3) Volume \n 4) Rhythm \n 5) Duration of beat" << std::endl;
        std::cin >> choice;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            choice=0;
        }
        if(choice == 1){
            sound = selectInstrument();
            notes[number][2]=sound[1];
            notes[number][1]=sound[0];
            hold=false;
        }
        else if(choice == 2){
            //not drum
            if(notes[number][0]!=9){
                note = selectNote();
                notes[number][2] = note;
            }
            //drum
            else{
                note=selectDrum();
                notes[number][2]=note;
            }
            
            hold=false;
        }
        else if(choice == 3){
            while(volume<0||volume>100){
                std::cout << "\n\nEnter the volume of the sound(0-100):" << std::endl ;
                std::cin >> volume;
                if(std::cin.fail()){
                    
                    std::cout<< "\nPlease enter an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(256,'\n');
                    volume=0;
                }
                else if(volume<0||volume>100){
                    std::cout << "\n\nPlease input a valid number" << std::endl;
                }
                
            }
            notes[number][4]=volume;
            hold=false;
        }
        else if(choice == 4){
            
            std::vector <int> rhythm=selectRhythm();
            rhythm = euclideanRhythmGenerator(rhythm[0],rhythm[1],rhythm[2]);
            sequences[number]=rhythm;
            
            hold=false;
        }
        else if(choice== 5){
            while (length < 0){
                std::cout << "\n\nEnter the length of the sound(in ms):" << std::endl ;
                std::cin >> length;
                if(std::cin.fail()){
                    
                    std::cout<< "\nPlease enter an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(256,'\n');
                    length=0;
                }
                else if(length < 0){
                    std::cout << "\n\nPlease input a number greater than 0" << std::endl;
                }
                
            }
            notes[number][3]=length;
            hold=false;
        }
        else{
            std::cout << "\n\nPlease select a valid input (1-5)" << std::endl;
        }
    }
    buildRhythmMenu(sequences,notes);
    
}

//notes format (channel, program change, note, velocity)
void printCurrentTracks(std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes){
    
    std::string note;
    std::string octave;
    std::string instrument;
    std::string noteAndOctave;
    std::string rhythm;
    for(int i =0; i < sequences.size(); i++){
        int volume =notes[i][4];
        int duration = notes[i][3];
        rhythm="[";
        for(int j =0; j < sequences[i].size(); j++){
            if(sequences[i][j]==1){
                rhythm+="x ";
            }
            else{
                rhythm+=". ";
            }
        }
        rhythm+="]";
        //checks if it is a drum
        if(notes[i][1]==0){
            instrument = " Drum ";
            switch(notes[i][2]){
                case 40:
                    noteAndOctave="Snare Drum";
                    break;
                case 42:
                    noteAndOctave="Hi Hats";
                    break;
                case 43:
                    noteAndOctave="High Floor Tom Drum";
                    break;
                case 41:
                    noteAndOctave="Low Floor Tom Drum";
                    break;
                case 35:
                    noteAndOctave="Snare Drum 2";
                    break;
                case 36:
                    noteAndOctave="Bass Drum";
                    break;
            }
        }
        //not a drum
        else{
            switch(notes[i][1]){
                case 5:
                    instrument="Piano";
                    break;
                case 39:
                    instrument="Bass Guitar";
                    break;
            }
            //finds the note
            switch(notes[i][2]%12){
                case 0:
                    note="C";
                    break;
                case 2:
                    note="D";
                    break;
                case 4:
                    note="E";
                    break;
                case 5:
                    note="F";
                    break;
                case 7:
                    note="G";
                    break;
                case 9:
                    note="A";
                    break;
                case 11:
                    note="B";
                    break;
            }
            //finds the octave
            switch(notes[i][2]/12){
                case 2:
                    octave="1";
                    break;
                case 3:
                    octave="2";
                    break;
                case 4:
                    octave="3";
                    break;
                case 5:
                    octave="4";
                    break;
                case 6:
                    octave="5";
                    break;
                case 7:
                    octave="6";
                    break;
                case 8:
                    octave="7";
                    break;
            }
            noteAndOctave=note+octave;
            
        }
        std::cout<<"TRACK "<< i+1 << "  Rhythm:" << rhythm << "  Instrument:" << instrument << "  Note:"<< noteAndOctave  << "  Volume: "<< volume<< "  Duration(ms): " << duration << std::endl;
        
    }
    
    
}

//allows user to delete a beat from the rhythm
int deleteTrack(std::vector< std::vector<int> >sequences, std::vector< std::vector<int> > notes){
    int number =0;
    std::cout<< std::endl;
    bool hold = true;
    //continously asks for a beat to delete until a valid response is given
    while(hold){
        std::cout<< "\n\nEnter the number of the track you wish to delete." << std::endl;
        std::cin >> number;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            number=0;
        }
        if(number>0&&number<sequences.size()+1){
            hold=false;
        }
        else{
            std::cout << "\n\nPlease enter a valid input" << std::endl;
        }
        
    }
    
    //deletes from rhythm
    notes.erase(notes.begin()+number-1);
    sequences.erase(sequences.begin()+number-1);
    
    buildRhythmMenu(sequences,notes);
    
    return 0;
}
std::vector<int> addNote(std::vector<std::vector<int> > sequences, std::vector<std::vector<int> > notes){
    
    int volume=-1;
    int length=-1;
    std::vector<int> sound;
    std::vector<int> note;
    
    while(volume<0||volume>100){
        std::cout << "\n\nEnter the volume of the sound(0-100):" << std::endl ;
        std::cin >> volume;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            volume=-1;
        }
        else if(volume<0||volume>100){
            std::cout << "\n\nPlease input a valid number" << std::endl;
        }
        
    }
    while (length < 0){
        std::cout << "\n\nEnter the length of the sound(in ms):" << std::endl ;
        std::cin >> length;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            length=-1;
        }
        else if(length < 0){
            std::cout << "\n\nPlease input a number greater than 0" << std::endl;
        }
        
    }
    
    std::cout << "\n\nEnter the sound pick from the list:" << std::endl ;
    
    sound=selectInstrument();
    if(sound[0]==0){
        note.push_back(9);
    }
    else{
        note.push_back(sequences.size());
    }
    
    
    
    //program [1]
    note.push_back(sound[0]);
    
    
    //sound [2]
    note.push_back(sound[1]);
    
    //length [3]
    note.push_back(length);
    
    //velocity [4]
    note.push_back(volume);
    
    //add note to list
    return note;
    
}
//generates the rhythm based on the number of steps and hits
std::vector<int> euclideanRhythmGenerator(int steps, int hits, int rotation){
    
    //create a vector with the size the same number of steps
    std::vector<int> rhythm (steps);
    int bucket=0;
    
    
    //spreads the number of hits as evenly as possible. Does this by adding the hits to a "bucket" with a capacity of the number of steps.
    //if the bucket overflows, that marks a hit. if not it marks a silence.
    for(int i =0;i< steps; i++){
        
        bucket+=hits;
        
        if(bucket >= steps){
            
            bucket = bucket % steps;
            rhythm[i]=1;
            
        }
        else{
            
            rhythm[i]=0;
            
        }
        
    }
    
    //checks if it needs to be rotated
    if(rotation!=0){
        
        rhythm= rotateRhythm(rhythm,rotation);
        
    }
    return rhythm;
}
int buildBeatMenu(std::vector< std::vector<int> > sequences, std::vector< std::vector<int> > notes){
    
    
    int selection =0;
    int size = sequences.size()-1;
    
    
    while(true){
        std::cout<< "\n\nGreat Choice! Please select one of the options below:\n 1) Play beat alone \n 2) Play beat with the rest of your rhythm \n 3) Add beat to your rhythm\n 4) Discard beat" << std::endl;
        std::cin>> selection;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            selection=0;
        }
        //play only the new beat
        else if(selection==1){
            playSingleTrack(sequences[size],notes[size]);
            buildBeatMenu(sequences,notes);
            
        }
        //play the whole thing together
        else if(selection==2){
            playWholeRhythm(sequences,notes);
            buildRhythmMenu(sequences,notes);
            
        }
        //add the beat and return to buildRhythm menu
        else if(selection==3){
            
            buildRhythmMenu( sequences, notes);
        }
        //remove the beat from the list and return to buildRhythmMenu
        else if(selection==4){
            sequences.pop_back();
            notes.pop_back();
            buildRhythmMenu(sequences,notes);
        }
        
        else{
            std::cout<< "\n\nInvalid input. Try again!" << std::endl;
            
        }
    }
    
}
std::vector<int> selectInstrument(){
    //init var
    int instrument=0;
    int selection=0;
    int note=0;
    
    //gathers which instrument the user wants and redirects based on choice
    while((selection!=1)&&(selection!=2)&&(selection!=3)){
        std::cout << "\n\nDrums(1), bass(2) or piano(3)?" << std::endl;
        std::cin >> selection;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            selection=0;
        }
        else if(selection==1){
            instrument=0;
            note = selectDrum();
        }
        else if(selection==2){
            instrument = 32;
            note = selectNote();
        }
        else if(selection==3){
            instrument = 2;
            note = selectNote();
        }
        else{
            std::cout << "\n\nPlease select a valid choice" << std::endl;
        }
    }
    //loads values and returns
    std::vector<int> sound;
    sound.push_back(instrument);
    sound.push_back(note);
    
    return sound;
    
}
int selectNote(){
    //init var
    int note =0;
    int octave = 0;
    int value =0;
    
    //gathers which note the user wants
    while(note<1||note>8){
        std::cout<<"\n\nWhich note do you want? A(1), B (2), C (3), D(4), E(5) F(6) G(7)" << std::endl;
        std::cin >> note;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            note=0;
        }
        else if(note<1||note>8){
            std::cout << "\n\nPlease enter a valid input" << std::endl;
        }
    }
    
    //gathers which octave the user wants
    while(octave<1||octave>8){
        std::cout<<"\n\nWhich octave? Select from 1-7" <<std::endl;
        std::cin >> octave;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            octave=0;
        }
        else if(octave<1||octave>8){
            std::cout << "\n\nPlease enter a valid input" << std::endl;
        }
    }
    octave--;
    
    //convert to midi value
    switch(note){
        case 1: //a1
            value = 33 + (12*octave);
            break;
        case 2: //b1
            value = 35 + (12*octave);
            break;
        case 3: //c1
            value = 24 + (12*octave);
            break;
        case 4: //d1
            value = 26 + (12*octave);
            break;
        case 5: //e1
            value = 28 + (12*octave);
            break;
        case 6: //f1
            value = 29 + (12*octave);
            break;
        case 7: //g1
            value = 31 + (12*octave);
            break;
            
    }
    
    return value;
    
}

//rotate the rhythm to give user access to different rhythms of the same "necklace"
std::vector<int> rotateRhythm(std::vector<int> rhy, int rotate){

    int length = rhy.size();
    std::vector<int> output (length);
    int shift = length-rotate;

    //shifts the sequence over, using modulo to ensure that the values stay in bound
    for(int i=0; i< length;i++){

        output[i]=rhy[(i+shift)%length];

    }
    //returns rotated rhythm
    return output;
}

//allows user to specify a the rhythm
std::vector<int> selectRhythm(){
    int steps=-1;
    int hits=0;
    int offset=-1;
    //gathers user input for size of rhythm
    while(steps<0){
        std::cout << "\n\nEnter the number of steps in your beat: " << std::endl;
        std::cin >> steps;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            steps=-1;
        }
        else if(steps<0){
            std::cout << "\n\nPlease enter a number greater than 0." << std::endl;
        }
    }
    hits=steps+1;
    //gather input for number of hits in rhythm
    while(hits>steps||hits<1){
        std::cout << "\n\nEnter the number of sounds you want in your beat(must be less than or equal to the length):" << std::endl;
        std::cin >> hits;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            hits=steps+1;
        }
        else if(hits>steps){
            std::cout << "\n\nPlease enter a number greater than 0 and less than or equal to " << steps <<std::endl;
        }
        
    }
    //gathers input for rotation
    while(offset<0||offset>steps){
        std::cout << "\n\nEnter the offset of your beat:" << std::endl;
        std::cin >> offset;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            offset=-1;
        }
        else if(offset<0||offset>steps){
            std::cout << "\n\nPlease enter a positive number less than " <<  steps << std::endl;
        }
    }
    //loads values into a vector and returns it
    
    std::vector<int> rhythmValues;
    rhythmValues.push_back(steps);
    rhythmValues.push_back(hits);
    rhythmValues.push_back(offset);
    
    return rhythmValues;
    

}
//allows user to select the drum sound they want
int selectDrum(){
    
    int instrument =0;
    int note=0;
    //lets user choose drum
    while(instrument<1||instrument>6){
        std::cout << "\n\nPick the drum: \n 1. Snare Drum\n 2. Snare Drum 2\n 3. Bass Drum\n 4. Hi-hats\n 5. Low Floor Tom Drum\n 6. High Floor Tom Drum\n" << std::endl;
        std::cin >> instrument;
        if(std::cin.fail()){
            
            std::cout<< "\nPlease enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(256,'\n');
            instrument=0;
        }
        if(instrument<1||instrument>6){
            std::cout<< "\n\nPlease enter a valid input from 1-6" << std::endl;
        }
    }
    //sets the correct values depending on the user's choice
    switch(instrument){
        case 1:
            note=40;
            instrument=0;
            break;
        case 2:
            note=35;
            instrument=0;
            break;
        case 3:
            note=36;
            instrument=0;
            break;
        case 4:
            note=42;
            instrument=0;
            break;
        case 5:
            note=41;
            instrument=0;
            break;
        case 6:
            note=43;
            instrument=0;
            break;
    }
    return note;

}














