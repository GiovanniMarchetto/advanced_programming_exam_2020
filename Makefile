# Compile and link options
	# Name for final executable file
	TARGET_EXEC = final_program.x

	# Compile options
	CXX = c++
	CXX_FLAGS = -Wall -Wextra -g -std=c++17

	# Define target directory
	BUILD_DIR = bin

	# Define source directories where to find source files
	SRC_DIRS = ./src ./test ./include


# Documentation options
	# Folder with everything about documentation
	DOC_DIR = Documentation
	# Init file with parameters for documentation
	DOC_INIT_FILE = $(DOC_DIR)/doxy.in
	# Output folder with generated documentation
	DOC_OUT_DIR = $(DOC_DIR)/out
	# Desired name for the output PDF file containing the documentation
	DOC_OUT_FILE_NAME = BST_documentation.pdf


###############################################################################
## END of configurable parameters											 ##
## --------------------- DO NOT MODIFY UNDER THIS LINE --------------------- ##
###############################################################################




# Find all C++ files we want to compile
SRCS = $(shell find $(SRC_DIRS) -maxdepth 1 -path "*.cpp")

# String substitution for every C++ file
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# String substitution for every object file
EXECS = $(OBJS:.o=.x)

# Every folder containing source files will need to be passed to GCC so that it can find header files
    # Now include all required files, without searching in excluded dirs
    INC_DIRS = $(shell find $(SRC_DIRS) -type d)
    # Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
    INC_FLAGS = $(addprefix -I,$(INC_DIRS))

# Final build step
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXX_FLAGS)

# Build step for C++ source
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(INC_FLAGS) $(CXX_FLAGS) -c $< -o $@


.PHONY: all
all: clean build run

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: build
build: build_tmp documentation
.PHONY: build_tmp
build:
	make

.PHONY: run
run:
	$(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: valgrind
valgrind:
	valgrind $(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: all_with_valgrind
all_with_valgrind: clean build valgrind

.PHONY: allv
allv: all_with_valgrind


.PHONY: documentation
documentation:
	# Remove old doxygen folder if it exists
	rm -rf $(DOC_DIR)
	# Create new doxygen folder if it does not exist
	mkdir $(DOC_DIR)
	# Generate init file for Doxygen
	doxygen -g $(DOC_INIT_FILE)
	# Append configuration options to doxygen init file (see Doxygen manual)
	@echo 'PROJECT_NAME = "Binary Search Tree"' >> $(DOC_INIT_FILE)
	@echo 'OUTPUT_DIRECTORY = "$(DOC_OUT_DIR)"' >> $(DOC_INIT_FILE)
	@echo 'INPUT = "."' >> $(DOC_INIT_FILE)
	@echo 'RECURSIVE = YES' >> $(DOC_INIT_FILE)
	@echo 'EXTRACT_ALL = YES' >> $(DOC_INIT_FILE)
	@echo 'EXTRACT_PRIVATE = YES' >> $(DOC_INIT_FILE)
	@echo 'EXTRACT_PACKAGE = YES' >> $(DOC_INIT_FILE)
	@echo 'EXTRACT_STATIC = YES' >> $(DOC_INIT_FILE)
	@echo 'EXTRACT_LOCAL_CLASSES = YES' >> $(DOC_INIT_FILE)
	@echo 'EXTRACT_LOCAL_METHODS = YES' >> $(DOC_INIT_FILE)
	# Create folder where to save the generated documentation
	mkdir $(DOC_OUT_DIR)
	# Generate documentation
	doxygen $(DOC_INIT_FILE)
	# Invoke doxygen makefile to create the PDF file with the documentation
	make --directory=$(DOC_OUT_DIR)/latex
	# Rename the file containing the documentation
	mv $(DOC_OUT_DIR)/latex/refman.pdf $(DOC_OUT_DIR)/latex/$(DOC_OUT_FILE_NAME)
	# Copy the pdf file with the documentation in the proper directories
	cp -i $(DOC_OUT_DIR)/latex/$(DOC_OUT_FILE_NAME) $(DOC_OUT_DIR)
	cp -i $(DOC_OUT_DIR)/latex/$(DOC_OUT_FILE_NAME) $(DOC_DIR)