#include <iostream>
#include <fstream>
#include <vector>
#include "AddressBook.pb.h"
 
using namespace std;

enum RDWT // read or write
{
    READ = 0,
    WRITE = 1
};

// This function fills in a Person message based on user input.
void PromptForAddress(tutorial::Person* person) 
{
    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');
 
    cout << "Enter name: ";
    getline(cin, *person->mutable_name());
 
    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty()) 
    {
        person->set_email(email);
    }
 
    while (true) 
    {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty()) 
        {
            break;
        }
 
        tutorial::Person::PhoneNumber* phone_number = person->add_phone();
        phone_number->set_number(number);
 
        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile") 
        {
            phone_number->set_type(tutorial::Person::MOBILE);
        } 
        else if (type == "home") 
        {
            phone_number->set_type(tutorial::Person::HOME);
        }
         else if (type == "work") 
        {
            phone_number->set_type(tutorial::Person::WORK);
        }
        else 
        {
            cout << "Unknown phone type.  Using default." << endl;
        }
    }
}


void DisplayInfo(tutorial::AddressBook *address)
{
    for (int i = 0; i < address->person_size(); ++i)
    {   
        tutorial::Person *person = address->mutable_person(i);
        std::cout << "name: " << person->name() 
                  << " id: " << person->id();
        if (person->has_email()) cout << " email:" << person->email();
        std::cout << endl;
        for (int p = 0; p < person->phone_size(); ++p) 
        {
            cout << " index: " << p
                 << " type: " << person->phone(p).type()
                 << " number: " << person->phone(p).number()
                 << std::endl;
        }
    }
}
 
int main(int argc, char *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
 
    if (argc != 2) 
    {
        cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
        return -1;
    }
 
    tutorial::AddressBook address_book;
    bool read_or_write = WRITE;
    {
        // Read the existing address book.
        fstream input(argv[1], ios::in | ios::binary);
        if (!input) {
          cout << argv[1] << ": File not found.  Creating a new file." << endl;
          read_or_write = WRITE;
        } else if (!address_book.ParseFromIstream(&input)) {
          cerr << "Failed to parse address book." << endl;
          return -1;
        } else {
          read_or_write = READ;
        }
    }
 
    // Add an address.
    if (read_or_write) PromptForAddress(address_book.add_person());
    else DisplayInfo(&address_book);
 
    {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
      cerr << "Failed to write address book." << endl;
      return -1;
    }
    }
 
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
 
    return 0;
}


