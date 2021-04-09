#include<fstream>
#include<iostream>
#include<clique.hpp>
#include<clique_block.hpp>
#include<definitions.hpp>

using namespace pyclustering;
using namespace pyclustering::clst;

 dataset read_data(const std::string & filename) {
        dataset data;

        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            std::stringstream stream(line);

            point coordinates;
            double value = 0.0;
            while (stream >> value) {
                coordinates.push_back(value);
            }

            data.push_back(coordinates);
        }

        file.close();
        return data;
    }

    int main() {
        // Read two-dimensional input data 'Target'.
        dataset data = read_data("Target.data");

        // Prepare algorithm's parameters.
        const std::size_t intervals = 10;   // defines amount of cells in grid in each dimension
        const std::size_t threshold = 4;    // no outliers

        // Create CLIQUE algorithm for processing.
        clique clique_instance = clique(intervals, threshold);

        // Run clustering process.
        clique_data result;
        clique_instance.process(data, result);

        // Obtain results.
        cluster_sequence & clusters = result.clusters();

        noise & outliers = result.noise();  // in this case it is empty because threshold is 0.

        // Display information about extracted clusters:
        std::cout << "Amount of clusters: " << clusters.size() << std::endl;
        std::cout << "Amount of outliers: " << outliers.size() << std::endl;

        return 0;
    }
