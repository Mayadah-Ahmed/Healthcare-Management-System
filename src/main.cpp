#include "Doctor.h"
#include "PrimaryIndex.h"
#include "AppointmentsFile.h"
#include <filesystem>

int main()
{
    //==============Doctors================
    initializedoctor_flie();

    Add_doctor({ "D001", "Dr. Smith", "Cardiology", "123 Main St" });
    Add_doctor({ "D002", "Dr. Johnson", "Neurology", "456 Elm St" });
    Add_doctor({ "D003", "Dr. Williams", "Pediatrics", "789 Oak St" });
    Add_doctor({ "D004", "Dr. Brown", "Dermatology", "321 Pine St" });
    Add_doctor({ "D005", "Dr. Jones", "Orthopedics", "654 Maple St" });

    // display_all_doctors();
    buildDoctorPrimaryIndex();
    displayDoctorPrimaryIndex();

    cout << "\nsearch D003 -> RRN " << binarySearchDoctor("D003") << " (expect 2)\n";
    cout << "search D001 -> RRN " << binarySearchDoctor("D001") << " (expect 0)\n";
    cout << "search D999 -> RRN " << binarySearchDoctor("D999") << " (expect -1)\n";

    cout << "\nduplicate insert D001: " << insertDoctorPrimaryIndex("D001", 9) << " (expect 0)\n";
    displayDoctorPrimaryIndex();

    cout << "\nkey update D001 -> D006: " << updateDoctorPrimaryIndex("D001", "D006") << " (expect 1)\n";
    displayDoctorPrimaryIndex();     

    cout << "\nremove D002: " << removeDoctorPrimaryIndex("D002") << " (expect 1)\n";
    cout << "remove D002 again: " << removeDoctorPrimaryIndex("D002") << " (expect 0)\n";
    displayDoctorPrimaryIndex();
  


    
    //==============Appointments================
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

    // display_all_appointments();
    buildAppointmentPrimaryIndex();
    displayAppointmentPrimaryIndex();
cout << "\nsearch A002 -> RRN " << binarySearchAppointment("A002") << " (expect 1)\n";

    // Delete in the file, then mirror it in the index
    int rrn = delete_appointment_by_id("A002");   
    if (rrn != -1) removeAppointmentIndex("A002");
    cout << "search A002 after delete -> " << binarySearchAppointment("A002") << " (expect -1)\n";

    // Add again: reuses the deleted slot (RRN 1), so the index gets that same RRN
    rrn = Add_appointment({ "A011", "2026-11-23 09:30", "D004" });
    if (rrn != -1) insertAppointmentIndex("A011", rrn);
    cout << "search A011 -> RRN " << binarySearchAppointment("A011") << " (expect 1, the reused slot)\n";

    displayAppointmentPrimaryIndex(); 
 // Rebuild from the file: the result must match the index we maintained by hand
    buildAppointmentPrimaryIndex();
    displayAppointmentPrimaryIndex();


    return 0;
}