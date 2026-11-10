#include <iostream>
#include <fstream>
#include <cstring>
#include "doctor.h"
#include "SecondaryIndex.h"
#include <filesystem>
#include<PrimaryIndex.h>
using namespace std;

void initializedoctor_flie() {

    filesystem::create_directories("data");

    if (filesystem::exists(DOCTOR_FILE)) {
        cout << "Doctors file already exists." << endl;
        return;
    }

    ofstream doctor_file(DOCTOR_FILE, ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Error: Could not create doctors file." << endl;
        return;
    }

    Doctor_header h;
    h.avail = -1;

    doctor_file.write((char*)&h, sizeof(h));
    doctor_file.close();

    cout << "Doctors file initialized successfully." << endl;
}

bool is_doctor_id_exists(const char* doctor_id) {
    //Search in the primary index file
    if (binarySearchDoctor(doctor_id) == -1)
        return false;

    else
        return true;

}

void Add_doctor(Doctor d) {
    // Validate the doctor ID
    if (d.doctor_id[0] == '\0' || d.doctor_id[0] == '*') {
        cout << "Invalid doctor ID." << endl;
        return;
    }
    // if the doctor already exists in the primary index file, we cannot add it again
    if (is_doctor_id_exists(d.doctor_id)) {
        cout << "Doctor with ID " << d.doctor_id << " already exists." << endl;
        return;
    }


    fstream doctor_file(DOCTOR_FILE, ios::in | ios::out | ios::binary);
    Doctor_header h;

    // Read the header to get the current avail list
    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    // Get the position to insert the new doctor
    int avail_position = h.avail;
    int rrn;

    if (h.avail == -1) {

        doctor_file.seekp(0, ios::end);
        rrn = (int)(((long)doctor_file.tellp() - (long)sizeof(Doctor_header)) / sizeof(Doctor));
        doctor_file.write((char*)&d, sizeof(Doctor));

    }
    else {

        rrn = h.avail;

        int next;

        doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor) + 1);
        doctor_file.read((char*)&next, sizeof(int));

        h.avail = next;

        doctor_file.seekp(0);
        doctor_file.write((char*)&h, sizeof(h));

        doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));
        doctor_file.write((char*)&d, sizeof(Doctor));


    }
    // Add the doctor to the primary index
    insertDoctorPrimaryIndex(d.doctor_id, rrn);

    //// Add doctor to the secandary index with name 
    insert_doctor(d.doctor_name, d.doctor_id);

    //// Add doctor to the secandary index with speciality
    insert_doctor_speciality(d.doctor_speciality, d.doctor_id);

};

void delete_doctor_by_id(const char* doctor_id) {

    // get the rrn of the doctor to be deleted from the primary index
    int rrn = binarySearchDoctor(doctor_id);

    if (rrn == -1)
        cout << "Doctor not found ";
    else {

        fstream doctor_file("data/doctors.dat", ios::in | ios::out | ios::binary);

        Doctor d;

        // Read the doctor BEFORE deleting the record
        doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));
        doctor_file.read((char*)&d, sizeof(Doctor));

        // header
        Doctor_header h;

        // read current header
        doctor_file.seekg(0);
        doctor_file.read((char*)&h, sizeof(h));

        // Save the old head of the free list
        int old_head = h.avail;

        // Make the deleted record the new head
        h.avail = rrn;

        // Update the avail list --> make the deleted record the first in the avail list
        doctor_file.seekp(0);
        doctor_file.write((char*)&h, sizeof(h));

        char marker = '*';

        doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));
        doctor_file.write(&marker, 1);
        doctor_file.write((char*)&old_head, sizeof(int));

        // Remove the doctor from Primary Index
        removeDoctorPrimaryIndex(doctor_id);

        // Remove the doctor from Secondary Indexes
        remove_doctor(d.doctor_name, d.doctor_id);

        // Remove the doctor from Secondary Indexes
        remove_doctor_speciality(d.doctor_speciality, d.doctor_id);


        cout << "Doctor deleted successfully." << endl;
        doctor_file.close();
    }

}

void search_doctor_by_id(const char* doctor_id) {

    // search in the primary index file
    int rrn = binarySearchDoctor(doctor_id);

    // Check if the doctor exists.
    if (rrn == -1) {
        cout << "Doctor Not Found" << endl;
        return;
    }

    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);

    //if file cannot open 
    if (!doctor_file.is_open()) {
        cout << "Cannot open file." << endl;
        return;
    }

    Doctor d;

    // Move the reading pointer to the required record using its RRN.
    doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    // Read the doctor's record from the data file.
    doctor_file.read((char*)&d, sizeof(Doctor));

    cout << "Doctor ID: " << d.doctor_id << endl;
    cout << "Doctor Name: " << d.doctor_name << endl;
    cout << "Doctor Speciality: " << d.doctor_speciality << endl;
    cout << "Address: " << d.address << endl;

    doctor_file.close();
}

void search_doctor_by_name(const char* doctor_name)
{

    find_doctors_by_name(doctor_name);
}

void search_doctor_by_speciality(const char* doctor_speciality)
{
    find_by_speciality(doctor_speciality);
}

void update_doctor_by_id(const char* doctor_id, const char* Udated_doctor_id)
{
    // if we have a doctor with same id for updated record
    if (is_doctor_id_exists(Udated_doctor_id)) {
        cout << "Doctor ID already exists." << endl;
        return;
    }

    // search in the primary index file
    int rrn = binarySearchDoctor(doctor_id);

    // Check if the doctor exists.
    if (rrn == -1) {
        cout << "Doctor Not Found" << endl;
        return;
    }

    fstream doctor_file("data/doctors.dat", ios::in | ios::out | ios::binary);

    //if file cannot open 
    if (!doctor_file.is_open()) {
        cout << "Cannot open file." << endl;
        return;
    }

    Doctor d;

    // Move the reading pointer to the required record using its RRN.
    doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    // Read the doctor's record from the data file.
    doctor_file.read((char*)&d, sizeof(Doctor));

    // update the id
    strcpy(d.doctor_id, Udated_doctor_id);

    //move write pointer to rrn
    doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    //write the updated doctor with the new id
    doctor_file.write((char*)&d, sizeof(Doctor));

    // update in the primary index file 
    updateDoctorPrimaryIndex(doctor_id, Udated_doctor_id);

    // Update Name Secondary Index
    remove_doctor(d.doctor_name, doctor_id);
    insert_doctor(d.doctor_name, Udated_doctor_id);

    // Update Speciality Secondary Index
    remove_doctor_speciality(d.doctor_speciality, doctor_id);
    insert_doctor_speciality(d.doctor_speciality, Udated_doctor_id);

    doctor_file.close();

}

void update_doctor_by_name(const char* doctor_id, const char* updated_doctor_name) {


    // search in the primary index file
    int rrn = binarySearchDoctor(doctor_id);

    // Check if the doctor exists.
    if (rrn == -1) {
        cout << "Doctor Not Found" << endl;
        return;
    }

    fstream doctor_file("data/doctors.dat", ios::in | ios::out | ios::binary);

    //if file cannot open 
    if (!doctor_file.is_open()) {
        cout << "Cannot open file." << endl;
        return;
    }

    Doctor d;

    // Move the reading pointer to the required record using its RRN.
    doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    // Read the doctor's record from the data file.
    doctor_file.read((char*)&d, sizeof(Doctor));

    // Store the old name for updating the secondary index
    char old_name[30];
    strcpy(old_name, d.doctor_name);

    // update the name
    strcpy(d.doctor_name, updated_doctor_name);

    //move write pointer to rrn
    doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    //write the updated doctor with the new name
    doctor_file.write((char*)&d, sizeof(Doctor));


    // Update Name Secondary Index
    update_doctor_name(old_name, updated_doctor_name, doctor_id);


    doctor_file.close();
}

void update_doctor_by_speciality(const char* doctor_id, const char* updated_doctor_speciality) {


    // search in the primary index file
    int rrn = binarySearchDoctor(doctor_id);

    // Check if the doctor exists.
    if (rrn == -1) {
        cout << "Doctor Not Found" << endl;
        return;
    }

    fstream doctor_file("data/doctors.dat", ios::in | ios::out | ios::binary);

    //if file cannot open 
    if (!doctor_file.is_open()) {
        cout << "Cannot open file." << endl;
        return;
    }

    Doctor d;

    // Move the reading pointer to the required record using its RRN.
    doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    // Read the doctor's record from the data file.
    doctor_file.read((char*)&d, sizeof(Doctor));

    // Store the old speciality for updating the secondary index
    char old_speciality[30];
    strcpy(old_speciality, d.doctor_speciality);

    // update the speciality
    strcpy(d.doctor_speciality, updated_doctor_speciality);

    //move write pointer to rrn
    doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    //write the updated doctor with the new name
    doctor_file.write((char*)&d, sizeof(Doctor));


    // Update Speciality Secondary Index
    update_doctor_speciality(old_speciality, updated_doctor_speciality, doctor_id);


    doctor_file.close();
}

void display_all_doctors() {

    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);

    Doctor_header h;

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