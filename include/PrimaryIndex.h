#ifndef PRIMARYINDEX_H
#define PRIMARYINDEX_H

#include "doctor.h"
struct PrimaryIndexEntry {
    char doctor_id[15];
    int RRN;
};
void buildDoctorPrimaryIndex();
int binarySearchDoctor(const char* doctorID);
void displayDoctorPrimaryIndex();

#endif