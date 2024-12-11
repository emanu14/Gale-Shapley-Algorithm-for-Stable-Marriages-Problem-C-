#include <vector>
#include <string>

#ifndef MATCH
#define MATCH

class Person {
    int id;
    std::vector<int> pref_list;

    Person* engaged_to;
    int next_propose_pointer;

    public:
        Person(int id, std::vector<int> pref_list) : id(id), pref_list(pref_list) {
            engaged_to = nullptr;
            next_propose_pointer = 0;
        };

        int getId() { return id; };
        std::vector<int> getPrefList() { return pref_list; };

        bool isFree() { return engaged_to == nullptr; };
        bool isEngaged() { return engaged_to != nullptr; };
        Person* getEngagedTo() { return engaged_to; };

        void setEngagedTo(Person* person) { engaged_to = person; };
        void setFree() { engaged_to = nullptr; };

        int nextProposeId();
        bool prefers(Person* person);
};

class PersonSet {
    std::vector<Person> personv;

    public:
        PersonSet(std::vector<Person> personv) : personv(personv) {};
        std::vector<Person> getSet();
        Person* oneFree();
        Person* findById(int id);
};

class Match {
    Person* m;
    Person* w;
    
    public:
        Match(Person* m, Person* w) : m(m), w(w) {};

        Person* getMan() { return m; };
        Person* getWoman() { return w; };

        std::string toString() {
            return "(" + std::to_string(m->getId()) + ", " + std::to_string(w->getId()) + ")";
        }
};

class Matchings {
    std::vector<Match> matchings;
    
    public:
        std::vector<Match> getMatchings() { return matchings; }

        void push(Person* m, Person* w);
        void remove(Person* m, Person* w);
};

std::vector<int> createRandomPreferenceList(int N);
void writePersons(int N, std::string path);
PersonSet* readPersons(std::string path);

Matchings* stableMatching(
    PersonSet* men_set, 
    PersonSet* women_set
);

#endif