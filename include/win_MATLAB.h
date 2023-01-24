/* win_MATLAB
* win_MATLAB(0, 0); Initialize the window with BLACK
* win_MATLAB(0, 1); Initialize the window with WHITE
* win_MATLAB(0, pattern); Initialize the window with stored pattern (a filename string)
* win_MATLAB(1, tweezer_loc, tweezer_pat); Project static tweezers at tweezer_loc with stored tweezer_pat
* win_MATLAB(2, tweezer_loc, tweezer_cal, tweezer_pat); 
*/

#include "mex.hpp"
#include "mexAdapter.hpp"
#include <win_lib.h>

class MexFunction : public matlab::mex::Function {
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    matlab::data::ArrayFactory factory;
public:
    MexFunction();
    ~MexFunction();
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);
};