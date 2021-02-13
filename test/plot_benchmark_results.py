#!usr/bin/env python3

import sys   # reading params from command line


### PARAMETERS READ FROM COMMAND LINES ####
# When invoking this script, following parameters must be set from command line
BENCHMARK_DIR_PATH = str(sys.argv[1])  # path of dir containing csv files
DPI                = int(sys.argv[2])  # DPI for plots
FORMAT             = str(sys.argv[3])  # format for plots


# Alternative: set parameters manually from the following lines
# BENCHMARK_DIR_PATH = "../benchmark_results" # path of dir containing csv files
# DPI = 300                                   # DPI for plots
# FORMAT =  "png"                             # format for plots
### END PARAMETERS ###

# %% Getting data from the dataframe
from statistics import mean

def get_data(df, field_names):
    """Given the dataframe and the list with its field names,
    a tuple with six elements is returned: those six elements
    are respectively: the label for the abscissa and the two
    labels for the y-values, then there are the lists containing
    the corresponding values for those labels."""

    # Header names
    size_n = field_names[0] # names of the header for the size of the bst
    t1_n   = field_names[1] # time for the first container in the comparison
    t2_n   = field_names[2] # time for the second container in the comparison


    # Grouping by bst size
    df_grouped_by_size = {int(row[size_n]):[] for row in df}
    for row in df :
        df_grouped_by_size[int(row[size_n])].append(row)
    
    # Computing averages
    nodes = [n for n in df_grouped_by_size.keys()]  # list of nodes
    t_container1 = []                               # mean time referring to the 1st container
    t_container2 = []                               # mean time referring to the 1st container
    
    for n in nodes:
        t_container1.append(mean([int(v[t1_n]) for v in list(df_grouped_by_size.values())[n-1]]))  # nodes start from 1 but index starts from 0
        t_container2.append(mean([int(v[t2_n]) for v in list(df_grouped_by_size.values())[n-1]]))  # nodes start from 1 but index starts from 0

    # print(nodes)
    # print(t_container1)
    # print(t_container2)

    return size_n, t1_n, t2_n, nodes, t_container1, t_container2
    


# %% Plotting results

import matplotlib.pyplot as plt
import re   # Regex for extracting measure unit from data

def draw_plot(nodes, t_container1, t_container2, size_n, t1_n, t2_n,
              title_plot, output_dir, dpi=300, format="png"):
    """This function creates a plot with data obtained from parameters.
    The first parameter represents the abscissa, the second and the third
    represent the y-values which will be compared, then size_n, t1_n and t2_n
    are the labels for the abscissa and the two y-values to be compared,
    respectively. Furthermore, the user can specify the title for the plot and
    the output directory where to save plots. Further parameters allow to set
    the resolution and the output format. The plot will be saved and the
    filename containing the plot will be the same as the title, but spaces
    will be replaced with underscores."""

    measure_unit = re.search('\[(.+)\]',t1_n).group(0) # extracts contents between [ ], brackets included

    plt.grid()
    plt.plot(nodes, t_container1, label=t1_n.replace(measure_unit,''))
    plt.plot(nodes, t_container2, label=t2_n.replace(measure_unit,''))
    plt.ylabel(f"Time {measure_unit}")
    plt.xlabel(size_n.capitalize())
    plt.legend()
    plt.title(title_plot)
    plt.xticks(nodes)    # only integer values on x axis
    plt.xlim(nodes[0], nodes[-1])
    plt.ylim(0, max(max(t_container1), max(t_container2)))
    #plt.show()
    plt.savefig(output_dir+"/"+title_plot.replace(" ", "_")+"."+format, dpi=dpi)    # save in png format
    plt.close()


# %% Main module
import os   # files
import csv  # read from csv

files_in_dir = [f for f in os.listdir(BENCHMARK_DIR_PATH) if os.path.isfile(os.path.join(BENCHMARK_DIR_PATH, f))]

for file_path in files_in_dir:
    file_path = os.path.join(BENCHMARK_DIR_PATH, file_path)
    with open(file_path) as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        
        field_names = reader.fieldnames
        df = [row for row in reader]   # data saved into a dataframe

        try:
            size_n, t1_n, t2_n, nodes, t_container1, t_container2 = get_data(df, field_names)

            title_plot = os.path.splitext(os.path.basename(file_path))[0].replace("_"," ").capitalize() # Title of the plot is extracted from the filename
            draw_plot(nodes, t_container1, t_container2, size_n, t1_n, t2_n,
                      title_plot, BENCHMARK_DIR_PATH, DPI, FORMAT)
        except Exception:
            pass    # exception can raise if csv file is ill-formed: it will be ignored

