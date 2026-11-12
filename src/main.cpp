
#include <iostream>
#include <limits>
#include "doctor.h"
#include "AppointmentsFile.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"
#include "QueryParser.h"
#include "QueryEngine.h"

using namespace std;

int main()
{
    // =====================================
    //       Initialize Files
    // =====================================

    cout << "\n===== INITIALIZE FILES =====\n";

    initializedoctor_flie();
    initialize_appointment_file();

    // =====================================
    //       Build Indexes
    // =====================================

    cout << "\n===== BUILD INDEXES =====\n";

    buildDoctorPrimaryIndex();
    buildDoctorNameIndex();
    buildDoctorSpeciality();

    buildAppointmentPrimaryIndex();
    buildDoctorAppointment();

    int choice;

    while (true) {
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
        case 11: {
            display_all_doctors();
            break;
        }
        case 12: {
            display_all_appointments();
            break;
        }
        case 13: {
            displayDoctorPrimaryIndex();
            break;
        }

        case 14: {
            displayAppointmentPrimaryIndex();
            break;
        }

        case 15: {
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

        case 16: {
            displayApptDoctorIndex();
            break;
        }

        case 17: {
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
