#include <vector>
#include <iostream>


class GridInterpolator{

private:
  std::vector< std::vector<double> > inputGrid;
  std::vector<double> outputValues;
  std::vector<double> stackedGrid;
  std::vector<int> offset;
  
  int numberOfElements;
  int inputDimension;
  int outputDimension;

  
  void stackGrid();

  void calculateWeights(std::vector<double> x, std::vector<int> & leftIndex, std::vector<double> & alpha);
  int getLowIndex(double xi, std::vector<double> subGrid, int subGridSize, int inputNumber);
  void addCornerContribution(std::vector<int> & leftIndex, std::vector<double> & alpha,
		   std::vector<int> & corner, std::vector<double> & coefficient, std::vector<double> & result);
  bool switchCorner(std::vector<int> & corner);

public:
  GridInterpolator(std::vector< std::vector<double> > inputGrid, std::vector<double> outputValues);
  std::vector<double> eval(std::vector<double> point);
};


GridInterpolator::GridInterpolator(std::vector< std::vector<double> > inputGrid,
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
  
  
  std::cout << "Constructor Executed Succesfully" << std::endl;
  std::cout << "Number of Elements is: " << this->numberOfElements << "\n";
}

std::vector<double> GridInterpolator::eval(std::vector<double> x){
  std::vector<int> leftIndex(inputDimension);
  std::vector<int> corner(inputDimension);
  std::vector<double> alpha(inputDimension);
  std::vector<double> coefficient(inputDimension);
  std::vector<double> result(outputDimension);

  std::cout << "Print alpha = { ";
  for(auto & val : alpha){
    std::cout << val << " ";
  }
  std::cout << "} \n";

  std::cout << "Print leftIndex = { ";
  for(auto & val : leftIndex){
    std::cout << val << " ";
  }
  std::cout << "} \n";

  this->calculateWeights(x, leftIndex, alpha);
  while(true){
    this->addCornerContribution(leftIndex, alpha, corner, coefficient, result);
    if (this->switchCorner(corner) == false){
      break;
    }
  }
  return result;
}

void GridInterpolator::addCornerContribution(std::vector<int> & leftIndex, std::vector<double> & alpha,
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

void GridInterpolator::calculateWeights(std::vector<double> x, std::vector<int> & leftIndex, std::vector<double> & alpha){
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

int GridInterpolator::getLowIndex(double xi, std::vector<double> subGrid, int subGridSize, int inputNumber){
  int ii;
  for(ii = 0; ii < (subGridSize - 2); ii++) {
    if(xi < subGrid.at(ii + 1)){
      break;
    }
  }
  return ii;
}

bool GridInterpolator::switchCorner(std::vector<int> & corner){
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

void GridInterpolator::stackGrid(){
  offset.clear();
  offset.push_back(0);
  for(std::vector<double> subGrid : inputGrid){
    offset.push_back( offset.back() + subGrid.size() );
  }

  stackedGrid.clear();
  for(std::vector<double> subGrid: inputGrid){
    stackedGrid.insert(stackedGrid.end(), subGrid.begin(), subGrid.end());
  }
  
  std::cout << "Print offset = { ";
  for(auto & val : offset){
    std::cout << val << " ";
  }
  std::cout << "} \n";

  std::cout << "Print stackeGrid = { ";
  for(auto & val : stackedGrid){
    std::cout << val << " ";
  }
  std::cout << "} \n";
}
