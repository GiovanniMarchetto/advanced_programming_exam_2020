
// --------  CONFIGURABLE PARAMETERS FOR TESTS         --------

/** Number of iterations to run the codes of test regarding the nodes.*/
constexpr int NUMBER_OF_NODES{ 20 };

/** Number of iterations to run the codes of test 
 * regarding the insertion of the nodes in bst. 
 * Determine the size of the tree.*/
constexpr int NUMBER_OF_NODES_BST{ 50 };

/** Limit for the value of the random key of the nodes.
 * If (MAX_NUMBER_OF_KEY < NUMBER_OF_NODES) 
 * --> test behaviour with duplicated keys.
*/
constexpr int MAX_NUMBER_OF_KEY{ 15 };

/** Number of iterations of the nodes test 
 * that are print in cout. 
 * If it is bigger or equal than the NUMBER_OF_NODES, 
 * all iterations are printed.*/
constexpr int NUMBER_OF_PRINT{ 2 };

// --------  END OF CONFIGURABLE PARAMETERS FOR TESTS  --------

/** Performs all tests about nodes and bst and prints results.*/
void tests();