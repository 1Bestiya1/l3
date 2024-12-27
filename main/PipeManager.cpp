#include "PipeManager.h"
#include "Pipe.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

extern map<int, Pipe> pipes;

string PipeManager::savePipe(const Pipe& pipe) const {
    return pipe.full_name + ";" + to_string(pipe.length) + ";" + to_string(pipe.diameter) + ";" + to_string(pipe.underRepair);
}

void PipeManager::toggleRepair(Pipe& pipe, int id) {
    if (pipe.length > 0) {
        pipe.underRepair = !pipe.underRepair;
        cout << "ID: " << id << " - 'In remont' edited to: " << (pipe.underRepair ? "Yes" : "No") << endl;
        log("Пользователь установил состояние трубы ID " + to_string(id) + string(pipe.underRepair ? " в ремонте" : " в исправном состоянии"));
    }
    else {
        cout << "There is no pipe" << endl;
    }
}

void PipeManager::addPipe() {
    Pipe newPipe;
    newPipe.read();

    while (pipes.find(nextId) != pipes.end()) {
        nextId++;
    }

    pipes[nextId] = newPipe;

    cout << "Pipe added. ID: " << nextId << endl;
    log("Труба добавлена в систему. ID: " + to_string(nextId));

    nextId++;
}

tuple<int, int, int> PipeManager::addPipeGTN() {
    Pipe newPipe;
    cout << "Add the pipe with diametr 500, 700, 1000 or 1400" << endl;

    newPipe.read();

    while (pipes.find(nextId) != pipes.end()) {
        nextId++;
    }

    while (newPipe.diameter != 500.0 && newPipe.diameter != 700.0 &&
        newPipe.diameter != 1000.0 && newPipe.diameter != 1400.0) {
        cout << "not nice diametr, try again." << endl;

        if (!pipes.empty()) {
            auto lastElement = --pipes.end();
            cout << "Deleted pipe with ID: " << lastElement->first << endl;
            pipes.erase(lastElement);
        }

        newPipe.read();
    }

    pipes[nextId] = newPipe;

    cout << "pipe added. ID: " << nextId << endl;

    return make_tuple(nextId, newPipe.diameter, newPipe.length);
}

void PipeManager::removeConnectionsUsingPipe(int pipeId) {
    for (auto it = connections.begin(); it != connections.end();) {
        if (it->second.pipeId == pipeId) {
            it = connections.erase(it);
        }
        else {
            ++it;
        }
    }
    cout << "Connections used pipe with ID " << pipeId << ", were deleted." << endl;
}

void PipeManager::editPipes() {
    if (pipes.empty()) {
        cout << "The list is empty" << endl;
        return;
    }
    cout << "Add ID of pipes, using ',' (or 'pls' to edit all): ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    if (input == "pls") {
        for (auto& pipePair : pipes) {
            toggleRepair(pipePair.second, pipePair.first);
            if (pipePair.second.underRepair) {
                removeConnectionsUsingPipe(pipePair.first);
            }
        }
        cout << "All pipes were updated." << endl;
        log("Пользователь обновил статус 'в ремонте' у всех труб.");
    }
    else {
        while (getline(ss, idStr, ',')) {
            try {
                int id = stoi(idStr);
                ids.push_back(id);
            }
            catch (const invalid_argument&) {
                cout << "Error. again" << endl;
                return;
            }
            catch (const out_of_range&) {
                cout << "Error. value is out of range" << endl;
                return;
            }
        }

        bool edited = false;
        for (int id : ids) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                toggleRepair(it->second, id);
                if (it->second.underRepair) {
                    removeConnectionsUsingPipe(id);
                }
                edited = true;
            }
            else {
                nonExistentIds.push_back(id);
            }
        }

        if (edited) {
            cout << "Status updated." << endl;
        }

        if (!nonExistentIds.empty()) {
            cout << "Pipe(s) with ID: ";
            for (size_t i = 0; i < nonExistentIds.size(); ++i) {
                cout << nonExistentIds[i];
                if (i < nonExistentIds.size() - 1) {
                    cout << ", ";
                }
            }
            cout << " doesn't exist." << endl;
        }
    }
}

void PipeManager::deletePipes() {
    if (pipes.empty()) {
        cout << "The list is empty." << endl;
        return;
    }
    cout << "add pipes ID, using ',' (or 'pls' to delete all): ";
    string input;
    getline(cin, input);

    if (input == "pls") {
        pipes.clear();
        connections.clear();
        cout << "All pipes and connections were deleted." << endl;
        log("Пользователь удалил весь список труб и соединений.");
        return;
    }

    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    while (getline(ss, idStr, ',')) {
        try {
            int id = stoi(idStr);
            ids.push_back(id);
        }
        catch (const invalid_argument&) {
            cout << "Error: '" << idStr << "' not nice number." << endl;
            log("Ошибка: '" + idStr + "' не является допустимым целым числом.");
            return;
        }
        catch (const out_of_range&) {
            cout << "Error: value '" << idStr << "' is out of the range." << endl;
            log("Ошибка: Значение '" + idStr + "' выходит за пределы допустимого диапазона.");
            return;
        }
    }

    for (int id : ids) {
        auto it = pipes.find(id);

        if (it != pipes.end()) {
            pipes.erase(it);
            cout << "pipe with ID " << id << " was deleted." << endl;
            log("Удалена труба с ID: " + to_string(id));

            for (auto connIt = connections.begin(); connIt != connections.end(); ) {
                if (connIt->second.pipeId == id) {
                    connIt = connections.erase(connIt);
                }
                else {
                    ++connIt;
                }
            }
        }
        else {
            nonExistentIds.push_back(id);
        }
    }

    if (!nonExistentIds.empty()) {
        cout << "Pipe(s) with ID: ";
        for (size_t i = 0; i < nonExistentIds.size(); ++i) {
            cout << nonExistentIds[i];
            if (i < nonExistentIds.size() - 1) {
                cout << ", ";
            }
        }
        cout << " doesn't exist." << endl;
    }
}

void PipeManager::displayAllPipes() {
    if (pipes.empty()) {
        cout << "There is no pipes." << endl;
    }
    else {
        cout << "List:" << endl;
        cout << endl;
        for (const auto& pipe : pipes) {
            cout << "ID " << pipe.first;
            pipe.second.display();
        }
        cout << endl;
    }

    log("Пользователь запросил список труб.");
}

void PipeManager::editPipesFound(const vector<int>& foundIds) {
    cout << "add pipes ID to edit, using ',' (or 'pls' to edit all): ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    if (input == "pls") {
        for (int id : foundIds) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                toggleRepair(it->second, id);
                if (it->second.underRepair) {
                    removeConnectionsUsingPipe(id); // Удаляем соединения, если труба в ремонте
                }
            }
        }
        cout << "all pipes were updated." << endl;
        log("Пользователь обновил статус 'в ремонте' у всех найденных труб.");
    }
    else {
        while (getline(ss, idStr, ',')) {
            try {
                int id = stoi(idStr);
                ids.push_back(id);
            }
            catch (const invalid_argument&) {
                cout << "Error: again." << endl;
                return;
            }
            catch (const out_of_range&) {
                cout << "Error: is out of the range." << endl;
                return;
            }
        }

        bool edited = false;
        for (int id : ids) {
            if (find(foundIds.begin(), foundIds.end(), id) != foundIds.end()) {
                auto it = pipes.find(id);
                if (it != pipes.end()) {
                    toggleRepair(it->second, id);
                    if (it->second.underRepair) {
                        removeConnectionsUsingPipe(id); // Удаляем соединения, если труба в ремонте
                    }
                    edited = true;
                }
                else {
                    nonExistentIds.push_back(id);
                }
            }

            if (edited) {
                cout << "Status updated." << endl;
            }

            if (!nonExistentIds.empty()) {
                cout << "Pipes with ID: ";
                for (size_t i = 0; i < nonExistentIds.size(); ++i) {
                    cout << nonExistentIds[i];
                    if (i < nonExistentIds.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << " doesn't exist." << endl;
            }
        }
    }
}

void PipeManager::deletePipesFound(const vector<int>& foundIds) {
    cout << "Add pipes ID, using ',' (or 'pls' to delete all): ";
    string input;
    getline(cin, input);

    if (input == "pls") {
        for (int id : foundIds) {
            auto it = pipes.find(id);
            if (it != pipes.end()) {
                removeConnectionsUsingPipe(id); // Удаляем соединения перед удалением трубы
                pipes.erase(it);
                cout << "Pipe with ID " << id << " was deleted." << endl;
                log("Удалена труба с ID: " + to_string(id));
            }
        }
        cout << "All pipes were deleted." << endl;
        log("Пользователь удалил все найденные трубы.");
        return;
    }

    stringstream ss(input);
    string idStr;
    vector<int> ids;
    vector<int> nonExistentIds;

    while (getline(ss, idStr, ',')) {
        try {
            int id = stoi(idStr);
            if (find(foundIds.begin(), foundIds.end(), id) != foundIds.end()) {
                ids.push_back(id);
            }
            else {
                nonExistentIds.push_back(id);
            }
        }
        catch (const invalid_argument&) {
            cout << "Error: '" << idStr << "' again." << endl;
            log("Ошибка: '" + idStr + "' не является допустимым целым числом.");
            return;
        }
        catch (const out_of_range&) {
            cout << "Error. value '" << idStr << "' is out of the range." << endl;
            log("Ошибка: Значение '" + idStr + "' выходит за пределы допустимого диапазона.");
            return;
        }
    }

    for (int id : ids) {
        auto it = pipes.find(id);
        if (it != pipes.end()) {
            removeConnectionsUsingPipe(id); // Удаляем соединения перед удалением трубы
            pipes.erase(it);
            cout << "Pipe with ID " << id << " was deleted." << endl;
            log("Удалена труба с ID: " + to_string(id));
        }
    }

    if (!nonExistentIds.empty()) {
        cout << "pipe(s) with ID: ";
        for (size_t i = 0; i < nonExistentIds.size(); ++i) {
            cout << nonExistentIds[i];
            if (i < nonExistentIds.size() - 1) {
                cout << ", ";
            }
        }
        cout << " doesn't exist." << endl;
    }
}

void PipeManager::displayPipeMenuFound() {
    cout << "-----------------------------" << endl;
    cout << "         Menu:      " << endl;
    cout << " 1. Delete pipe          " << endl;
    cout << " 2. Edit pipe    " << endl;
    cout << " 0. Back                  " << endl;
    cout << "-----------------------------" << endl;
}

void PipeManager::handleFoundPipeMenu(const vector<int>& foundPipeIds) {
    int choice;
    while (true) {
        displayPipeMenuFound();
        choice = inputIntInRange("Action: ", 0, 2);
        log("Пользователь выбрал действие: " + to_string(choice));


        switch (choice) {
        case 1:
            deletePipesFound(foundPipeIds);
            break;
        case 2:
            editPipesFound(foundPipeIds);
            break;
        case 0:
            log("Пользователь перешёл в 'Фильтрация труб'");
            return;
        default:
            cout << "Again." << endl;
            log("Некорректный выбор: " + choice);
            break;
        }
    }
}

void PipeManager::searchPipesName() {
    string searchName;
    log("Пользователь запустил поиск труб по фильтру 'Имя'.");
    cout << "Add pipe name: ";
    cin >> searchName;
    cin.ignore();
    log("Поиск труб 'Имя': Введено имя:" + searchName);

    if (pipes.empty()) {
        cout << "Nothing" << endl;
        log("Пользователь ничего не нашёл по данному фильтру.");
    }
    else {
        cout << "results:\n";
        vector<int> foundPipeIds;
        bool found = false;

        for (const auto& pipe : pipes) {
            if (pipe.second.full_name.find(searchName) != string::npos) {
                cout << "ID " << pipe.first;
                pipe.second.display();
                log("Пользователь нашёл по фильтру 'Имя' трубу с ID: " + to_string(pipe.first));
                foundPipeIds.push_back(pipe.first);
                found = true;
            }
        }
        if (!found) {
            cout << "Nothing" << endl;
            log("Пользователь ничего не нашёл по данному фильтру.");
            return;
        }
        else {
            handleFoundPipeMenu(foundPipeIds);
        }
    }
}

void PipeManager::searchPipesRepair() {
    bool repairStatus;
    log("Пользователь запустил поиск труб по фильтру 'Статус ремонта'.");
    cout << "In remont? (1 - yes, 0 - no):";
    repairStatus = inputIntInRange(" ", 0, 1);
    log("Поиск труб 'Статус ремонта': Введён статус (1 - да, 0 - нет):" + to_string(repairStatus));

    if (pipes.empty()) {
        cout << "Nothing" << endl;
        log("Пользователь ничего не нашёл по данному фильтру.");
        return;
    }
    cout << "Results:\n";

    vector<int> foundPipeIds;

    for (const auto& pipe : pipes) {
        if (pipe.second.underRepair == repairStatus) {
            cout << "ID " << pipe.first;
            pipe.second.display();
            log("Пользователь нашёл по фильтру 'Статус ремонта' трубу с ID: " + to_string(pipe.first));
            foundPipeIds.push_back(pipe.first);

        }
    }
    handleFoundPipeMenu(foundPipeIds);
    if (foundPipeIds.empty()) {
        cout << "Nothing" << endl;
        log("Пользователь ничего не нашёл по данному фильтру.");
        return;
    }
}

void PipeManager::searchPipeMenu() {
    cout << "-----------------------------" << endl;
    cout << "            Menu:          " << endl;
    cout << " 1. Name        " << endl;
    cout << " 2. Satus      " << endl;
    cout << " 0. Back                  " << endl;
    cout << "-----------------------------" << endl;
}

void PipeManager::searchPipeSwitch() {
    if (pipes.empty()) {
        cout << "The list is empty" << endl;
        return;
    }
    int choice;
    while (true) {
        searchPipeMenu();
        choice = inputIntInRange("Action: ", 0, 5);
        log("Пользователь выбрал действие: " + to_string(choice));

        switch (choice) {
        case 1:
            searchPipesName();
            break;
        case 2:
            searchPipesRepair();
            break;
        case 0:
            PipeSwitch();
            log("Пользователь перешёл в меню 'Управление трубами'");
        default:
            cout << "Again." << endl;
            log("Некорректный выбор: " + choice);
            break;
        }
    }
}

void PipeManager::displayPipeMenu() {
    cout << "-----------------------------" << endl;
    cout << "            Menu:          " << endl;
    cout << " 1. Add pipe         " << endl;
    cout << " 2. Edit pipe    " << endl;
    cout << " 3. Delete pipe          " << endl;
    cout << " 4. List of pipes            " << endl;
    cout << " 5. Filter       " << endl;
    cout << " 0. Back                  " << endl;
    cout << "-----------------------------" << endl;
}

void PipeManager::PipeSwitch() {
    void runProgram();
    int choice;
    while (true) {
        displayPipeMenu();
        choice = inputIntInRange("Action: ", 0, 5);
        log("Пользователь выбрал действие: " + to_string(choice));
        switch (choice) {
        case 1:
            addPipe();
            break;
        case 2:
            editPipes();
            break;
        case 3:
            deletePipes();
            break;
        case 4:
            displayAllPipes();
            break;
        case 5:
            searchPipeSwitch();
            log("Пользователь перешёл в меню 'Фильрация труб'");
            break;
        case 0:
            log("Пользователь перешёл в 'Основное меню'");
            runProgram();
        default:
            cout << "Again." << endl;
            log("Некорректный выбор: " + choice);
            break;
        }
    }
}