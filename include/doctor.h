
#ifndef DOCTOR_H
#define DOCTOR_H

#include <iostream>
using namespace std;

#define DOCTOR_FILE "data/Doctors.dat"

struct header {
    int avail;
};

struct Doctor {
    char doctor_id[15];
    char doctor_name[30];
    char doctor_speciality[20];
    char address[30];
    int next;
};


void initializedoctor_flie();


bool is_doctor_id_exists(const char* doctor_id);


void Add_doctor(Doctor d);

void search_doctor_by_id(const char* doctor_id);
void search_doctor_by_name(const char* doctor_name);
void search_doctor_by_speciality(const char* doctor_speciality);


void update_doctor_by_id(const char* doctor_id, Doctor updated_doctor);

void delete_docotor_by_id(const char* doctor_id);

void display_all_doctors();

#endif
