#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include<iostream>
using namespace std;

#define APPOINTMENT_FILE "data/appointments.dat"

struct appointment_header { 
	int avail; 
};

struct Appointment {
	char appointment_id[15];
	char appointment_date[30];
	char doctor_id[15];
};

static_assert(sizeof(Appointment) == 60, "Appointment struct size is not 60 bytes");

void initialize_appointment_file();
int Add_appointment(Appointment a);
bool is_appointment_id_exists(const char* appointment_id);
void search_appointment_by_id(const char* appointment_id);
bool update_appointment_date(const char* appointment_id,const char* new_date);
int delete_appointment_by_id(const char* appointment_id);
void display_all_appointments();


#endif 