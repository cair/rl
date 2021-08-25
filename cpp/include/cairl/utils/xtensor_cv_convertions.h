//
// Created by per on 8/15/21.
//

#ifndef CLASSIC_CONTROL_XTENSOR_CV_CONVERTIONS_H
#define CLASSIC_CONTROL_XTENSOR_CV_CONVERTIONS_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xadapt.hpp"

/**
 * Converts xt::xarray to cv::mat.
 *
 * First few elements are wrong (not reproducible).
 * This can be repaired by using call by reference: xt::xarray<float> xarr -> xt::xarray<float> & xarr
 */
cv::Mat xarray_to_mat(xt::xarray<float> xarr)
{
    cv::Mat mat (xarr.shape()[0], xarr.shape()[1], CV_32FC1, xarr.data(), 0);
    return mat;
}

xt::xarray<float> mat_to_xarray(cv::Mat mat)
{
    xt::xarray<float> res = xt::adapt(
            (float*) mat.data, mat.cols * mat.rows, xt::no_ownership(), std::vector<std::size_t> {static_cast<unsigned long>(mat.rows), static_cast<unsigned long>(mat.cols)});
    return res;
}

cv::Mat xarray_to_mat_elementwise(xt::xarray<float> xarr)
{
    int ndims = xarr.dimension();
    assert(ndims == 2  && "can only convert 2d xarrays");
    int nrows = xarr.shape()[0];
    int ncols = xarr.shape()[1];
    cv::Mat mat(nrows, ncols, CV_32FC1);
    for (int rr=0; rr<nrows; rr++)
    {
        for (int cc=0; cc<ncols; cc++)
        {
            mat.at<float>(rr, cc) = xarr(rr, cc);
        }
    }
    return mat;
}

xt::xarray<float> mat_to_xarray_elementwise(cv::Mat mat)
{
    int ndims = mat.dims;
    assert(ndims == 2  && "can only convert 2d xarrays");
    int nrows = mat.rows;
    int ncols = mat.cols;
    xt::xarray<float> xarr = xt::empty<float>({nrows, ncols});
    for (int rr=0; rr<nrows; rr++)
    {
        for (int cc=0; cc<ncols; cc++)
        {
            xarr(rr, cc) = mat.at<float>(rr, cc);
        }
    }
    return xarr;
}
#endif //CLASSIC_CONTROL_XTENSOR_CV_CONVERTIONS_H
