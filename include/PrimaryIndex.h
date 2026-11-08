#ifndef PRIMARYINDEX_H
#define PRIMARYINDEX_H

#include "doctor.h"
struct PrimaryIndexEntry
{
    char id[15];
    int RRN;
};

// Doctors
void buildDoctorPrimaryIndex();
int binarySearchDoctor(const char *doctorID);                       // RRN or -1
int insertDoctorPrimaryIndex(const char *doctorID, int RRN);        // 1 = added, 0 = invalid/duplicate
int removeDoctorPrimaryIndex(const char *doctorID);                 // 1 = removed, 0 = not found
int updateDoctorPrimaryIndex(const char *oldID, const char *newID); // 1 = updated, 0 = rejected
void displayDoctorPrimaryIndex();

// Appointments
void buildAppointmentPrimaryIndex();
int binarySearchAppointment(const char *appointmentID); // RRN or -1
bool insertAppointmentIndex(const char *appointmentID, int RRN);
bool removeAppointmentIndex(const char *appointmentID);
bool updateAppointmentKeyIndex(const char *oldID, const char *newID);
void displayAppointmentPrimaryIndex();

#endif