
//
//  main.cpp
//  tracker3d_cpp
//
//  Created by Ryan Morshead on 7/16/14.
//  Copyright (c) 2014 RyanMorshead. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <array>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>

using namespace std;

class Voxel
{/*The basic data unit; represents pixels in 3 dimensions (two spatial and one temporal) and it's associated data.
  <data_member: description>
  VRCB: an array of four values corrisponding to the volume and coordinates of the voxel:
    {volume (chamber), row (spatial), collumn(spatial), and bucket(temporal)}.
  ADC: The adc value is the magnitude of the signal recorded. */
public:
    Voxel(int vol, int col, int row, int bkt, int adc)
    {
        VRCB[0] = vol;
        VRCB[1] = col;
        VRCB[2] = row;
        VRCB[3] = bkt;
        ADC = adc;
    }
    
    int* getVRCB() {
        return VRCB;
    }

    int getADC() {
        return ADC;
    }
    
    bool operator!=(const Voxel& other) {
        return
            this->VRCB[0] != other.VRCB[0] &&
            this->VRCB[1] != other.VRCB[1] &&
            this->VRCB[2] != other.VRCB[2] &&
            this->VRCB[3] != other.VRCB[3] &&
            this->ADC != other.ADC;
    }
    
private:
    int VRCB[4];
    int ADC;
};


class Event
{
public:

    //Public Data Members:
    
    //Constructor:
    Event(int gradThresh, int dirThresh) {
        //eventId determined by index position in eventContainer
        //gradThresh initializes the gradient threshhold
        //dirThresh initialize the directional threshhold
    }
    
    void addVoxel(Voxel vox) {
        voxPortfolio.push_back(vox);
    }
    
    /*void closePortfolio() {
        //upon closing a portfolio, voxels will be sorted
        //from highest ADC to lowest.
        touch = false;
        sort(voxPortfolio.begin(), voxPortfolio.end(), compADC);
        
    }*/
    
    vector<Voxel>::const_iterator getPortfolioIter(int index=0) const {
        return voxPortfolio.begin() + index;
    }
    
    vector<Voxel> getPortfolio(int index=0) const {
        return voxPortfolio;
    }
    
    void makeTrajectories() {
        
    }
    
private:
    
    //Private Data Members:
    vector<Voxel> voxPortfolio;
    int gradThresh;
    int dirThresh;
    int Id;
    
    static bool compADC (Voxel vox1, Voxel vox2) {
        return vox1.getADC() > vox2.getADC();
    }
    
    /*PrivateMemberFunction(2):
     using a specified voxVRCB as a refernce point, a collection
     of its neighboring voxels is made and then returned. Handling
     the hexagonal geometry results in a maximum of 20 possible
     neighbors. A sifter then determines how many of those 20
     candidates are actually present in the public data member
     voxPortfolio.*/
    int* getNeighbors(int voxVRCB[4]) {
        int neighborsVRCB[80]; //will contain coordinates to voxels adjacent to voxVRCB
        int* pnVRCB= neighborsVRCB;
        
        int transRule[3] = {-1,0,1};
        
        /*using transRule, the position given by voxVRCB is translated
         to adjecent voxels which are then saved to stampNeighbors.
         Since the voxels exist in a hexagonal geometry it requires
         some manipulation to fit them into cartesian coordinates.*/
        int count=0;
        for(int i=0; i<3; i++) {
            if (i!=1) {
                pnVRCB[4*count+0] = voxVRCB[0];
                pnVRCB[4*count+3] = voxVRCB[3]+transRule[i];
                pnVRCB[4*count+1] = voxVRCB[1];
                pnVRCB[4*count+2] = voxVRCB[2];
                count+=1;
            }
            for(int j=0; j<3; j++) {
                for(int k=0; k<2; k++) {
                    pnVRCB[4*count+3] = voxVRCB[3]+transRule[i];
                    pnVRCB[4*count+1] = voxVRCB[1]+transRule[j];
                    if (voxVRCB[1]%2 == 1) {
                        pnVRCB[4*count+2] = voxVRCB[2]+transRule[k+1];
                    }
                    else {
                        pnVRCB[4*count+2] = voxVRCB[2]+transRule[k];
                    }
                    pnVRCB[4*count+0] = voxVRCB[0];
                    count+=1;
                }
            }
        }
        
        /*Method to sift though possible neighbors and find those
         which are present in voxPortfolio. If a voxel in voxPortfolio
         is not a neighbor then its index in voxSift is turn to -1.
         Otherwise the index remains untouched and can be used to call
         the voxel from voxPortfolio. */
        vector<Voxel>::size_type portfolioSize;
        portfolioSize = voxPortfolio.size();
        
        int indSift[portfolioSize];
        for (int i=0; i<portfolioSize; i++) {
            indSift[i] = i;
        }
        
        for (int i=0; i<4; i++) {
            for (int j=0; j<portfolioSize; j++) {
                for (int k=0; k<20; k++) {
                    if (voxPortfolio[j].getVRCB()[i]==pnVRCB[4*k+i]) {
                        break;
                    }
                    else if (k==19) {
                        indSift[j] = -1;
                    }
                }
            }
        }
        int* pindSift = indSift;
        return pindSift;
    }
    
};

class eventDuct
{/*Simple class which is used solely to to house events safely.
  Events can be added to a container and if it's desired an event
  iterator begining at the specified index can be obtained.*/
public:
    
    eventDuct(int fPos = 0, char evntDlm = '#', int dlmInd = 0, int dlmThck = 1) {
        savedFilePosition = fPos;
        eventDelim = evntDlm;
        delimIndex = dlmInd;
        delimThickness = dlmThck;
    }
    
    void lugAllEvents(string File, vector<Event> &container) {
        string line;
        ifstream reader (File);
        if (reader.is_open()) {
            int i=-1;
            while (getline(reader,line)) {
                if (line[delimIndex] == eventDelim) {
                    if (i!=-1) {
                        //container[i].closePortfolio();
                    }
                    container.push_back(Event(0,0));
                    i++;
                }
                else {
                    std::istringstream iss(line);
                    int vrcb_adc[5];
                    
                    //while the iss is a number
                    int j=0;
                    while (iss >> vrcb_adc[j]) {j++;}
                    Voxel vox = Voxel(vrcb_adc[0],vrcb_adc[1],vrcb_adc[2],vrcb_adc[3],vrcb_adc[4]);
                    
                    container[i].addVoxel(vox);
                    //vector<Voxel> voxport = container[i].getPortfolio();
                    
                    
                }
            }
            reader.close();
        }
        else cout << "Unable to open " << File << endl;
    }
    
    void lugEvent(string File, Event &container) {
        string line;
        ifstream reader (File);
        if (reader.is_open()) {
            int linecount=0;
            while (linecount < savedFilePosition) {
                reader.ignore(numeric_limits<streamsize>::max(), '\n');
                linecount++;
            }
            
            while (getline(reader,line)) {
                if (line[delimIndex] == eventDelim) {
                    //container.closePortfolio();
                    savedFilePosition = linecount+delimThickness;
                    break;
                }
                else {
                    std::istringstream iss(line);
                    int vrcb_adc[5];
                    
                    //while the iss is a number
                    int j=0;
                    while (iss >> vrcb_adc[j]) {j++;}
                    Voxel vox = Voxel(vrcb_adc[0],vrcb_adc[1],vrcb_adc[2],vrcb_adc[3],vrcb_adc[4]);
                    //add voxel to container
                    container.addVoxel(vox);
                }
                linecount++;
            }
        }
        else {
            cout << "Unable to open " << File << endl;
        }
    }
    
    void setFilePosition (int line_number) {
        savedFilePosition = line_number;
    }
    
    int getFilePosition () {
        return savedFilePosition;
    }
    
private:
    int savedFilePosition;
    char eventDelim;
    int delimIndex;
    int delimThickness;
    
    
};



//================================================================================================
//================================================================================================



int main(int argc, const char * argv[])
{
    
    //test reading in data from text file
    eventDuct Pipe;
    Event event0(0,0);
    Event event1(0,0);
    vector<Event> EventCache;
    vector<Event> &rEventCache = EventCache;
    Pipe.setFilePosition(1);
    Pipe.lugEvent("/Users/RyanMorshead/Coding/repos/niffte-tracker3d/tracker3d_cpp/niffte_data.txt", event0);
    Pipe.lugEvent("/Users/RyanMorshead/Coding/repos/niffte-tracker3d/tracker3d_cpp/niffte_data.txt", event1);
    Pipe.lugAllEvents("/Users/RyanMorshead/Coding/repos/niffte-tracker3d/tracker3d_cpp/niffte_data.txt", rEventCache);
    cout<<Pipe.getFilePosition()<<", ";
    cout<<event0.getPortfolio().size()<<", ";
    cout<<event1.getPortfolio().size()<<"\n";
    
    
//------------------------------------------------------------------------------------------------
//------ Testing vector vs array sorting of neighbor voxels --------------------------------------
//------------------------------------------------------------------------------------------------
    
/*
    unsigned long neighVectCalcTime[18033];
    unsigned long neighArryCalcTime[18033];
    int testnum = 0;
    
    for (vector<Event>::iterator event = EventCache.begin();
         event != EventCache.end();
         ++event) {
        
        vector<Voxel> portfolio = event->getPortfolio();
        for (vector<Voxel>::iterator voxel = portfolio.begin();
             voxel != portfolio.end();
             ++voxel) {
            
            vector<Voxel>::size_type portfolioSize;
            portfolioSize = event->getPortfolio().size();
            
            //test: hexagonal to cartesian translation
            int* voxVRCB = voxel->getVRCB();
            int neighborsVRCB[80]; //will contain coordinates to voxels adjacent to voxVRCB
            int *pnVRCB= neighborsVRCB;
            
            int transRule[3] = {-1,0,1};
            
            int count=0;
            //if (stmpV[1]%2 == 1)
            for (int i=0; i<3; i++) {
                if (i!=1) {
                    pnVRCB[4*count+0] = voxVRCB[0];
                    pnVRCB[4*count+3] = voxVRCB[3]+transRule[i];
                    pnVRCB[4*count+1] = voxVRCB[1];
                    pnVRCB[4*count+2] = voxVRCB[2];
                    count+=1;
                }
                
                for (int j=0; j<3; j++) {
                    for (int k=0; k<2; k++) {
                        pnVRCB[4*count+3] = voxVRCB[3]+transRule[i];
                        pnVRCB[4*count+1] = voxVRCB[1]+transRule[j];
                        
                        if (voxVRCB[1]%2 == 1) {
                            pnVRCB[4*count+2] = voxVRCB[2]+transRule[k+1];
                        }
                        else {
                            pnVRCB[4*count+2] = voxVRCB[2]+transRule[k];
                        }
                        pnVRCB[4*count+0] = voxVRCB[0];
                        count+=1;
                    }
                }
            }
            
     
            for (int i=0; i<4; i++) {
                cout<<voxVRCB[i]<<" ";
            }
            cout<<"\n-------\n";
            
            for (int i=0; i<20; i++) {
                for (int j=0; j<4; j++) {
                    cout << pnVRCB[4*i+j] << " ";
                    if (j==3) {
                        cout << "\n-------\n";
                    }
                }
            }
     
            //------------------------------------------------------------------
            
            
            //test: voxel sifter for getNeighbors
             
            //test: using vector<Voxel>
            
            std::clock_t c_start1 = std::clock();
            vector<Voxel> voxSift = event->getPortfolio();
            
            for (int i=0; i<4; i++) {
                if (voxSift.size()!=0) {
                    int j=0;
                    int n=0;
                    while (j<voxSift.size()) {
                        for (int k=0; k<20; k++) {
                            if (voxSift[j].getVRCB()[i]==pnVRCB[4*k+i]) {
                                j++;
                                break;
                            }
                            else if (k==19) {
                                voxSift.erase(voxSift.begin() + j);
                                break;
                            }
                            n++;
                        }
                    }
                }
            }
            std::clock_t c_end1 = std::clock();
            neighVectCalcTime[testnum] = c_end1-c_start1;
            
            //---------------------------------
            
            
             
            //test: using int[n]
            std::clock_t c_start2 = std::clock();
            
            int indSift[portfolioSize];
            for (int i=0; i<portfolioSize; i++) {
                indSift[i] = i;
            }
            
            for (int i=0; i<4; i++) {
                for (int j=0; j<portfolioSize; j++) {
                    if (indSift[j] != -1) {
                        for (int k=0; k<20; k++) {
                            if (voxSift[j].getVRCB()[i]==pnVRCB[4*k+i]) {
                                break;
                            }
                            else if (k==19) {
                                indSift[j] = -1;
                                break;
                            }
                        }
                    }
                    ++j;
                }
            }
            
            std::clock_t c_end2 = std::clock();
            neighArryCalcTime[testnum] = c_end2-c_start2;
            
             
            //---------
            
            testnum++;
            
        }

    }
    cout<<testnum<<" iterations completed"<<endl;
    
    fstream histoWrite;
    histoWrite.open("/Users/RyanMorshead/Coding/repos/niffte-tracker3d/tracker3d_cpp/sifterTimes.txt", fstream::out | fstream::trunc);
    for (int i=0; i<18033; i++) {
        histoWrite << neighVectCalcTime[i] << "\t" << neighArryCalcTime[i] << endl;
    }
*/
    
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
    
    
    return 0;
}
