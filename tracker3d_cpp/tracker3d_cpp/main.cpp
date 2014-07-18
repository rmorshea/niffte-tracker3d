
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
using namespace std;

//utlity functions:


class Voxel
{/*The basic data unit; represents pixels in 3 dimensions (two spatial and one temporal) and it's associated data.
    <data_member(abbreviation): description>
    voxStamp(stmp): contains four values corrisponding to the chamber and coordinates of the voxel:
        {chamber, row (spatial), collumn(spatial), and bucket(temporal)}.
    acdValue(adc): The adc value is the magnitude of the signal recorded*/
public:
    Voxel(int stmp[4], int adc)
    {
        voxStamp = stmp;
        adcValue = adc;
    }
    
    int *getVoxDat()
    {
        int stmpArray[5];
        int* pStamp=stmpArray;
        for (int i; i<4; i++)
        {
            pStamp[i] = voxStamp[i];
        }
        pStamp[4]=adcValue;
        return pStamp;
    }
    
private:
    int *voxStamp;
    int adcValue;
};

class Event
{
public:
    Event(int eventId) {
        Id = eventId;
    }
    
    void addVoxel(int voxArray[5]) {
        int stamp[4] = {
            voxArray[0],
            voxArray[1],
            voxArray[2],
            voxArray[4]
        };
        voxPortfolio.push_back(Voxel(stamp, voxArray[5]));
    }
    
    //!temporary change!
    /*vector<Voxel>*/ int getNeighbors(int voxStamp[4])
    //!temporary change!
    {
        int stampNeighbors[80];
        int *pstmpN= stampNeighbors;
        
        int transRule[3] = {-1,0,1};
        
        int count=0;
        //if (stmpV[1]%2 == 1)
        for(int i=0; i<3; i++) {
            if (i!=1) {
                pstmpN[4*count+0] = voxStamp[0];
                pstmpN[4*count+3] = voxStamp[3]+transRule[i];
                pstmpN[4*count+1] = voxStamp[1];
                pstmpN[4*count+2] = voxStamp[2];
                count+=1;
            }
            
            for(int j=0; j<3; j++) {
                for(int k=0; k<2; k++) {
                    pstmpN[4*count+3] = voxStamp[3]+transRule[i];
                    pstmpN[4*count+1] = voxStamp[1]+transRule[j];
                    
                    if (voxStamp[1]%2 == 1) {
                        pstmpN[4*count+2] = voxStamp[2]+transRule[k+1];
                    }
                    else {
                        pstmpN[4*count+2] = voxStamp[2]+transRule[k];
                    }
                    pstmpN[4*count+0] = voxStamp[0];
                    count+=1;
                }
            }
        }
        
        //!temporary change!
        return 0;
        //!temporary change!
    }
        
        
    
    
private:
    int Id;
    vector<Voxel> voxPortfolio;
    vector<Voxel> trajectories;
    vector<Voxel> orphans;
};







int main(int argc, const char * argv[])
{
    int voxStamp[4]={3,3,3,3};
    int stampNeighbors[80];
    int *pstmpN= stampNeighbors;
    
    int transRule[3] = {-1,0,1};
    
    int count=0;
    //if (stmpV[1]%2 == 1)
    for(int i=0; i<3; i++) {
        if (i!=1) {
            pstmpN[4*count+0] = voxStamp[0];
            pstmpN[4*count+3] = voxStamp[3]+transRule[i];
            pstmpN[4*count+1] = voxStamp[1];
            pstmpN[4*count+2] = voxStamp[2];
            count+=1;
        }
        
        for(int j=0; j<3; j++) {
            for(int k=0; k<2; k++) {
                pstmpN[4*count+3] = voxStamp[3]+transRule[i];
                pstmpN[4*count+1] = voxStamp[1]+transRule[j];
                
                if (voxStamp[1]%2 == 1) {
                    pstmpN[4*count+2] = voxStamp[2]+transRule[k+1];
                }
                else {
                    pstmpN[4*count+2] = voxStamp[2]+transRule[k];
                }
                pstmpN[4*count+0] = voxStamp[0];
                count+=1;
            }
        }
    }
    
    for (int i=0; i<20; i++) {
        for(int j=0; j<4; j++) {
            cout << pstmpN[4*i+j] << " ";
            if (j==3) {
                cout << "\n";
            }
        }
    }


}

