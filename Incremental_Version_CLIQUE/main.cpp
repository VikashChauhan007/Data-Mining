#include<fstream>
#include<iostream>
#include<clique.hpp>
#include<clique_block.hpp>
#include<definitions.hpp>
#include<thread>
#include<chrono>
#include<cmath>
#include<algorithm>
#include <unordered_map>
#include<bits/stdc++.h>
#include<windows.h>
#include<psapi.h>

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

std::string get_string_logical_location(clique_block_location &log_loc)
{
    std::string loc = "";
    for (auto itr = log_loc.begin() ; itr != log_loc.end() ; itr++)
    {
        loc = loc + std::to_string(*itr) + ",";
    }
    return loc;
}


void point_recreate(std::string str, const char delim, std::vector<std::string> &out)
{
    //std::cout<<"\npoint to be created is : "<<str<<std::endl;
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}


// Stores the dictionary with the given point in static version
std::size_t Store_dict(std::unordered_map<std::string, int> &dict,std::string filename)
{

    std::string line;
    std::ifstream data_info_file(filename);
    std::string key;
    std::size_t linenum = -1;
    if(data_info_file.is_open())
    {
        while(!data_info_file.eof())
        {
            getline(data_info_file,line);
            if (line != "")
            {
                linenum++;

                std::stringstream linestream(line);
                std::string val;
                key = "";
                while(linestream >> val)
                {
                    key = key + val + "," ;
                }

                dict[key] = linenum;
            }
        }
        data_info_file.close();
    }
    return linenum+1;
}


void Delete_dict(std::unordered_map<std::string, int> &dict)
{
    std::string line;
    std::ifstream data_info_file;
    data_info_file.clear();
    data_info_file.open("Target_Incremental1.data");
    std::ofstream tempfile;
    tempfile.open("temp.txt");

    std::string key;
    std::size_t linenum = -1;
	if(data_info_file.is_open())
    {
        while(!data_info_file.eof())
        {
            getline(data_info_file,line);
            if (line != "")
            {
                linenum++;

                std::stringstream linestream(line);
                std::string val;
                key = "";
                while(linestream >> val)
                {
                    key = key + val + "," ;
                }
                auto item = dict.find(key);
                if (item == dict.end())
                {
                    // element not found in dict so added as new point
                    tempfile<<line<<"\n";
                }
                else
                {
                    // Since the element is present then we need to delete in the dict
                    dict.erase(item);
                }
            }
        }

        data_info_file.close();
        tempfile.close();
    }
}


int main()
{

    std::string filename = "res.data",  file2 = "temp.txt";
    std::size_t f1 = 0, f2 = 0;

    //To store the points
    std::unordered_map<std::string, int> dict;

    //To store the points
    std::unordered_map<std::string, int> clique_block_map;



    // Storing the points to dictionary
    f1 = Store_dict(dict, filename);

    // Read two-dimensional input data 'Target'.
    dataset data = read_data(filename);

    // Prepare algorithm's parameters.
    const std::size_t intervals = 8;   // defines amount of cells in grid in each dimension
    const std::size_t threshold = 10;    // no outliers

    // Create CLIQUE algorithm for processing.
    clique clique_instance = clique(intervals, threshold);

    // Run clustering process.
    clique_data result;
    clique_instance.process(data, result);

    // Obtain results.
    cluster_sequence & clusters = result.clusters();
    clique_block_sequence & blocks = result.blocks();
    noise & outliers = result.noise();  // in this case it is empty because threshold is 0.


    //Initializing the clique block map
    int i = 0;
    for(auto it = blocks.begin() ; it < blocks.end() ; it++, i++)
    {
        clique_block_map[(it)->get_logical_location_str()] = i;
    }




    // Display information about extracted clusters:
    std::cout << "Details of Static: \n";
    std::cout << "Amount of clusters: " << clusters.size() << std::endl;
    std::cout << "Amount of outliers: " << outliers.size() << std::endl;


    std::this_thread::sleep_for (std::chrono::seconds(2));

    //Start working on the incremental design


    // Reading the min and max corner of each of the dimension
    std::string line;
    std::ifstream data_info_file("data_info_details.txt");
    point m_min_corner;
    point m_max_corner;
    double val;
    std::vector<double> m_sizes;
    if(data_info_file.is_open())
    {
        while(!data_info_file.eof())
        {
            getline(data_info_file,line);
            if (line != "")
            {
                // since the line in file is space separated values so reading each value
                std::stringstream linestream(line);
                while(linestream >> val)
                {
                    m_max_corner.push_back(val);
                }
            }

            getline(data_info_file,line);
            if (line != "")
            {
                // since the line in file is space separated values so reading each value
                std::stringstream linestream(line);
                while(linestream >> val)
                {
                    m_min_corner.push_back(val);
                }
            }

            getline(data_info_file,line);
            if (line != "")
            {
                // since the line in file is space separated values so reading each value
                std::stringstream linestream(line);
                while(linestream >> val)
                {
                    m_sizes.push_back(val);
                }
            }
        }
        data_info_file.close();
    }

    // Reading the new points that are being added in the Incremental dataset
    dataset new_data = read_data(file2);

    auto start = std::chrono::high_resolution_clock::now();
    // Taking Each point in the new list of added points
    for(auto new_point = new_data.begin() ; new_point < new_data.end() ; new_point++)
    {
        point p = *new_point;              // Stores the location of the point in hyper-space
        clique_block_location c_lof_loc;   // Stores the logical address of a clique block for the point
        std::size_t logical_grid_val;      // Stores the logical value of a particular dimension
        int i = 0;
        c_lof_loc.clear();

        // Calculating the logical grid location value to which the point belong
        for(auto point_itr = p.begin(); point_itr < p.end() ; point_itr++, i++)
        {
            //calculating the each dimension block value
            logical_grid_val = floor((*point_itr - m_min_corner[i]) / (m_sizes[i] / intervals));

            c_lof_loc.push_back(logical_grid_val);

        }

        auto clique_block_index_itr = clique_block_map.find(get_string_logical_location(c_lof_loc));
        clique_block &temp = blocks[clique_block_index_itr->second];


        // Add the new point to this clique block
        std::size_t previous_number_of_points, next_number_of_points;
        std::list<std::size_t> block_points = temp.get_points();
        previous_number_of_points = block_points.size();

        //std::cout<<"Number of points before new point addition:  "<<previous_number_of_points<<"\n";
        temp.add_new_point(++f1);

        block_points = temp.get_points();
        //std::cout<<"Number of points after new point addition:  "<<block_points.size()<<"\n";

        next_number_of_points = previous_number_of_points + 1;

        if(previous_number_of_points <= threshold && next_number_of_points > threshold)
        {
           // std::cout<<"Changed to dense unit after addition"<<"\n";
            std::vector<clique_block_location>  block_neighbors;
            block_neighbors.clear();

            // Getting neighbour of the block unit where the points have been added
            temp.get_location_neighbors(intervals, block_neighbors);

            //Finding from neighbour which is already dense
            bool is_added_to_clusters = false; // use to check if added in any clusters
            for(auto neighbor = block_neighbors.begin() ; neighbor < block_neighbors.end() ; neighbor++)
            {
                for(std::size_t b = 0 ; b < blocks.size() ; b++)
                {
                    if(blocks[b].match_logical_location(*neighbor))
                    {
                        //neighbor block found
                        std::list<std::size_t> neighbour_block_points = blocks[b].get_points();
                        if (neighbour_block_points.size() > threshold)
                        {
                            // this unit is dense
                            is_added_to_clusters = true;

                            // Setting the dfs value of the m_visited attribute
                            temp.put_is_visited(blocks[b].is_visited());

                            for(auto c = clusters.begin() ; c != clusters.end() ; c++)
                            {

                                std::vector<std::size_t> cluster_list = *c;

                                std::size_t first_elem =  neighbour_block_points.front(); // stores the first point of the stored cluster

                                //std::cout<<"Cluster points before addition: ";
                                for(int point_inside_cluster_list  = 0 ; point_inside_cluster_list < cluster_list.size() ; point_inside_cluster_list++)
                                {
                                    //std::cout<<cluster_list[point_inside_cluster_list]<<"  ";
                                }

                                //std::cout<<std::endl;


                                // Each cluster ko access kar raha hu
                                //std::cout<<"Element to be found   "<<first_elem<<"\n";
                                if (std::find(cluster_list.cbegin(), cluster_list.cend(), first_elem) != cluster_list.cend())
                                {
                                    //std::cout<<"Element Found\n"<<"Points to be added in clusters: ";

                                    for(std::size_t index : block_points)
                                    {
                                        //std::cout<<index<<"  ";
                                        (*c).push_back(std::size_t(index));
                                    }

                                }

                                cluster_list = *c;
                                //std::cout<<"\nCluster points after addition: ";
                                for(int point_inside_cluster_list  = 0 ; point_inside_cluster_list < cluster_list.size() ; point_inside_cluster_list++)
                                {
                                    //std::cout<<cluster_list[point_inside_cluster_list]<<"  ";
                                }
                                //std::cout<<"\n";
                            }

                        }
                        break;
                    }
                }
                if(is_added_to_clusters)
                    break;
            }

            if(!is_added_to_clusters)
            {
                // No cluster in neighbours present
                cluster new_cluster;
                for(std::size_t index : block_points)
                {
                    //std::cout<<index<<"  ";
                    new_cluster.push_back(std::size_t(index));
                }
                clusters.push_back(new_cluster);

            }
        }
        else
        {
            // Either the unit was already dense
            // Or it is still non-dense
           // std::cout<<"No Action Required"<<"\n";
        }
    }

    std::cout<<"\nAfter Addition of Points(if any): \n";
    std::cout<<"Amount of clusters: " << clusters.size() << std::endl;
    std::cout<<"Amount of outliers: " << outliers.size() << std::endl<<std::endl;

     //logical location = 1,2,
    //auto map_pointer = clique_block_map.find("1,2,");
    //int block_index = map_pointer->second;
    cluster  possible_new_cluster;


    for(auto it = blocks.begin() ; it < blocks.end() ; it++)
    {
        clique_block_location c = (it)->get_logical_location();
        std::list<std::size_t> points_val = (it)->get_points();
    }

   // std::cout<<"\n\nData In Clique\n\n";


    // Processing deleted points below this comment

    Delete_dict(dict);
    //std::cout <<"number of points to be deleted in dict: "<<dict.size()<<"\n";

    const char delim = ',';

    for (auto del_point = dict.begin() ; del_point != dict.end() ; del_point++)
    {

        std::vector<std::string> out;      // Stores the string value of each dimension for the recreated deleted point
        clique_block_location c_lof_loc;   // Stores the logical address of a clique block for the del point
        std::size_t logical_grid_val;      // Stores the logical value of a particular dimension
        c_lof_loc.clear();
        int i = 0;

        point_recreate(del_point->first, delim, out);

        // Calculating the logical grid location value to which the point belong
        for(auto point_itr = out.begin(); point_itr < out.end() ; point_itr++, i++)
        {
            //calculating the each dimension block value
            std::string val = *point_itr;

            logical_grid_val = floor(( std::stof(val) - m_min_corner[i]) / (m_sizes[i] / intervals));
           // std::cout<<"Original Point "<<val<<"  logical value "<<logical_grid_val<<"  ";
            c_lof_loc.push_back(logical_grid_val);
        }
       // std::cout<<"\n";
        // Finding corresponding clique block



        // Matching the logical address
        auto clique_block_index_itr = clique_block_map.find(get_string_logical_location(c_lof_loc));
        clique_block &temp = blocks[clique_block_index_itr->second];


        std::size_t previous_number_of_points, next_number_of_points;
        std::list<std::size_t> block_points = temp.get_points();
        previous_number_of_points = block_points.size();


        clique_block_location c = temp.get_logical_location();

       // std::cout<<"Number of points before deleting point:  "<<previous_number_of_points<<"\n";

        //Deleting from the unit block
        temp.delete_point(del_point->second);

        // Deleting from the Existing cluster
        std::size_t del_elem = del_point->second;
        for(auto c = clusters.begin() ; c != clusters.end() ; c++)
        {

            auto it = std::find((*c).begin(), (*c).end(), del_elem);
            if(it != (*c).end())
            {
                (*c).erase(it);
                //std::cout<<"\n\nDeleted from the clusters\n\n";
            }

        }



       // std::cout<<"deleting: "<<del_point->second<<"\n";

        block_points = temp.get_points();


        //std::cout<<"Number of points after deleted point:  "<<block_points.size()<<"\n";
        next_number_of_points = block_points.size();

        if (previous_number_of_points > threshold && next_number_of_points <= threshold)
        {
            std::vector<clique_block_location>  block_neighbors;
            block_neighbors.clear();

            // Getting neighbour of the block unit where the points have been deleted
            temp.get_location_neighbors(intervals, block_neighbors);

            int count_dense_cells = 0;
            for(auto neighbor : block_neighbors)
            {
                std::string log_loc = get_string_logical_location(neighbor);
                auto block_index_itr = clique_block_map.find(log_loc);
                clique_block &temp = blocks[block_index_itr->second];
                std::list<std::size_t> points_inside_cell = temp.get_points();
                if (points_inside_cell.size() > threshold)
                {
                    count_dense_cells = count_dense_cells + 1;
                }
            }

            if (count_dense_cells == 0)
            {
                // Since it is converted from dense to non-dense so now it has to just delete the
                // entire cluster matching it one of its point
                for(auto c = clusters.begin() ; c != clusters.end() ; c++)
                {

                    std::vector<std::size_t> cluster_list = *c;

                    std::size_t first_elem =  block_points.front(); // stores the first point of the stored cluster

                    // Each cluster ko access kar raha hu
                    //std::cout<<"Element to be found   "<<first_elem<<"\n";
                    if (std::find(cluster_list.cbegin(), cluster_list.cend(), first_elem) != cluster_list.cend())
                    {
                        //(*c) is to be deleted from cluster lists
                        c = clusters.erase(c);
                        break;

                    }
                }


            }
            else if(count_dense_cells == 1)
            {
                // Since it has once cluster present so what it will do is it will delete the points from the cluster
                // which belong to this cell
                for(auto c = clusters.begin() ; c != clusters.end() ; c++)
                {

                    std::vector<std::size_t> cluster_list = *c;

                    std::size_t first_elem =  block_points.front(); // stores the first point of the stored cluster

                    // Each cluster ko access kar raha hu
                    //std::cout<<"Element to be found   "<<first_elem<<"\n";
                    if (std::find(cluster_list.cbegin(), cluster_list.cend(), first_elem) != cluster_list.cend())
                    {
                        for(std::size_t del_index : block_points)
                        {
                            auto it = std::find((*c).begin(), (*c).end(), del_index);
                            if(it != (*c).end())
                                (*c).erase(it);
                        }
                    }

                }


            }
            else
            {
                // Apply DFS
                // Applying DFS in Iterative way


                // First Delete the entire cluster
                for(auto c = clusters.begin() ; c != clusters.end() ; c++)
                {

                    std::vector<std::size_t> cluster_list = *c;

                    std::size_t first_elem =  block_points.front(); // stores the first point of the stored cluster

                    // Each cluster ko access kar raha hu
                    if (std::find(cluster_list.cbegin(), cluster_list.cend(), first_elem) != cluster_list.cend())
                    {
                        //(*c) is to be deleted from cluster lists
                        c = clusters.erase(c);
                        break;

                    }
                }

                // DFS Implementation for the nodes
                std::size_t cur_dfs_val = temp.is_visited();
                cur_dfs_val++;
                temp.put_is_visited(cur_dfs_val);


                std::vector<clique_block_location>  non_dense_cell_neighbors;

                temp.get_location_neighbors(intervals, non_dense_cell_neighbors);

                for(auto neighbor : non_dense_cell_neighbors)
                {
                    cluster new_possible_cluster;
                    std::string log_loc = get_string_logical_location(neighbor);
                    auto block_index_itr = clique_block_map.find(log_loc);
                    clique_block & instant_neighbor = blocks[block_index_itr->second];
                    std::list<std::size_t> instant_neighbor_points = instant_neighbor.get_points();
                    if (instant_neighbor_points.size() > threshold)
                    {
                        // DFS Search from node instant neighbor

                        std::stack<clique_block> stk;
                        stk.push(instant_neighbor);
                        //std::cout<< std::endl<< std::endl <<"Processing "<<instant_neighbor.get_logical_location_str()<<"\n\n";
                        while(!stk.empty())
                        {


                            // Removing element from stack
                            clique_block cur_cell = stk.top();
                            stk.pop();

                            std::string t = cur_cell.get_logical_location_str();
                           // std::cout<<" Process Neighbor : "<<t<<"\n";
                            if (cur_cell.is_visited() >= cur_dfs_val)
                            {
                                continue;
                            }
                            std::list<std::size_t> cell_points = cur_cell.get_points();
                            cur_cell.put_is_visited(cur_dfs_val);
                            if (cell_points.size() > threshold)
                            {
                                //process this node
                                // add its points in cluster
                                for(std::size_t index : cell_points)
                                {
                                    new_possible_cluster.push_back(std::size_t(index));
                                }


                                //add its neighbor
                                std::vector<clique_block_location> cur_cell_neighbors;
                                cur_cell.get_location_neighbors(intervals, cur_cell_neighbors);
                                for(auto cur_neighbor : cur_cell_neighbors)
                                {
                                    auto block_index_itr = clique_block_map.find(get_string_logical_location(cur_neighbor));
                                    stk.push(blocks[block_index_itr->second]);
                                }

                            }
                            else
                            {
                                continue;
                            }
                        }
                        if(new_possible_cluster.size() > 0)
                        {
                            clusters.push_back(new_possible_cluster);
                        }

                    }
                }
            }
        }

    }


    std::cout << "\nFinal Results: \n";
    std::cout << "Amount of clusters: " << clusters.size() << std::endl;
    std::cout << "Amount of outliers: " << outliers.size() << std::endl;

    auto finish = std::chrono::high_resolution_clock::now();


    auto duration=std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
    std::cout << "Elapsed time : "<< (duration.count())<< " microsec"<<std::endl<<std::endl<<std::endl;

    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    std::cout<<"RAM used :"<<virtualMemUsedByMe/(1024)<<" KB"<<std::endl;




    return 0;
}
