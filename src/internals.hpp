void NumericMatrix2openCVMat(Rcpp::NumericMatrix dmat, cv::Mat &odmat);
Rcpp::NumericMatrix openCVMat2NumericMatrix(cv::Mat &odmat);
void scale64ToDepth(cv::Mat &M, std::string typ);
template <typename T> void replaceNaNs(cv::Mat &M, T substitute);
int borderTypeCode(std::string borderType);