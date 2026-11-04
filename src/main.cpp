#include <filesystem>
#include "AppointmentsFile.h"


int main()
{
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
    Add_appointment({ "A010", "2026-11-24 14:00", "D005" }); 

    display_all_appointments();

    //delete_appointment_by_id("A003");
   // Add_appointment({ "A011", "2026-11-25 10:00", "D001" });
	//display_all_appointments();
    //update_appointment_date("A002", "2027-01-01");
   // search_appointment_by_id("A002");
   // search_appointment_by_id("A003");

    return 0;
}