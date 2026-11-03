#include "Doctor.h"
#include <filesystem>
int main()
{


    initializedoctor_flie();
    Add_doctor({ "D001", "Dr. Smith", "Cardiology", "123 Main St" });
    Add_doctor({ "D002", "Dr. Johnson", "Neurology", "456 Elm St" });
    Add_doctor({ "D003", "Dr. Williams", "Pediatrics", "789 Oak St" });
    Add_doctor({ "D004", "Dr. Brown", "Dermatology", "321 Pine St" });
    Add_doctor({ "D005", "Dr. Jones", "Orthopedics", "654 Maple St" });
    Add_doctor({ "D005", "Dr. Jones", "Orthopedics", "654 Maple St" });//the doctor already exists.

    display_all_doctors();

    //delete_docotor_by_id("D003");

    //delete_docotor_by_id("D005");

    //Add_doctor({ "D006", "Dr. Jones", "Orthopedics", "654 Maple St" });

    //display_all_doctors;


    //delete_docotor_by_id("D003");

    //display_all_doctors();

    //delete_docotor_by_id("D003");

    //update_doctor_by_id("D003", { "D002", "Dr. Johnson", "Neurosurgery", "456 Elm St" });
    //cout << endl;
    //search_doctor_by_id("D002");
    //cout << endl;
    //search_doctor_by_name("Dr. Johnson");
    //cout << endl;
    //search_doctor_by_speciality("Cardiology");
    //cout << endl;
    //update_doctor_by_id("D002", { "D002", "Dr. Johnson", "Neurosurgery", "456 Elm St" });
    //cout << endl;
    //display_all_doctors();

    return 0;
}