#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <algorithm>
using namespace std;

//variable declearations
string algorithm; //Algo type
int memorySize; //Size of memory  use for Algo
string fileName; //Name of file
vector<string>inputCommands; //vector containing all command read for input file

void readFile(string algo,string memsize,string fname){
    //variables for reading
    algorithm = algo;
    memorySize = atoi(memsize.c_str());
    fileName = fname;
    string inpStr;
    ifstream inpfile;
    inpfile.open(fname);
    //open file and push the commands into vectors
    while(!inpfile.eof()){
        getline(inpfile,inpStr,' ');
        inputCommands.push_back(inpStr);
    }
}

void pagedLRU(){
    cout<<"Running LRU"<<endl;
    cout<<"==============================================================================="<<endl;
    int delays; //delay counter
    int pageFaults; //page fault counter
    int pageReference; //counter for page refernce.
    list<int>LRU; //queue for lru
    queue<int>nobandQ; //queue for full fifo queue
    set<int> s; //this set makes sure there isnt any duplicates in the buffer
    //reading file and parsing inputs
    for(int i = 0; i < inputCommands.size();i++){
        int number;
        string waitsignal;
        //parse inputs into two varibles
        number = atoi(inputCommands[i].substr(0,1).c_str());
        waitsignal = inputCommands[i].substr(2,2);
        cout<<number<<" : "<<waitsignal<<": ";
        if(waitsignal == "a"){
            //if there is banwidth available we add to buffer
            if(LRU.size() < memorySize){
                if(nobandQ.size() > 0){
                    if(s.find(nobandQ.front()) == s.end() ){
                        s.insert(nobandQ.front());
                        cout<<"Put Page "<< nobandQ.front() <<" into the memory."<<endl;
                        pageFaults++;
                        LRU.push_front(nobandQ.front());
                        nobandQ.pop();
                    }
                }
                //if there is space in buffer and the number isnt in buffer yet we add it
                else if (s.find(number) == s.end()){
                    s.insert(number);
                    cout<<"Put Page "<< number <<" into the memory."<<endl;
                    pageFaults++;
                    LRU.push_back(number);
                }
                //this portion is used to reorder LRU
                else{
                    cout<<"Found Page "<<number<<" in memory."<<endl;
                    list<int>::iterator it = find(LRU.begin(), LRU.end(), number);
                    LRU.erase(it);
                    LRU.push_back(number);
                }
            }
            //if there is no space in buffer we remove the LRU
            else if(LRU.size() == memorySize){
                pageReference++;
                //adds the new number to set and performs LRU
                if(s.find(number) == s.end() ){
                    int var = LRU.front();
                    cout<<"Remove Page "<<var<< " from the memory."<<endl;
                    LRU.pop_front();
                    s.erase(var);
                    s.insert(number);
                    cout<<"Put Page "<< number <<" into the memory"<<endl;
                    LRU.push_back(number);
                    pageFaults++;
                }
                //continue if number is in the buffer
                else{
                    cout<<"Found Page "<<number<<" in memory."<<endl;
                    list<int>::iterator it = find(LRU.begin(), LRU.end(), number);
                    LRU.erase(it);
                    LRU.push_front(number);
                }

            }
        }
        //no bandwidth if available so we add to fifo queue or continue if found in buffer
        else if (waitsignal == "n"){
            cout<<"Tries to put Page "<< number<< " into the memory, but air bandwidth is not available."<<endl;
            if(s.find(number) == s.end() ){
                nobandQ.push(number);
                delays++;
            }
            //continue if number is already in buffer
            else{
                cout<<"Found Page "<<number<<" in memory, so not queued."<<endl;
            }
        }

    }
    cout<<"Report:"<<endl;
    cout<<"The total number of page references: "<<pageReference<<endl;
    cout<<"The number of page faults: "<<pageFaults<<endl;
    cout<<"The number of delayed page replacements due to unavailable air bandwidth: "<<delays<<endl;
    cout<<"==============================================================================="<<endl;
}


int main(int argc, char *argv[]){
    string a;
    string b;
    string c;
    a = argv[1];
    b = argv[2];
    c = argv[3];
    readFile(a,b,c);
    pagedLRU();
    return 0;
}