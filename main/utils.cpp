#include "utils.h"
using namespace std;

void log(const string& action) {
    ofstream logFile("log.txt", ios::app);
    if (logFile.is_open()) {
        logFile << " - " << action << endl;
        logFile.close();
    }
    else {
        cerr << "Error i can't open file for logging (<>_<>)" << endl;
    }
}

template <typename T>
T readPositive(const string& prompt, const string& errorMessage, const T maxValue) {
    T value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() ||
            (std::is_floating_point<T>::value ? value <= 0 : value < 0) ||
            value >= maxValue) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << errorMessage << endl;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            log("(I dead) Added value: " + to_string(value));
            return value;
        }
    }
}

template double readPositive<double>(const string& prompt, const string& errorMessage, const double);
template int readPositive<int>(const string& prompt, const string& errorMessage, const int);

long numberOrDefault(const string& input, long defaultValue) {
    if (input.empty() ||
        find_if(input.begin(), input.end(), [](unsigned char c) { return !isdigit(c); }) != input.end()) {
        return defaultValue;
    }
    return strtol(input.c_str(), NULL, 10);
}

int inputIntInRange(const string& prompt, int minValue, int maxValue) {
    int value;

    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        stringstream ss(input);
        if (ss >> value) {
            if (ss.eof()) {
                if (value >= minValue && value <= maxValue) {
                    return value;
                }
                else {
                    cout << "Ошибка: введите число от " << minValue << " до " << maxValue << "." << endl;
                }
            }
            else {
                cout << "Incorrect value. So that your hands wither (*_*)" << endl;
            }
        }
        else {
            cout << "Error. Try again (maybe pomozhet..hi-hi-ha-ha)" << endl;
        }
    }
}