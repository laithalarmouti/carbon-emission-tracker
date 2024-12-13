#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;

struct CarbonData {
    double electricityBill;
    double gasBill;
    double fuelBill;
    double wasteGenerated;
    double wasteRecycledPercentage;
    double kilometersTraveled;
    double fuelEfficiency;
};

// Prototypes
void saveData(const CarbonData& data, const string& filename);
CarbonData loadData(const string& filename);
void calculateData(const CarbonData& data);
CarbonData getInputData();
void giveSuggestions(const CarbonData& data);
void plotCarbonEmissions(const vector<double>& emissions);

int main() {
    CarbonData data;
    string companyName;
    string filename;
    int choice;

    cout << "Carbon Footprint Monitoring Program\n";
    cout << "Enter your company name: ";
    cin.ignore();
    getline(cin, companyName);
    filename = companyName + "_carbon_data.txt";

    cout << "1. Input new data\n";
    cout << "2. Load data and generate report\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        data = getInputData();
        saveData(data, filename);
    } else if (choice == 2) {
        data = loadData(filename);
    } else {
        cout << "Invalid choice!\n";
    }

    return 0;
}
