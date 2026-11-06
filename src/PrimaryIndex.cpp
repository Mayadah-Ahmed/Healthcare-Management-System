#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include<vector>
#include "PrimaryIndex.h"
#include "doctor.h"
#include "AppointmentsFile.h"

using namespace std;

namespace {
    struct IndexTable {
        vector<PrimaryIndexEntry> e;

        // Binary search returns position if found, else -1
        int find(const char* id, int& pos) {
            int left = 0;
            int right = (int)e.size() - 1;

            while (left <= right) {
                int mid = left + (right - left) / 2;
                int result = strncmp(e[mid].id, id, 15);

                if (result == 0) {
                    pos = mid;
                    return mid;
                }

                if (result < 0)
                    left = mid + 1;  //search the right half
                else
                    right = mid - 1; //search the left half
            }

            pos = left;
            return -1;
        }

        int search(const char* id) {
            int pos;
            int i = find(id, pos);
            return i == -1 ? -1 : e[i].RRN;
        }

        //insert while maintaining sorted order
        bool insert(const char* id, int RRN) {

        if (id == nullptr || id[0] == '\0' || id[0] == '*')
            return false;

        int pos;

        if (find(id, pos) != -1) //duplicate key found
            return false;

        PrimaryIndexEntry entry{};
        strncpy(entry.id, id, sizeof(entry.id));
        entry.RRN = RRN;

        e.insert(e.begin() + pos, entry);

        return true;
    }

    //remove while maintaining sorted order
      bool remove(const char* id) {
        int pos;

        if (find(id, pos) == -1)
            return false;

        e.erase(e.begin() + pos);

        return true;
    }

     //update an ID while keeping the same RRN
    bool updateKey(const char* oldID, const char* newID) {

        int pos;
        int i = find(oldID, pos);

        if (i == -1)
            return false;

        if (newID == nullptr || newID[0] == '\0' || newID[0] == '*')
            return false;

        if (find(newID, pos) != -1)
            return false;

        int rrn = e[i].RRN;

        e.erase(e.begin() + i);

        return insert(newID, rrn);
    }

      // Display the index
    void display(const char* title) const {

        cout << "\n--- " << title << " ---\n";

        if (e.empty()) {
            cout << "Primary index is empty\n";
            return;
        }

        for (const auto& entry : e) {
            cout << "ID: " << entry.id
                << " | RRN: " << entry.RRN << endl;
        }
    }
    };

    //Build index from file
void buildFromFile(IndexTable& table, const char* path, 
    size_t headerSize, size_t recordSize, const char* name) {

         table.e.clear();

        ifstream f(path, ios::binary);
        if (!f.is_open()) {
            cout << "Error opening " << name << " file" << endl;
            return;
    }
f.seekg(headerSize);

vector<char> record(recordSize); 
int RRN = 0;
while (f.read(record.data(), recordSize)) {
     if (record[0] != '*' && record[0] != '\0') {
         PrimaryIndexEntry entry{}; 
         memcpy(entry.id, record.data(), 15); 
         entry.RRN = RRN;
   table.e.push_back(entry);
     }
     RRN++; 
} 
sort(table.e.begin(), table.e.end(),
 [](const PrimaryIndexEntry& a, const PrimaryIndexEntry& b) {
     return strncmp(a.id, b.id, 15) < 0; });
      cout << name << " primary index built successfully (" << table.e.size() 
      << " entries)\n"; } 
IndexTable doctorTable;
IndexTable appointmentTable;
 }

//=====================Doctors===========================
void buildDoctorPrimaryIndex(){
   buildFromFile(doctorTable, DOCTOR_FILE, sizeof(header), sizeof(Doctor), "Doctors");
}
int binarySearchDoctor(const char*id){
    return doctorTable.search(id);
}
int insertDoctorPrimaryIndex(const char* id, int RRN){
    return doctorTable.insert(id, RRN);
}
int removeDoctorPrimaryIndex(const char* id){
    return doctorTable.remove(id);
}
int updateDoctorPrimaryIndex(const char* oldID, const char* newID){
    return doctorTable.updateKey(oldID, newID);
}
void displayDoctorPrimaryIndex(){
    doctorTable.display("Doctors");
}

//====================Appointments===========================
void buildAppointmentPrimaryIndex(){
     buildFromFile(appointmentTable, APPOINTMENT_FILE, sizeof(appointment_header),
                  sizeof(Appointment), "Appointments");
}
int  binarySearchAppointment(const char* id)         { return appointmentTable.search(id); }
bool insertAppointmentIndex(const char* id, int RRN) { return appointmentTable.insert(id, RRN); }
bool removeAppointmentIndex(const char* id)          { return appointmentTable.remove(id); }
bool updateAppointmentKeyIndex(const char* o, const char* n) { return appointmentTable.updateKey(o, n); }
void displayAppointmentPrimaryIndex()                { appointmentTable.display("Appointments Primary Index"); }