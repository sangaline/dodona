#ifndef FitnessResult_h
#define FitnessResult_h

class FitnessResult {
  protected:
    unsigned int iterations;
    double fitness, error;
  public:
    FitnessResult(unsigned int iterations, double fitness, double error);
    FitnessResult operator+(const FitnessResult& other);

    double Fitness() { return fitness; }
    double Error() { return error; }
    int Iterations() { return iterations; }

    void SetFitness(double f) { fitness = f; }
    void SetError(double e) { error = e; }
    void SetIterations(unsigned int i) { iterations = i; }
};

#endif
