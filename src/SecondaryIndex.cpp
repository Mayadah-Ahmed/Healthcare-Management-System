#include "PrimaryIndex.h"
#include "SecondaryIndex.h"
#include "doctor.h"
#include "AppointmentsFile.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

SecondaryIndexEntry nameIndex[100];
int nameIndexCount = 0;

SpecialityIndexEntry specialityIndex[100];
int specialityIndexCount = 0;

ApptDoctorIndex apptDoctorIndex[100];
int apptDoctorIndexCount = 0;

////////////////////////  Insert  ////////////////////////////////////////
void insert_doctor(const char* name, const char* doctor_Id) {
    //  search for the key
    int pos = BinarySearchSec(name);
    if (pos == -1 && nameIndexCount >= 100) { cout << "Index is Full\n"; return; }
    fstream file(NAME_LINKED_FILE, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        ofstream create(NAME_LINKED_FILE, ios::binary);
        create.close();
        file.open(NAME_LINKED_FILE, ios::in | ios::out | ios::binary);
    }
    if (!file.is_open()) { cout << "Error opening file\n "; return; }

    //make a new node
    SecondaryLinkedList node = {};
    strncpy(node.doctor_id, doctor_Id, sizeof(node.doctor_id) - 1);
    node.doctor_id[sizeof(node.doctor_id) - 1] = '\0';
    node.nextRRN = (pos == -1) ? -1 : nameIndex[pos].headRRN;

    file.seekp(0, ios::end);
    int nodeRRN = (int)file.tellp() / sizeof(SecondaryLinkedList);
    file.write((char*)&node, sizeof(node));
    file.close();

    //  if pos=-1 we add a new key and sort
    if (pos == -1) {

        strncpy(nameIndex[nameIndexCount].doctor_name, name, sizeof(nameIndex[nameIndexCount].doctor_name) - 1);

        nameIndex[nameIndexCount].doctor_name[sizeof(nameIndex[nameIndexCount].doctor_name) - 1] = '\0';

        nameIndex[nameIndexCount].headRRN = nodeRRN;
        nameIndexCount++;
        sort(nameIndex, nameIndex + nameIndexCount, []
        (const SecondaryIndexEntry& a, const SecondaryIndexEntry& b) {
                return strcmp(a.doctor_name, b.doctor_name) < 0;
            });
    }
    else {
        nameIndex[pos].headRRN = nodeRRN;
    }
}

void insert_doctor_speciality(const char* speciality, const char* doctor_Id) {
    int pos = BinarySearchSpeciality(speciality);
    if (pos == -1 && specialityIndexCount >= 100) { cout << "Index is Full\n"; return; }
    fstream file(SPECIALITY_LINKED_FILE, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        ofstream create(SPECIALITY_LINKED_FILE, ios::binary);
        create.close();
        file.open(SPECIALITY_LINKED_FILE, ios::in | ios::out | ios::binary);
    }
    if (!file.is_open()) { cout << "Error opening file\n "; return; }

    //make a new node
    SecondaryLinkedList node = {};
    strncpy(node.doctor_id, doctor_Id, sizeof(node.doctor_id) - 1);
    node.doctor_id[sizeof(node.doctor_id) - 1] = '\0';
    node.nextRRN = (pos == -1) ? -1 : specialityIndex[pos].headRRN;

    file.seekp(0, ios::end);
    int nodeRRN = (int)file.tellp() / sizeof(SpecialityLinkedList);
    file.write((char*)&node, sizeof(node));
    file.close();

    if (pos == -1) {

        strncpy(specialityIndex[specialityIndexCount].doctor_speciality, speciality,
            sizeof(specialityIndex[specialityIndexCount].doctor_speciality) - 1);
        specialityIndex[specialityIndexCount].doctor_speciality[
            sizeof(specialityIndex[specialityIndexCount].doctor_speciality) - 1] = '\0';

        specialityIndex[specialityIndexCount].headRRN = nodeRRN;

        specialityIndexCount++;

        sort(specialityIndex, specialityIndex + specialityIndexCount, []
        (const SpecialityIndexEntry& a, const SpecialityIndexEntry& b) {
                return strcmp(a.doctor_speciality, b.doctor_speciality) < 0;
            });
    }
    else {
        specialityIndex[pos].headRRN = nodeRRN;
    }
}



void insert_appointment(const char* doctor_id, const char* appointment_id) {
    //  search for the key
    int pos = BinarySearchAppt(doctor_id);
    if (pos == -1 && apptDoctorIndexCount >= 100) { cout << "Index is Full\n"; return; }
    fstream file(APPT_DOCTOR_LINKED_FILE, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        ofstream create(APPT_DOCTOR_LINKED_FILE, ios::binary);
        create.close();
        file.open(APPT_DOCTOR_LINKED_FILE, ios::in | ios::out | ios::binary);
    }
    if (!file.is_open()) { cout << "Error opening file\n "; return; }

    //make a new node
    ApptDoctorLinkedList node = {};
    strncpy(node.appointment_id, appointment_id, sizeof(node.appointment_id) - 1);
    node.appointment_id[sizeof(node.appointment_id) - 1] = '\0';
    node.nextRRN = (pos == -1) ? -1 : apptDoctorIndex[pos].headRRN;

    file.seekp(0, ios::end);
    int nodeRRN = (int)file.tellp() / sizeof(ApptDoctorLinkedList);
    file.write((char*)&node, sizeof(node));
    file.close();

    if (pos == -1) {
        strncpy(apptDoctorIndex[apptDoctorIndexCount].doctor_id, doctor_id, sizeof(apptDoctorIndex[apptDoctorIndexCount].doctor_id) - 1);

        apptDoctorIndex[apptDoctorIndexCount].doctor_id[sizeof(apptDoctorIndex[apptDoctorIndexCount].doctor_id) - 1] = '\0';

        apptDoctorIndex[apptDoctorIndexCount].headRRN = nodeRRN;
        apptDoctorIndexCount++;
        sort(apptDoctorIndex, apptDoctorIndex + apptDoctorIndexCount, []
        (const ApptDoctorIndex& a, const ApptDoctorIndex& b) {
                return strcmp(a.doctor_id, b.doctor_id) < 0;
            });
    }
    else {
        apptDoctorIndex[pos].headRRN = nodeRRN;
    }
}

////////////////////////  build  ////////////////////////////////////////
void buildDoctorNameIndex() {
    ifstream doctorFile(DOCTOR_FILE, ios::binary);
    if (!doctorFile.is_open()) { cout << "Error opening file\n"; return; }

    ofstream clearFile(NAME_LINKED_FILE, ios::binary | ios::trunc);
    clearFile.close();
    nameIndexCount = 0;

    Doctor_header h;
    doctorFile.read((char*)&h, sizeof(h));

    Doctor d;
    while (doctorFile.read((char*)&d, sizeof(d))) {
        if (d.doctor_id[0] == '*') continue;
        insert_doctor(d.doctor_name, d.doctor_id);
    }
    doctorFile.close();

    cout << "Doctor Name Secondary Index built Successfully!\n";
}
////////////////////////  Build index  ////////////////////////////////////////

void buildDoctorSpeciality() {
    ifstream doctorFile(DOCTOR_FILE, ios::binary);
    if (!doctorFile.is_open()) { cout << "Error opening file\n"; return; }

    ofstream clearFile(SPECIALITY_LINKED_FILE, ios::binary | ios::trunc);
    clearFile.close();
    specialityIndexCount = 0;

    Doctor_header h;
    doctorFile.read((char*)&h, sizeof(h));

    Doctor d;
    while (doctorFile.read((char*)&d, sizeof(d))) {
        if (d.doctor_id[0] == '*') continue;
        insert_doctor_speciality(d.doctor_speciality, d.doctor_id);
    }
    doctorFile.close();

    cout << "Doctor Speciality Secondary Index built Successfully!\n";
}

void buildDoctorAppointment() {
    ifstream apptFile(APPOINTMENT_FILE, ios::binary);
    if (!apptFile.is_open()) { cout << "Error opening appointments file\n"; return; }

    ofstream clearFile(APPT_DOCTOR_LINKED_FILE, ios::binary | ios::trunc);
    clearFile.close();
    apptDoctorIndexCount = 0;

    appointment_header h;
    apptFile.read((char*)&h, sizeof(h));

    Appointment a;
    while (apptFile.read((char*)&a, sizeof(a))) {
        if (a.appointment_id[0] == '*') continue;
        insert_appointment(a.doctor_id, a.appointment_id);
    }
    apptFile.close();

    cout << "Appointment-Doctor Secondary Index built Successfully!\n";
}

////////////////////////  Binary Search  ////////////////////////////////////////
//to find secondary key inside sorted index array
int BinarySearchSec(const char* name) {
    int left = 0, right = nameIndexCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int result = strcmp(nameIndex[mid].doctor_name, name);
        if (result == 0)  return mid;
        else if (result < 0)  left = mid + 1;
        else  right = mid - 1;
    }
    return -1;
}

int BinarySearchSpeciality(const char* speciality) {
    int left = 0, right = specialityIndexCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int result = strcmp(specialityIndex[mid].doctor_speciality, speciality);
        if (result == 0)  return mid;
        else if (result < 0)  left = mid + 1;
        else  right = mid - 1;
    }
    return -1;
}

int BinarySearchAppt(const char* doctor_id) {
    int left = 0, right = apptDoctorIndexCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int result = strcmp(apptDoctorIndex[mid].doctor_id, doctor_id);
        if (result == 0)  return mid;
        else if (result < 0)  left = mid + 1;
        else  right = mid - 1;
    }
    return -1;
}
////////////////////////  Find  ////////////////////////////////////////

void find_doctors_by_name(const char* name) {
    int pos = BinarySearchSec(name);
    if (pos == -1) { cout << "No doctors found" << endl; return; }

    ifstream nameLinkedFile(NAME_LINKED_FILE, ios::in | ios::binary);
    if (!nameLinkedFile.is_open()) { cout << "Error opening file\n "; return; }

    int current = nameIndex[pos].headRRN;
    SecondaryLinkedList node;

    while (current != -1) {
        nameLinkedFile.seekg((long)current * sizeof(SecondaryLinkedList));
        nameLinkedFile.read((char*)&node, sizeof(node));

        int dataRRN = binarySearchDoctor(node.doctor_id);
        if (dataRRN != -1) {
            ifstream doctorFile(DOCTOR_FILE, ios::binary);
            Doctor d;
            doctorFile.seekg(sizeof(Doctor_header) + (long)dataRRN * sizeof(d));
            doctorFile.read((char*)&d, sizeof(d));

            cout << "Doctor ID:" << d.doctor_id << endl;
            cout << "Doctor Name:" << d.doctor_name << endl;
            cout << "Speciality:" << d.doctor_speciality << endl;
            cout << "----\n";
        }
        current = node.nextRRN;
    }
    nameLinkedFile.close();

}

void find_by_speciality(const char* speciality) {
    int pos = BinarySearchSpeciality(speciality);
    if (pos == -1) { cout << "No doctors found" << endl; return; }

    ifstream file(SPECIALITY_LINKED_FILE, ios::in | ios::binary);
    if (!file.is_open()) { cout << "Error opening file\n "; return; }

    int current = specialityIndex[pos].headRRN;
    SpecialityLinkedList node;

    while (current != -1) {
        file.seekg((long)current * sizeof(SpecialityLinkedList));
        file.read((char*)&node, sizeof(node));

        int dataRRN = binarySearchDoctor(node.doctor_id);
        if (dataRRN != -1) {
            ifstream doctorFile(DOCTOR_FILE, ios::binary);
            Doctor d;
            doctorFile.seekg(sizeof(Doctor_header) + (long)dataRRN * sizeof(Doctor));
            doctorFile.read((char*)&d, sizeof(d));

            cout << "Doctor ID:" << d.doctor_id << endl;
            cout << "Doctor Name:" << d.doctor_name << endl;
            cout << "Speciality:" << d.doctor_speciality << endl;
            cout << "----\n";
        }
        current = node.nextRRN;
    }
    file.close();
}

void find_appointments_by_doctor(const char* doctor_id) {
    int pos = BinarySearchAppt(doctor_id);
    if (pos == -1) { cout << "No appointments for this doctor\n"; return; }

    ifstream linked(APPT_DOCTOR_LINKED_FILE, ios::binary);
    if (!linked.is_open()) { cout << "Error opening file\n"; return; }

    int current = apptDoctorIndex[pos].headRRN;
    ApptDoctorLinkedList node;

    while (current != -1) {
        linked.seekg((long)current * sizeof(ApptDoctorLinkedList));
        linked.read((char*)&node, sizeof(node));

        int rrn = binarySearchAppointment(node.appointment_id);
        if (rrn != -1) {
            ifstream apptFile(APPOINTMENT_FILE, ios::binary);
            Appointment a;
            apptFile.seekg(sizeof(appointment_header) + (long)rrn * sizeof(Appointment));
            apptFile.read((char*)&a, sizeof(a));
            cout << "Appointment ID: " << a.appointment_id << endl;
            cout << "Date: " << a.appointment_date << endl;
            cout << "Doctor ID: " << a.doctor_id << endl;
            cout << "----\n";
            apptFile.close();
        }
        current = node.nextRRN;
    }
    linked.close();
}
////////////////////////  REMOVE  ////////////////////////////////////////
bool remove_doctor(const char* name, const char* doctor_Id) {
    int pos = BinarySearchSec(name);
    if (pos == -1) { cout << "No doctors found" << endl; return false; }

    fstream linked(NAME_LINKED_FILE, ios::in | ios::out | ios::binary);
    if (!linked.is_open()) { cout << "Error opening file\n "; return false; }

    int current = nameIndex[pos].headRRN;
    int prev = -1;
    SecondaryLinkedList node = {}, prevNode = {};
    bool found = false;

    while (current != -1) {
        linked.seekg((long)current * sizeof(node));
        linked.read((char*)&node, sizeof(node));
        if (strncmp(node.doctor_id, doctor_Id, 15) == 0) { found = true; break; }
        prev = current;
        prevNode = node;
        current = node.nextRRN;
    }
    if (!found) { cout << "Node not found\n"; linked.close(); return false; }

    if (prev == -1) { nameIndex[pos].headRRN = node.nextRRN; }
    else {
        prevNode.nextRRN = node.nextRRN;
        linked.seekp((long)prev * sizeof(node));
        linked.write((char*)&prevNode, sizeof(prevNode));
    }

    if (nameIndex[pos].headRRN == -1) {
        for (int i = pos; i < nameIndexCount - 1; i++) {
            nameIndex[i] = nameIndex[i + 1];
        }
        nameIndexCount--;
    }
    linked.close();
    cout << "Removed From name index\n";
    return true;
}


bool remove_doctor_speciality(const char* speciality, const char* doctor_Id) {
    int pos = BinarySearchSpeciality(speciality);
    if (pos == -1) { cout << "No doctors found\n"; return false; }

    fstream linked(SPECIALITY_LINKED_FILE, ios::in | ios::out | ios::binary);
    if (!linked.is_open()) { cout << "Error opening file\n"; return false; }

    int current = specialityIndex[pos].headRRN;
    int prev = -1;
    SpecialityLinkedList node = {}, prevNode = {};
    bool found = false;

    while (current != -1) {
        linked.seekg((long)current * sizeof(SpecialityLinkedList));
        linked.read((char*)&node, sizeof(node));
        if (strncmp(node.doctor_id, doctor_Id, 15) == 0) { found = true; break; }
        prev = current;
        prevNode = node;
        current = node.nextRRN;
    }

    if (!found) { cout << "Node not found\n"; linked.close(); return false; }

    if (prev == -1) { specialityIndex[pos].headRRN = node.nextRRN; }
    else {
        prevNode.nextRRN = node.nextRRN;
        linked.seekp((long)prev * sizeof(SpecialityLinkedList));
        linked.write((char*)&prevNode, sizeof(prevNode));
    }

    if (specialityIndex[pos].headRRN == -1) {
        for (int i = pos; i < specialityIndexCount - 1; i++)
            specialityIndex[i] = specialityIndex[i + 1];
        specialityIndexCount--;
    }
    linked.close();
    cout << "Removed from speciality index\n";
    return true;
}


bool remove_appointment_doctor(const char* doctor_id, const char* appointment_id) {
    int pos = BinarySearchAppt(doctor_id);
    if (pos == -1) return false;

    fstream linked(APPT_DOCTOR_LINKED_FILE, ios::in | ios::out | ios::binary);
    if (!linked.is_open()) return false;

    int current = apptDoctorIndex[pos].headRRN;
    int prev = -1;
    ApptDoctorLinkedList node = {}, prevNode = {};
    bool found = false;

    while (current != -1) {
        linked.seekg((long)current * sizeof(ApptDoctorLinkedList));
        linked.read((char*)&node, sizeof(node));
        if (strncmp(node.appointment_id, appointment_id, 15) == 0) { found = true; break; }
        prev = current;
        prevNode = node;
        current = node.nextRRN;
    }

    if (!found) { linked.close(); return false; }

    if (prev == -1) {
        apptDoctorIndex[pos].headRRN = node.nextRRN;
    }
    else {
        prevNode.nextRRN = node.nextRRN;
        linked.seekp((long)prev * sizeof(ApptDoctorLinkedList));
        linked.write((char*)&prevNode, sizeof(prevNode));
    }

    if (apptDoctorIndex[pos].headRRN == -1) {
        for (int i = pos; i < apptDoctorIndexCount - 1; i++)
            apptDoctorIndex[i] = apptDoctorIndex[i + 1];
        apptDoctorIndexCount--;
    }
    linked.close();
    return true;
}


////////////////////////  Update  ////////////////////////////////////////
void update_doctor_name(const char* oldName, const char* newName, const char* doctor_Id) {
    if (!remove_doctor(oldName, doctor_Id)) { cout << "Update failed\n"; return; }
    insert_doctor(newName, doctor_Id);
    cout << "Name updated\n";
}

void update_doctor_speciality(const char* oldSpec, const char* newSpec, const char* doctor_Id) {
    if (!remove_doctor_speciality(oldSpec, doctor_Id)) { cout << "Update failed\n"; return; }
    insert_doctor_speciality(newSpec, doctor_Id);
    cout << "Speciality updated\n";
}

void update_appointment(const char* oldDoctorId, const char* newDoctorId, const char* appointment_id) {
    if (!remove_appointment_doctor(oldDoctorId, appointment_id)) { cout << "Update failed\n"; return; }
    insert_appointment(newDoctorId, appointment_id);
    cout << "Appointment doctor updated\n";
}


////////////////////////  Display  ////////////////////////////////////////
void displayDoctorNameIndex() {
    cout << "\n--- Doctor Name Secondary Index ---\n";
    ifstream f(NAME_LINKED_FILE, ios::binary);
    for (int i = 0; i < nameIndexCount; i++) {
        cout << nameIndex[i].doctor_name << " -> ";
        int cur = nameIndex[i].headRRN;
        SecondaryLinkedList n;
        while (cur != -1) {
            f.clear(); f.seekg((long)cur * sizeof(n)); f.read((char*)&n, sizeof(n));
            cout << n.doctor_id << " -> ";
            cur = n.nextRRN;
        }
        cout << "NULL\n";
    }
}

void displayDoctorSpecialityIndex() {
    cout << "\n--- Doctor Speciality Secondary Index ---\n";
    ifstream f(SPECIALITY_LINKED_FILE, ios::binary);
    for (int i = 0; i < specialityIndexCount; i++) {
        cout << specialityIndex[i].doctor_speciality << " -> ";
        int cur = specialityIndex[i].headRRN;
        SpecialityLinkedList n;
        while (cur != -1) {
            f.clear(); f.seekg((long)cur * sizeof(n)); f.read((char*)&n, sizeof(n));
            cout << n.doctor_id << " -> ";
            cur = n.nextRRN;
        }
        cout << "NULL\n";
    }
}

void displayApptDoctorIndex() {
    cout << "\n--- Appointment/Doctor Secondary Index ---\n";
    ifstream f(APPT_DOCTOR_LINKED_FILE, ios::binary);
    for (int i = 0; i < apptDoctorIndexCount; i++) {
        cout << apptDoctorIndex[i].doctor_id << " -> ";
        int cur = apptDoctorIndex[i].headRRN;
        ApptDoctorLinkedList n;
        while (cur != -1) {
            f.clear(); f.seekg((long)cur * sizeof(n)); f.read((char*)&n, sizeof(n));
            cout << n.appointment_id << " -> ";
            cur = n.nextRRN;
        }
        cout << "NULL\n";
    }
}