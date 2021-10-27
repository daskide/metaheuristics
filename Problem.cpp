#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

const int END = -9;

class City {
private:
    int cit_id;
    float cor_x;
    float cor_y;
    int dem;
public:
    int id() {
        return cit_id;
    }
    void id(int new_id) {
        this->cit_id = new_id;
    }
    void demand(int new_demand) {
        dem = new_demand;
    }
    int demand() {
        return dem;
    }
    float x() {
        return cor_x;
    }
    float y() {
        return cor_y;
    }
    void setX(float new_x) {
        this->cor_x = new_x;
    }
    void setY(float new_y) {
        this->cor_y = new_y;
    }
    float distance(City &other) {
        return sqrt(pow(this->x() - other.x(), 2) + pow(this->y() - other.y(), 2));
    }
};

class Problem {
public:
    int dimension;
    int capacity;
    City *cities;
    float **distances;
    int *demands;
    int magazine;

    Problem() {
        magazine = 0;
    }

    ~Problem() {
        //delete cities; ////////////////////////////////////////////// TOOOOO DOOOOO
        //delete demands;
        //for(int i=0; i<dimension; i++) delete distances[i];
        //delete distances;
    }

    void generateMatrix() {
        distances = new float*[dimension];
        for(int i=0; i<dimension; i++) {
            distances[i] = new float[dimension];
        }
        cities = new City[dimension];
        demands = new int[dimension];
    }

    void calculateDistances() {
        for(int i=0; i<dimension; i++) {
            for(int j=0; j<dimension; j++) {
                distances[i][j] = sqrt(pow(cities[i].x() - cities[j].x(), 2) + pow(cities[i].y() - cities[j].y(), 2));
            }
        }
    }

    // int calculateDemands(int *path) {
    //     for(int i=0; i<dimension*2; i++) {
    //         if (path[i] == END) {
    //             return
    //         }
    //     }
    // }
};

class Individual {
public:
    int *path;
    int current_id = 0;
    int max_dim;
    int *demands;
    Problem *problem;
    //vector<int> cities_left;

    Individual(int new_max_dim) {
        max_dim = new_max_dim;
        current_id = 0;
    }

    ~Individual() {
        delete path;
    }

    Individual(Problem &problem) {
        max_dim = problem.dimension * 2;
        this->problem = &problem;
        current_id = 0;
        this->path = new int[max_dim];
        this->demands = new int[max_dim];
        //for (int i=0; i<problem.dimension; i++)
        //    cities_left.push_back(i);
    }

    Individual(const Individual& obj) {
        this->max_dim = obj.max_dim;
        this->path = new int[max_dim];
        this->demands = new int[max_dim];
        //cout<<"lol";
        int i=0;
        while(obj.path[i] != END) {
            this->path[i] = obj.path[i];
            this->demands[i] = obj.demands[i];
            i++;
        }
        this->path[i] = END;
        current_id = obj.current_id;
        this->problem = obj.problem;
    }

    int lastCity() {
        return path[current_id - 1];
    }

    bool addCity(int city_id) {
        int old_demand = current_id == 0 ? 0 : demands[current_id - 1];
        int new_demand = old_demand + problem->cities[city_id].demand();
        if (new_demand > problem->capacity) ////////////////////////////////////////////////////// OR >= ???
            return false;
        else
            demands[current_id] = new_demand;
        path[current_id] = city_id;

        current_id++;
        //scities_left
        return true;
    }

    void addMagazine(int city_id) {
        path[current_id] = city_id;
        demands[current_id] = 0;
        current_id++;
    }

    void addEnd() {
        path[current_id] = END;
    }

    float calculateDistance() {
        float distance = 0;
        for(int i=1; i<current_id; i++) {
            distance += problem->distances[path[i]][path[i-1]];
        }
        distance += problem->distances[path[current_id-1]][0];
        return distance;
    }
};

ostream& operator<<(ostream& os, Individual& ind) {
        os << "- - - Ind:\nDistance: " << ind.calculateDistance() << "\n" << "Path: ";
        for(int i=0; i < ind.max_dim; i++) {
            if(ind.path[i] == END) break;
            os << ind.path[i] << " ";
        }
        os << endl;
        for(int i=0; i < ind.max_dim; i++) {
            if(ind.demands[i] == END) break;
            os << ind.demands[i] << " ";
        }
        os << "\n- - -\n";
        return os;
};

class Loader {
public:
    Loader() {

    }

    void setData(int& data, string line) {
        data = stoi(line.substr(line.find(": ") + 2));
    }

    void loadCoords(Problem& problem, ifstream& myfile) {
        int a;
        for (int i=0; i<problem.dimension; i++) {
            //myfile >> a >> problem.cities[a-1][0] >> problem.cities[a-1][1];
            float x,y;
            myfile >> a >> x >> y;
            problem.cities[a-1].id(a-1);
            problem.cities[a-1].setX(x);
            problem.cities[a-1].setY(y);
        }
    }

    void loadDemands(Problem& problem, ifstream& myfile) {
        int a;
        int demand;
        while (myfile >> a >> demand) {
            problem.cities[a-1].demand(demand);
        }
    }

    void load(Problem& problem) {
        ifstream myfile;
        string path, line;
        //cout<<"Enter path:\n";
        //cin>>path;

        path = "t1.txt";
        myfile.open(path);
        while (getline(myfile, line))
        {
            if(line.find("DIMENSION") == 0) {
                setData(problem.dimension, line);
                problem.generateMatrix();
            }
            else if(line.find("CAPACITY") == 0) {
                setData(problem.capacity, line);
            }
            else if(line.find("NODE_COORD_SECTION") == 0) {
                loadCoords(problem, myfile);
                myfile >> line;
                cout<<line << endl;
                // myfile >> line;
                // cout<<line << endl;
                loadDemands(problem, myfile);
                problem.calculateDistances();
            }
        }
        myfile.close();
    }
};



/*

reprezentacja trasY:


*/


