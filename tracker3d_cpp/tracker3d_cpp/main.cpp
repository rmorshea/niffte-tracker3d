
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



class Voxel
{/*The basic data unit; represents pixels in 3 dimensions (two spatial and one temporal) and it's associated data.
    <data_member(abbreviation): description>
    voxStamp(stmp): contains four values corrisponding to the chamber and coordinates of the voxel:
        {chamber, row (spatial), collumn(spatial), and bucket(temporal)}.
    acdValue(adc): The adc value is the magnitude of the signal recorded*/
public:
    
    int *voxStamp;
    int adcValue;
    
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
//end class
};

class Event
{
public:
    //Public Data Members:
    int Id;
    vector<Voxel> voxPortfolio;
    vector<Voxel> trajectories;
    vector<Voxel> orphans;
    
    //Constructor:
    Event(int eventId, int voxArray[5]) {
        Id = eventId;
        this->addVoxel(voxArray);
    }
    
    //PublicMemberFunction(1):
    //adds a voxel to the public data member voxPortfolio.
    void addVoxel(int voxArray[5]) {
        int stamp[4] = {
            voxArray[0],
            voxArray[1],
            voxArray[2],
            voxArray[4]
        };
        voxPortfolio.push_back(Voxel(stamp, voxArray[5]));
    }
    
    //PublicMemberFunction(2):
    //using a specified voxStamp as a refernce point, a collection
    //of its neighboring voxels is made and then returned. Handling
    //the hexagonal geometry results in a maximum of 20 possible
    //neighbors. A sifter then determines how many of those 20
    //candidates are actually present in the public data member
    //voxPortfolio.
    vector<Voxel> getNeighbors(int voxStamp[4]) {
        int stampNeighbors[80];
        int *pstmpN= stampNeighbors;
        
        int transRule[3] = {-1,0,1};
        int count=0;
        
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
        //Method to sift though possible neighbors and find those
        //which are present in voxPortfolio
        vector<Voxel> voxSiftA;
        vector<Voxel> voxSiftB = voxPortfolio;
        for(int i; i<4; i++) {
            if(i==0 or i==2) {
                if(voxSiftB.size()!=0) {
                    for(int j; i<voxSiftA.size(); j++) {
                        for (int k; j<20; k++) {
                            if(*voxPortfolio[j].voxStamp==pstmpN[4*j+i]) {
                                voxSiftA.push_back(voxPortfolio[j]);
                            }
                        }
                    }
                }
                else {
                    return emptyBorder();
                }
                voxSiftB.clear();
            }
            if(i==1 or i==3) {
                if(voxSiftA.size()!=0) {
                    for(int j; i<voxSiftB.size(); j++) {
                        for (int k; j<20; k++) {
                            if(*voxPortfolio[j].voxStamp==pstmpN[4*j+i]) {
                                voxSiftA.push_back(voxPortfolio[j]);
                            }
                        }
                    }
                }
                voxSiftB.clear();
            }
            else {
                return emptyBorder();
            }
        }
        return voxSiftA;
    }
    
private:
    
    //PrivateMemberFunction(1):
    vector<Voxel> emptyBorder() {
        vector<Voxel> emptyNeighbors;
        int emptyStamp[4];
        emptyNeighbors.push_back(Voxel(emptyStamp,-1));
        return emptyNeighbors;
    }
    
//end class
};

//utlity functions:







int main(int argc, const char * argv[])
{
    
    //test: hexagonal to cartesian translation
    int voxStamp[4]={2,2,2,2};
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
                cout << "\n-------\n";
            }
        }
    }
    
    int vStamp[4] = {2,1,1,1};
    vector<Voxel> voxPortfolio = {Voxel(vStamp,1)};
    //test: voxel sifter for getNeighbors
    vector<Voxel> voxSiftA;
    vector<Voxel> voxSiftB = voxPortfolio;
    
    for(int i=0; i<4; i++) {
        if(i==0 or i==2) {
            cout<<voxSiftB.size()<<"\n";
            if(voxSiftB.size()!=0) {
                for(int j=0; j<voxSiftB.size(); j++) {
                    int k=0;
                    cout<<voxSiftB[j].voxStamp[i]<<"=="<<pstmpN[4*k+i]<<"\n";
                    if(voxSiftB[j].voxStamp[i]==pstmpN[4*k+i]) {
                        voxSiftA.push_back(voxSiftB[j]);
                    }
                    else {
                        k+=1;
                        while (voxSiftB[j].voxStamp[i]!=pstmpN[4*k+i]) {
                            cout<<voxSiftB[j].voxStamp[i]<<"=="<<pstmpN[4*k+i]<<"\n";
                            if(voxSiftB[j].voxStamp[i]==pstmpN[4*k+i]) {
                                voxSiftA.push_back(voxSiftB[j]);
                            }
                            k+=1;
                        }
                    }
                }
            }
            else {
                cout<<"NO NEIGHBORS\n";
            }
        }
        voxSiftB.clear();
        
        if(i==1 or i==3) {
            cout<<voxSiftA.size()<<"\n";
            if(voxSiftA.size()!=0) {
                for(int j=0; j<voxSiftA.size(); j++) {
                    int k=0;
                    cout<<voxSiftA[j].voxStamp[i]<<"=="<<pstmpN[4*k+i]<<"\n";
                    
                    if(voxSiftA[j].voxStamp[i]==pstmpN[4*k+i]) {
                        voxSiftB.push_back(voxSiftB[j]);
                    }
                    else {
                        k+=1;
                        while (voxSiftA[j].voxStamp[i]!=pstmpN[4*k+i]) {
                            cout<<voxSiftA[j].voxStamp[i]<<"=="<<pstmpN[4*k+i]<<"\n";
                            if(voxSiftA[j].voxStamp[i]==pstmpN[4*k+i]) {
                                voxSiftB.push_back(voxSiftA[j]);
                            }
                            k+=1;
                        }
                    }
                }
            }
            else {
                cout<<"NO NEIGHBORS\n";
            }
        }
        voxSiftA.clear();
    }
}
