
//
//  main.cpp
//  tracker3d_cpp
//
//  Created by Ryan Morshead on 7/16/14.
//  Copyright (c) 2014 RyanMorshead. All rights reserved.
//

#include <iostream>
#include <istream>
#include <stdio.h>
#include <array>
#include <vector>
#include <chrono>

using namespace std;

class Voxel
{/*The basic data unit; represents pixels in 3 dimensions (two spatial and one temporal) and it's associated data.
  <data_member: description>
  VRCB: an array of four values corrisponding to the volume and coordinates of the voxel:
    {volume (chamber), row (spatial), collumn(spatial), and bucket(temporal)}.
  ADC: The adc value is the magnitude of the signal recorded. */
public:
    Voxel(int vol=-1, int col=-1, int row=-1, int bkt=-1, int adc=-1)
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
    
private:
    int VRCB[4];
    int ADC;
};



class Event
{
public:
    
    //Public Data Members:
    
    //Constructor:
    Event(int eventId, int gradThresh, int dirThresh, vector<Voxel> voxPortfolio) {
        //eventId gives the event an Id number
        //gradThresh initializes the gradient threshhold
        //dirThresh initialize the directional threshhold
        //voxPortfolio contains all voxels in the event
    }
    
    
    
private:
    
    //Private Data Members:
    int gradThresh;
    int dirThresh;
    int Id;
    vector<Voxel> voxPortfolio;
    
    /*PrivateMemberFunction(1):
     using a specified voxVRCB as a refernce point, a collection
     of its neighboring voxels is made and then returned. Handling
     the hexagonal geometry results in a maximum of 20 possible
     neighbors. A sifter then determines how many of those 20
     candidates are actually present in the public data member
     voxPortfolio.*/
    int* getNeighbors(int voxVRCB[4]) {
        int neighborsVRCB[80]; //will contain coordinates to voxels adjacent to voxVRCB
        int *pnVRCB= neighborsVRCB;
        
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
    
    
    
//end class
};

//utlity functions:







int main(int argc, const char * argv[])
{
    
    vector<Voxel> voxPortfolio = {Voxel(2,1,1,1,1),Voxel(2,4,2,3,2)};
    
    //test: hexagonal to cartesian translation
    int voxVRCB[4]={2,2,2,2};
    int neighborsVRCB[80]; //will contain coordinates to voxels adjacent to voxVRCB
    int *pnVRCB= neighborsVRCB;
    
    int transRule[3] = {-1,0,1};
    
    int count=0;
    //if (stmpV[1]%2 == 1)
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
    
    for (int i=0; i<20; i++) {
        for(int j=0; j<4; j++) {
            cout << pnVRCB[4*i+j] << " ";
            if (j==3) {
                cout << "\n-------\n";
            }
        }
    }
    cout<<"\n";
    
    vector<Voxel>::size_type portfolioSize;
    portfolioSize = voxPortfolio.size();
    
    std::clock_t c_start1 = std::clock();
    //test: voxel sifter for getNeighbors
    vector<Voxel> voxSift = voxPortfolio;
    
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
    cout<<voxSift.size()<<"\n";
    cout<<(c_end1-c_start1)<<"\n\n";
    
    std::clock_t c_start2 = std::clock();
    int indSift[portfolioSize];
    for (int i=0; i<portfolioSize; i++) {
        indSift[i] = i;
    }
    
    for (int i=0; i<4; i++) {
        int j=-1;
        for (vector<Voxel>::iterator voxel = voxPortfolio.begin();
             voxel!=voxPortfolio.end();
             ++voxel) {
            ++j;
            if (indSift[j] != -1) {
                for (int k=0; k<20; k++) {
                    if (voxel->getVRCB()[i]==pnVRCB[4*k+i]) {
                        break;
                    }
                    else if (k==19) {
                        indSift[j] = -1;
                    }
                }
            }
        }
    }
    
    std::clock_t c_end2 = std::clock();
    cout<<(c_end2-c_start2)<<"\n";
    
    for (int i=0; i<portfolioSize; ++i) {
        cout<<indSift[i]<<", ";
    }
    cout<<"\n";
}
