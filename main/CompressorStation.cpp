#include "CompressorStation.h"
#include "utils.h"

using namespace std;

void CompressorStation::read() {
    cout << "Enter the name of the station: ";
    getline(cin, full_name);
    log("The name of the CS has been entered: " + full_name);
    totalShops = readPositive<int>("Enter the number of workshops: ", "Error! The number of workshops must be a non-negative number. Try again.");
    log("The user entered the total number of workshops: " + to_string(totalShops));
    operationalShops = readPositive<int>("Enter the number of workshops in operation: ", "Error! The number of workshops in operation must be a non-negative number and not exceed the total number of workshops. Try again.", totalShops + 1);
    log("The user entered the number of workshops: " + to_string(totalShops));
    efficiency = readPositive<double>("Enter efficiency in % (1-100): ", "Error! The efficiency must be a non-negative number and less than 100. Try again.", 101);
    log("The user entered the efficiency percentage: " + to_string(totalShops) + "%");
    unusedOperationalShops = ((totalShops - operationalShops) / static_cast<double>(totalShops)) * 100;
}

void CompressorStation::display() const {
    if (full_name.empty() && totalShops <= 0 && operationalShops <= 0) {
        cout << "No stations" << endl;
    }
    else {
        cout << " : station: " << full_name
            << ", Total workshops: " << totalShops
            << ", Workshops at work: " << operationalShops
            << setprecision(2) << fixed
            << ", Percentage of idle workshops: " << unusedOperationalShops << "%"
            << ", Efficiency: " << efficiency << "%" << endl;
    }
}

void CompressorStation::load(const string& data) {
    size_t pos1 = data.find(';');
    size_t pos2 = data.find(';', pos1 + 1);
    size_t pos3 = data.find(';', pos2 + 1);
    size_t pos4 = data.find(';', pos3 + 1);

    full_name = data.substr(0, pos1);
    totalShops = stoi(data.substr(pos1 + 1, pos2 - pos1 - 1));
    operationalShops = stoi(data.substr(pos2 + 1, pos3 - pos2 - 1));
    unusedOperationalShops = stoi(data.substr(pos3 + 1, pos4 - pos3 - 1));
    efficiency = stod(data.substr(pos4 + 1, data.length() - pos4 - 1));
}