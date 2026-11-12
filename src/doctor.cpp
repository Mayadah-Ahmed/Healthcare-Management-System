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

    // Validate doctor fields length
    if (strlen(d.doctor_id) >= 15 ||
        strlen(d.doctor_name) >= 30 ||
        strlen(d.doctor_speciality) >= 20 ||
        strlen(d.address) >= 30) {

        cout << "One or more fields exceed maximum length." << endl;
        return;
    }

    // Validate doctor ID
    if (d.doctor_id[0] == '\0' || d.doctor_id[0] == '*') {
        cout << "Invalid doctor ID." << endl;
        return;
    }

    // Check if doctor already exists
    if (is_doctor_id_exists(d.doctor_id)) {
        cout << "Doctor with ID " << d.doctor_id  << " already exists." << endl;
        return;
    }

    fstream doctor_file(DOCTOR_FILE,ios::in | ios::out | ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Cannot open doctors file." << endl;
        return;
    }

    Doctor_header h;

    // Read header
    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    if (!doctor_file) {
        cout << "Error reading doctor header." << endl;
        doctor_file.close();
        return;
    }

    int rrn;

    if (h.avail == -1) {

        // No deleted records, insert at end
        doctor_file.seekp(0, ios::end);
        rrn = (int)(((long)doctor_file.tellp() - (long)sizeof(Doctor_header)) / sizeof(Doctor));
        doctor_file.write((char*)&d, sizeof(Doctor));

    }
    else {

        // avail contains Byte Offset
        int recordOffset = h.avail;

        // Convert Byte Offset to RRN
        rrn = (recordOffset - sizeof(Doctor_header)) / sizeof(Doctor);

        int next;

        // Read next pointer from deleted record
        doctor_file.seekg(recordOffset + 1);
        doctor_file.read((char*)&next, sizeof(int));

        if (!doctor_file) {
            cout << "Error reading available list." << endl;
            doctor_file.close();
            return;
        }

        // Update header with next available record
        h.avail = next;

        doctor_file.seekp(0);
        doctor_file.write((char*)&h, sizeof(h));

        // Write doctor in deleted record
        doctor_file.seekp(recordOffset);
        doctor_file.write((char*)&d, sizeof(Doctor));

    }

    if (!doctor_file) {
        cout << "Error writing doctor record." << endl;
        doctor_file.close();
        return;
    }

    doctor_file.close();

    // Update Primary Index
    insertDoctorPrimaryIndex(d.doctor_id, rrn);

    // Update Secondary Indexes
    insert_doctor(d.doctor_name, d.doctor_id);

    insert_doctor_speciality(d.doctor_speciality, d.doctor_id);

    cout << "Doctor added successfully." << endl;
}

void delete_doctor_by_id(const char* doctor_id) {

    int rrn = binarySearchDoctor(doctor_id);

    if (rrn == -1) {
        cout << "Doctor not found." << endl;
        return;
    }

    fstream doctor_file(DOCTOR_FILE, ios::in | ios::out | ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Cannot open doctors file." << endl;
        return;
    }

    Doctor d;

    int recordOffset = sizeof(Doctor_header) + rrn * sizeof(Doctor);

    // Read doctor before deleting
    doctor_file.seekg(recordOffset);
    doctor_file.read((char*)&d, sizeof(Doctor));

    if (!doctor_file) {
        cout << "Error reading doctor record." << endl;
        doctor_file.close();
        return;
    }

    Doctor_header h;

    // Read header
    doctor_file.seekg(0);
    doctor_file.read((char*)&h, sizeof(h));

    if (!doctor_file) {
        cout << "Error reading doctor header." << endl;
        doctor_file.close();
        return;
    }

    int old_head = h.avail;

    // Update available list head
    h.avail = recordOffset;

    doctor_file.seekp(0);
    doctor_file.write((char*)&h, sizeof(h));

    if (!doctor_file) {
        cout << "Error updating header." << endl;
        doctor_file.close();
        return;
    }

    // Mark record as deleted
    char marker = '*';

	// Write marker and next pointer
    doctor_file.seekp(recordOffset);
    doctor_file.write(&marker, 1);
    doctor_file.write((char*)&old_head, sizeof(int));

    if (!doctor_file) {
        cout << "Error deleting doctor record." << endl;
        doctor_file.close();
        return;
    }

    doctor_file.close();

    removeDoctorPrimaryIndex(doctor_id);

    remove_doctor(d.doctor_name, d.doctor_id);

    remove_doctor_speciality(d.doctor_speciality, d.doctor_id);

    cout << "Doctor deleted successfully." << endl;
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


void update_doctor_by_id(const char* doctor_id,const char* Udated_doctor_id)
{
    // Check if new ID is empty
    if (Udated_doctor_id[0] == '\0' ||
        Udated_doctor_id[0] == '*') {
        cout << "Invalid Doctor ID." << endl;
        return;
    }

    // Check ID length
    if (strlen(Udated_doctor_id) >= 15) {
        cout << "Doctor ID is too long." << endl;
        return;
    }

    // Check if the new ID is the same as old ID
    if (strcmp(doctor_id, Udated_doctor_id) == 0) {
        cout << "New ID is the same as old ID." << endl;
        return;
    }

    // Check if new ID already exists
    if (is_doctor_id_exists(Udated_doctor_id)) {
        cout << "Doctor ID already exists." << endl;
        return;
    }

    // Search for doctor
    int rrn = binarySearchDoctor(doctor_id);

    if (rrn == -1) {
        cout << "Doctor Not Found." << endl;
        return;
    }

    fstream doctor_file(DOCTOR_FILE,
        ios::in | ios::out | ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Cannot open file." << endl;
        return;
    }

    Doctor d;

    // Read doctor record
    doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    doctor_file.read((char*)&d, sizeof(Doctor));

    if (!doctor_file) {
        cout << "Error reading doctor record." << endl;
        doctor_file.close();
        return;
    }

    // Save old ID
    char old_id[15];
    strcpy(old_id, d.doctor_id);

    // Update ID
    strcpy(d.doctor_id, Udated_doctor_id);

    // Write updated record
    doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    doctor_file.write((char*)&d, sizeof(Doctor));

    if (!doctor_file) {
        cout << "Error updating doctor record." << endl;
        doctor_file.close();
        return;
    }

    doctor_file.close();

    // Update Primary Index
    updateDoctorPrimaryIndex(old_id, Udated_doctor_id);

    // Update Name Secondary Index
    remove_doctor(d.doctor_name, old_id);
    insert_doctor(d.doctor_name, Udated_doctor_id);

    // Update Speciality Secondary Index
    remove_doctor_speciality(d.doctor_speciality, old_id);
    insert_doctor_speciality(d.doctor_speciality, Udated_doctor_id);

    cout << "Doctor ID updated successfully." << endl;
}

void update_doctor_by_name(const char* doctor_id, const char* updated_doctor_name) {

    // Check name length
    if (updated_doctor_name[0] == '\0' || updated_doctor_name[0] == '*'||strlen(updated_doctor_name) >= 30) {
        cout << "Invalid doctor name." << endl;
        return;
    }

    // Search in Primary Index
    int rrn = binarySearchDoctor(doctor_id);

    if (rrn == -1) {
        cout << "Doctor Not Found" << endl;
        return;
    }

    fstream doctor_file(DOCTOR_FILE,
        ios::in | ios::out | ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Cannot open file." << endl;
        return;
    }

    Doctor d;

    // Read doctor record
    doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    doctor_file.read((char*)&d, sizeof(Doctor));

    if (!doctor_file) {
        cout << "Error reading doctor record." << endl;
        doctor_file.close();
        return;
    }

    // Store old name
    char old_name[30];
    strcpy(old_name, d.doctor_name);

    // Check if new name is the same as old name
    if (strcmp(d.doctor_name, updated_doctor_name) == 0) {
        cout << "New name is the same as old name." << endl;
        doctor_file.close();
        return;
    }

    // Update name
    strcpy(d.doctor_name, updated_doctor_name);

    // Write updated record
    doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    doctor_file.write((char*)&d, sizeof(Doctor));

    if (!doctor_file) {
        cout << "Error updating doctor name." << endl;
        doctor_file.close();
        return;
    }

    doctor_file.close();

    // Update Name Secondary Index
    update_doctor_name(old_name, updated_doctor_name, doctor_id);

    cout << "Doctor name updated successfully." << endl;
}

void update_doctor_by_speciality(const char* doctor_id,const char* updated_doctor_speciality) {

    // Check speciality length
    if (updated_doctor_speciality[0] == '\0' || updated_doctor_speciality[0] == '*'||strlen(updated_doctor_speciality) >= 20) {
        cout << "Invalid doctor speciality." << endl;
        return;
    }

    // Search in Primary Index
    int rrn = binarySearchDoctor(doctor_id);

    if (rrn == -1) {
        cout << "Doctor Not Found" << endl;
        return;
    }

    fstream doctor_file(DOCTOR_FILE,
        ios::in | ios::out | ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Cannot open file." << endl;
        return;
    }

    Doctor d;

    // Read doctor record
    doctor_file.seekg(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    doctor_file.read((char*)&d, sizeof(Doctor));

    if (!doctor_file) {
        cout << "Error reading doctor record." << endl;
        doctor_file.close();
        return;
    }

    // Store old speciality
    char old_speciality[20];
    strcpy(old_speciality, d.doctor_speciality);

    // Check if new speciality is the same as old speciality
    if (strcmp(d.doctor_speciality, updated_doctor_speciality) == 0) {
        cout << "New speciality is the same as old speciality." << endl;
        doctor_file.close();
        return;
    }

    // Update speciality
    strcpy(d.doctor_speciality, updated_doctor_speciality);

    // Write updated record
    doctor_file.seekp(sizeof(Doctor_header) + rrn * sizeof(Doctor));

    doctor_file.write((char*)&d, sizeof(Doctor));

    if (!doctor_file) {
        cout << "Error updating doctor speciality." << endl;
        doctor_file.close();
        return;
    }

    doctor_file.close();

    // Update Speciality Secondary Index
    update_doctor_speciality(old_speciality,
        updated_doctor_speciality,
        doctor_id);

    cout << "Doctor speciality updated successfully." << endl;
}

void display_all_doctors() {

    fstream doctor_file("data/doctors.dat", ios::in | ios::binary);

    if (!doctor_file.is_open()) {
        cout << "Cannot open doctors file." << endl;
        return;
    }

    Doctor_header h;

    if (!doctor_file) {
        cout << "Error reading doctor header." << endl;
        doctor_file.close();
        return;
    }

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