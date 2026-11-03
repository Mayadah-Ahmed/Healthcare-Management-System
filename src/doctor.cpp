#include <iostream>
#include <fstream>
#include <cstring>
#include "doctor.h"
#include <filesystem>
using namespace std;


void initializedoctor_flie() {

    if (filesystem::exists("data/doctors.dat")) {
        cout << "Doctors file already exists. Initialization skipped." << endl;
        return;
    }

    ofstream doctor_file("data/doctors.dat", ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Error: Could not create doctors file." << endl;
        return;
    }

    header h;
    h.avail = -1;

    doctor_file.write((char*)&h, sizeof(h));

    if (!doctor_file.good()) {
        cout << "Error: Failed to initialize doctors file." << endl;
        doctor_file.close();
        return;
    }

    doctor_file.close();

    cout << "Doctors file initialized successfully." << endl;
}
bool is_doctor_id_exists(const char* doctor_id) {
    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);
    header h;
    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));
    Doctor d;
    while (doctor_file.read((char*)&d, sizeof(Doctor))) {
        if (d.doctor_id[0] == '*') {
            continue;
        }
        if (strcmp(d.doctor_id, doctor_id) == 0) {
            return true;
        }
    }
    return false;
}

void Add_doctor(Doctor d) {

    fstream doctor_file("data/doctors.dat", ios::in | ios::out | ios::binary);

    if (is_doctor_id_exists(d.doctor_id)) {
        cout << "Doctor with ID " << d.doctor_id << " already exists." << endl;
        return;
    }

    header h;

    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    int avail_position = h.avail;

    if (h.avail == -1) {

        doctor_file.seekp(0, ios::end);
        doctor_file.write((char*)&d, sizeof(Doctor));

    }
    else {

        Doctor deletedRecord;

        doctor_file.seekg(sizeof(header) + avail_position * sizeof(Doctor));
        doctor_file.read((char*)&deletedRecord, sizeof(Doctor));

        h.avail = deletedRecord.next;

        doctor_file.seekp(0);
        doctor_file.write((char*)&h, sizeof(h));

        doctor_file.seekp(sizeof(header) + avail_position * sizeof(Doctor));
        doctor_file.write((char*)&d, sizeof(Doctor));

    }

};

void search_doctor_by_id(const char* doctor_id) {

    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);
    header h;

    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    Doctor d;
    int i = 0;

    while (doctor_file.read((char*)&d, sizeof(Doctor))) {
        if (d.doctor_id[0] == '*') {
            i++;
            continue;
        }
        if (strcmp(d.doctor_id, doctor_id) == 0) {

            cout << "Doctor ID: " << d.doctor_id << endl;
            cout << "Doctor Name: " << d.doctor_name << endl;
            cout << "Doctor Speciality: " << d.doctor_speciality << endl;
            cout << "Address: " << d.address << endl;
            return;
        }
        i++;
    }
    cout << "Doctor not found." << endl;
}

void search_doctor_by_name(const char* doctor_name) {

    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);

    header h;

    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    Doctor d;
    int i = 0;

    while (doctor_file.read((char*)&d, sizeof(Doctor))) {
        if (d.doctor_id[0] == '*') {
            i++;
            continue;
        }

        if (strcmp(d.doctor_name, doctor_name) == 0) {
            cout << "Doctor ID: " << d.doctor_id << endl;
            cout << "Doctor Name: " << d.doctor_name << endl;
            cout << "Doctor Speciality: " << d.doctor_speciality << endl;
            cout << "Address: " << d.address << endl;
            return;
        }
        i++;
    }
    cout << "Doctor not found." << endl;
}

void search_doctor_by_speciality(const char* doctor_speciality) {

    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);

    header h;

    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    Doctor d;
    int i = 0;

    while (doctor_file.read((char*)&d, sizeof(Doctor))) {
        if (d.doctor_id[0] == '*') {
            i++;
            continue;
        }

        if (strcmp(d.doctor_speciality, doctor_speciality) == 0) {
            cout << "Doctor ID: " << d.doctor_id << endl;
            cout << "Doctor Name: " << d.doctor_name << endl;
            cout << "Doctor Speciality: " << d.doctor_speciality << endl;
            cout << "Address: " << d.address << endl;
            return;
        }
        i++;
    }
    cout << "Doctor not found." << endl;
}

void update_doctor_by_id(const char* doctor_id, Doctor updated_doctor)
{
    fstream doctor_file("data/doctors.dat", ios::in | ios::out | ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Error opening Doctors file." << endl;
        return;
    }

    Doctor d;
    int i = 0;

    doctor_file.seekg(sizeof(header));

    while (doctor_file.read((char*)&d, sizeof(Doctor))) {

        if (d.doctor_id[0] == '*') {
            i++;
            continue;
        }

        if (strcmp(d.doctor_id, doctor_id) == 0) {

            doctor_file.seekp(sizeof(header) + i * sizeof(Doctor));

            doctor_file.write((char*)&updated_doctor, sizeof(Doctor));

            cout << "Doctor updated successfully." << endl;

            doctor_file.close();
            return;
        }

        i++;
    }

    cout << "Doctor not found." << endl;

    doctor_file.close();
}

void delete_docotor_by_id(const char* doctor_id) {

    fstream doctor_file("data/doctors.dat", ios::in | ios::out | ios::binary);

    header h;

    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    Doctor d;
    int i = 0;

    while (doctor_file.read((char*)&d, sizeof(Doctor))) {

        if (d.doctor_id[0] == '*') {
            i++;
            continue;
        }

        if (strcmp(d.doctor_id, doctor_id) == 0) {
            d.next = h.avail;
            h.avail = i;
            d.doctor_id[0] = '*';

            doctor_file.seekp(0);
            doctor_file.write((char*)&h, sizeof(h));

            doctor_file.seekp(sizeof(header) + i * sizeof(Doctor));
            doctor_file.write((char*)&d, sizeof(Doctor));

            cout << "Doctor deleted successfully." << endl;
            return;
        }

        i++;
    }

    cout << "Doctor not found." << endl;
}

void display_all_doctors() {

    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);

    header h;

    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    Doctor d;
    int i = 0;

    while (doctor_file.read((char*)&d, sizeof(Doctor))) {
        if (d.doctor_id[0] == '*') {
            i++;
            continue;
        }

        cout << "Doctor ID: " << d.doctor_id << endl;
        cout << "Doctor Name: " << d.doctor_name << endl;
        cout << "Doctor Speciality: " << d.doctor_speciality << endl;
        cout << "Address: " << d.address << endl;
        cout << endl;

        i++;
    }
}
