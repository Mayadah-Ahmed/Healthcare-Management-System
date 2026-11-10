#include<iostream>
#include<fstream>
#include<cstring>
#include<filesystem>
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"
#include "AppointmentsFile.h"
using namespace std;



void initialize_appointment_file() {

	if (filesystem::exists(APPOINTMENT_FILE)) {
		cout << "Appointment file already exists." << endl;
		return;
	}

	filesystem::create_directories("data");
	ofstream appointment_file(APPOINTMENT_FILE, ios::binary);

	if (!appointment_file.is_open()) {
		cout << "error: could not create appointments file." << endl;
		return;
	}
	appointment_header h;
	h.avail =-1;
	appointment_file.write((char*)&h,sizeof(h));
	appointment_file.close();
	cout <<"appointments file initialized sucessfully"<<endl;
}




bool is_appointment_id_exists(const char* appointment_id) {
	//search in the primary index
	if (binarySearchAppointment(appointment_id) == -1)
		return false;
	else
		return true;
}




int Add_appointment(Appointment a) {

	if (a.appointment_id[0] == '\0' || a.appointment_id[0] == '*') {
		cout << "Invalid appointment ID." << endl;
		return -1;
	}

	if (is_appointment_id_exists(a.appointment_id)) {
		cout << "Appointment with ID " << a.appointment_id << " already exists." << endl;
		return -1;
	}

	fstream appointment_file(APPOINTMENT_FILE, ios::in | ios::out | ios::binary);

	appointment_header h;
	appointment_file.seekg(0);
	appointment_file.read((char*)&h, sizeof(h));

	int rrn;

	if (h.avail == -1) {

		appointment_file.seekp(0, ios::end);
		rrn = (int)(((long)appointment_file.tellp() - (long)sizeof(appointment_header)) / sizeof(Appointment));
		appointment_file.write((char*)&a, sizeof(Appointment));

	}
	else {

		rrn = h.avail;

		int next;
		appointment_file.seekg(sizeof(appointment_header) + rrn * sizeof(Appointment) + 1);
		appointment_file.read((char*)&next, sizeof(int));
		h.avail = next;

		appointment_file.seekp(0);
		appointment_file.write((char*)&h, sizeof(h));

		appointment_file.seekp(sizeof(appointment_header) + rrn * sizeof(Appointment));
		appointment_file.write((char*)&a, sizeof(Appointment));
	}

	appointment_file.close();

	//add the appointment to the primary index
	insertAppointmentIndex(a.appointment_id, rrn);

	//add the appointment to doctor id secondary index
	insert_appointment(a.doctor_id, a.appointment_id);

	return rrn;
}




bool update_appointment_date(const char* appointment_id, const char* new_date){
	if (strlen(new_date) > 30) {
		cout << "Date is longer than 30 characters" << endl;
		return false;
	}

	//search in primary index
	int rrn = binarySearchAppointment(appointment_id);

	if (rrn == -1) {
		cout << "Appointment not found" << endl;
		return false;
	}
	fstream appointment_file(APPOINTMENT_FILE, ios::in | ios::out | ios::binary);

	char date[30];
	strncpy(date, new_date, sizeof(date));

	appointment_file.seekp(sizeof(appointment_header) + rrn * sizeof(Appointment) + sizeof(Appointment::appointment_id));
	appointment_file.write(date, sizeof(date));
	appointment_file.close();

	cout << "Appointment updated successfully" << endl;
	return true;

}




void search_appointment_by_id(const char* appointment_id) {
	
	//search in the primary index
	int rrn = binarySearchAppointment(appointment_id);

	if (rrn == -1) {
		cout << "Appointment not found" << endl;
		return;
	}

	fstream appointment_file(APPOINTMENT_FILE, ios::in | ios::binary);

    Appointment a;
	appointment_file.seekg(sizeof(appointment_header) + rrn * sizeof(Appointment));
	appointment_file.read((char*)&a, sizeof(Appointment));

	cout << "Appointment ID: " << a.appointment_id << endl;
	cout << "Appointment Date: " << a.appointment_date << endl;
	cout << "Doctor ID: " << a.doctor_id << endl;

	appointment_file.close();

	}




int delete_appointment_by_id(const char* appointment_id) {

	//get rrn from the primary index
	int rrn = binarySearchAppointment(appointment_id);

	if (rrn == -1) {
		cout << "warning: appointment not found" << endl;
		return -1;
	}

	fstream appointment_file(APPOINTMENT_FILE, ios::in | ios::out | ios::binary);

	Appointment a;
	appointment_file.seekg(sizeof(appointment_header) + rrn * sizeof(Appointment));
	appointment_file.read((char*)&a, sizeof(Appointment));

	appointment_header h;
	appointment_file.seekg(0);
	appointment_file.read((char*)&h, sizeof(h));

	int old_head = h.avail;
	h.avail = rrn;

	appointment_file.seekp(0);
	appointment_file.write((char*)&h, sizeof(h));

	char marker = '*';
	appointment_file.seekp(sizeof(appointment_header) + rrn * sizeof(Appointment));
	appointment_file.write(&marker, 1);
	appointment_file.write((char*)&old_head, sizeof(int));
	appointment_file.close();

	//remove the appointment from primary index
	removeAppointmentIndex(a.appointment_id);

	//remove the appointment from the doctor id secondary index
	remove_appointment_doctor(a.doctor_id, a.appointment_id);

	cout << "Appointment deleted successfully" << endl;
	return rrn;
}




void display_all_appointments() {

		fstream appointment_file(APPOINTMENT_FILE, ios::in | ios::binary);

		appointment_header h;
		appointment_file.seekg(0);
		appointment_file.read((char*)&h, sizeof(h));

		Appointment a;

		while (appointment_file.read((char*)&a, sizeof(Appointment))) {
			if (a.appointment_id[0] == '*') {
				continue;
			}

			cout << "Appointment ID: " << a.appointment_id << endl;
			cout << "Appointment Date: " << a.appointment_date << endl;
			cout << "Doctor ID: " << a.doctor_id << endl;
			cout << endl;
		}
	}