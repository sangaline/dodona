#ifndef TrainingDataFileFormat_h
#define TrainingDataFileFormat_h

#include "WordList.h"
#include "Keyboard.h"
#include "FitnessResult.h"

#include <utility>
#include <string>
#include <vector>
#include "boost/serialization/vector.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/utility.hpp"


namespace boost {namespace serialization {class access;}}

class TrainingDataFileFormat {
  public:
    typedef std::pair<Keyboard, std::vector< std::pair<std::string, FitnessResult> > > resultEntry;
  private:

    /******* Metadata ********/
    WordList w;
    std::string word_list;
    std::string input_model;
    std::string distance_measure;

    /******* Results Vector *******/
    //Each result2 will be stored as a pair containing the specific keyboard and a vector of
    //pairs of the efficiency string (i.e. "MC", "fast", etc) and the fitness result2s (efficiency)
    std::vector<resultEntry> result_vector;

  public: 
    TrainingDataFileFormat();
    TrainingDataFileFormat(TrainingDataFileFormat const& td);
    TrainingDataFileFormat(WordList& wordlist, std::string wordlistname, std::string inputmodel, std::string distancemeasure);
    ~TrainingDataFileFormat();
    TrainingDataFileFormat operator+(const TrainingDataFileFormat& other);
    bool operator==(const TrainingDataFileFormat& other);

    void AddEntry(Keyboard k, std::string effType, FitnessResult eff);
    resultEntry GetEntry(const unsigned int index) const;
    unsigned int Entries() const { return result_vector.size(); }

    WordList GetWordList() const { return w; }
    void SetWordList(WordList& wl) { w = wl; }

    std::string GetWordListName() const { return word_list; }
    void SetWordListName(std::string wName) { word_list = wName; }

    std::string GetInputModelName() const { return input_model; }
    void SetInputModelName(std::string imName) { input_model = imName; }

    std::string GetDistanceMeasureName() const { return distance_measure; }
    void SetDistanceMeasureName(std::string dmName) { distance_measure = dmName; }


  private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version) {
        ar & w & input_model & distance_measure & result_vector;
    }
};

#endif
