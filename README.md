# GridInterpolant
GridInterpolant is a header file only class that provides a way to linearly interpolate data on an N-D gridded data set. The class should provide an equivalent functionality to MATLAB's griddedInterpolant class or SciPy's RegularGridInterpolator class in Python. 

In MATLAB, gridded data sets can be generated and the griddedInterpolant object instantiated using the following code.
```MATLAB
% Setup an N-D grid and instantiate griddedInterpolant
x = linspace(0, 5, 6);
y = linspace(6, 10, 5);
[X, Y] = ndgrid(x,y);
Z = X.^2 + 3.*Y
F = griddedInterpolant(X, Y, Z, 'linear')
```

In Python, the equivalent gridded data set is generated and the RegularGridInterpolator object is instantiated using the code below.
```Python
# Setup an N-D grid and instantiate RegularGridInterpolator
import numpy as np
from scipy.interpolate import RegularGridInterpolator
x = np.linspace(0, 5, 6)
y = np.linspace(6, 10, 5)
X, Y = np.meshgrid(x, y, indexing='ij')
Z = X^2 + 3*Y
F = RegularGridInterpolator((x,y), Z)
```

The GridInterpolant class constructor is 
```c++
GridInterpolant(std::vector< std::vector<double> > inputGrid, std::vector<double> outputValues);
```
where an equivalent representation of the grids in Python and MATLAB above is 
```c++
#include<vector>

int main(){
  std::vector<double> x = {0, 1, 2, 3, 4, 5};
  std::vector<double> y = {6, 7, 8, 9, 10};
  std::vector<std::vector<double>> inputGrid = {x, y};
  std::vector<std::vector<double>> inputMesh = GridInterpolant::meshgrid(inputGrid);
  return 0;
}
```

The outputValues for the constructor of the GridInterpolant take the values of Z in the MATLAB and Python codes above in column major format. This can be done using the following commands in MATLAB and Python
```MATLAB
% Setup column major outputValues
outputValues = Z(:)
```

```Python
# Setup column major outputValues
outputValues = Z.ravel(order='F')
```

The handling of loading external data in C++ is outside the scope of interpolation class as there are numerous ways to achieve this. The MATLAB and Python codes above are provided to provide an understanding of GridInterpolant constructor and its use.
