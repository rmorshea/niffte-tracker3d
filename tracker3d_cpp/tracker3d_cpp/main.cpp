
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
    Event(int gradThresh, int dirThresh, bool touch=true) {
        //eventId determined by index position in eventContainer
        //gradThresh initializes the gradient threshhold
        //dirThresh initialize the directional threshhold
    }
    
    void addVoxel(Voxel vox) {
        if (touch==true) {
            voxPortfolio.push_back(vox);
        }
    }
    
    void closePortfolio() {
        touch = false;
    }
    
    vector<Voxel>::const_iterator getPortfolioIter(int index=0) {
        return voxPortfolio.begin() + index;
    }
    
    vector<Voxel> getPortfolio(int index=0) {
        return voxPortfolio;
    }
    
    void makeTrajectories() {
        
        
    }
    
private:
    
    //Private Data Members:
    vector<Voxel> voxPortfolio;
    bool touch;
    int gradThresh;
    int dirThresh;
    int Id;
    
    /*PrivateMemberFunction(1):
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
    void lugAllEvents(string File, vector<Event>& container) {
        string line;
        char eventDelim = '#';
        ifstream reader (File);
        if (reader.is_open()) {
            int i=-1;
            while (getline(reader,line)) {
                if (line[0] == eventDelim ) {
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
    
    void lugEvent(string File, Event& container) {
        //to be written...
    }
    
};



//================================================================================================
//================================================================================================



int main(int argc, const char * argv[])
{
    
    //test reading in data from text file
    eventDuct Pipe;
    vector<Event> EventCache;
    vector<Event>& rEventCache = EventCache;
    
    Pipe.lugAllEvents("/Users/RyanMorshead/Coding/repos/niffte-tracker3d/tracker3d_cpp/niffte_data.txt", rEventCache);
    
    /*
    cout<<EventCache.size()<<endl;
    cout<<EventCache[1].getPortfolio().size()<<endl;
    vector<Event>::iterator event = EventCache.begin();
    vector<Voxel>::iterator voxel = event->getPortfolioIter();
    
    voxel++;voxel++;
    
    for (int i=0; i<4; i++) {
        cout<<voxel->getVRCB()[i]<<" ";
    }
    
    cout<<endl<<voxel->getADC()<<endl;
    */
    
//------------------------------------------------------------------------------------------------
    //unsigned long neighVectCalcTime[50000];
    //unsigned long neighArryCalcTime[50000];
    int testnum = 0;
    
    for (vector<Event>::iterator event = EventCache.begin();
         event != EventCache.end();
         ++event) {
        int inner_testnum = 0;
        vector<Voxel> mylist = event->getPortfolio();
        for (vector<Voxel>::iterator voxel = mylist.begin();
             voxel != mylist.end();
             ++voxel) {
            
            /*
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
            cout<<(c_end1-c_start1)<<"\n";
            */
            
            //neighVectCalcTime[testnum] = c_end1-c_start1;
            
            //---------------------------------
            
            
             
            //test: using int[n]
            //std::clock_t c_start2 = std::clock();
            /*
            vector<Voxel> voxSift = event->getPortfolio();
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
            */
            //std::clock_t c_end2 = std::clock();
            
            //neighArryCalcTime[testnum] = c_end2-c_start2;
            
             
            //---------
            
            //cout<<testnum<<", ";//cout<<indSift[portfolioSize-1]<<"::"<<testnum<<", ";
            inner_testnum++;
            
        }
        cout<<"Inner: "<<inner_testnum<<", "<<endl;
        testnum++;
    }
    cout<<"Outer:"<<testnum<<", "<<endl;
    //------------------------------------------------------------------
    return 0;
}
