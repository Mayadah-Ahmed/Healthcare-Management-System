
#include <iostream>
#include <limits>
#include <filesystem>
#include "doctor.h"
#include "AppointmentsFile.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"
#include "QueryParser.h"
#include "QueryEngine.h"

using namespace std;

template <size_t N>
void setField(char(&field)[N], const char* value)
{
    strncpy(field, value, N - 1);
    field[N - 1] = '\0';
}

void initializeInitialData()
{
    cout << "\n===== CHECK INITIAL DATA =====\n";

    // Check whether both files contain only their headers
    bool doctorsEmpty =
        filesystem::file_size(DOCTOR_FILE) == sizeof(Doctor_header);

    bool appointmentsEmpty =
        filesystem::file_size(APPOINTMENT_FILE) == sizeof(appointment_header);

    // Do not insert initial data again
    if (!doctorsEmpty || !appointmentsEmpty)
    {
        cout << "Initial data already exists. Skipping insertion.\n";
        return;
    }

    cout << "Inserting initial Doctors and Appointments...\n";

    // =====================================
    //          INITIAL DOCTORS
    // =====================================

    Doctor doctors[5]{};

    setField(doctors[0].doctor_id, "D001");
    setField(doctors[0].doctor_name, "Ahmed Ali");
    setField(doctors[0].doctor_speciality, "Cardiology");
    setField(doctors[0].address, "Cairo");

    setField(doctors[1].doctor_id, "D002");
    setField(doctors[1].doctor_name, "Sara Mohamed");
    setField(doctors[1].doctor_speciality, "Neurology");
    setField(doctors[1].address, "Giza");

    setField(doctors[2].doctor_id, "D003");
    setField(doctors[2].doctor_name, "Omar Hassan");
    setField(doctors[2].doctor_speciality, "Pediatrics");
    setField(doctors[2].address, "Cairo");

    setField(doctors[3].doctor_id, "D004");
    setField(doctors[3].doctor_name, "Mona Adel");
    setField(doctors[3].doctor_speciality, "Dermatology");
    setField(doctors[3].address, "Alexandria");

    setField(doctors[4].doctor_id, "D005");
    setField(doctors[4].doctor_name, "Youssef Samir");
    setField(doctors[4].doctor_speciality, "Surgery");
    setField(doctors[4].address, "Giza");

    for (int i = 0; i < 5; i++)
    {
        Add_doctor(doctors[i]);
    }

    cout << "5 Doctors inserted successfully.\n";

    // =====================================
    //        REBUILD DOCTOR INDEXES
    // =====================================

    buildDoctorPrimaryIndex();
    buildDoctorNameIndex();
    buildDoctorSpeciality();

    // =====================================
    //        INITIAL APPOINTMENTS
    // =====================================

    Appointment appointments[10]{};

    setField(appointments[0].appointment_id, "A001");
    setField(appointments[0].appointment_date, "2026-11-10");
    setField(appointments[0].doctor_id, "D001");

    setField(appointments[1].appointment_id, "A002");
    setField(appointments[1].appointment_date, "2026-11-11");
    setField(appointments[1].doctor_id, "D002");

    setField(appointments[2].appointment_id, "A003");
    setField(appointments[2].appointment_date, "2026-11-12");
    setField(appointments[2].doctor_id, "D001");

    setField(appointments[3].appointment_id, "A004");
    setField(appointments[3].appointment_date, "2026-11-13");
    setField(appointments[3].doctor_id, "D003");

    setField(appointments[4].appointment_id, "A005");
    setField(appointments[4].appointment_date, "2026-11-14");
    setField(appointments[4].doctor_id, "D004");

    setField(appointments[5].appointment_id, "A006");
    setField(appointments[5].appointment_date, "2026-11-15");
    setField(appointments[5].doctor_id, "D005");

    setField(appointments[6].appointment_id, "A007");
    setField(appointments[6].appointment_date, "2026-11-16");
    setField(appointments[6].doctor_id, "D002");

    setField(appointments[7].appointment_id, "A008");
    setField(appointments[7].appointment_date, "2026-11-17");
    setField(appointments[7].doctor_id, "D003");

    setField(appointments[8].appointment_id, "A009");
    setField(appointments[8].appointment_date, "2026-11-18");
    setField(appointments[8].doctor_id, "D001");

    setField(appointments[9].appointment_id, "A010");
    setField(appointments[9].appointment_date, "2026-11-19");
    setField(appointments[9].doctor_id, "D005");

    for (int i = 0; i < 10; i++)
    {
        Add_appointment(appointments[i]);
    }

    cout << "10 Appointments inserted successfully.\n";

    // =====================================
    //          REBUILD ALL INDEXES
    // =====================================

    buildDoctorPrimaryIndex();
    buildDoctorNameIndex();
    buildDoctorSpeciality();

    buildAppointmentPrimaryIndex();
    buildDoctorAppointment();

    cout << "\nInitial data and indexes created successfully!\n";
}

int main()
{
    cout << "\n===== INITIALIZE FILES =====\n";

    initializedoctor_flie();
    initialize_appointment_file();

    cout << "\n===== BUILD INITIAL DATA =====\n";

    initializeInitialData();

    // Build all indexes once after checking/inserting initial data
    buildDoctorPrimaryIndex();
    buildDoctorNameIndex();
    buildDoctorSpeciality();

    buildAppointmentPrimaryIndex();
    buildDoctorAppointment();
    int choice;

    while (true)
    {
        cout << "\n========== HEALTHCARE MANAGEMENT SYSTEM ==========\n";
        cout << "1.  Add Doctor\n";
        cout << "2.  Add Appointment\n";
        cout << "3.  Update Doctor Name\n";
        cout << "4.  Update Doctor ID\n";
        cout << "5.  Update Appointment Date\n";
        cout << "6.  Delete Appointment\n";
        cout << "7.  Delete Doctor\n";
        cout << "8.  Print Doctor Info\n";
        cout << "9.  Print Appointment Info\n";
        cout << "10. Write Query\n";
        cout << "11. Display All Doctors\n";
        cout << "12. Display All Appointments\n";
        cout << "13. DisplayDoctorPrimaryIndex\n";
        cout << "14. DisplayAppointmentPrimaryIndex\n";
        cout << "15. DisplayDoctorSecondaryIndex\n";
        cout << "16. DisplayAppointmentSecondaryIndex\n";
        cout << "17. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Doctor d;
            memset(&d, 0, sizeof(d));

            cout << "Enter Doctor ID: ";
            cin >> d.doctor_id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Doctor Name: ";
            cin.getline(d.doctor_name, 30);

            cout << "Enter Doctor Specialty: ";
            cin.getline(d.doctor_speciality, 20);

            cout << "Enter Doctor Address: ";
            cin.getline(d.address, 30);

            Add_doctor(d);

            break;
        }

        case 2:
        {
            Appointment a;
            memset(&a, 0, sizeof(a));

            cout << "Enter Appointment ID: ";
            cin >> a.appointment_id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Appointment Date: ";
            cin.getline(a.appointment_date, 30);

            cout << "Enter Doctor ID: ";
            cin >> a.doctor_id;

            Add_appointment(a);

            break;
        }

        case 3:
        {
            char doctor_id[15];
            char new_name[30];

            cout << "Enter Doctor ID: ";
            cin >> doctor_id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter New Doctor Name: ";
            cin.getline(new_name, 30);

            update_doctor_by_name(doctor_id, new_name);

            break;
        }

        case 4:
        {
            char old_id[15];
            char new_id[15];

            cout << "Enter Current Doctor ID: ";
            cin >> old_id;

            cout << "Enter New Doctor ID: ";
            cin >> new_id;

            update_doctor_by_id(old_id, new_id);

            break;
        }

        case 5:
        {
            char appointment_id[15];
            char new_date[30];

            cout << "Enter Appointment ID: ";
            cin >> appointment_id;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter New Appointment Date: ";
            cin.getline(new_date, 30);

            update_appointment_date(appointment_id, new_date);

            break;
        }

        case 6:
        {
            char appointment_id[15];

            cout << "Enter Appointment ID to Delete: ";
            cin >> appointment_id;

            delete_appointment_by_id(appointment_id);

            break;
        }

        case 7:
        {
            char doctor_id[15];

            cout << "Enter Doctor ID to Delete: ";
            cin >> doctor_id;

            delete_doctor_by_id(doctor_id);

            break;
        }

        case 8:
        {
            char doctor_id[15];

            cout << "Enter Doctor ID: ";
            cin >> doctor_id;

            search_doctor_by_id(doctor_id);

            break;
        }

        case 9:
        {
            char appointment_id[15];

            cout << "Enter Appointment ID: ";
            cin >> appointment_id;

            search_appointment_by_id(appointment_id);

            break;
        }

        case 10:
        {
            string input;
            Query query;

            cout << "Enter your SQL Query:\n";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            getline(cin, input);

            if (parseQuery(input, query))
            {
                executeQuery(query);
            }
            else
            {
                cout << "Invalid Query!\n";
            }

            break;
        }
        case 11:
        {
            display_all_doctors();
            break;
        }
        case 12:
        {
            display_all_appointments();
            break;
        }
        case 13:
        {
            displayDoctorPrimaryIndex();
            break;
        }

        case 14:
        {
            displayAppointmentPrimaryIndex();
            break;
        }

        case 15:
        {
            cout << "You Want to Display Doctor Index with Name [1] or Specialty [2]: ";

            int c;
            cin >> c;

            if (c == 1)
                displayDoctorNameIndex();
            else if (c == 2)
                displayDoctorSpecialityIndex();
            else
                cout << "Invalid choice";

            break;
        }

        case 16:
        {
            displayApptDoctorIndex();
            break;
        }

        case 17:
        {
            cout << "Exiting Healthcare Management System...\n";
            return 0;
        }
        default:
        {
            cout << "Invalid choice! Please try again.\n";
        }
        }
    }

    return 0;
}

//=====Qureries========
//select all from doctors where doctor name ='Youssef Samir' and doctor specialty='Surgery'
//select doctor name from doctors where doctor name ='Mona Adel'
//select all from doctors where doctor id='D001'
//Select all from Appointments where Doctor ID='D001'
//select all from doctors where doctor specialty='Surgery'
//select all from doctors where doctor name ='Youssef Samir' or doctor specialty='Pediatrics'
