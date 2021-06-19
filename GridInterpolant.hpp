/**
 * @file GridInterpolant.hpp
 * This header file contains a class GridInterpolant that performs linear interpolation on N-D 
 * gridded data sets. It provides similar functionality compared to MATLAB's griddedInterpolant
 * or SciPy's RegularGridInterpolator.
 *
 * @author Josh Cook
 *
**/
#include <vector>
#include <iostream>
#include <algorithm>

// -------------------------------------------------------------------
// GridInterpolant class
// -------------------------------------------------------------------
// GridInterpolant performs linear interpolation on N-D gridded data sets.
// It provides similar functionality compared to MATLAB's griddedInterpolant
// or SciPy's RegularGridInterpolator.
// 
// inputGrid: defines the unique grid points on each axis
// outputvalues: contains the vector of outputs for all combinations of grid points and must
//               be in column major format 
//
// An example of how to construct the GridInterpolant and how to evaluate it are provided below:
// 
// std::vector<std::vector<double>> inputGrid = {{0, 1, 2, 3, 4, 5},
//                                               {6, 7, 8, 9, 10}};
//  
//  std::vector<double> outputValues = {18, 19, 22, 27, 34, 43, 21, 22, 25,
//                                      30, 37, 46, 24, 25, 28, 33, 40, 49,
//                                      27, 28, 31, 36, 43, 52, 30, 31, 34,
//                                      39, 46, 55};
//
//  GridInterpolant gridInterpolant(inputGrid, outputValues);
//  std::vector<double> outputVal = gridInterpolant.eval({0.5, 9.9});

class GridInterpolant{
  public:

    // Constructor
    GridInterpolant(std::vector< std::vector<double> > inputGrid, std::vector<double> outputValues);

    // Evaluate the interpolant object
    std::vector<double> eval(std::vector<double> point);

    // Build an ndgrid (MATLAB) or meshgrid using 'ij' indexing and column major format (Python)
    static std::vector<std::vector<double>> meshgrid(std::vector<std::vector<double>> inputGrid);

  private:

    // grid of unique input points in each dimension
    std::vector< std::vector<double> > inputGrid;

    // output values in column-major format
    std::vector<double> outputValues;

    // stack inputGrid into a single vector
    std::vector<double> stackedGrid;

    // offset from 0 to the first element of each dimension
    // in inputGrid inside stackedGrid. For the example in the class docstring 
    // std::vector<int> offset = {0, 6, 11}
    std::vector<int> offset;
    
    // Number of values in outputValues
    int numberOfElements;

    // The number of input dimensions. This is equal to inputGrid.size()
    int inputDimension;

    // The number of output dimensions. Right now only 1 is supported for each
    // GridInterpolant Option
    int outputDimension;

    // stacks inputGrid in order into stackedGrid
    void stackGrid();

    // For each input dimension, determine which index for the interval and the normalized weight for 
    // how far the point lies in the interval
    void calculateWeights(std::vector<double> x, std::vector<int> & leftIndex, std::vector<double> & alpha);

    // Get the index for the interval for a single input dimension
    int getLowIndex(double xi, std::vector<double> subGrid, int subGridSize, int inputNumber);

    // Add the corner contribution on the grid segment to the interpolated value
    void addCornerContribution(std::vector<int> & leftIndex, std::vector<double> & alpha,
        std::vector<int> & corner, std::vector<double> & coefficient, std::vector<double> & result);

    // Move to next corner on the grid segment to determine its contribution to the interpolated value
    bool switchCorner(std::vector<int> & corner);
};


// class Constructor
GridInterpolant::GridInterpolant(std::vector< std::vector<double> > inputGrid,
					 std::vector<double> outputValues): inputGrid(inputGrid),
									    outputValues(outputValues),
									    stackedGrid(),
									    numberOfElements(1),
									    inputDimension(inputGrid.size()),
									    outputDimension(1)
									    
{
  // Compute number of elements on the grid
  for(std::vector<double> inputGridTics : this->inputGrid){
    this->numberOfElements *= inputGridTics.size();
  }

  // Check dimension consistency
  bool isConsistent = ((this->outputValues.size() % this->numberOfElements) == 0);
  if(!isConsistent){
    throw "The input grid and output values provided have in consistent dimensions.";
  }

  //Stack grid
  this->stackGrid();

  // Check input and output dimensions
  bool isOutputDimensionConsistent = this->outputDimension ==
    this->outputValues.size() / this->numberOfElements;
  if (!isOutputDimensionConsistent){
    throw "The input dimension is not consistent with the provided grid";
  }
  
  bool isInputDimensionConsistent = this->inputDimension == (this->offset.size() - 1);
  if (!isOutputDimensionConsistent){
    throw "The input dimension is not consistent with the provided grid";
  }
}


// Evaluate the interpolant
std::vector<double> GridInterpolant::eval(std::vector<double> x){
  // TO DO: add check that vector is right length
  std::vector<int> leftIndex(inputDimension);
  std::vector<int> corner(inputDimension);
  std::vector<double> alpha(inputDimension);
  std::vector<double> coefficient(inputDimension);
  std::vector<double> result(outputDimension);

  this->calculateWeights(x, leftIndex, alpha);
  while(true){
    this->addCornerContribution(leftIndex, alpha, corner, coefficient, result);
    if (this->switchCorner(corner) == false){
      break;
    }
  }
  return result;
}


// Add the corner contribution on the grid segment to the interpolated value
void GridInterpolant::addCornerContribution(std::vector<int> & leftIndex, std::vector<double> & alpha,
				   std::vector<int> & corner, std::vector<double> & coefficient,
				   std::vector<double> & result){
  double cornerCoefficient = 1;
  int leadDimension = 1;
  int valuesIndex = 0;
  for(int ii = 0; ii < this->inputDimension; ii++){
    coefficient.at(ii) = cornerCoefficient;
    if(corner.at(ii) == 1){
      cornerCoefficient *= alpha.at(ii);
    } else {
      cornerCoefficient *= 1 - alpha.at(ii);
    }
    valuesIndex += (leftIndex.at(ii) + corner.at(ii)) * leadDimension * this->outputDimension;
    leadDimension *= this->offset.at(ii + 1) - this->offset.at(ii);
  }

  for(int ii = 0; ii < outputDimension; ii++){
    result.at(ii) += cornerCoefficient * this->outputValues.at(valuesIndex + ii);
  }
}


// For each input dimension, determine which index for the interval and the normalized weight for 
// how far the point lies in the interval
void GridInterpolant::calculateWeights(std::vector<double> x, std::vector<int> & leftIndex, std::vector<double> & alpha){
  for(int ii = 0; ii < this->inputDimension; ii++){
    double xi = x.at(ii);
    int stackedGridEntryIndex = this->offset.at(ii);
    int subGridSize = this->offset.at(ii+1) - this->offset.at(ii);
    std::vector<double> subGrid(this->stackedGrid.begin() + stackedGridEntryIndex,
				this->stackedGrid.begin() + stackedGridEntryIndex + subGridSize);
    int jj = getLowIndex(xi, subGrid, subGridSize, ii);
    leftIndex.at(ii) = jj;
    alpha.at(ii) = (xi - subGrid.at(jj)) / (subGrid.at(jj+1) - subGrid.at(jj));
  }
}


// Linear search for finding the low index on a given dimension
int GridInterpolant::getLowIndex(double xi, std::vector<double> subGrid, int subGridSize, int inputNumber){
  int ii;
  for(ii = 0; ii < (subGridSize - 2); ii++) {
    if(xi < subGrid.at(ii + 1)){
      break;
    }
  }
  return ii;
}


// switch to the next corner on the grid segment
bool GridInterpolant::switchCorner(std::vector<int> & corner){
  for(int ii = 0; ii < this->inputDimension; ii++){
    if(corner.at(ii) == 1){
      corner.at(ii) = 0;
    } else {
      corner.at(ii) = 1;
      return true;
    }
  }
  return false;
}


// stack input grid into a single vector stacked grid and compute offsets
void GridInterpolant::stackGrid(){
  offset.clear();
  offset.push_back(0);
  for(std::vector<double> subGrid : inputGrid){
    offset.push_back( offset.back() + subGrid.size() );
  }

  stackedGrid.clear();
  for(std::vector<double> subGrid: inputGrid){
    stackedGrid.insert(stackedGrid.end(), subGrid.begin(), subGrid.end());
  }
}

// method similar to Python's meshgrid using column major format or MATLAB's ndgrid
std::vector<std::vector<double>> GridInterpolant::meshgrid(std::vector<std::vector<double>> inputGrid){

  // Get the size of the grid on input dimension
  int numberOfElements = 1;
  std::vector<double> inputGridSizes = {};
  for(std::vector<double> grid : inputGrid){
     inputGridSizes.push_back(grid.size());
     numberOfElements *= grid.size();
  }
  int numberOfInputDimensions = inputGrid.size();

  std::vector<std::vector<double>> mesh;
  for(int ii = numberOfInputDimensions-1; ii > -1;  ii--){
    // Product of remaining dimensions
    int backwardProduct = 1;
    for(int jj = ii-1; jj > -1; jj--){
      backwardProduct *= inputGrid.at(jj).size();
    }

    // create mesh along input dimension
    std::vector<double> meshdim;
    for(int kk = 0; kk < inputGridSizes.at(ii); kk++){
      for(int mm = 0; mm < backwardProduct; mm++){
        meshdim.push_back( inputGrid.at(ii).at(kk) );
      }
    }
    mesh.push_back(meshdim);

    // replicate vector approprate amount along each input dimension
    int numberOfReplicates = numberOfElements/mesh.back().size() - 1;
    std::vector<double> tempMeshCopy = mesh.back();
    for(int mm = 0; mm < numberOfReplicates; mm++){
      mesh.back().insert(mesh.back().end(), tempMeshCopy.begin(), tempMeshCopy.end());
    }
  }
  // Return the meshgrid in the order it was recieved
  std::reverse(mesh.begin(), mesh.end());
  return mesh;
}