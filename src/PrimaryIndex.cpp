#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include "PrimaryIndex.h"
#include "doctor.h"

using namespace std;
//array to hold the primary index entries for doctors
PrimaryIndexEntry doctorIndex[100];

int doctorIndexCount =0;

void buildDoctorPrimaryIndex(){
    ifstream doctorFile(DOCTOR_FILE, ios::binary);
    
    if(!doctorFile.is_open())
    {
        cout<<"Error Opening Doctors File"<<endl;
        return;
    }
    //read the header record
    header h;
    doctorFile.read((char*)&h,sizeof(header));

    cout << "File opened: " << doctorFile.is_open() << endl;
    cout << "Header size: " << sizeof(header) << endl;
    cout << "Doctor size: " << sizeof(Doctor) << endl;

    //read the doctor records and build the primary index
    Doctor d;
    int RRN=0;

    doctorIndexCount=0;

    while(doctorFile.read((char*)&d,sizeof(Doctor))){
        //skip logically deleted records
        if(d.doctor_id[0]!='*'){
            strcpy(doctorIndex[doctorIndexCount].doctor_id,d.doctor_id);
            doctorIndex[doctorIndexCount].RRN=RRN;
            doctorIndexCount++;
        }
        cout << "Records read: " << RRN << endl;
        RRN++;
    }

    doctorFile.close();

//sort the index array based on doctor_id
    sort(doctorIndex,doctorIndex+doctorIndexCount,
        [](const PrimaryIndexEntry &a,const PrimaryIndexEntry &b){
        return strcmp(a.doctor_id,b.doctor_id)<0;
    });
    cout<<"Doctors Primary Index Built Successfully"<<endl;

}
//Binary search
int binarySearchDoctor(const char* doctorID){
    int left=0;
    int right=doctorIndexCount-1;

    while(left<=right){

        int mid= left + (right - left)/2;

        int result = strcmp(doctorIndex[mid].doctor_id, doctorID);

        if(result==0){
            return doctorIndex[mid].RRN;
        }
        else if(result<0){
            left=mid+1;
        }
        else{
            right=mid-1;
        }
    }
    return -1;
}

//display all entries in doctor file primary index
void displayDoctorPrimaryIndex(){
    cout<<"\n---Doctors Primary Index---\n";

    if(doctorIndexCount==0)
    {
        cout<<"Primary index is empty"<<endl;
        return;
    }

    for(int i=0;i <doctorIndexCount;i++){
        cout<<"Doctor ID: "<<doctorIndex[i].doctor_id
        <<" | RRN: "<<doctorIndex[i].RRN <<endl;
    }
}