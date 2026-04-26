#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>
// For clocks, vectors, threads

using namespace std;

class Bus;
class Bus_system;

// ============ UI HELPER FUNCTIONS ============
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printHeader(string title)
{
    system("cls");
    setColor(11); // Cyan
    cout << "\n";
    cout << "  ============================================================\n";
    cout << "  ";
    setColor(14); // Yellow
    int padding = (58 - title.length()) / 2;
    cout << string(padding, ' ') << title << string(58 - padding - title.length(), ' ');
    setColor(11);
    cout << "\n";
    cout << "  ============================================================\n";
    setColor(7); // White
    cout << "\n";
}

void printDivider()
{
    setColor(8); // Gray
    cout << "  ────────────────────────────────────────────────────────────\n";
    setColor(7);
}

void printBox(string content, int color = 15)
{
    setColor(color);
    cout << "  +--------------------------------------------------------+\n";
    cout << "  |  " << left << setw(54) << content << "|\n";
    cout << "  +--------------------------------------------------------+\n";
    setColor(7);
}

void printSuccess(string msg)
{
    setColor(10); // Green
    cout << "\n  [SUCCESS] " << msg << "\n";
    setColor(7);
}

void printError(string msg)
{
    setColor(12); // Red
    cout << "\n  [ERROR] " << msg << "\n";
    setColor(7);
}

void printInfo(string msg)
{
    setColor(14); // Yellow
    cout << "  [INFO] " << msg << "\n";
    setColor(7);
}

void pause()
{
    setColor(8);
    cout << "\n  Press any key to continue...";
    setColor(7);
    system("pause>nul");
}

class Route
{
    string routecode;
    string start;
    string end;
    int distance;
    bool valid;

public:
    Route()
    {
        routecode = "0";
        start = "null";
        end = "null";
        distance = 0;
        valid = false;
    }
    Route(string code, string s, string e, int d)
    {
        if ((code == "R001" && s == "ISLAMABAD" && e == "LAHORE") ||
            (code == "R002" && s == "ISLAMABAD" && e == "KARACHI") ||
            (code == "R003" && s == "ISLAMABAD" && e == "MULTAN"))
        {
            routecode = code;
            start = s;
            end = e;
            distance = d;
            valid = true;
        }
        else
        {
            printError("Invalid route");
            valid = false;
        }
    }
    string get_route() { return routecode; }
    string get_start() { return start; }
    string get_end() { return end; }
    int getdistance() { return distance; }
    bool isvalid() { return valid; }
    void displayroute()
    {
        if (valid)
        {
            setColor(14);
            cout << "  " << start;
            setColor(8);
            cout << " --> ";
            setColor(14);
            cout << end;
            setColor(8);
            cout << " (" << distance << " km)";
            setColor(7);
            cout << "\n";
        }
        else
        {
            printError("No valid route");
        }
    }
};

class Driver
{
    string name;
    string id;
    int experience;
    bool valid_driver;

public:
    Driver()
    {
        name = "";
        id = "0";
        experience = 0;
        valid_driver = false;
    }
    Driver(string n, string i, int e)
    {
        if (e >= 2)
        {
            name = n;
            id = i;
            experience = e;
            valid_driver = true;
        }
        else
        {
            printError("Driver not valid (experience < 2 years)");
            valid_driver = false;
        }
    }
    bool isvalid_driver() { return valid_driver; }
    string getname() { return name; }
};

class passenger
{
    string name;
    char gender;
    int age;
    string destination;

public:
    passenger()
    {
        name = "";
        gender = 'E';
        age = 0;
        destination = "";
    }
    passenger(string n, char g, int a, string dest)
    {
        name = n;
        gender = g;
        age = a;
        destination = dest;
    }
    void setname(string n) { name = n; }
    void setgender(char c) { gender = c; }
    void setage(int a) { age = a; }
    void setdestination(string d) { destination = d; }

    string getname() { return name; }
    char getgender() { return gender; }
    int getage() { return age; }
    string getdestination() { return destination; }
    bool IEmpty_passenger() { return gender == 'E'; }
};

class Bus
{
    string bus_number;
    int row, columns;
    passenger seat[5][4];
    Route route;
    Driver driver;
    string departureTime;
    bool valid_bus;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

public:
    Bus(string bus_num, int r, int c, Route route, Driver driver, string depTime)
    {
        bus_number = bus_num;
        row = r;
        columns = c;
        this->route = route;
        this->driver = driver;
        departureTime = depTime;

        if (!route.isvalid() || !driver.isvalid_driver())
        {
            valid_bus = false;
            printError("Cannot create bus. Invalid route or driver");
        }
        else
        {
            valid_bus = true;
            for (int i = 0; i < row; i++)
                for (int j = 0; j < columns; j++)
                    seat[i][j].setgender('E');
        }
    }

    bool isvalid_bus() { return valid_bus; }
    string ret_routecode() { return route.get_route(); }
    string get_bus_number() { return bus_number; }
    string get_destination() { return route.get_end(); }
    string get_departure_time() { return departureTime; }
    int calulate_fare() { return route.getdistance() * 2; }
    string get_driver_name() { return driver.getname(); }

    void status()
    {
        int booked = 0;
        int empty = 0;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < columns; j++)
                seat[i][j].IEmpty_passenger() ? empty++ : booked++;

        setColor(8);
        cout << "  Total: ";
        setColor(15);
        cout << row * columns;
        setColor(8);
        cout << " | Booked: ";
        setColor(12);
        cout << booked;
        setColor(8);
        cout << " | Available: ";
        setColor(10);
        cout << empty;
        setColor(7);
        cout << "\n";
    }

    int getAvailableSeats()
    {
        int empty = 0;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < columns; j++)
                if (seat[i][j].IEmpty_passenger())
                    empty++;
        return empty;
    }

    void displayBusInfo()
    {
        setColor(11);
        cout << "\n  ========================================================\n";
        setColor(7);

        cout << "  |  ";
        setColor(14);
        cout << "Bus ID: ";
        setColor(15);
        cout << left << setw(44) << bus_number;
        setColor(11);
        cout << "|\n";

        setColor(7);
        cout << "  |  ";
        setColor(14);
        cout << "Route:  ";
        setColor(7);
        route.displayroute();

        cout << "  |  ";
        setColor(14);
        cout << "Departure: ";
        setColor(15);
        cout << left << setw(41) << departureTime;
        setColor(11);
        cout << "|\n";

        setColor(7);
        cout << "  |  ";
        setColor(14);
        cout << "Fare: Rs. ";
        setColor(10);
        cout << left << setw(41) << calulate_fare();
        setColor(11);
        cout << "|\n";

        setColor(7);
        cout << "  |  ";
        setColor(14);
        cout << "Driver: ";
        setColor(15);
        cout << left << setw(43) << driver.getname();
        setColor(11);
        cout << "|\n";

        setColor(7);
        cout << "  |  ";
        status();
        cout << "                                            ";
        setColor(11);
        cout << "|\n";

        cout << "  ========================================================\n";
        setColor(7);
    }

    string search_passenger_from_struct(string name)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < columns; j++)
                if (seat[i][j].getname() == name)
                {
                    printSuccess(name + " found at seat (" + to_string(i) + "," + to_string(j) + ")");
                    return seat[i][j].getname();
                }
        printError(name + " not found");
        return "";
    }

    bool cancelledseat(int r, int c)
    {
        if (!valid_bus)
            return false;
        if (r >= 0 && r < row && c >= 0 && c < columns)
        {
            if (!seat[r][c].IEmpty_passenger())
            {
                seat[r][c].setgender('E');
                seat[r][c].setname("");
                printSuccess("Seat cancelled successfully");
                return true;
            }
            else
            {
                printError("The seat is already empty");
                return false;
            }
        }
        printError("Invalid seat position");
        return false;
    }

    bool isempty(int r, int c)
    {
        if (!valid_bus)
            return false;
        if (r >= 0 && r < row && c >= 0 && c < columns)
            return seat[r][c].IEmpty_passenger();
        printError("Seat out of bounds");
        return false;
    }

    bool bookSeat(int r, int c, char g, string username, string entered_otp, Bus_system &system);

    void display()
    {
        if (!valid_bus)
        {
            printError("Bus invalid");
            return;
        }

        cout << "\n";
        setColor(14);
        cout << "  [BUS] SEAT LAYOUT\n\n";
        setColor(7);

        // Legend
        setColor(8);
        cout << "  ";
        setColor(10);
        cout << "[E] Empty  ";
        setColor(9);
        cout << "[M] Male  ";
        setColor(13);
        cout << "[F] Female";
        setColor(7);
        cout << "\n\n";

        // Header
        setColor(8);
        cout << "      ";
        for (int j = 0; j < columns; j++)
            cout << " " << j << "  ";
        cout << "\n";
        cout << "    +";
        for (int j = 0; j < columns; j++)
            cout << "---+";
        cout << "\n";
        setColor(7);

        // Seats
        for (int i = 0; i < row; i++)
        {
            setColor(8);
            cout << "  " << i << " |";
            setColor(7);

            for (int j = 0; j < columns; j++)
            {
                char g = seat[i][j].getgender();
                if (g == 'E')
                    setColor(10); // Green
                else if (g == 'M')
                    setColor(9); // Blue
                else if (g == 'F')
                    setColor(13); // Pink

                cout << " " << g << " ";
                setColor(8);
                cout << "|";
            }
            setColor(7);
            cout << "\n";

            if (i < row - 1)
            {
                setColor(8);
                cout << "    +";
                for (int j = 0; j < columns; j++)
                    cout << "---+";
                cout << "\n";
            }
        }

        setColor(8);
        cout << "    +";
        for (int j = 0; j < columns; j++)
            cout << "---+";
        cout << "\n";
        setColor(7);
    }
};

class Bus_system
{
private:
    vector<Bus> buses;

public:
    void adminLogin()
    {
        printHeader("ADMIN LOGIN");

        string adminID, password;
        setColor(14);
        cout << "  Admin ID: ";
        setColor(7);
        cin >> adminID;

        setColor(14);
        cout << "  Password: ";
        setColor(7);
        cin >> password;

        if (adminID == "admin" && password == "admin123")
        {
            printSuccess("Login successful!");
            pause();

            int choice;
            do
            {
                printHeader("ADMIN PANEL");

                setColor(11);
                cout << "  ========================================================\n";
                cout << "  |  ";
                setColor(15);
                cout << "[1] View All Buses                                   ";
                setColor(11);
                cout << "|\n  |  ";
                setColor(15);
                cout << "[2] View All Passengers                              ";
                setColor(11);
                cout << "|\n  |  ";
                setColor(15);
                cout << "[3] View All Tickets                                 ";
                setColor(11);
                cout << "|\n  |  ";
                setColor(15);
                cout << "[4] Search Bus by Route Code                         ";
                setColor(11);
                cout << "|\n  |  ";
                setColor(15);
                cout << "[5] Logout                                           ";
                setColor(11);
                cout << "|\n";
                cout << "  ========================================================\n";
                setColor(7);

                setColor(14);
                cout << "\n  Your choice: ";
                setColor(7);
                cin >> choice;

                switch (choice)
                {
                case 1:
                    viewAllBusesDetailed();
                    pause();
                    break;
                case 2:
                    displayAllPassengers();
                    pause();
                    break;
                case 3:
                    displayAllTickets();
                    pause();
                    break;
                case 4:
                {
                    printHeader("SEARCH BUS");
                    string code;
                    setColor(14);
                    cout << "  Enter route code (R001/R002/R003): ";
                    setColor(7);
                    cin >> code;
                    search_route_code(code);
                    pause();
                    break;
                }
                case 5:
                    printSuccess("Logged out successfully!");
                    pause();
                    break;
                default:
                    printError("Invalid choice!");
                    pause();
                }
            } while (choice != 5);
        }
        else
        {
            printError("Invalid credentials!");
            pause();
        }
    }

    void displayAllPassengers()
    {
        printHeader("ALL PASSENGERS");

        ifstream file("passengers.txt");
        if (!file)
        {
            printError("No passengers found or file doesn't exist");
            return;
        }

        string line;
        bool hasData = false;
        setColor(15);
        while (getline(file, line))
        {
            cout << "  " << line << endl;
            hasData = true;
        }
        setColor(7);

        if (!hasData)
            printInfo("No passengers have booked tickets yet");

        file.close();
    }

    void displayAllTickets()
    {
        printHeader("ALL TICKETS");

        ifstream file("ticket.txt");
        if (!file)
        {
            printError("No tickets found or file doesn't exist");
            return;
        }

        string line;
        setColor(15);
        while (getline(file, line))
        {
            if (line.find("====") != string::npos)
                setColor(11);
            else if (line.find("BUS TICKET") != string::npos)
                setColor(14);
            else
                setColor(7);
            cout << "  " << line << endl;
        }
        setColor(7);
        file.close();
    }

    void viewAllBusesDetailed()
    {
        printHeader("ALL BUSES");

        for (int i = 0; i < buses.size(); i++)
        {
            buses[i].displayBusInfo();
            buses[i].display();
            cout << "\n";
        }
    }

    string generate_otp()
    {

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(1000, 9000);
        int d = dist(gen);
        string otp = to_string(d);
        return otp;
    }

    void save_to_file_otp(string username, string otp)
    {
        ofstream file("otp.txt", ios::app);
        time_t now = time(0);
        file << username << " " << otp << " " << now << endl;
        file.close();
    }

    bool valid_otp(string username, string entered_otp)
    {
        ifstream file("otp.txt");
        string un, otp;
        time_t timestamp;

        while (file >> un >> otp >> timestamp)
        {
            if (username == un && entered_otp == otp)
            {
                time_t now = time(0);
                if (difftime(now, timestamp) < 60)
                {
                    file.close();
                    return true;
                }
                else
                {
                    printError("OTP expired");
                    file.close();
                    return false;
                }
            }
        }
        file.close();
        return false;
    }

    void addbuses(Bus &b)
    {
        buses.push_back(b);
    }

    bool search_route_code(string route_code)
    {
        bool found = false;
        for (int i = 0; i < buses.size(); i++)
        {
            if (buses[i].ret_routecode() == route_code)
            {
                buses[i].displayBusInfo();
                buses[i].display();
                found = true;
            }
        }
        if (!found)
        {
            printError("No bus found with route code: " + route_code);
        }
        return found;
    }

    void passengerMenu()
    {
        int choice;
        do
        {
            printHeader("PASSENGER MENU");

            setColor(11);
            cout << "  ========================================================\n";
            cout << "  |  ";
            setColor(15);
            cout << "[1] Book Ticket                                      ";
            setColor(11);
            cout << "|\n  |  ";
            setColor(15);
            cout << "[2] View Ticket                                      ";
            setColor(11);
            cout << "|\n  |  ";
            setColor(15);
            cout << "[3] Cancel Ticket                                    ";
            setColor(11);
            cout << "|\n  |  ";
            setColor(15);
            cout << "[4] Back to Main Menu                                ";
            setColor(11);
            cout << "|\n";
            cout << "  ========================================================\n";
            setColor(7);

            setColor(14);
            cout << "\n  Your choice: ";
            setColor(7);
            cin >> choice;

            switch (choice)
            {
            case 1:
                bookTicketFlow();
                pause();
                break;
            case 2:
                displayAllTickets();
                pause();
                break;
            case 3:
                cancelTicketFlow();
                pause();
                break;
            case 4:
                printSuccess("Returning to main menu...");
                pause();
                break;
            default:
                printError("Invalid choice!");
                pause();
            }
        } while (choice != 4);
    }
    string cap(string s)
    {
        string res;
        for (char c : s)
        {
            res += toupper(c);
        }

        return res;
    }
    void bookTicketFlow()
    {
        string name, destination;
        char gender;
        int age;

        cin.ignore();
        printHeader("BOOK YOUR TICKET");

        setColor(14);
        cout << "  Your name: ";
        setColor(7);
        getline(cin, name);

        setColor(14);
        cout << "  Gender (M/F): ";
        setColor(7);
        cin >> gender;
        gender = toupper(gender);

        setColor(14);
        cout << "  Age: ";
        setColor(7);
        cin >> age;

        cin.ignore();

        cout << "\n";
        setColor(14);
        cout << "  Available Destinations:\n\n";
        setColor(7);
        cout << "     ";
        setColor(10);
        cout << "[1] Lahore\n";
        cout << "     ";
        setColor(10);
        cout << "[2] Karachi\n";
        cout << "     ";
        setColor(10);
        cout << "[3] Multan\n\n";
        setColor(14);
        cout << "  Enter destination: ";
        setColor(7);
        getline(cin, destination);

        printHeader("BUSES TO " + destination);

        vector<int> availableBusIndices;

        for (int i = 0; i < buses.size(); i++)
        {
            if (buses[i].get_destination() == cap(destination))
            {
                setColor(11);
                cout << "\n  ========================================================\n";
                cout << "  |  ";
                setColor(14);
                cout << "OPTION " << (availableBusIndices.size() + 1);
                cout << string(48 - to_string(availableBusIndices.size() + 1).length(), ' ');
                setColor(11);
                cout << "|\n";
                cout << "  ========================================================\n";
                setColor(7);

                buses[i].displayBusInfo();
                buses[i].display();
                availableBusIndices.push_back(i);
            }
        }

        if (availableBusIndices.empty())
        {
            printError("No buses available to " + destination);
            return;
        }

        int userChoice;
        setColor(14);
        cout << "\n  Select bus option (1-" << availableBusIndices.size() << "): ";
        setColor(7);
        cin >> userChoice;

        if (userChoice < 1 || userChoice > availableBusIndices.size())
        {
            printError("Invalid bus selection!");
            return;
        }

        int actualBusIndex = availableBusIndices[userChoice - 1];

        string otp = generate_otp();
        cout << "\n";
        setColor(14);
        cout << "  Your OTP: ";
        setColor(10);
        cout << otp;
        setColor(8);
        cout << " (valid for 5 minutes)\n";
        setColor(7);
        save_to_file_otp(name, otp);

        string entered_otp;
        setColor(14);
        cout << "  Enter OTP to confirm: ";
        setColor(7);
        cin >> entered_otp;

        int row, col;
        setColor(14);
        cout << "\n  Select your seat:\n";
        cout << "     Row (0-4): ";
        setColor(7);
        cin >> row;
        setColor(14);
        cout << "     Column (0-3): ";
        setColor(7);
        cin >> col;

        if (buses[actualBusIndex].bookSeat(row, col, gender, name, entered_otp, *this))
        {
            savePassengerToFile(name, gender, age, destination,
                                buses[actualBusIndex].get_bus_number(), row, col);

            generateTicket(name, gender, age, destination,
                           buses[actualBusIndex].get_bus_number(),
                           buses[actualBusIndex].get_departure_time(),
                           buses[actualBusIndex].calulate_fare(), row, col);

            printSuccess("TICKET BOOKED SUCCESSFULLY!");

            cout << "\n";
            setColor(14);
            cout << "  Updated seat layout:\n";
            setColor(7);
            buses[actualBusIndex].display();
        }
    }

    void savePassengerToFile(string name, char gender, int age,
                             string destination, string busID, int row, int col)
    {
        ofstream file("passengers.txt", ios::app);
        file << "Name: " << name
             << " | Gender: " << gender
             << " | Age: " << age
             << " | Destination: " << destination
             << " | Bus: " << busID
             << " | Seat: (" << row << "," << col << ")" << endl;
        file.close();
    }

    void generateTicket(string name, char gender, int age,
                        string destination, string busID, string depTime,
                        int fare, int row, int col)
    {
        ofstream file("ticket.txt", ios::app);

        time_t now = time(0);
        char *dt = ctime(&now);

        file << "========================================\n";
        file << "          BUS TICKET\n";
        file << "========================================\n";
        file << "Passenger Name : " << name << endl;
        file << "Gender         : " << gender << endl;
        file << "Age            : " << age << endl;
        file << "Destination    : " << destination << endl;
        file << "Bus ID         : " << busID << endl;
        file << "Departure Time : " << depTime << endl;
        file << "Seat Number    : Row " << row << ", Column " << col << endl;
        file << "Fare           : Rs. " << fare << endl;
        file << "Booking Date   : " << dt;
        file << "========================================\n\n";

        file.close();

        printSuccess("Ticket saved to ticket.txt");
    }

    void cancelTicketFlow()
    {
        printHeader("CANCEL TICKET");

        string name;
        cin.ignore();
        setColor(14);
        cout << "  Enter passenger name: ";
        setColor(7);
        getline(cin, name);

        bool found = false;
        for (int i = 0; i < buses.size(); i++)
        {
            if (buses[i].search_passenger_from_struct(name) != "")
            {
                found = true;
                printInfo("Found on bus " + to_string(i + 1));

                int row, col;
                setColor(14);
                cout << "  Seat row to cancel: ";
                setColor(7);
                cin >> row;
                setColor(14);
                cout << "  Seat column to cancel: ";
                setColor(7);
                cin >> col;

                if (buses[i].cancelledseat(row, col))
                {
                    printSuccess("Ticket cancelled!");
                }
                break;
            }
        }

        if (!found)
        {
            printError("Passenger not found!");
        }
    }
};

bool Bus::bookSeat(int r, int c, char g, string username, string entered_otp, Bus_system &system)
{
    if (!valid_bus)
    {
        printError("Bus is not valid!");
        return false;
    }

    if (!system.valid_otp(username, entered_otp))
    {
        printError("Invalid or expired OTP");
        return false;
    }

    if (r < 0 || r >= row || c < 0 || c >= columns)
    {
        printError("Invalid seat position");
        return false;
    }

    if (!seat[r][c].IEmpty_passenger())
    {
        printError("Seat already booked");
        return false;
    }

    g = toupper(g);

    if (g != 'M' && g != 'F')
    {
        printError("Invalid gender. Must be M or F");
        return false;
    }

    seat[r][c].setgender(g);
    seat[r][c].setname(username);
    return true;
}

int main()
{
    srand(time(0));

    Bus_system system;

    Route r1("R001", "ISLAMABAD", "LAHORE", 380);
    Route r2("R002", "ISLAMABAD", "KARACHI", 1400);
    Route r3("R003", "ISLAMABAD", "MULTAN", 350);

    Driver d1("Ali Hassan", "LIC1234", 5);
    Driver d2("Ahmed Khan", "LIC5678", 3);
    Driver d3("Usman Ali", "LIC9012", 4);

    Bus b1("BUS101", 5, 4, r1, d1, "09:00 AM");
    Bus b2("BUS102", 5, 4, r2, d2, "10:30 AM");
    Bus b3("BUS103", 5, 4, r3, d3, "02:00 PM");

    system.addbuses(b1);
    system.addbuses(b2);
    system.addbuses(b3);

    int choice;
    do
    {
        printHeader("[BUS] BUS TICKET MANAGEMENT SYSTEM");

        setColor(11);
        cout << "  ========================================================\n";
        cout << "  |                                                      |\n";
        cout << "  |  ";
        setColor(14);
        cout << "[1] Login as Admin                                   ";
        setColor(11);
        cout << "|\n";
        cout << "  |  ";
        setColor(14);
        cout << "[2] Login as Passenger                               ";
        setColor(11);
        cout << "|\n";
        cout << "  |  ";
        setColor(14);
        cout << "[3] Exit                                             ";
        setColor(11);
        cout << "|\n";
        cout << "  |                                                      |\n";
        cout << "  ========================================================\n";
        setColor(7);

        setColor(14);
        cout << "\n  Enter your choice: ";
        setColor(7);
        cin >> choice;

        switch (choice)
        {
        case 1:
            system.adminLogin();
            break;
        case 2:
            system.passengerMenu();
            break;
        case 3:
            printHeader("THANK YOU!");
            setColor(10);
            cout << "\n  Thank you for using our Bus Ticket Management System!\n";
            cout << "  Have a safe journey!\n\n";
            setColor(7);
            Sleep(2000);
            break;
        default:
            printError("Invalid choice! Please try again");
            pause();
        }
    } while (choice != 3);

    return 0;
}
