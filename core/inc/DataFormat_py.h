#ifndef DataFormat_py_h
#define DataFormat_py_h

#include "DataFormat.h"

#include <utility>
#include <string>
#include <vector>

//boost include files
#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/dict.hpp>

using namespace boost::python;


tuple DataFormatPythonForm(DataFormat& d) {
    dict metadata;
    metadata["wordlist"] = d.GetWordList();
    metadata["wordlistname"] = d.GetWordListName();
    metadata["inputmodel"] = d.GetInputModelName();
    metadata["distancemeasure"] = d.GetDistanceMeasureName();

    list l;
    for(unsigned int i = 0; i < d.Entries(); i++) {
        DataFormat::resultEntry r = d.GetEntry(i);
        dict efficiencies;
        for(unsigned int j = 0; j < r.second.size(); j++) {
            efficiencies[r.second[j].first] = r.second[j].second;
        }
        l.append(make_tuple(r.first, efficiencies));
    }

    return make_tuple(metadata, l);
}

#endif
