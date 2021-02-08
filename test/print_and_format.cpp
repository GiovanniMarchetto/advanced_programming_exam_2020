#include <iostream>
#include <string>
#include <vector>
#include <regex> // regex used for substring replacement

#include "bst.h"

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec)
{
    os << "\n";
    for (size_t i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    os << std::endl;
    return os;
}
void print_tree_from_iterator(bst<char>::iterator &iterator, bst<char> &bst)
{
    while (iterator != bst.end())
    {
        std::cout << *iterator << " ";
        ++iterator;
    }
    std::cout << std::endl;
}
void print_tree_from_iterator(bst<char>::const_iterator &iterator, const bst<char> &bst)
{
    while (iterator != bst.cend())
    {
        std::cout << *iterator << " ";
        ++iterator;
    }
    std::cout << std::endl;
}

std::string formatting_title(std::string title,const bool center)
{
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
                title.append("\n" + indentation + str_buff.substr(0, number_of_chars_to_insert));
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
