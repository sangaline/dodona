#include "DataFormat.h"

#include <iostream>
using namespace std;

DataFormat::DataFormat() {
}

DataFormat::DataFormat(DataFormat const& td) {
    w = td.w; 
    word_list = td.word_list;
    input_model = td.input_model;
    distance_measure = td.distance_measure;
    
    for(unsigned int i = 0; i != td.result_vector.size(); i++) {
        resultEntry re = td.GetEntry(i);
        Keyboard k = re.first;
        vector<pair<string, FitnessResult> > effPair;
        for(unsigned int j = 0; j != re.second.size(); j++) {
            effPair.push_back(make_pair(re.second[j].first, re.second[j].second));
        }
        result_vector.push_back(make_pair(k, effPair));
    }
}

DataFormat::DataFormat(WordList& wl, string wln, string im, string dm) {
    w = wl;
    word_list = wln;
    input_model = im;
    distance_measure = dm;
}

DataFormat::~DataFormat() {
}

void DataFormat::AddEntry(Keyboard k, string effType, FitnessResult eff, int index) { 
    //If no index is specified then just look for an existing keyboard entry to append to.
    //If no entry matches the current keyboard then create a new entry
    if(index<0) {
        //check to see if any efficiencies for this keyboard layout have been entered yet
        for(unsigned int i = 0; i != result_vector.size(); i++) {
            if(result_vector[i].first == k) {
                result_vector[i].second.push_back(make_pair(effType, eff));
                return;
            }        
        }
    }

    //If appending to an existing entry by index then the index must be smaller than the maximum possible index.
    //Also insure that the keyboards match before appending to an existing entry.
    else if((unsigned) index < result_vector.size()) {
       if(result_vector[index].first == k) {
            result_vector[index].second.push_back(make_pair(effType, eff));
            return;
       }

       else {
            cerr << "An entry already exists at index: " << index << " with a different keyboard.  ";
            cerr << "The current result cannot be written here." << endl;
            return;
       }
    }

    //If an index is provided that is larger than the size of the result_vector (i.e. trying to add a new entry
    //at a position beyond the end of the list) then print an error message and return without writing anything.
    else if((unsigned) index > result_vector.size()) {
        cerr << "The size of the result vector is: " << result_vector.size();
        cerr << " so an entry cannot be placed at position: " << index << "." << endl;
        cerr << "To add a new entry the index must be equal to the size of the result vector." << endl;
        return;
    }
    
    //If the function makes it down here than one of two things happened: a) An index was provided equal to the
    //size of the result vector.  This means a new, unique entry is requested to be put at the end of the result
    //vector, regardless if an entry already exists with an identical keyboard. b) an index was not provided and
    //no matching keyboard already exists in the results vector so we also create a new entry to be put at the 
    //end of the results vector.
    
    vector<pair<string, FitnessResult> > effPair;
    effPair.push_back(make_pair(effType, eff));
    result_vector.push_back(make_pair(k, effPair));

    return;
}

DataFormat::resultEntry DataFormat::GetEntry(const unsigned int index) const {
    if(index > result_vector.size()) {
        cerr << "ERROR: Index of result_vector is out of range" << endl;
        exit(1);
    }
    return result_vector[index];
}

DataFormat DataFormat::operator+(const DataFormat& other) {
    if(*this == other) {
        cerr << "ERROR: You can not add the same file to itself, you'll bias the data!!!!" << endl;
        exit(1);
    }
    if(word_list != other.GetWordListName()) {
        cerr << "ERROR: These two training data files do not use the same word list." << endl;
        exit(1);
    }
    if(input_model != other.GetInputModelName()) {
        cerr << "ERROR: These two training data files do not use the same input model." << endl;
        exit(1);
    }
    if(distance_measure != other.GetDistanceMeasureName()) {
        cerr << "ERROR: These two training data files do not use the same distance measure." << endl;
        exit(1);
    }   

    DataFormat td = DataFormat(other);
    for(unsigned int i =0; i != result_vector.size(); i++) {
        resultEntry re = GetEntry(i);
        Keyboard k = re.first;
        
        for(unsigned int j = 0; j != re.second.size(); j++ ) {
            string effType = re.second[j].first;
            FitnessResult eff = re.second[j].second;
            td.AddEntry(k,effType,eff);
        }        
    }

    return td;
}

bool DataFormat::operator==(const DataFormat& other) {
    if(word_list != other.GetWordListName()) 
        return false;
    if(input_model != other.GetInputModelName())
        return false;
    if(distance_measure != other.GetDistanceMeasureName())
        return false;
    if(result_vector.size() != other.Entries())
        return false;

    for(unsigned int i = 0; i != result_vector.size(); i++) {
        resultEntry re = GetEntry(i);
        resultEntry reOther = other.GetEntry(i);

        if(re.second.size() != reOther.second.size())
            return false;
        if(!(re.first == reOther.first))
            return false;
        for(unsigned int j = 0; j != re.second.size(); j++) {
            if(re.second[j].first != reOther.second[j].first)
                return false;
            if(re.second[j].second.Fitness() != reOther.second[j].second.Fitness())
                return false;
        }
    }
    return true;
}


