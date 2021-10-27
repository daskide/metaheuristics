#include <iostream>
#include "Problem.cpp"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <limits>

using namespace std;

const int REPEATS = 100 * 100;

struct Case {
public:
    float worst;
    float best;
    float avg;
};

class Result {
public:
    vector<float> evals;
    vector<float> times;
    Case eval;
    Case c_time;
    int best_id;

    void add_eval(float new_eval) {
        evals.push_back(new_eval);
    }

    float avg(vector<float> &v) {
        float sum = 0;
        for(auto it=v.begin(); it!=v.end(); it++) {
            sum += *it;
        }
        return sum / v.size();
    }

    float worst(vector<float> &v) {
        float b = -1;
        for(auto it=v.begin(); it!=v.end(); it++) {
            if (*it > b) b=*it;
        }
        return b;
    }

    float best(vector<float> &v) {
        float w = numeric_limits<float>::infinity();
        for(auto it=v.begin(); it!=v.end(); it++) {
            if (*it < w) {
                w=*it;
                best_id = it - v.begin();
            }
        }
        return w;
    }

    void calculateCases(Case& c, vector<float> &v) {
        c.avg = avg(v);
        c.best = best(v);
        c.worst = worst(v);

        cout << c.avg << " " << c.best;
    }

    void calculate() {
        calculateCases(eval, evals);
        //calculateTimes(c_time, times);
    }
};

ostream& operator<<(ostream& os, Case& c) {
        os << "Best: " << c.best << endl;
        os << "Worst: " << c.worst << endl;
        os << "Avg: " << c.avg << endl;
        return os;
};

ostream& operator<<(ostream& os, Result& res) {
        os << "Result:\n";
        os << "Eval:\n";
        os << res.eval;
        os << "\n- - -\n";
        return os;
};


class Algorithm {
public:
    string name;

    Algorithm(string new_name) {
        this->name = new_name;
    }

    virtual Individual solve(Problem &problem) = 0;

    float fitnessEval(Individual &ind) {
        return ind.calculateDistance();
    }
    void resultsEval(Individual &ind, Result &res) {
        res.add_eval(fitnessEval(ind));
    }
};

class RandomAlgorithm : Algorithm {
private:
    int repeats = REPEATS;

public:
    RandomAlgorithm() : Algorithm("RandomAlgorithm"){}
    RandomAlgorithm(int repeats) : Algorithm("RandomAlgorithm"){
        RandomAlgorithm();
        this->repeats = repeats;
    }

    Individual solve(Problem &problem) {
        vector<Individual> inds;
        Result res;
        for(int i=0; i<repeats; i++) {
            inds.push_back(*solve(problem, i));
        }

        for(int i=0; i<repeats; i++) {
            resultsEval(inds.at(i), res);
        }
        res.calculate();

        cout << res;
        cout<<inds.at(res.best_id);
        return inds.at(res.best_id);
    }

    Individual* solve(Problem &problem, int repeats) {
        Individual* ind = new Individual(problem);
        int dim = problem.dimension;
        int starting_city = rand() % dim;
        problem.magazine = 0;

        ind->addMagazine(0);

        vector<int> cities_left;
        for(int i=0;i<dim; i++) cities_left.push_back(i);
        cities_left.erase(cities_left.begin());

        for(int i=0; i<dim-1; i++) {
            int rand_city = rand() % cities_left.size();
            if (! ind->addCity(cities_left[rand_city])) {
                ind->addMagazine(0);
                i--;
            }
            else
                cities_left.erase(cities_left.begin()+rand_city);
        }
        ind->addEnd();
        return ind;
    }

};

class GreedyAlgorithm : Algorithm {
private:
    int findNearest(Problem &problem, int city_id, vector<int>& cities_left) {
        int id_nearest;
        int nearest = numeric_limits<int>::infinity();
        for(int i=0; i<cities_left.size(); i++) {
            cout << city_id << " " << cities_left.at(i);
            int dist = problem.distances[city_id][cities_left.at(i)];
            if (dist < nearest) {
                nearest = dist;
                id_nearest = i;
            }
        }
        return id_nearest;
    }
public:
    GreedyAlgorithm() : Algorithm("GreedyAlgorithm"){};

    Individual solve(Problem &problem) {
        vector<Individual> inds;
        Result res;
        for(int i=0; i<1; i++) {
            inds.push_back(*solve(problem, i));
        }

        for(int i=0; i<1; i++) {
            resultsEval(inds.at(i), res);
        }
        res.calculate();

        cout << res;
        cout<<inds.at(res.best_id);
        return inds.at(res.best_id);
    }

    Individual* solve(Problem &problem, int starting_city) {
        Individual *ind = new Individual(problem);
        // int dim = problem.dimension;
        // problem.magazine = starting_city;

        // ind.addMagazine(starting_city);

        // vector<int> cities_left;
        // for(int i=0;i<dim; i++) cities_left.push_back(i);
        // cities_left.erase(cities_left.begin()+starting_city);

        // cout<<"lol";

        // for(int i=0; i<dim-1; i++) {
        //     int nearest_city = findNearest(problem, ind.lastCity(), cities_left);
        //     cout<<nearest_city << " " << cities_left[nearest_city];
        //     if (! ind.addCity(cities_left[nearest_city])) {
        //         ind.addMagazine(starting_city);
        //         i--;
        //     }

        //     else
        //         cities_left.erase(cities_left.begin()+nearest_city);
        // }
        // ind.addEnd();

        // cout << ind;
        return ind;
    }

};

int main() {
    srand (time(NULL));

    Loader loader;
    Problem problem;
    loader.load(problem);
    cout<<problem.dimension;

    for(int i=0; i<problem.dimension; i++) {
        cout<<problem.demands[i]<< " ";
    }

    cout << "\n";

    for(int i=0; i<problem.dimension; i++) {
        for(int j=0; j<problem.dimension; j++) {
            cout<<problem.distances[i][j]<< " ";
        }
        cout<<"\n";
    }

    cout<<"\n\n";
    RandomAlgorithm ra;
    //ra.solve(problem);
    cout << endl;
    Individual ind = ra.solve(problem);

    //GreedyAlgorithm ga;
    //Individual ind = ga.solve(problem);
    //cout << ind;




    return 0;
}