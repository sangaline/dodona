#include "FitnessResult.h"

#include "math.h"

FitnessResult::FitnessResult() {
    iterations = 0;
    fitness = 0;
    error = 0;
}

FitnessResult::FitnessResult(unsigned int iterations, double fitness, double error) 
    : iterations(iterations), fitness(fitness), error(error) {

}

FitnessResult FitnessResult::operator+(const FitnessResult& other) {
    unsigned int newi = iterations + other.iterations;
    double weight1 = double(iterations)/double(newi);
    double weight2 = double(other.iterations)/double(newi);
    double newfitness = weight1*fitness + weight2*other.fitness;
    double newerror =  sqrt(pow(error*weight1, 2) + pow(other.error*weight2, 2));

    return FitnessResult(newi, newfitness, newerror);
}
