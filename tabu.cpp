#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <map>
using namespace std;

#define LENGTH 1000

int *readData(string filename){
    int *array = new int[LENGTH];
    int i = 0;
    ifstream file;
    file.open(filename);
    while (file >> array[i]){
        i++;
    }
    file.close();
    return array;
}
const int *DATA = readData("data.txt");

string readBits(string filename){
    string bits;
    ifstream file;
    file.open(filename);
    file >> bits;
    file.close();
    return bits;
}

vector<int> bitsToSet(string bits){
    vector<int> set;
    for (int i = 0; i < bits.length(); i++){
        if (bits[i] == '1')
            set.push_back(DATA[i]);
    }
    return set;
}

int calculateDistance(string bits){
    vector<int> set = bitsToSet(bits);
    int sum = 0;
    for (int i = 0; i < set.size(); i++)
        sum += set[i];
    
    return abs(sum);
}

string switchBit(string bits, int i){
    if (bits[i] == '1'){
        bits[i] = '0';
    } else {
        bits[i] = '1';
    }
    return bits;
}

bool doesntContainAllFalse(string bits, char c){
    for (int i = 0; i < bits.length(); i++){
        if (bits[i] == c){
            return true;
        }
    }
    return false;
}

vector<string> getAllNeighbours(string bits){
    vector<string> neighbors;
    for (int i = 0; i < bits.length(); i++){
        string neighbor = switchBit(bits, i);
        if (doesntContainAllFalse(neighbor, '1')){
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

string tabuAlgo(string startBits, int iterations_limit){
    set<string> tabuList;   // tabu list
    list<string> stepsHeap; // steps list
    stepsHeap.push_back(startBits);
    string bestBits = startBits;
    string bits = startBits;
    int i = 0;


    while (i < iterations_limit && calculateDistance(bestBits) != 0 && stepsHeap.size() != 0){
        i += 1;
        vector<string> allNeighbours = getAllNeighbours(bits);

        // Get rid of member of tabu
        vector<string> allPossibleNeighbours;
        for (int i = 0; i < allNeighbours.size(); i++){
            if (!tabuList.count(allNeighbours[i])){
                allPossibleNeighbours.push_back(allNeighbours[i]);
            }
        }

        tabuList.insert(bits);
        if(allPossibleNeighbours.size() == 0){
            bits = stepsHeap.back();
            stepsHeap.pop_front();

        } else {

            // Find best neighbour
            int     bestDistance    = calculateDistance(allPossibleNeighbours[0]);
            string  bestNeighbour   = allPossibleNeighbours[0];
            for (int i = 1; i < allPossibleNeighbours.size(); i++){
                int distance = calculateDistance(allPossibleNeighbours[i]);
                if (distance < bestDistance){
                    bestDistance = distance;
                    bestNeighbour = allPossibleNeighbours[i];
                }
            }

            // Do we save best neighbour ?
            if (calculateDistance(bestNeighbour) < calculateDistance(bestBits)){
                stepsHeap.push_back(bits);
                bestBits = bestNeighbour;
            };

            bits = bestNeighbour;
        }

    }
    return bestBits;
}

int main(){
    string bits     = readBits("bits.txt");
    string bestBits = tabuAlgo(bits, 100);

    cout << "Przed : " << calculateDistance(bits) << endl;
    cout << "Po    : " << calculateDistance(bestBits) << endl;
    return 0;
}