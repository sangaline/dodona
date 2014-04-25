//boost include files
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

//cruller include files
#include "FitnessFunctions.h"
#include "FitnessResult.h"

#include "InputModels/SimpleGaussianModel.h"
#include "InputModels/Interpolation.h"

//special boost-python headers that contain functinos necessary for the python interface
#include "Polygon_py.h"
#include "Keyboard_py.h"
#include "WordList_py.h"
#include "InputModels/InputModel_py.h"


using namespace boost::python;

//for general use in deep copying
template<typename T> const T DeepCopy(const T& v, dict d) { return T(v); }

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
        .def("LetterOccurances", &WordList::LetterOccurances)
        .def("TotalLetterOccurances", &WordList::TotalLetterOccurances)
        .def("Reset", &WordList::Reset)
        .def(self + self)
        .def("WordListDict",&WordListMapDict)
        .def("SetWordListDict",&SetWordListMapDict)
        .def("__deepcopy__", &DeepCopy<WordList>)
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
        .def(self + self)
        .def("Fitness", &FitnessResult::Fitness)
        .def("Error", &FitnessResult::Error)
        .def("Iterations", &FitnessResult::Iterations)
    ;
/********************************************************/

/***************** FitnessFunctions ********************/
    def("MonteCarloEfficiency", &FitnessFunctions::MonteCarloEfficiency);
/********************************************************/

/***************** Interpolation ************************/
    def("SpatialInterpolation", &SpatialInterpolation);
/********************************************************/
}
