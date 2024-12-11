#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <typeinfo>

#include <string.h>

#include "match.h"

int Person::nextProposeId() {
    return pref_list[next_propose_pointer++];
}

bool Person::prefers(Person* p) {
    Person* pprime = getEngagedTo();
    if (pprime == nullptr) {
        return true;
    }

    for (int id : pref_list) {
        if  (p->id == id) {
            return true;
        }
        if (pprime->id == id) {
            return false;
        }
    }
}

std::vector<Person> PersonSet::getSet() {
    return personv;
}

Person* PersonSet::oneFree() {
    for (Person& person: personv) {
        if (person.isFree()) {
            return &person;
        }
    }
    return nullptr;
};

Person* PersonSet::findById(int id) {
    for (Person& person: personv) {
        if (person.getId() == id) {
            return &person;
        }
    }
    return nullptr;
};

void Matchings::push(Person* m, Person* w) {
    Match match = Match(m, w);

    matchings.push_back(match);

    Person* mprime = w->getEngagedTo();
    if (mprime != nullptr) {
        remove(mprime, w);
    }

    Person* wprime = m->getEngagedTo();
    if (wprime != nullptr) {
        remove(m, wprime);
    }

    m->setEngagedTo(w);
    w->setEngagedTo(m);
}

void Matchings::remove(Person* m, Person* w) {
    for (int i=0; i<matchings.size(); i++) {
        if (matchings[i].getMan() == m && matchings[i].getWoman() == w) {
            matchings.erase(matchings.begin() + i);
            m->setFree();
            w->setFree();

        }
    }
}












std::vector<int> createRandomPreferenceList(int N) {

    std::vector<int> ids; // index corresponds to id
    std::vector<int> random_preference_list;

    for (int i=0; i<N; i++) {
        ids.push_back(i);
    }

    for (int i=0; i<N; i++) {

        int index = rand() % N;

        for (int j=0; j<N; j++) {
            int k = (j + index) % N;
            if (ids[k] != -1) {
                random_preference_list.push_back(k);
                ids[k] = -1;
                break;
            }
        }
    }
    return random_preference_list;
};

void writePersons(int N, std::string path) {

    std::fstream PersonListFile;
    PersonListFile.open(path, std::fstream::trunc | std::fstream::out);
    PersonListFile.close();
    PersonListFile.open(path, std::fstream::app);

    for (int i=0; i<N; i++) {
        std::vector<int> preference_list = createRandomPreferenceList(N);
        for (int j=0; j<N; j++) {
            PersonListFile << preference_list[j] << ' ';
        }
        PersonListFile << '\n';
    }

    PersonListFile.close();
};

PersonSet* readPersons(std::string path) {

    std::vector<Person> person_list;

    std::fstream PersonListFile;
    PersonListFile.open(path, std::fstream::in);

    int i = 0;

    std::string line;
    while (std::getline(PersonListFile, line)) {

        std::vector<int> person_pref_list;

        std::string token;
        std::string delimiter = " ";
        do {
            token = line.substr(0, line.find(delimiter));
            line.erase(0, line.find(delimiter) + delimiter.length());
            person_pref_list.push_back(std::stoi(token));
        } while(line.length());

        person_list.push_back(Person(i++, person_pref_list));
    }
    PersonListFile.close();

    PersonSet* person_set = new PersonSet(person_list);
    return person_set;
;
};











// Gale Shapley Algorithm implementation

Matchings* stableMatching(PersonSet* men_set, PersonSet* women_set) {

    // initially all m and w are free

    Matchings* matchings = new Matchings;

    // 'matchings' object takes care of creating/destroying engagements

    Person* m;

    // while there is a free m
    // (mathematically)...
    // (it cannot be that he has proposed to every w and still is free)

    // choose such a m

    while((m = men_set->oneFree())) {

        // find the next w on m's list

        Person* w = women_set->findById(m->nextProposeId());

        // if they are both free...

        if (w->isFree()) {

            // get enganged

            matchings->push(m, w);        
        }
        
        // if w is engaged...
        
        else if (w->isEngaged()) {

            // check if w prefers m over her current engagement

            if (w->prefers(m)) {

                // get engaged

                matchings->push(m, w);
            }
        }
    };

    return matchings;
};
