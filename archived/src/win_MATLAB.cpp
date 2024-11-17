#include <win_MATLAB.h>

//The constructor locks the function
MexFunction::MexFunction(){
    mexLock();
}

//The destructor closes the window and clears the memory
MexFunction::~MexFunction(){

}

void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs){
    
}