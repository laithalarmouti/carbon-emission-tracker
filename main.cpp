#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include<sstream>
#include <limits>

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


//Main function
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
        cout << "Data saved successfully as " << filename << "!\n";
        calculateData(data); 
        giveSuggestions(data); 

        // Collecting emission data for plotting and placing in vector
        vector<double> emissions = {
            (data.electricityBill * 12 * 0.0005) + (data.gasBill * 12 * 0.0053) + (data.fuelBill * 12 * 2.32),
            data.wasteGenerated * 12 * (0.57 - (data.wasteRecycledPercentage / 100.0)),
            data.kilometersTraveled * (1/data.fuelEfficiency)* 2.31
        };

        // Plot the emissions data
        plotCarbonEmissions(emissions);
    } else if (choice == 2) {
        try {
            data = loadData(filename);
            calculateData(data);
            giveSuggestions(data); 
            
            vector<double> emissions = {
                (data.electricityBill * 12 * 0.0005) + (data.gasBill * 12 * 0.0053) + (data.fuelBill * 12 * 2.32),
                data.wasteGenerated * 12 * (0.57 - (data.wasteRecycledPercentage / 100.0)),
                data.kilometersTraveled * (1/data.fuelEfficiency) * 2.31
            };

            // Plot the emissions data
            plotCarbonEmissions(emissions);

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else {
        cout << "Invalid choice!\n";
    }

    return 0;
}



//function to validate input data as postive integers only
int validate(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < 0) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            cout << "Invalid input. Please enter a valid value." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard any extra input
            return value;
        }
    }
}

//input from user
CarbonData getInputData() {
    CarbonData data;
    data.electricityBill = validate("Enter your average monthly electricity bill (in euros): ");
    data.gasBill = validate("Enter your average monthly natural gas bill (in euros): ");
    data.fuelBill = validate("Enter your average monthly fuel bill for transportation (in euros): ");
    data.wasteGenerated = validate("Enter the amount of waste you generate per month (in kilograms): ");
    data.wasteRecycledPercentage = validate("Enter the percentage of waste recycled or composted: ");
    data.kilometersTraveled = validate("Enter the total kilometers traveled per year for business purposes: ");
    data.fuelEfficiency = validate("Enter the average fuel efficiency of vehicles (liters per 100 kilometers): ");
    return data;
}


//Saves data to the given file according to company name and calculates
void saveData(const CarbonData& data, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        throw runtime_error("Error opening file.");
    }

    outFile << "Electricity Bill: " << data.electricityBill << " euros\n"
            << "Gas Bill: " << data.gasBill << " euros\n"
            << "Fuel Bill: " << data.fuelBill << " euros\n"
            << "Waste Generated: " << data.wasteGenerated << " kg\n"
            << "Waste Recycled Percentage: " << data.wasteRecycledPercentage << "%\n"
            << "Kilometers Traveled: " << data.kilometersTraveled << " km\n"
            << "Fuel Efficiency: " << data.fuelEfficiency << " liters/100 km\n";

 double energyCO2 = (data.electricityBill*12*0.0005) +
                       (data.gasBill*12*0.0053) +
                       (data.fuelBill*12*2.32);
    //Equation given returns negative values if recycling percentage is above 57? 
    //tried flooring rounding to 0 if it goes below 0, messed up the plotting part.
    double wasteCO2 = data.wasteGenerated*12*(0.57-(data.wasteRecycledPercentage/100.0));
    double travelCO2 = data.kilometersTraveled * (1.0 / data.fuelEfficiency) * 2.31;


    double totalCO2 = energyCO2 + wasteCO2 + travelCO2;

    // Add the total carbon footprint
    outFile << "---------------------------------\n";
    outFile << "Energy Usage Emissions: " << energyCO2 << " kgCO2/year\n";
    outFile << "Waste Emissions: " << wasteCO2 << " kgCO2/year\n";
    outFile << "Business Travel Emissions: " << travelCO2 << " kgCO2/year\n";
    outFile << "---------------------------------\n";
    outFile << "Total Carbon Footprint: " << totalCO2 << " kgCO2/year\n";

    outFile.close();
}


//loading data from example_carbon_data.txt and saving it temporarly on emissions_data.txt for plotting.
CarbonData loadData(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        throw std::runtime_error("Error opening the file.");
    }

    CarbonData data;
    std::string line;

    // Read and parse each line to extract the numbers only from report
    std::getline(inFile, line);
    std::istringstream(line.substr(line.find(":") + 1)) >> data.electricityBill;

    std::getline(inFile, line);
    std::istringstream(line.substr(line.find(":") + 1)) >> data.gasBill;

    std::getline(inFile, line);
    std::istringstream(line.substr(line.find(":") + 1)) >> data.fuelBill;

    std::getline(inFile, line);
    std::istringstream(line.substr(line.find(":") + 1)) >> data.wasteGenerated;

    std::getline(inFile, line);
    std::istringstream(line.substr(line.find(":") + 1)) >> data.wasteRecycledPercentage;

    std::getline(inFile, line);
    std::istringstream(line.substr(line.find(":") + 1)) >> data.kilometersTraveled;

    std::getline(inFile, line);
    std::istringstream(line.substr(line.find(":") + 1)) >> data.fuelEfficiency;

    inFile.close();
    return data;
}


//calculations
void calculateData(const CarbonData& data) {
    double energyCO2 = (data.electricityBill*12*0.0005) +
                       (data.gasBill*12*0.0053) +
                       (data.fuelBill*12*2.32);
    //Equation given returns negative values if recycling percentage is above 57? 
    //tried flooring rounding to 0 if it goes below 0, messed up the plotting part.
    double wasteCO2 = data.wasteGenerated*12*(0.57-(data.wasteRecycledPercentage/100.0));
    double travelCO2 = data.kilometersTraveled * (1.0 / data.fuelEfficiency) * 2.31;


    double totalCO2 = energyCO2 + wasteCO2 + travelCO2;

    //Report
    cout << fixed << setprecision(2);
    cout << "\nCarbon Footprint Report:\n";
    cout << "---------------------------------\n";
    cout << "Energy Usage Emissions: " << energyCO2 << " kgCO2/year\n";
    cout << "Waste Emissions: " << wasteCO2 << " kgCO2/year\n";
    cout << "Business Travel Emissions: " << travelCO2 << " kgCO2/year\n";
    cout << "-----------------------------\n";
    cout << "Total Carbon Footprint: " << totalCO2 << " kgCO2/year\n";
}


// Suggestions
void giveSuggestions(const CarbonData& data) {
    cout << "\nSuggestions to Reduce Carbon Emissions:\n";
    cout << "---------------------------------\n";

    double energyCO2 = (data.electricityBill * 12 * 0.0005) +
                       (data.gasBill * 12 * 0.0053) +
                       (data.fuelBill * 12 * 2.32);
    double wasteCO2 = data.wasteGenerated * 12 * (0.57 - (data.wasteRecycledPercentage / 100.0));
    double travelCO2 = data.kilometersTraveled * (1.0 / data.fuelEfficiency) * 2.31;

    if (energyCO2 > wasteCO2 && energyCO2 > travelCO2) {
        cout << "- Consider installing energy-efficient appliances and using renewable energy sources like solar panels.\n";
        cout << "- Implement energy-saving practices such as turning off unused equipment and optimizing heating/cooling systems.\n";
    }

    if (wasteCO2 > energyCO2 && wasteCO2 > travelCO2) {
        cout << "- Increase the percentage of waste recycled or composted.\n";
        cout << "- Reduce waste generation by adopting reusable materials and improving waste management practices.\n";
    }

    if (travelCO2 > energyCO2 && travelCO2 > wasteCO2) {
        cout << "- Use fuel-efficient vehicles, such as hybrids or electric cars, for business travel.\n";
        cout << "- Optimize travel routes to minimize distance and adopt carpooling practices.\n";
    }

    cout << "---------------------------------\n";
}

void plotCarbonEmissions(const vector<double>& emissions) {
    // Create a txt file store temp data, which will be overwritten
    ofstream dataFile("emissions_data.txt");
    dataFile << "Energy " << emissions[0] << endl;
    dataFile << "Waste " << emissions[1] << endl;
    dataFile << "Travel " << emissions[2] << endl;
    dataFile.close();

//plotting to gnuplot
    system("gnuplot -e \"set title 'Carbon Emissions'; \
                        set xlabel 'Category'; \
                        set ylabel 'Emissions (kgCO2)'; \
                        set style data histograms; \
                        set style fill solid 1.0; \
                        set boxwidth 0.5; \
                        set xtics rotate by -45; \
                        plot 'emissions_data.txt' using 2:xtic(1) with boxes lc rgb 'blue' title 'Emissions'; \
                        pause -1\"");
}