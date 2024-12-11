#include <string>
#include <vector>
#include <iostream>
#include <ctime>

#include "match.h"


int main(int argc, char *argv[]) {
    const int N = std::stoi(argv[1]);

    const std::string men_file_path = "files/men.txt";
    const std::string women_file_path = "files/women.txt";

    srand(time(0));
    writePersons(N, men_file_path);
    writePersons(N, women_file_path);

    Matchings* matchings = stableMatching(
        readPersons(men_file_path), 
        readPersons(women_file_path)
    );

    for (Match match : matchings->getMatchings()) {
        std::cout << match.toString();
    }

    return 0;
}
