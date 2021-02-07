#include "benchmark_test.h"

/** Simple utitilty function to properly format the titles of tests in an
 * uniform way.*/
std::string formatting_title(std::string title = "", bool center = false)
{
    // TODO : to be optimized (use std::move?)

    // Eventually segment the given title onto more lines
    constexpr int MAX_CHARS_OF_TITLE_IN_A_LINE{60};                     // param
    constexpr int INDENTATION_OF_FOLLOWING_LINES_IF_TITLE_TOO_LONG{15}; // param
    constexpr int MAX_CHARS_OF_TITLE_IN_FOLLOWING_LINES{
        MAX_CHARS_OF_TITLE_IN_A_LINE - INDENTATION_OF_FOLLOWING_LINES_IF_TITLE_TOO_LONG};

    if (title == "")
        title = std::string(MAX_CHARS_OF_TITLE_IN_A_LINE, '-');

    if (center) // title must be centered
    {
        int remaining_padding{MAX_CHARS_OF_TITLE_IN_A_LINE -
                              static_cast<int>(title.length())};
        if (remaining_padding)
        {
            // Add spaces to center the title
            std::string padding_for_centering(remaining_padding / 2, ' ');
            title = padding_for_centering + title + padding_for_centering;
        }
    }

    const std::string title_delimiter{"--------------"};
    int number_of_chars_to_insert{static_cast<int>(title.length())};

    bool entire_title_in_one_line{true};

    if (title.length() > MAX_CHARS_OF_TITLE_IN_A_LINE)
    {
        entire_title_in_one_line = false;

        const std::string indentation(INDENTATION_OF_FOLLOWING_LINES_IF_TITLE_TOO_LONG, ' ');
        std::string str_buff{title.substr(MAX_CHARS_OF_TITLE_IN_A_LINE)};
        title = title.substr(0, MAX_CHARS_OF_TITLE_IN_A_LINE);

        while (str_buff.length() > 0)
        {
            number_of_chars_to_insert =
                static_cast<int>(MAX_CHARS_OF_TITLE_IN_FOLLOWING_LINES >= str_buff.length()
                                     ? str_buff.length()
                                     : MAX_CHARS_OF_TITLE_IN_FOLLOWING_LINES);

            if (number_of_chars_to_insert > 0)
                title.append("\n" + indentation + str_buff.substr(0, number_of_chars_to_insert)); // TODO : substring with move?
            str_buff = str_buff.substr(number_of_chars_to_insert);
        }
    }

    title = std::regex_replace(title, std::regex("\n"), title_delimiter + "\n" + title_delimiter);

    {
        // add spaces between and of the string and start of title
        // delimiter in order to have alignment over different lines
        int number_of_spaces_to_add{MAX_CHARS_OF_TITLE_IN_A_LINE - number_of_chars_to_insert};
        if (!entire_title_in_one_line)
            number_of_spaces_to_add -= INDENTATION_OF_FOLLOWING_LINES_IF_TITLE_TOO_LONG; // subtract the indentation for lines following the first

        if (number_of_spaces_to_add > 0)
        {
            std::string spaces_to_add = std::string(number_of_spaces_to_add, ' ');
            title.append(spaces_to_add);
        }
    }

    return title_delimiter + title + title_delimiter;
}

void benchmark_test(bool to_file)
{

    if( to_file )   // results will be print to a file
    {
        // Remove the directory containing old results (if it exists)
        std::filesystem::remove_all(OUTPUT_RESULTS_DIR);

        // Create dir for results
        std::filesystem::create_directory(OUTPUT_RESULTS_DIR);    // TODO : permission of the directory??
    }
    
    
    std::ofstream f; // fstream just declared in this scope

    /** Returns the output stream: if to_file is true, then the output stream 
     * to the file with the name given as argument will be set. If the file
     * does not exist, it will be created, otherwise it will be replaced.
     * @param filename The string with the name of the file wher to save the
     *                  results, withou neither the full path nor the file
     *                  extension.*/
    auto get_ostream = [to_file, &f] (const std::string &filename) -> std::ostream& {
        
        if(!to_file)
            return std::cout;

        // Otherwis, write to file
        std::ofstream tmp{OUTPUT_RESULTS_DIR + "/" + filename + ".csv"};    //previous content will be overwritten
        f = std::move(tmp);
        // Note: fstream will be closed by the dctor
        return f;
    };


    Benchmark_test<char> benchmark_test_{};
    std::cout << std::endl;

    std::cout << formatting_title() << "\n";
    std::cout << formatting_title("##### BENCHMARK TESTS ####", true) << "\n";
    std::cout << formatting_title() << "\n";
    std::cout << NUMBER_OF_ITERATIONS << " iterations will be executed for each test.\n";
    if( to_file )
        std::cout << "Results will be written to files in the directory " +
                     OUTPUT_RESULTS_DIR + "\n";

    std::cout << formatting_title("## Insertion  ") << "\n";
    benchmark_test_.insertion_test(get_ostream("insertion"));
    std::cout << "\n";

    std::cout << formatting_title("## Finding a node by key  ") << "\n";
    benchmark_test_.find_test(get_ostream("find_by_key"));
    std::cout << "\n";

    std::cout << formatting_title("## Balancing the tree  ") << "\n";
    benchmark_test_.balancing_test(get_ostream("balancing"));
    std::cout << "\n";

    std::cout << formatting_title("## Finding a node by key, after balancing the BST  ") << "\n";
    benchmark_test_.find_test(get_ostream("find_by_key_after_balancing"));
    std::cout << std::endl;

    std::cout << formatting_title("## Copying the BST  ") << "\n";
    benchmark_test_.copy_bst_test(get_ostream("copy"));
    std::cout << std::endl;
}

/** Receives a lambda function as argument and executes it iteratively
 * for NUMBER_OF_ITERATIONS times.*/
template<typename F>
void iterate(F &lambda_fun)
{
    for(size_t i{0}; i<NUMBER_OF_ITERATIONS; ++i)
        lambda_fun();
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::
insertion_test(std::ostream &os,
               const size_t NUMBER_OF_NODES_INSERTION_BENCHMARK)
{

    if (!(std::is_same<key_type, int>::value &&
          std::is_same<value_type, char>::value)) // Check if right template types
    {
        os << "This test is currenty available only for integer key type and char value type";
        return os;
    }

    // Random generation of pairs with distinct keys (temporarily saved in a
    // vector to time only the insertion time)
    auto create_vector_of_nodes = [](const std::size_t &number_of_nodes_to_create, std::vector<std::pair<int, char>> &vec) {
        std::srand(std::time(NULL)); // random seed initialization

        auto find_pair_by_key_in_vector = [](const auto &key, const auto &vector) {
            for (const auto &el : vector)
                if (el.first == key)
                    return true;
            return false;
        };

        while (vec.size() < number_of_nodes_to_create)
        {
            // Random generation of a pair
            int random_key{rand()};
            char random_val{static_cast<char>(rand() % 26 + static_cast<int>('a'))}; // between 'a' and 'z'
            if (!find_pair_by_key_in_vector(random_key, vec))                        // if not already present
                vec.push_back(std::pair<int, char>(random_key, random_val));         // move insert
        }
    };
    std::vector<std::pair<int, char>> vector_of_nodes_to_insert{};
    create_vector_of_nodes(NUMBER_OF_NODES_INSERTION_BENCHMARK, vector_of_nodes_to_insert);

    os << HEADER_FOR_RESULTS;

    auto actual_test = [this, NUMBER_OF_NODES_INSERTION_BENCHMARK, &vector_of_nodes_to_insert, &os]() {
        
        bst_.clear();   // Clear the bst before inserting

        long int duration_insertion_in_our_tree{},
        duration_insertion_in_std_map{};
        for (std::size_t i{0}; i < NUMBER_OF_NODES_INSERTION_BENCHMARK; ++i)
        {
            std::pair<int, char> pair_to_insert{vector_of_nodes_to_insert.at(i)}; // the pair is temporarely saved here just before being inserted

            // Insertion in our tree
            auto t1 = std::chrono::high_resolution_clock::now();
            bst_.insert(pair_to_insert); // pair is copied
            auto t2 = std::chrono::high_resolution_clock::now();
            duration_insertion_in_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

            // Insertion in std::map
            t1 = std::chrono::high_resolution_clock::now();
            map_.insert(pair_to_insert); // pair is copied
            t2 = std::chrono::high_resolution_clock::now();
            duration_insertion_in_std_map += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

            // Printing results for this iteration
            os << "\n" << i+1 << "," << duration_insertion_in_our_tree << "," << duration_insertion_in_std_map;

        }
    };

    iterate(actual_test);

    // os << "\n";
    // os << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into the tree in " << duration_insertion_in_our_tree << " us. ";
    // os << " Average insertion time: " << (duration_insertion_in_our_tree / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us.\n";
    // os << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into an std::map in " << duration_insertion_in_std_map << " us. ";
    // os << " Average insertion time: " << (duration_insertion_in_std_map / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us.";

    return os;
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::find_test(std::ostream &os, const key_t key_to_find)
{

    os << HEADER_FOR_RESULTS;

    auto actual_test = [this, &key_to_find, &os]() {

        long int duration_search_in_our_tree{},
        duration_search_in_std_map{};
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            bst_.find(key_to_find);
            auto t2 = std::chrono::high_resolution_clock::now();
            duration_search_in_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

            t1 = std::chrono::high_resolution_clock::now();
            bst_.find(key_to_find);
            t2 = std::chrono::high_resolution_clock::now();
            duration_search_in_std_map += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        }
    };

    iterate(actual_test);



    // os << "Time for finding a key (" << bst_.get_size() << " nodes in the tree): " << duration_search_in_our_tree << " us.\n";
    // os << "Time for finding a key (" << bst_.get_size() << " nodes in the std::map): " << duration_search_in_std_map << " us."
    //    << std::endl;

    return os;
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::balancing_test(std::ostream &os)
{
    long int duration_balancing_our_tree{};

    auto t1 = std::chrono::high_resolution_clock::now();
    bst_.balance();
    auto t2 = std::chrono::high_resolution_clock::now();

    duration_balancing_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    os << "Time for balancing the tree (" << bst_.get_size() << " nodes in the tree): " << duration_balancing_our_tree << " us." << std::endl;

    return os;
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::copy_bst_test(std::ostream &os)
{
    long int duration_copy_in_our_tree{},
        duration_copy_in_std_map{};

    auto t1 = std::chrono::high_resolution_clock::now();
    bst<value_type, key_type, OP> bst_copy{bst_};
    auto t2 = std::chrono::high_resolution_clock::now();
    duration_copy_in_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    t1 = std::chrono::high_resolution_clock::now();
    std::map<key_type, value_type> map_copy{map_};
    t2 = std::chrono::high_resolution_clock::now();
    duration_copy_in_std_map += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    os << "Time for deep copy of our tree (" << bst_copy.get_size() << " nodes in the tree): " << duration_copy_in_our_tree << " us.\n";
    os << "Time for deep copy of std::map (" << bst_copy.get_size() << " nodes in the std::map): " << duration_copy_in_std_map << " us.\n\n"
       << std::endl;

    return os;
}