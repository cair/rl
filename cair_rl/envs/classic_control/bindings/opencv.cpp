//
// Created by per on 7/26/21.
//
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>



namespace py = pybind11;

void init_cvmat(py::module& m)
{
    /*
    pybind11::class_<cv::Mat>(m, "Image", pybind11::buffer_protocol())
            .def_buffer([](cv::Mat& im) -> pybind11::buffer_info {
                return pybind11::buffer_info(
                        // Pointer to buffer
                        im.data,
                        // Size of one scalar
                        sizeof(unsigned char),
                        // Python struct-style format descriptor
                        pybind11::format_descriptor<unsigned char>::format(),
                        // Number of dimensions
                        3,
                        // Buffer dimensions
                        { im.rows, im.cols, im.channels() },
                        // Strides (in bytes) for each index
                        {
                                sizeof(unsigned char) * im.channels() * im.cols,
                                sizeof(unsigned char) * im.channels(),
                                sizeof(unsigned char)
                        }
                );
            });*/

}
