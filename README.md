# GridInterpolant
GridInterpolant is a header file only class that provides a way to linearly interpolate data on an N-D gridded data set. The class should provide an equivalent functionality to MATLAB's griddedInterpolant class or SciPy's RegularGridInterpolator class in Python. 

In MATLAB, gridded data sets can be generated and the griddedInterpolant object instantiated using the following code.
```MATLAB
% Setup an N-D grid and instantiate griddedInterpolant
x = linspace(0, 5, 6);
y = linspace(6, 10, 5);
[X, Y] = ndgrid(x,y);
outputValues = -6*X + 3.*Y
F = griddedInterpolant(X, Y, outputValues, 'linear')
```

In Python, the equivalent gridded data set is generated and the RegularGridInterpolator object is instantiated using the code below.
```Python
# Setup an N-D grid and instantiate RegularGridInterpolator
import numpy as np
from scipy.interpolate import RegularGridInterpolator
x = np.linspace(0, 5, 6)
y = np.linspace(6, 10, 5)
X, Y = np.meshgrid(x, y, indexing='ij')
outputValues = -6*X + 3*Y
F = RegularGridInterpolator((x,y), outputValues)
```

The GridInterpolant class constructor is 
```c++
GridInterpolant(std::vector< std::vector<double> > inputGrid, std::vector<double> outputValues);
```
where an equivalent representation of the grids in Python and MATLAB in C++ above is 
```c++
#include<vector>
#include "GridInterpolant.hpp"

int main(){
  std::vector<double> x = {0, 1, 2, 3, 4, 5};
  std::vector<double> y = {6, 7, 8, 9, 10};
  std::vector<std::vector<double>> inputGrid = {x, y};
  std::vector<std::vector<double>> inputMesh = GridInterpolant::meshgrid(inputGrid2D);
  std::vector<double> X = inputMesh.at(0);
  std::vector<double> Y = inputMesh.at(1);
  std::vector<double> outputValues = {};
  for(unsigned int ii = 0; ii < X.size(); ii++){
    outputValues.push_back( -6*X.at(ii) + 3*Y.at(ii) );
  }
  GridInterpolant gridInterpolant(inputGrid2D, outputValues);
  std::vector<double> interpolatedOutput = gridInterpolant.eval({1.3, 9.3}})
  return 0;
}
```
Here, the elements of inputMesh are equivalent to X and Y except they are in column major format.

The outputValues for the constructor of the GridInterpolant take the values of Z in the MATLAB and Python codes above in column major format. This can be done using the following commands in MATLAB and Python
```MATLAB
% Setup column major outputValues
outputValues = outputValues(:)
```

```Python
# Setup column major outputValues
outputValues = outputValues.ravel(order='F')
```

## Build and Test the Examples in main.cpp
To build and test the two examples in main.cpp CMake and a compiler that supports the C++11 standard.

Once these are obtained, cd the the directory with repository is cloned and type the following
commands at the terminal.
```shell
mkdir build
cd build
cmake ..
make
./Main
```
These commands compile the examples and run them.
