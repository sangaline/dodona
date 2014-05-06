//boost include files
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

//cruller include files
#include "FitnessFunctions.h"
#include "FitnessResult.h"

#include "InputModels/SimpleGaussianModel.h"
#include "InputModels/Interpolation.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//special boost-python headers that contain functinos necessary for the python interface
#include "Polygon_py.h"
#include "Keyboard_py.h"
#include "WordList_py.h"
#include "InputModels/InputModel_py.h"


using namespace boost::python;

//for general use in deep copying
template<typename T> const T DeepCopy(const T& v, dict d) { return T(v); }

//for general use in pickling
template<typename T> struct serialization_pickle_suite : pickle_suite {
    static object getstate(const T& t) {
        std::ostringstream os;
        boost::archive::text_oarchive oa(os);
        oa << t;
        return str(os.str());
    };
    static void setstate(T& t, object entries) {
        str s = extract<str>(entries)();
        std::string st = extract<std::string>(s)();
        std::istringstream is(st);

        boost::archive::text_iarchive ia(is);
        ia >> t;
    };
};


/********************************************************/
/***************** Python Module ************************/
/********************************************************/

BOOST_PYTHON_MODULE(cruller)
{

/***************** WordList class ***********************/
    
    class_<WordList>("WordList")
        .def("AddWord", &WordList::AddWord, AddWord_overloads())
        .def("Occurances", Occurances1)
        .def("Occurances", Occurances2)
        .def("TotalOccurances", &WordList::TotalOccurances)
        .def("Word", &WordList::Word)
        .def("Words", &WordList::Words)
        .def("RandomWord", &WordList::RandomWord)
        .def("WordIndex", &WordList::WordIndex)
        .def("LetterOccurances", &WordList::LetterOccurances)
        .def("TotalLetterOccurances", &WordList::TotalLetterOccurances)
        .def("Reset", &WordList::Reset)
        .def(self + self)
        .def("WordListDict",&WordListMapDict)
        .def("SetWordListDict",&SetWordListMapDict)
        .def("__deepcopy__", &DeepCopy<WordList>)
        .def_pickle(serialization_pickle_suite<WordList>())
    ;
/********************************************************/

/***************** Polygon class ************************/
    
    class_<Polygon>("Polygon")
        .def("Translate", &Polygon::Translate)
        .def("AddVertex", &Polygon::AddVertex)
        .def("ReplaceVertex", &Polygon::ReplaceVertex)
        .def("Reset", &Polygon::Reset)
        .def("VertexList", &PolygonVertexList)
        .def("VertexList", &PolygonVertexListClosed)
        .def("LeftExtreme", &Polygon::LeftExtreme)
        .def("RightExtreme", &Polygon::RightExtreme)
        .def("TopExtreme", &Polygon::TopExtreme)
        .def("BottomExtreme", &Polygon::BottomExtreme)
        .def("IsInside", &Polygon::IsInside)
        .def("__deepcopy__", &DeepCopy<Polygon>)
        .def_pickle(serialization_pickle_suite<Polygon>())
    ;
/********************************************************/

/***************** Keyboard class ***********************/
    
    class_<Keyboard>("Keyboard")
        .def("AddKey", &AddKeyStr)
        .def("RemoveKey", &RemoveKeyStr)
        .def("GetKey", &GetKeyStr)
        .def("PolygonDict", &KeyboardPolygonDict)
        .def("SetPolygonDict", &SetKeyboardPolygonDict)
        .def("OrderedKeyList", &KeyboardOrderedKeyList)
        .def("__deepcopy__", &DeepCopy<Keyboard>)
        .def_pickle(serialization_pickle_suite<Keyboard>())
    ;
/********************************************************/

/***************** InputModel classes ***********************/

    class_<InputModelWrapper, boost::noncopyable>("InputModel")
        .def("RandomVector", pure_virtual(&InputModel::RandomVector))
        .def("Distance", pure_virtual(&InputModel::Distance))
    ;
    
    class_<SimpleGaussianModel, bases<InputModel> >("SimpleGaussianModel")
        .def("RandomVector", &SimpleGaussianModel::RandomVector)
        .def("MarginalProbability", &SimpleGaussianModel::MarginalProbability)
        //.def("Distance", &SimpleGaussianModel::Distance)
        .def("SetXScale", &SimpleGaussianModel::SetXScale)
        .def("SetYScale", &SimpleGaussianModel::SetYScale)
        .def("SetScale", &SimpleGaussianModel::SetScale)
//        .def("__deepcopy__", &DeepCopy<Keyboard>)
    ;
/********************************************************/

/***************** InputVector class ********************/
    
    class_<InputVector>("InputVector")
        .def("Length", &InputVector::Length)
        .def("AddPoint", &InputVector::AddPoint)
        .def("X", &InputVector::X)
        .def("Y", &InputVector::Y)
        .def("T", &InputVector::T)
        .def("SpatialLength", &InputVector::SpatialLength)
        .def("TemporalLength", &InputVector::TemporalLength)
        .def("PointList", &InputVectorList)
        .def("DeltaPhi", &InputVector::DeltaPhi)
        .def("StringForm", &InputVector::StringForm)
    ;
/********************************************************/

/***************** FitnessResult class ********************/
    
    class_<FitnessResult>("FitnessResult", init<unsigned int, double, double>())
        .def(init<>())
        .def(self + self)
        .def("Fitness", &FitnessResult::Fitness)
        .def("Error", &FitnessResult::Error)
        .def("Iterations", &FitnessResult::Iterations)
        .def_pickle(serialization_pickle_suite<FitnessResult>())
    ;
/********************************************************/

/***************** FitnessFunctions ********************/
    def("MonteCarloEfficiency", &FitnessFunctions::MonteCarloEfficiency);
/********************************************************/

/***************** Interpolation ************************/
    def("SpatialInterpolation", &SpatialInterpolation);
/********************************************************/
}
