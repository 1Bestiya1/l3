#include "CompressorStationManager.h"
#include "CompressorStation.h"
#include <iostream>
#include <string>

extern std::map<int, CompressorStation> stations;

using namespace std;

string CompressorStationManager::saveCS(const CompressorStation& stations) const {
    return stations.full_name + ";" + to_string(stations.totalShops) + ";" +
        to_string(stations.operationalShops) + ";" +
        to_string(stations.unusedOperationalShops) + ";" +
        to_string(stations.efficiency);
}

void CompressorStationManager::toggleShop(CompressorStation& stations, int id) {
    if (stations.full_name.empty() && stations.totalShops <= 0 && stations.operationalShops <= 0) {
        cout << "There is no station." << endl;
    }
    else {
        int choice;
        cout << "-----------------------------" << endl;
        cout << " 1. Start new workshop    " << endl;
        cout << " 2. Stop workshop           " << endl;
        cout << "-----------------------------" << endl;
        choice = inputIntInRange("action: ", 1, 3);
        cin.ignore();
        log("������������ ������ ��������: " + choice);


        switch (choice) {
        case 1:
            log("������������ ������ ������ ����");
            if (stations.operationalShops < stations.totalShops) {
                stations.operationalShops++;
                stations.unusedOperationalShops = ((stations.totalShops - stations.operationalShops) / static_cast<double>(stations.totalShops)) * 100;
                cout << "Starting new workshop. now in work: " << stations.operationalShops << endl;
            }
            else {
                cout << "All workshops in work." << endl;
            }
            break;
        case 2:
            log("������������ ������ ��������� ����");
            if (stations.operationalShops > 0) {
                stations.operationalShops--;
                stations.unusedOperationalShops = ((stations.totalShops - stations.operationalShops) / static_cast<double>(stations.totalShops)) * 100;
                cout << "Stoped workshop. Now in work: " << stations.operationalShops << endl;
            }
            else {
                cout << "No working workshops." << endl;

            }
            break;
        default:
            cout << "Again." << endl;

            break;
        }

    }
}

void CompressorStationManager::addStation() {
    CompressorStation station;

    while (stations.find(nextIdKS) != stations.end()) {
        nextIdKS++;
    }

    station.read();
    stations[nextIdKS] = station;

    cout << "station were added." << endl;
    log("������������� ������� ��������� � �������. ID: " + to_string(nextIdKS));

    nextIdKS++;
}

void CompressorStationManager::displayStations() const {
    if (stations.empty()) {
        cout << "No nice stations." << endl;
        return;
    }
    cout << "The list of stations:" << endl;
    cout << endl;
    for (const auto& station : stations) {
        cout << "ID " << station.first;
        station.second.display();
    }
    cout << endl;
    log("������������ �������� ������ ������������� �������.");
}

void CompressorStationManager::removeStation() {
    if (stations.empty()) {
        cout << "the list is empty." << endl;
        return;
    }
    int id;
    id = readPositive<int>("Add ID of the station: ", "Not nice ID.");
    if (id == -1) {
        cout << "Again!" << endl;
        return;
    }

    auto it = stations.find(id);
    if (it == stations.end()) {
        cout << "Error. Station with that ID doesn't exist" << endl;
        return;
    }

    // ������� ����������, ��������� � ������ ������������� ��������
    for (auto connIt = connections.begin(); connIt != connections.end(); ) {
        const Connection& conn = connIt->second;
        if (conn.entryStationId == id || conn.exitStationId == id) {
            // ������� ����������
            connIt = connections.erase(connIt);
            cout << "Connection was deleted with ID of pipe: " << conn.pipeId << endl;
        }
        else {
            ++connIt;
        }
    }

    stations.erase(it);
    cout << "Station was deleted" << endl;
    log("������������� ������� � ID: " + to_string(id) + " ���� ������� ������ � � ������������.");
}

void CompressorStationManager::editStation() {
    if (stations.empty()) {
        cout << "The list is empty." << endl;
        return;
    }
    cout << "Add ID for editing: ";
    string command;
    long id;
    getline(cin, command);
    id = numberOrDefault(command);
    if (id == -1) {
        cout << "Again!" << endl;
        return;
    }

    auto it = stations.find(id);
    if (it != stations.end()) {
        cout << "edit station with ID " << id << endl;
        toggleShop(it->second, it->first);
        log("������������ ������� ������ ���� �� �� � ID: " + to_string(id));
    }
    else {
        cout << "station with ID " << id << " not found." << endl;
    }
}

void CompressorStationManager::searchStationName() {
    string searchName;
    log("������������ �������� ����� �� �� ������� '���'.");
    cout << "Add name of station: ";
    cin >> searchName;
    cin.ignore();
    log("����� �� '���': ������� ���:" + searchName);

    if (stations.empty()) {
        cout << "Nothing" << endl;
        log("������������ ������ �� ����� �� ������� �������.");
    }
    else {
        cout << "Results:\n";
        bool found = false;
        for (const auto& station : stations) {
            if (station.second.full_name.find(searchName) != string::npos) {
                cout << "ID " << station.first;
                station.second.display();
                log("������������ ����� �� ������� '���' �� � ID: " + to_string(station.first));
                found = true;
            }
        }

        if (!found) {
            cout << "nothing" << endl;
            log("������������ ������ �� ����� �� ������� �������.");
        }
    }
}

void CompressorStationManager::searchStationUnused() {
    double unusedOperationalShopsCheck = 0;
    long aroundUnusedOperationalShops;
    string command;
    log("������������ �������� ����� �� �� ������� '����������������� ����'.");
    cout << "Add % of unused cexov: ";
    getline(cin, command);
    aroundUnusedOperationalShops = numberOrDefault(command);
    if (aroundUnusedOperationalShops == -1) {
        cout << "Again" << endl;
        return;
    }

    log("%: " + to_string(unusedOperationalShopsCheck));


    cout << "-----------------------------" << endl;
    cout << "              Menu:            " << endl;
    cout << " 1. = %                         " << endl;
    cout << " 2. > %                         " << endl;
    cout << " 3. < %                        " << endl;
    cout << "-----------------------------" << endl;
    int choice;
    choice = inputIntInRange("Action: ", 1, 3);
    log("������������ ������ ��������: " + to_string(choice));

    if (stations.empty()) {;
        cout << "nothing" << endl;
        log("������������ ������ �� ����� �� ������� �������.");
        return;
    }

    cout << "Results:\n";

    for (const auto& station : stations) {
        aroundUnusedOperationalShops = station.second.unusedOperationalShops;

        bool conditionMet = false;
        switch (choice) {
        case 1:
            conditionMet = (aroundUnusedOperationalShops == unusedOperationalShopsCheck);
            log("������������ ������ ��������: ����� ��������� ��������");
            break;
        case 2:
            conditionMet = (aroundUnusedOperationalShops > unusedOperationalShopsCheck);
            log("������������ ������ ��������: ������ ��������� ��������");
            break;
        case 3:
            conditionMet = (aroundUnusedOperationalShops < unusedOperationalShopsCheck);
            log("������������ ������ ��������: ������ ��������� ��������");
            break;
        default:
            cout << "Again!" << endl;
            return;
        }

        if (conditionMet) {
            cout << "ID " << station.first;
            station.second.display();
            log("������������ ����� �� ������� '����������������� ����' �� � ID: " + to_string(station.first));
        }
    }
}

void CompressorStationManager::searchStationMenu() {
    cout << "-----------------------------" << endl;
    cout << "            Menu:          " << endl;
    cout << " 1. Name                   " << endl;
    cout << " 2. %                      " << endl;
    cout << " 0. back                  " << endl;
    cout << "-----------------------------" << endl;
}

void CompressorStationManager::searchStationSwitch() {
    if (stations.empty()) {
        cout << "The list is emty" << endl;
        return;
    }
    int choice;
    while (true) {
        searchStationMenu();
        choice = inputIntInRange("action: ", 0, 2);
        log("������������ ������ ��������: " + to_string(choice));

        switch (choice) {
        case 1:
            searchStationName();
            break;
        case 2:
            searchStationUnused();
            break;
        case 0:
            CompressorStationSwitch();
            log("������������ ������� � ���� '���������� ��'");
        default:
            cout << "Again" << endl;
            log("������������ �����: " + choice);
            break;
        }
    }
}

void CompressorStationManager::displayCompressorStationMenu() {
    cout << "-----------------------------" << endl;
    cout << "            Menu:          " << endl;
    cout << " 1. Add station            " << endl;
    cout << " 2. edit station       " << endl;
    cout << " 3. delete station             " << endl;
    cout << " 4. List of stations  " << endl;
    cout << " 5. filter       " << endl;
    cout << " 0. back                  " << endl;
    cout << "-----------------------------" << endl;

}

void CompressorStationManager::CompressorStationSwitch() {
    void runProgram();
    int choice;

    while (true) {
        displayCompressorStationMenu();
        choice = inputIntInRange("action: ", 0, 5);
        log("������������ ������ ��������: " + to_string(choice));

        switch (choice) {
        case 1:
            addStation();
            break;
        case 2:
            editStation();
            break;
        case 3:
            removeStation();
            break;
        case 4:
            displayStations();
            break;
        case 5:
            log("������������ ������� � ���� '��������� ��'");
            searchStationSwitch();
            break;
        case 0:
            log("������������ ������� � '�������� ����'");
            runProgram();
        default:
            cout << "Again." << endl;
            log("������������ �����: " + choice);
            break;
        }
    }
}