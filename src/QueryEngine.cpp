
#include "QueryEngine.h"
#include "SecondaryIndex.h"
#include "PrimaryIndex.h"
#include "doctor.h"
#include "AppointmentsFile.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


// Get Matching IDs
vector<string> getMatchingIDs(const Condition& condition,const string& tableName)
{
    vector<string> result;

    // Currently supporting only =
    if (condition.operation != "=")
        return result;

    if (tableName == "Doctors")
    {
        if (condition.field == "Doctor Name")
        {
            result = getDoctorIDsByName(condition.value.c_str());
        }
        else if (condition.field == "Doctor Specialty")
        {
            result = getDoctorIDsBySpeciality(condition.value.c_str());
        }
        else if (condition.field == "Doctor ID")
        {
            int rrn = binarySearchDoctor(condition.value.c_str());

            if (rrn != -1)
                result.push_back(condition.value);
        }
    }
    else if (tableName == "Appointments")
    {
        if (condition.field == "Doctor ID")
        {
            result = getAppointmentIDsByDoctor(condition.value.c_str());
        }
        else if (condition.field == "Appointment ID")
        {
            int rrn = binarySearchAppointment(condition.value.c_str());

            if (rrn != -1)
                result.push_back(condition.value);
        }
        else if (condition.field == "Appointment Date")
        {
            ifstream file(APPOINTMENT_FILE, ios::binary);

            if (!file.is_open())
                return result;

            appointment_header h;
            file.read((char*)&h, sizeof(h));

            if (!file)
            {
                file.close();
                return result;
            }

            Appointment a;

            while (file.read((char*)&a, sizeof(Appointment)))
            {
                if (a.appointment_id[0] == '*')
                    continue;

                if (strcmp(a.appointment_date, condition.value.c_str()) == 0)
                {
                    result.push_back(a.appointment_id);
                }
            }

            file.close();
        }
    }


    return result;
}


//  Combine Results
vector<string> combineResults( vector<string> first, vector<string> second, string operation)
{
    vector<string> result;

    if (operation == "AND")
    {
        for (string id : first)
        {
            if (find(second.begin(), second.end(), id) != second.end())
            {
                result.push_back(id);
            }
        }
    }
    else if (operation == "OR")
    {
        result = first;

        for (string id : second)
        {
            if (find(result.begin(), result.end(), id) == result.end())
            {
                result.push_back(id);
            }
        }
    }

    return result;
}


// Read Doctor By ID
bool readDoctorByID(const string& doctorID, Doctor& d)
{
    int rrn = binarySearchDoctor(doctorID.c_str());

    if (rrn == -1)
        return false;

    ifstream file(DOCTOR_FILE, ios::binary);

    if (!file.is_open())
        return false;

    long offset = sizeof(Doctor_header) +
        (long)rrn * sizeof(Doctor);

    file.seekg(offset, ios::beg);

    if (!file.read((char*)&d, sizeof(Doctor)))
    {
        file.close();
        return false;
    }

    file.close();

    return true;
}


// Read Appointment By ID
bool readAppointmentByID(const string& appointmentID, Appointment& a)
{
    int rrn = binarySearchAppointment(appointmentID.c_str());

    if (rrn == -1)
        return false;

    ifstream file(APPOINTMENT_FILE, ios::binary);

    if (!file.is_open())
        return false;

    long offset = sizeof(appointment_header) +
        (long)rrn * sizeof(Appointment);

    file.seekg(offset, ios::beg);

    if (!file.read((char*)&a, sizeof(Appointment)))
    {
        file.close();
        return false;
    }

    file.close();

    return true;
}


// Display Doctor
void displayDoctor(const Doctor& d, const string& selectedField)
{
    if (selectedField == "*" || selectedField == "ALL"||selectedField=="all")
    {
        cout << left
            << setw(15) << d.doctor_id
            << setw(30) << d.doctor_name
            << setw(22) << d.doctor_speciality
            << setw(30) << d.address << endl;
    }
    else if (selectedField == "Doctor ID")
    {
        cout << left << setw(15) << d.doctor_id << endl;
    }
    else if (selectedField == "Doctor Name")
    {
        cout << left << setw(30) << d.doctor_name << endl;
    }
    else if (selectedField == "Doctor Specialty")
    {
        cout << left << setw(22) << d.doctor_speciality << endl;
    }
}


// Display Appointment
void displayAppointment(const Appointment& a, const string& selectedField)
{
    if (selectedField == "*" || selectedField == "ALL")
    {
        cout << left
            << setw(18) << a.appointment_id
            << setw(32) << a.appointment_date
            << setw(15) << a.doctor_id << endl;
    }
    else if (selectedField == "Appointment ID")
    {
        cout << left << setw(18) << a.appointment_id << endl;
    }
    else if (selectedField == "Appointment Date")
    {
        cout << left << setw(32) << a.appointment_date << endl;
    }
    else if (selectedField == "Doctor ID")
    {
        cout << left << setw(15) << a.doctor_id << endl;
    }
}


// Execute Query
void executeQuery(const Query& query)
{
    // Query without WHERE
    if (query.conditions.empty())
    {
        if (query.tableName == "Doctors")
        {
            ifstream file(DOCTOR_FILE, ios::binary);

            if (!file.is_open()) {
                cout << "Cannot open doctors file." << endl;
                return;
            }

            Doctor_header h;
            file.read((char*)&h, sizeof(h));

            if (!file) {
                cout << "Error reading doctors header." << endl;
                file.close();
                return;
            }

            cout << "\n========== DOCTORS TABLE ==========\n";

            if (query.selectedField == "*" ||
                query.selectedField == "ALL" ||
                query.selectedField == "all") {
                cout << left
                    << setw(15) << "Doctor ID"
                    << setw(30) << "Doctor Name"
                    << setw(22) << "Specialty"
                    << setw(30) << "Address" << endl;
                cout << string(97, '-') << endl;
            }
            else {
                cout << query.selectedField << endl;
                cout << string(30, '-') << endl;
            }

            Doctor d;
            int count = 0;

            while (file.read((char*)&d, sizeof(Doctor))) {
                if (d.doctor_id[0] == '*')
                    continue;

                displayDoctor(d, query.selectedField);
                count++;
            }

            cout << "\n" << count << " record(s) found.\n";
            file.close();
        }
        else if (query.tableName == "Appointments")
        {
            ifstream file(APPOINTMENT_FILE, ios::binary);

            if (!file.is_open()) {
                cout << "Cannot open appointments file." << endl;
                return;
            }

            appointment_header h;
            file.read((char*)&h, sizeof(h));

            if (!file) {
                cout << "Error reading appointments header." << endl;
                file.close();
                return;
            }

            cout << "\n======= APPOINTMENTS TABLE =======\n";

            if (query.selectedField == "*" ||
                query.selectedField == "ALL" ||
                query.selectedField == "all") {
                cout << left
                    << setw(18) << "Appointment ID"
                    << setw(32) << "Appointment Date"
                    << setw(15) << "Doctor ID" << endl;
                cout << string(65, '-') << endl;
            }
            else {
                cout << query.selectedField << endl;
                cout << string(32, '-') << endl;
            }

            Appointment a;
            int count = 0;

            while (file.read((char*)&a, sizeof(Appointment))) {
                if (a.appointment_id[0] == '*')
                    continue;

                displayAppointment(a, query.selectedField);
                count++;
            }

            cout << "\n" << count << " record(s) found.\n";
            file.close();
        }
        else {
            cout << "Invalid table name." << endl;
        }

        return;
    }

    //  Query with WHERE
    vector<string> matchingIDs;

    // Get IDs matching the first condition
    matchingIDs = getMatchingIDs(
        query.conditions[0],
        query.tableName
    );

    // Process remaining conditions
    for (int i = 1; i < query.conditions.size(); i++)
    {
        vector<string> currentIDs = getMatchingIDs(
            query.conditions[i],
            query.tableName
        );

        matchingIDs = combineResults(
            matchingIDs,
            currentIDs,
            query.logicalOperators[i - 1]
        );
    }

    // Check if there are matching records
    if (matchingIDs.empty())
    {
        cout << "\nNo matching records found.\n";
        return;
    }

    // Display Doctors
    if (query.tableName == "Doctors")
    {
        cout << "\n================ QUERY RESULT ================\n";

        if (query.selectedField == "*" || query.selectedField == "ALL" || query.selectedField == "all")
        {
            cout << left
                << setw(15) << "Doctor ID"
                << setw(30) << "Doctor Name"
                << setw(22) << "Specialty"
                << setw(30) << "Address" << endl;

            cout << string(97, '-') << endl;
        }
        else
        {
            cout << query.selectedField << endl;
            cout << string(30, '-') << endl;
        }

        int count = 0;

        for (string id : matchingIDs)
        {
            Doctor d;

            if (readDoctorByID(id, d))
            {
                displayDoctor(d, query.selectedField);
                count++;
            }
        }

        cout << string(97, '-') << endl;
        cout << count << " record(s) found.\n";
    }

    // Display Appointments
    else if (query.tableName == "Appointments")
    {
        cout << "\n================ QUERY RESULT ================\n";

        if (query.selectedField == "*" || query.selectedField == "ALL" || query.selectedField == "all")
        {
            cout << left
                << setw(18) << "Appointment ID"
                << setw(32) << "Appointment Date"
                << setw(15) << "Doctor ID" << endl;

            cout << string(65, '-') << endl;
        }
        else
        {
            cout << query.selectedField << endl;
            cout << string(32, '-') << endl;
        }

        int count = 0;

        for (string id : matchingIDs)
        {
            Appointment a;

            if (readAppointmentByID(id, a))
            {
                displayAppointment(a, query.selectedField);
                count++;
            }
        }

        cout << string(65, '-') << endl;
        cout << count << " record(s) found.\n";
    }

    else
    {
        cout << "Invalid table name." << endl;
    }
}


