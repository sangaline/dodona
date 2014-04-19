#include "WordList.h"
#include "Polygon.h"
#include "Keyboard.h"
#include "FitnessFunctions.h"
#include "FitnessResult.h"

#include "InputModels/InputModel.h"
#include "InputModels/SimpleGaussianModel.h"
#include "InputModels/Interpolation.h"

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/str.hpp>

using namespace boost::python;

//for general use in deep copying
template<typename T> const T DeepCopy(const T& v, dict d) { return T(v); }

/***************** WordList class ***********************/
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(AddWord_overloads, AddWord, 1, 2)
unsigned int (WordList::*Occurances1)(const char*) = &WordList::Occurances;
unsigned int (WordList::*Occurances2)(const unsigned int) = &WordList::Occurances;
/********************************************************/

/***************** Polygon class ************************/
list PolygonVertexList(Polygon& p, bool close_loop) {
    list l;
    for(unsigned int i = 0; i < p.VertexCount(); i++) {
        l.append(make_tuple(p.VertexX(i), p.VertexY(i)));
    }
    if(close_loop && p.VertexCount() > 0) { l.append(make_tuple(p.VertexX(0), p.VertexY(0))); }
    return l;
}
list PolygonVertexListClosed(Polygon& p) {
    return PolygonVertexList(p, true);
}
/********************************************************/

/***************** Keyboard class ************************/
dict KeyboardPolygonDict(Keyboard& k) {
    dict d;
    for(unsigned int c = 0; c < 128; c++) {
        Polygon p( k.GetKey(char(c)) );
        if( p.VertexCount() > 0 ) {
            d[char(c)] = p;
        }
    }
    return d;
}

void SetKeyboardPolygonDict(Keyboard& k, dict d) {
    k.Reset();
    for(unsigned int c = 0; c < 128; c++) {
        if(d.has_key(str(char(c)))) {
            Polygon p = extract<Polygon>(d[str(char(c))]);
            k.AddKey(char(c), p);
        }
        else {
            Polygon p;
            k.AddKey(char(c), p);
        }
    }
}

list KeyboardOrderedKeyList(Keyboard& k) {
    list l;
    for(unsigned int c = 0; c < k.NKeys(); c++) {
        Polygon p( k.GetKey(k.CharN(c)) );
        if( p.VertexCount() > 0 ) {
            l.append(char(k.CharN(c)));
        }
    }
    return l;
}

void AddKeyStr(Keyboard &k, str s, const Polygon& p) {
    char const* c_str = extract<char const*>(s); k.AddKey(c_str[0], p);
}
void RemoveKeyStr(Keyboard &k, str s) {
    char const* c_str = extract<char const*>(s); k.RemoveKey(c_str[0]);
}
Polygon GetKeyStr(Keyboard &k, str s) {
    char const* c_str = extract<char const*>(s); return k.GetKey(c_str[0]);
}
/********************************************************/

/***************** InputVector class ********************/
list InputVectorList(InputVector& sigma) {
    list l;
    for(unsigned int i = 0; i < sigma.Length(); i++) {
        l.append(make_tuple(sigma.X(i), sigma.Y(i), sigma.T(i)));
    }
    return l;
}
/********************************************************/

/***************** InputModel wrappers ******************/
class InputModelWrapper : public InputModel, public boost::python::wrapper<InputModel> {
    double Distance(InputVector& vector, const char* word, Keyboard& k) {
        return this->get_override("Distance")(vector, word, k);
    }
    InputVector RandomVector(const char* word, Keyboard& k) {
        return this->get_override("RandomVector")(word, k);
    }
};
/********************************************************/



/********************************************************/
/********************************************************/
/***************** Python Module ************************/
/********************************************************/
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
