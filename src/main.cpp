
#include <iostream>
#include "doctor.h"
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

    // =====================================
    // 2. Build Indexes
    // =====================================

    cout << "\n===== BUILD INDEXES =====\n";

    buildDoctorPrimaryIndex();
    buildDoctorNameIndex();
    buildDoctorSpeciality();

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



    return 0;
}
