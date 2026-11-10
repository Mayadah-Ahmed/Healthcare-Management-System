
#include <iostream>
#include "doctor.h"
#include "AppointmentsFile.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"

using namespace std;

// Display all indexes
void display_all_indexes()
{
    cout << "\n===== DISPLAY PRIMARY INDEX =====\n";
    displayDoctorPrimaryIndex();

    cout << "\n===== DISPLAY NAME SECONDARY INDEX =====\n";
    displayDoctorNameIndex();

    cout << "\n===== DISPLAY SPECIALITY SECONDARY INDEX =====\n";
    displayDoctorSpecialityIndex();
}

int main()
{
    // =====================================
    // 1. Initialize Files
    // =====================================

    cout << "\n===== INITIALIZE FILES =====\n";

    initializedoctor_flie();
    initialize_appointment_file();

    // =====================================
    // 2. Build Indexes
    // =====================================

    cout << "\n===== BUILD INDEXES =====\n";

    buildDoctorPrimaryIndex();
    buildDoctorNameIndex();
    buildDoctorSpeciality();

    buildAppointmentPrimaryIndex();
    buildDoctorAppointment();


    // =====================================
    // 3. Add Doctors
    // =====================================

    cout << "\n===== ADD DOCTORS =====\n";

    Doctor d1 = { "D001", "Ahmed", "Cardiology", "Cairo" };
    Doctor d2 = { "D002", "Mona", "Neurology", "Giza" };
    Doctor d3 = { "D003", "Omar", "Cardiology", "Alex" };
    Doctor d4 = { "D004", "Sara", "Pediatrics", "Cairo" };
    Doctor d5 = { "D005", "Ali", "Neurology", "Giza" };

    Add_doctor(d1);
    Add_doctor(d2);
    Add_doctor(d3);
    Add_doctor(d4);
    Add_doctor(d5);

    // =====================================
    // 4. Display All Doctors
    // =====================================

    cout << "\n===== DISPLAY ALL DOCTORS =====\n";

    display_all_doctors();

    // Display all indexes after insertion
    display_all_indexes();

    // =====================================
    // 5. Search By ID
    // =====================================

    cout << "\n===== SEARCH BY ID =====\n";

    search_doctor_by_id("D001");

    // =====================================
    // 6. Search By Name
    // =====================================

    cout << "\n===== SEARCH BY NAME =====\n";

    search_doctor_by_name("Ahmed");

    // =====================================
    // 7. Search By Speciality
    // =====================================

    cout << "\n===== SEARCH BY SPECIALITY =====\n";

    search_doctor_by_speciality("Cardiology");

    // =====================================
    // 8. Update Doctor ID
    // =====================================

    cout << "\n===== UPDATE DOCTOR ID =====\n";

    update_doctor_by_id("D001", "D010");

    search_doctor_by_id("D010");

    display_all_indexes();

    // =====================================
    // 9. Update Doctor Name
    // =====================================

    cout << "\n===== UPDATE DOCTOR NAME =====\n";

    update_doctor_by_name("D010", "Mohamed");

    cout << "\nSearch by old name:\n";
    search_doctor_by_name("Ahmed");

    cout << "\nSearch by new name:\n";
    search_doctor_by_name("Mohamed");

    display_all_indexes();

    // =====================================
    // 10. Update Doctor Speciality
    // =====================================

    cout << "\n===== UPDATE DOCTOR SPECIALITY =====\n";

    update_doctor_by_speciality("D010", "Surgery");

    cout << "\nSearch by old speciality:\n";
    search_doctor_by_speciality("Cardiology");

    cout << "\nSearch by new speciality:\n";
    search_doctor_by_speciality("Surgery");

    display_all_indexes();

    // =====================================
    // 11. Delete Doctor
    // =====================================

    cout << "\n===== DELETE DOCTOR =====\n";

    delete_doctor_by_id("D010");

    display_all_indexes();

    // =====================================
    // 12. Test After Delete
    // =====================================

    cout << "\n===== TEST AFTER DELETE =====\n";

    cout << "\nSearch by deleted ID:\n";
    search_doctor_by_id("D010");

    cout << "\nSearch by deleted name:\n";
    search_doctor_by_name("Mohamed");

    cout << "\nSearch by deleted speciality:\n";
    search_doctor_by_speciality("Surgery");

    // =====================================
    // 13. Final Display
    // =====================================

    cout << "\n===== FINAL DOCTORS =====\n";

    display_all_doctors();

    display_all_indexes();


    //=====================================
    //===== appointment tests==============
    //=====================================

    cout<< "\n===== APPOINTMENTS: ADD =====\n";

    Add_appointment({ "A001", "2026-11-20 10:00", "D001" });
    Add_appointment({ "A002", "2026-11-20 11:00", "D001" });
    Add_appointment({ "A003", "2026-11-21 09:00", "D002" });
    Add_appointment({ "A004", "2026-11-21 10:30", "D002" });
    Add_appointment({ "A005", "2026-11-22 12:00", "D003" });
    Add_appointment({ "A006", "2026-11-22 13:00", "D003" });
    Add_appointment({ "A007", "2026-11-23 09:30", "D004" });
    Add_appointment({ "A008", "2026-11-23 11:15", "D004" });
    Add_appointment({ "A009", "2026-11-24 10:00", "D005" });
    Add_appointment({ "A010", "2026-11-24 14:00", "D005" });
    Add_appointment({ "A010", "2026-11-24 14:00", "D005" });

    cout << "\n===== APPOINTMENTS: DISPLAY ALL =====\n";
    display_all_appointments();


    cout << "\n===== SEARCH A002, THEN UPDATE ITS DATE =====\n";
    search_appointment_by_id("A002");
    update_appointment_date("A002", "2027-01-01");
    search_appointment_by_id("A002");



    cout << "\n===== D002 APPOINTMENTS =====\n";
    find_appointments_by_doctor("D002");


    cout << "\n===== DELETE A003 =====\n";
    delete_appointment_by_id("A003");
    search_appointment_by_id("A003");
    find_appointments_by_doctor("D002");


    cout << "\n===== ADD A011\n";
    cout << "A011 written at RRN = " << Add_appointment({ "A011", "2026-11-25 10:00", "D002" }) << endl;
    display_all_appointments();


    cout << "\n===== DELETE A MISSING ID =====\n";
    delete_appointment_by_id("A999");


    return 0;
}
