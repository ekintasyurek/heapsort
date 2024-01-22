#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct City {
    string name;
    int population;
};

void swap(City& x, City& y){
    City temp = x;
    x = y;
    y = temp;
}

void max_heapify(vector<City>& arr, int n, int i){
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].population > arr[largest].population)
        largest = left;

    if (right < n && arr[right].population > arr[largest].population)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        max_heapify(arr, n, largest);
    }
}

void build_max_heap(vector<City>& arr){
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        max_heapify(arr, n, i);
    }
}

void heapsort(vector<City>& arr){
    int n = arr.size();

    build_max_heap(arr);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        max_heapify(arr, i, 0);
    }
}

void max_heap_insert(vector<City>& arr, const City& newCity){
    arr.push_back(newCity);

    int i = arr.size() - 1;
    while (i > 0 && arr[(i - 1) / 2].population < arr[i].population) {
        swap(arr[i], arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

City heap_extract_max(vector<City>& arr){
    City maxCity = arr[0];
    arr[0] = arr.back();
    arr.pop_back();
    max_heapify(arr, arr.size(), 0);

    return maxCity;
}

void heap_increase_key(vector<City>& arr, int i, int newKey){
    arr[i].population = newKey;

    while (i > 0 && arr[(i - 1) / 2].population < arr[i].population) {
        swap(arr[i], arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

City heap_maximum(vector<City>& arr){
    max_heapify(arr, arr.size(), 0);
    return arr[0];
}


void dary_max_heapify(vector<City>& arr, int n, int i, int d) {
    int largest = i;
    int startChildIndex = d * i + 1;
    int endChildIndex = min(startChildIndex + d, n + 1);

    for (int childIndex = startChildIndex; childIndex < endChildIndex; ++childIndex) {
        if (childIndex <= n && arr[childIndex].population > arr[largest].population) {
            largest = childIndex;
        }
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        dary_max_heapify(arr, n, largest, d);
    }
}


void build_dary_heap(vector<City>& arr, int d) {
    int n = arr.size();
    int lastNonLeaf = n / d - 1;

    for (int i = lastNonLeaf; i >= 0; --i) {
        dary_max_heapify(arr, n, i, d);
    }
}


int dary_calculate_height(int n, int d){
    int height = 0;
    while (n > 0) {
        n = (n-1) / d;
        height++;
    }
    return height;
}

City dary_extract_max(vector<City>& arr) {
    int n = arr.size();
    if (n == 0) {
        return City();
    }

    City maxCity = arr[0];
    arr[0] = arr[n - 1];
    arr.pop_back();

    return maxCity;
}

void dary_insert_element(vector<City>& arr, const City& newCity, int d) {
    arr.push_back(newCity);

    int i = arr.size() - 1;
    while (i > 0) {
        int parent = (i - 1) / d;

        if (arr[parent].population < arr[i].population) {
            swap(arr[i], arr[parent]);
            i = parent;
        } else {
            break;
        }
    }
}


void dary_increase_key(vector<City>& arr, int i, int newKey, int d) {
    arr[i].population = newKey;

    while (i > 0) {
        int parent = (i - 1) / d;

        if (arr[parent].population < arr[i].population) {
            swap(arr[i], arr[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

void writeCityArrayToFile(const vector<City>& cityArray, const string& outputFileName) {
    ofstream outputFile(outputFileName);

    if (!outputFile.is_open()) {
        cerr << "Error opening output file: " << outputFileName << endl;
        return;
    }

    for (const auto& city : cityArray) {
        outputFile << city.name << ";" << city.population << endl;
    }

    outputFile.close();
}

void writeCityToFile(const City& city, const string& outputFileName) {
    ofstream outputFile(outputFileName);

    if (!outputFile.is_open()) {
        cerr << "Error opening output file: " << outputFileName << endl;
        return;
    }

    outputFile << city.name << ";" << city.population << endl;

    outputFile.close();
}

void writeHeightToFile(int height, const string& outputFileName) {
    ofstream outputFile(outputFileName);

    if (!outputFile.is_open()) {
        cerr << "Error opening output file: " << outputFileName << endl;
        return;
    }

    outputFile << "Height: " << height << endl;

    outputFile.close();
}


int main(int argc, char* argv[]){

    string inputFileName = argv[1];
    string functionName = argv[2];
    string outputFileName = argv[3];

    int numChildren = -1; // the number of children of non-leaf nodes, d
    int nodeIndex = -1; //i
    int keyValue = -1; //k
    string cityName = "";

    for (int i = 4; i < argc; ++i) {
        string option = argv[i];
        char prefix = option[0];

        switch (prefix) {
            case 'd':
                numChildren = stoi(option.substr(1));
                break;
            case 'i':
                nodeIndex = stoi(option.substr(1));
                break;
            case 'k':
                if (option[1] == '_') {
                    size_t underscorePos = option.find('_', 2);
                    if (underscorePos != string::npos && underscorePos < option.size() - 1) {
                        cityName = option.substr(2, underscorePos - 2);
                        keyValue = stoi(option.substr(option.rfind('_') + 1));
                    }
                } else {
                    keyValue = stoi(option.substr(1));
                }

                break;
            default:
                cerr << "Unknown option: " << option << endl;
                return 1;
        }
    }


    ifstream inputFile(inputFileName);

    if(!inputFile.is_open()) {
        cerr << "Error opening file: " << inputFileName << endl;
        return 1;
    }

    vector<City> cityArray;
    City city;
    string line;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        if (getline(ss, city.name, ';')) {
            if (ss >> city.population) {
                cityArray.push_back(city);
            }
        }
    }

    inputFile.close();

    auto begin = high_resolution_clock::now();

    if (functionName == "max_heapify") {
        max_heapify(cityArray, cityArray.size(), nodeIndex - 1);
        writeCityArrayToFile(cityArray, outputFileName);
    } else if (functionName == "build_max_heap") {
        build_max_heap(cityArray);
        writeCityArrayToFile(cityArray, outputFileName);
    } else if (functionName == "heapsort") {
        heapsort(cityArray);
        writeCityArrayToFile(cityArray, outputFileName);
    } else if (functionName == "max_heap_insert") {
        build_max_heap(cityArray);
        City newCity = {cityName, keyValue};
        max_heap_insert(cityArray, newCity);
        writeCityArrayToFile(cityArray, outputFileName);
    } else if (functionName == "heap_extract_max") {
        build_max_heap(cityArray);
        City maxCity = heap_extract_max(cityArray);
        writeCityToFile(maxCity, outputFileName);
    } else if (functionName == "heap_increase_key") {
        build_max_heap(cityArray);
        heap_increase_key(cityArray, nodeIndex - 1, keyValue);
        writeCityArrayToFile(cityArray, outputFileName);
    } else if (functionName == "heap_maximum") {
        build_max_heap(cityArray);
        City maxCity = heap_maximum(cityArray);
        writeCityToFile(maxCity, outputFileName);
    } else if (functionName == "build_dary_heap") {
        build_dary_heap(cityArray,numChildren);
        writeCityArrayToFile(cityArray, outputFileName);
    } else if (functionName == "dary_calculate_height") {
        int height = dary_calculate_height(cityArray.size(), numChildren);
        writeHeightToFile(height, outputFileName);
    } else if (functionName == "dary_extract_max") {
        build_dary_heap(cityArray,numChildren);
        City maxCity = dary_extract_max(cityArray);
        writeCityToFile(maxCity, outputFileName);
    } else if (functionName == "dary_insert_element") {
        build_dary_heap(cityArray,numChildren);
        City newCity = {cityName, keyValue};
        dary_insert_element(cityArray, newCity, numChildren);
        writeCityArrayToFile(cityArray, outputFileName);
    } else if (functionName == "dary_increase_key") {
        build_dary_heap(cityArray,numChildren);
        dary_increase_key(cityArray, nodeIndex - 1, keyValue, numChildren);
        writeCityArrayToFile(cityArray, outputFileName);

    } else {
        cerr << "Unknown function name: " << functionName << endl;
        return 1;
    }

    auto end = high_resolution_clock::now();

    cout << "Time taken by " << functionName << ": " << duration_cast<nanoseconds>(end - begin).count() << " ns." << endl;

}