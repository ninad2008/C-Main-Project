#include <iostream>

using namespace std;

const int ROWS = 5;
const int COLS = 20;
const double GST = 0.18;
const string ADMIN_PASS = "admin123";

struct Seat {
    bool booked = false;
};

struct Show {
    string movieName;
    string timing;
    vector<vector<Seat>> seats;
    double revenue = 0;
};

vector<Show> shows;

string getClass(char row) {
    if (row == 'A') return "Platinum";
    if (row == 'B' || row == 'C') return "Gold";
    return "Silver";
}

double getBasePrice(string seatClass, bool weekend) {
    if (seatClass == "Platinum") return weekend ? 500 : 400;
    if (seatClass == "Gold") return weekend ? 350 : 300;
    return weekend ? 250 : 200;
}

void initializeShows() {
    Show s1 = {"Avengers", "2:00 PM", vector<vector<Seat>>(ROWS, vector<Seat>(COLS))};
    Show s2 = {"Oppenheimer", "6:00 PM", vector<vector<Seat>>(ROWS, vector<Seat>(COLS))};
    Show s3 = {"Dune 2", "9:00 PM", vector<vector<Seat>>(ROWS, vector<Seat>(COLS))};
    shows.push_back(s1);
    shows.push_back(s2);
    shows.push_back(s3);
}

void displaySeats(Show &show) {
    cout << "\nSeat Layout (X = Booked)\n\n";
    for (int i = 0; i < ROWS; i++) {
        char rowChar = 'A' + i;
        cout << rowChar << " ";
        for (int j = 0; j < COLS; j++) {
            if (show.seats[i][j].booked)
                cout << "X ";
            else
                cout << j+1 << " ";
        }
        cout << endl;
    }
}

string generateBookingID() {
    return "BK" + to_string(rand() % 100000);
}

void bookTickets() {
    int choice;
    cout << "\nAvailable Shows:\n";
    for (int i = 0; i < shows.size(); i++)
        cout << i+1 << ". " << shows[i].movieName << " - " << shows[i].timing << endl;

    cout << "Select Show: ";
    cin >> choice;
    choice--;

    if (choice < 0 || choice >= shows.size()) return;

    Show &selected = shows[choice];
    displaySeats(selected);

    int ticketCount;
    cout << "Number of tickets: ";
    cin >> ticketCount;

    vector<string> bookedSeats;
    double subtotal = 0;
    bool weekend;
    cout << "Is it weekend show? (1-Yes 0-No): ";
    cin >> weekend;

    for (int i = 0; i < ticketCount; i++) {
        char row;
        int col;
        cout << "Enter Seat (A 5): ";
        cin >> row >> col;

        row = toupper(row);
        int r = row - 'A';
        int c = col - 1;

        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || selected.seats[r][c].booked) {
            cout << "Invalid or Booked! Try again.\n";
            i--;
            continue;
        }

        selected.seats[r][c].booked = true;
        bookedSeats.push_back(string(1,row) + to_string(col));

        string seatClass = getClass(row);
        double price = getBasePrice(seatClass, weekend);
        subtotal += price;
    }

    // Food Section
    int foodChoice, qty;
    double foodTotal = 0;
    cout << "\nFood Menu:\n1.Popcorn(200)\n2.Combo(350)\n3.None\nChoice: ";
    cin >> foodChoice;

    if (foodChoice == 1) {
        cout << "Quantity: ";
        cin >> qty;
        foodTotal = qty * 200;
    }
    else if (foodChoice == 2) {
        cout << "Quantity: ";
        cin >> qty;
        foodTotal = qty * 350;
    }

    subtotal += foodTotal;

    double tax = subtotal * GST;
    double total = subtotal + tax;

    // Payment Simulation
    int payChoice;
    cout << "\nPayment Mode:\n1.UPI\n2.Card\n3.Cash\nChoice: ";
    cin >> payChoice;

    cout << "Processing Payment...\nPayment Successful!\n";

    selected.revenue += total;

    string bookingID = generateBookingID();

    cout << "\n===== BOOKING CONFIRMED =====\n";
    cout << "Booking ID: " << bookingID << endl;
    cout << "Movie: " << selected.movieName << endl;
    cout << "Show: " << selected.timing << endl;
    cout << "Seats: ";
    for (auto s : bookedSeats) cout << s << " ";
    cout << "\nSubtotal: ₹" << subtotal;
    cout << "\nGST: ₹" << tax;
    cout << "\nTotal Paid: ₹" << total;
    cout << "\n=============================\n";
}

void adminPanel() {
    string pass;
    cout << "Enter Admin Password: ";
    cin >> pass;

    if (pass != ADMIN_PASS) {
        cout << "Wrong Password!\n";
        return;
    }

    cout << "\n--- Admin Dashboard ---\n";
    double totalRevenue = 0;
    for (auto &s : shows) {
        cout << s.movieName << " Revenue: ₹" << s.revenue << endl;
        totalRevenue += s.revenue;
    }
    cout << "Total Revenue: ₹" << totalRevenue << endl;
}

int main() {
    srand(time(0));
    initializeShows();

    int choice;
    do {
        cout << "\n🎬 SMART MULTIPLEX MANAGEMENT SYSTEM 🎬\n";
        cout << "1. Book Tickets\n";
        cout << "2. Admin Panel\n";
        cout << "3. Exit\nChoice: ";
        cin >> choice;

        switch(choice) {
            case 1: bookTickets(); break;
            case 2: adminPanel(); break;
            case 3: cout << "Thank You!\n"; break;
            default: cout << "Invalid Option!\n";
        }
    } while (choice != 3);

    return 0;
}