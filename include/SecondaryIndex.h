#ifndef SECONDARYINDEX_H
#define SECONDARYINDEX_H
#include "PrimaryIndex.h"
#include "doctor.h"
#include "AppointmentsFile.h"

struct SecondaryIndexEntry{
    char doctor_name[30];
    int headRRN;
};

struct SecondaryLinkedList{
  char doctor_id[15];
  int nextRRN;
};

struct SpecialityIndexEntry{
  char doctor_speciality[20];
  int headRRN;
};

struct SpecialityLinkedList{
  char doctor_id[15];
  int nextRRN;
};

struct ApptDoctorIndex{
  char doctor_id[15];
  int headRRN;
};

struct ApptDoctorLinkedList{
  char appointment_id[15];
  int nextRRN;
};


void insert_doctor(const char* name, const char* doctor_Id);
void insert_doctor_speciality(const char* speciality, const char* doctor_Id);
void insert_appointment(const char* doctor_Id,const char* appointment_id );

void buildDoctorNameIndex();
void buildDoctorSpeciality();
void buildDoctorAppointment();

int BinarySearchSec(const char* name);
int BinarySearchSpeciality(const char* speciality);
int BinarySearchAppt(const char* doctor_id);


bool remove_doctor(const char* name, const char* doctor_id);
bool remove_doctor_speciality(const char* speciality, const char* doctor_id);
bool remove_appointment_doctor(const char* doctor_id, const char* appointment_id);

void find_doctors_by_name(const char* name);
void find_by_speciality(const char* speciality);
void find_appointments_by_doctor(const char* doctor_id);

void update_doctor_name(const char* oldName, const char* newName, const char* doctor_Id);
void update_doctor_speciality(const char* oldSpec, const char* newSpec, const char* doctor_Id);
void update_appointment(const char* oldDoctorId, const char * newDoctorId, const char* appointment_Id);

void displayDoctorNameIndex();
void displayDoctorSpecialityIndex();
void displayApptDoctorIndex();


#define NAME_INDEX_FILE  "data/name_index.dat"
#define NAME_LINKED_FILE  "data/name_linked.dat"
#define SPECIALITY_INDEX_FILE "data/speciality_index.dat"
#define SPECIALITY_LINKED_FILE "data/speciality_linked.dat"
#define APPT_DOCTOR_INDEX_FILE "data/appt_doctor_index.dat"
#define APPT_DOCTOR_LINKED_FILE "data/appt_doctor_linked.dat"

#endif //SECONDARYINDEX_H
