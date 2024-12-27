#include "Pipe.h"
#include "utils.h"

using namespace std;

void Pipe::read() {
    cout << "Add name to the puppy (Pipe): ";
    getline(cin, full_name);
    log("Added name: " + full_name);

    length = readPositive<double>("Add the lenght: ", "Nadoel..Try again.");
    log("Added lenght: " + to_string(length));

    diameter = readPositive<double>("Add the diametr: ", "Pu-pu-pu. Try again.");
    log("Added diametr: " + to_string(diameter));
    underRepair = false;
}

void Pipe::display() const {
    if (length <= 0 && diameter <= 0) {
        cout << "There is no pipes (~_~)" << endl;
        log("There is no info about pipes");
    }
    else {
        cout << " : Pipe: " << full_name
            << ", Lenght: " << length
            << " Diametr: " << diameter
            << " In Remont: "
            << (underRepair ? "Yes" : "No")
            << endl;
        log("Let's watch the data about pipes: " + full_name);
    }
}

void Pipe::load(const string& data) {
    size_t pos1 = data.find(';');
    size_t pos2 = data.find(';', pos1 + 1);
    size_t pos3 = data.find(';', pos2 + 1);

    full_name = data.substr(0, pos1);
    length = stod(data.substr(pos1 + 1, pos2 - pos1 - 1));
    diameter = stod(data.substr(pos2 + 1, pos3 - pos2 - 1));
    underRepair = data.substr(pos3 + 1) == "1";
}