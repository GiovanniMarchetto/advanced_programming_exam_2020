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

void benchmark_test()
{
    constexpr size_t NUMBER_OF_NODES{1000};
    Benchmark_test<char> benchmark_test_{};
    std::cout << std::endl;

    std::cout << formatting_title() << std::endl;
    std::cout << formatting_title("##### BENCHMARK TESTS ####", true) << std::endl;
    std::cout << formatting_title() << std::endl;

    std::cout << formatting_title("## Insertion  ") << std::endl;
    benchmark_test_.insertion_test(std::cout, NUMBER_OF_NODES) << std::endl;
    std::cout << std::endl;

    std::cout << formatting_title("## Finding a node by key  ") << std::endl;
    benchmark_test_.find_test(std::cout) << std::endl;
    std::cout << std::endl;

    std::cout << formatting_title("## Balancing the tree  ") << std::endl;
    benchmark_test_.balancing_test(std::cout) << std::endl;
    std::cout << std::endl;

    std::cout << formatting_title("## Finding a node by key, after balancing the BST  ") << std::endl;
    benchmark_test_.find_test(std::cout) << std::endl;
    std::cout << std::endl;

    std::cout << formatting_title("## Copying the BST  ") << std::endl;
    benchmark_test_.copy_bst_test(std::cout) << std::endl;
    std::cout << std::endl;
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::insertion_test(std::ostream &os, const size_t NUMBER_OF_NODES_INSERTION_BENCHMARK)
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
    }

    os << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into the tree in " << duration_insertion_in_our_tree << " us. ";
    os << " Average insertion time: " << (duration_insertion_in_our_tree / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us.\n";
    os << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into an std::map in " << duration_insertion_in_std_map << " us. ";
    os << " Average insertion time: " << (duration_insertion_in_std_map / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us.";

    return os;
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::find_test(std::ostream &os, const key_t key_to_find)
{
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

    os << "Time for finding a key (" << bst_.get_size() << " nodes in the tree): " << duration_search_in_our_tree << " us.\n";
    os << "Time for finding a key (" << bst_.get_size() << " nodes in the std::map): " << duration_search_in_std_map << " us."
       << std::endl;

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