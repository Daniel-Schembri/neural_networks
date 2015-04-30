// neural-net-tutorial.cpp

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#include "neuron.hpp"
#include "feedForwardNet.hpp"
#include "competitive.hpp"
#include "srn.hpp"
#include "commonDefs.hpp"

using namespace std;

// Simple class to read training data from a text file.

class TrainingData
{
public:
    TrainingData(const string filename);
    bool isEof() {return m_trainingDataFile.eof();}
    void getTopology(vector<unsigned> &topology);

    // Returns the number of input values read from the file:
    unsigned getNextInputs(vector<double> &inputVals);
    unsigned getTargetOutputs(vector<double> &targetOutputVals);

private:
    ifstream m_trainingDataFile;
};

void TrainingData::getTopology(vector<unsigned> &topology)
{
    string line;
    string label;

    getline(m_trainingDataFile, line);
    stringstream ss(line);

    ss >> label;
    if (this->isEof() || label.compare("topology:") != 0)
        abort();

    while (!ss.eof()) 
    {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }

    return;
}

TrainingData::TrainingData(const string filename)
{
    m_trainingDataFile.open(filename.c_str());
    if (m_trainingDataFile.fail())
    {
        std::cerr << "\Error, could not open file '" << filename << "' !";
        exit(1);
    }
}

unsigned TrainingData::getNextInputs(vector<double> &inputVals)
{
    inputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("in:") == 0) 
    {
        double oneValue;
        while (ss >> oneValue)
            inputVals.push_back(oneValue);
    }

    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals)
{
    targetOutputVals.clear();

    string line;
    getline(m_trainingDataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("out:") == 0) 
    {
        double oneValue;
        while (ss >> oneValue)
            targetOutputVals.push_back(oneValue);
    }

    return targetOutputVals.size();
}

void showVectorVals(string label, vector<double> &v)
{
    cout << label << " ";
    for (unsigned i = 0; i < v.size(); ++i) 
        cout << v[i] << " ";

    cout << endl;
}


int main()
{
    //.
    //├── main.cpp
    //└── training
    //    └── trainingData.txt
    TrainingData trainData("./src/training/trainingDataV.txt");

    // e.g., { 3, 2, 1 }
    vector<unsigned> topology;
    trainData.getTopology(topology);

    // use bias neurons
    FeedForwardNet myNet(topology, true);

    vector<double> inputVals, targetVals, resultVals;
    int trainingPass = 0;

    while (!trainData.isEof()) 
    {
        ++trainingPass;
        cout << endl << "Pass " << trainingPass;

        // Get new input data and feed it forward:
        if (trainData.getNextInputs(inputVals) != topology[0])
            break;

        showVectorVals(": Inputs:", inputVals);
        myNet.feedForward(inputVals);

        // Collect the net's actual output results:
        myNet.getResults(resultVals);
        showVectorVals("Outputs:", resultVals);

        // Train the net what the outputs should have been:
        trainData.getTargetOutputs(targetVals);
        showVectorVals("Targets:", targetVals);
        assert(targetVals.size() == topology.back());

        myNet.learn(targetVals);

        // Report how well the training is working, average over recent samples:
        cout << "Net recent average error: " << myNet.getRecentAverageError() << endl;
    }

    cout << endl << "Done" << endl;
}

