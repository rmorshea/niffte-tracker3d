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

class Voxel
{
//The basic data unit; represents pixels in 3 dimensions (two spatial and one temporal) and it's associated data.
// <data_member(abbreviation): description>
// chamberId(cId): a chamber identifier and isn't used much here.
// rcbArray(rcb): contains three values corrisponding to coordinates; row (spatial), collumn(spatial), and bucket(temporal).
// acdValue(adc): The adc value is the magnitude of the signal recorded
public:
    Voxel(int cId, int rcb[3], int adc)
    {
        chamberId = cId;
        rcbArray = rcb;
        adcValue = adc;
    }
    
    array<int,5> datReturn()
    {
        array<int,5> dat;
        for (int i; i<5; i++)
        {
            if (i==0)
                {dat[i]=chamberId;}
            if (i==4)
                {dat[i]=adcValue;}
            else
                {dat[i]=rcbArray[i];}
        }
        return dat;
    }
    
private:
    int adcValue;
    int chamberId;
    int *rcbArray;
};


class Event
{
public:
    Event(int eventId)
    {
        Id = eventId;
    }
    
    void addVoxel(int array[5])
    {
        int rcb[3] = {array[1],array[2],array[3]};
        voxPortfolio.push_back(Voxel(array[0], rcb, array[4]));
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

