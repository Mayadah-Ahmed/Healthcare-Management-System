#include "Doctor.h"
#include "PrimaryIndex.h"
#include "AppointmentsFile.h"
#include <filesystem>

int main()
{
    // Doctors
    initializedoctor_flie();

    Add_doctor({ "D001", "Dr. Smith", "Cardiology", "123 Main St" });
    Add_doctor({ "D002", "Dr. Johnson", "Neurology", "456 Elm St" });
    Add_doctor({ "D003", "Dr. Williams", "Pediatrics", "789 Oak St" });
    Add_doctor({ "D004", "Dr. Brown", "Dermatology", "321 Pine St" });
    Add_doctor({ "D005", "Dr. Jones", "Orthopedics", "654 Maple St" });

    display_all_doctors();

    // Doctors Primary Index
    buildDoctorPrimaryIndex();
    displayDoctorPrimaryIndex();

    int result = binarySearchDoctor("D003");

    if (result != -1)
    {
        cout << "Doctor found at RRN: " << result << endl;
    }
    else
    {
        cout << "Doctor not found" << endl;
    }

    // Appointments
    initialize_appointment_file();

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

    display_all_appointments();

    return 0;
}