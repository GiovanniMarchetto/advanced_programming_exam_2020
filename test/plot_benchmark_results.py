#!usr/bin/env python3

import sys  # reading params from command line
from statistics import mean
import matplotlib.pyplot as plt
import re  # Regex for extracting measure unit from data
import os  # files
import csv  # read from csv

# PARAMETERS READ FROM COMMAND LINE
# When invoking this script, following parameters must be set from command line
BENCHMARK_DIR_PATH = str(sys.argv[1])  # path of dir containing csv files
DPI = int(sys.argv[2])  # DPI for plots
FORMAT = str(sys.argv[3])  # format for plots


# Alternative: set parameters manually from the following lines
# BENCHMARK_DIR_PATH = "../benchmark_results" # path of dir containing csv files
# DPI = 300                                   # DPI for plots
# FORMAT =  "png"                             # format for plots
# END PARAMETERS

# %% Getting data from the dataframe
def get_data(df_, field_names_):
    """Given the dataframe and the list with its field names,
    a tuple with six elements is returned: those six elements
    are respectively: the label for the abscissa and the two
    labels for the y-values, then there are the lists containing
    the corresponding values for those labels."""

    # Header names
    size_n_ = field_names_[0]  # names of the header for the size of the bst
    t1_n_ = field_names_[1]  # time for the first container in the comparison
    try:
        t2_n_ = field_names_[2]  # time for the second container in the comparison
    except ValueError:
        pass  # ignore if the second y-values series is not present

    # Grouping by bst size
    df_grouped_by_size = {int(row[size_n_]): [] for row in df_}
    for row in df_:
        df_grouped_by_size[int(row[size_n_])].append(row)

    # Computing averages
    size__ = [n for n in df_grouped_by_size.keys()]  # list of sizes
    t_container1_ = []  # mean time referring to the 1st container
    t_container2_ = []  # mean time referring to the 1st container

    for n in size__:
        values_this_group = [int(v[t1_n_]) for v in list(df_grouped_by_size.values())[n - 1]]
        # Max and min values are removed
        values_this_group.remove(min(values_this_group))
        values_this_group.remove(max(values_this_group))
        t_container1_.append(mean(values_this_group))  # size__ start from 1 but index starts from 0

    try:
        for n in size__:
            values_this_group = [int(v[t2_n_]) for v in list(df_grouped_by_size.values())[n - 1]]
            # Max and min values are removed
            values_this_group.remove(min(values_this_group))
            values_this_group.remove(max(values_this_group))
            t_container2_.append(mean(values_this_group))  # size__ start from 1 but index starts from 0
    except Exception:
        pass  # ignore if the second y-values series is not present

    # print(size__)
    # print(t_container1_)
    # print(t_container2_)

    return size_n_, t1_n_, t2_n_, size__, t_container1_, t_container2_


# Plotting results

def draw_plot(size__, t_container1_, t_container2_, size_n_, t1_n_, t2_n_,
              title_plot_, output_dir, dpi=300, format_="png"):
    """This function creates a plot with data obtained from parameters.
    The first parameter represents the abscissa, the second and the third
    represent the y-values which will be compared, then size_n, t1_n and t2_n
    are the labels for the abscissa and the two y-values to be compared,
    respectively. Furthermore, the user can specify the title for the plot and
    the output directory where to save plots. Further parameters allow to set
    the resolution and the output format. The plot will be saved and the
    filename containing the plot will be the same as the title, but spaces
    will be replaced with underscores."""

    measure_unit = re.search(r'\[(.+)]', t1_n_).group(0)  # extracts contents between [ ], brackets included

    plt.grid()
    plt.plot(size__, t_container1_, label=t1_n_.replace(measure_unit, ''))
    try:
        plt.plot(size__, t_container2_, label=t2_n_.replace(measure_unit, ''))
    except ValueError:
        t_container2_ = [0]  # ignore if the second y-values series is not present

    plt.ylabel(f"Time {measure_unit}")
    plt.xlabel(size_n_.capitalize())
    plt.legend()
    plt.title(title_plot_)

    percentage_step_x_axis = 10
    plt.xticks([int(x) for x in size__ if
                (x * percentage_step_x_axis % len(size__)) == 0])  # range with step percentage_step_x_axis%
    plt.xlim(size__[0], size__[-1])
    plt.ylim(0, max(max(t_container1_), max(t_container2_)))
    # plt.show()
    plt.savefig(os.path.join(output_dir, title_plot_.replace(" ", "_") + "." + format_), dpi=dpi)  # save in png format
    plt.close()


# Main part

files_in_dir = [f for f in os.listdir(BENCHMARK_DIR_PATH) if os.path.isfile(os.path.join(BENCHMARK_DIR_PATH, f))]

for file_path in files_in_dir:
    file_path = os.path.join(BENCHMARK_DIR_PATH, file_path)
    with open(file_path) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        field_names = reader.fieldnames
        df = [row for row in reader]  # data saved into a dataframe

    size_n, t1_n, t2_n, size_, t_container1, t_container2 = get_data(df, field_names)

    title_plot = os.path.splitext(os.path.basename(file_path))[0].replace("_", " ").capitalize()  # Title of the plot is extracted from the filename
    draw_plot(size_, t_container1, t_container2, size_n, t1_n, t2_n,
              title_plot, BENCHMARK_DIR_PATH, DPI, FORMAT)
