//
//  main.cpp
//  tracker3d_cpp
//
//  Created by Ryan Morshead on 7/16/14.
//  Copyright (c) 2014 RyanMorshead. All rights reserved.
//

#include <iostream>
#include <istream>
#include <array>
#include <vector>
using namespace std;

//utlity functions:


class Voxel
{/*The basic data unit; represents pixels in 3 dimensions (two spatial and one temporal) and it's associated data.
    <data_member(abbreviation): description>
    voxStamp(vst): contains four values corrisponding to the chamber and coordinates of the voxel:
        {chamber, row (spatial), collumn(spatial), and bucket(temporal)}.
    acdValue(adc): The adc value is the magnitude of the signal recorded*/
public:
    Voxel(int vst[4], int adc)
    {
        voxStamp = vst;
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
    Event(int eventId)
    {
        Id = eventId;
    }
    
    void addVoxel(int voxArray[5])
    {
        int stamp[4] = {
            voxArray[0],
            voxArray[1],
            voxArray[2],
            voxArray[4]
        };
        voxPortfolio.push_back(Voxel(stamp, voxArray[5]));
    }
    
    
    
    
private:
    int Id;
    vector<Voxel> voxPortfolio;
    vector<Voxel> trajectories;
    vector<Voxel> orphans;
};







int main(int argc, const char * argv[])
{

}

