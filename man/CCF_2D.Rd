% Generated by roxygen2 (4.1.0): do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{CCF_2D}
\alias{CCF_2D}
\title{CCF_2D}
\usage{
NumericMatrix CCF_2D(NumericMatrix dmat, NumericMatrix tmplte, int x_maxlag, int y_maxlag, std::string bitdepth)
}
\arguments{
\item{dmat}{\code{NumericMatrix}. The reference (query) surface/image. See Details section.}

\item{tmplte}{\code{NumericMatrix}. The template surface/image. See Details section.}

\item{x_maxlag}{\code{int}. Minimum max-lag in the x-direction (column shift) required. If it is less than the
number of columns of the query, it's ignored.}

\item{y_maxlag}{\code{int} Minimum max-lag in the y-direction (row shift) required. If it is less than the
number of rows of the query, it's ignored.}

\item{bitdepth}{\code{std::string}. "8bit" or "32bitfloat". Bit-depth to scale surface/image down to. See
                Details section.}
}
\value{
A \code{NumericMatrix} of correlation values over shifts of the template with respect to the query.
NOTE: the correlation values are tabulated by openCV in single percision, and then converted to double percision
when they are returned to the R-side.
}
\description{
2D Cross-correlation function.
}
\details{
Sets up and calls openCV's \code{matchTemplate} with method=CV_TM_CCOEFF_NORMED (i.e. compute the
nomalized correlation coefficients). NOTE: The surface must be converted to 8- or 32-bit floating point
(bitdepth option = "8bit" or "32bitfloat") for use with \code{matchTemplate}. If the surface/image is
not converted, \code{matchTemplate} will throw an error.

The \code{matchTemplate} uses the FFT trick for speed and as such computed the CCF over all possible forward
(right) lags of the template surface/image over the M by N reference (query) surface/image. \code{matchTemplate} assumes
the P by Q template fits inside the querry. If it does not, \code{CCF_2D} pads the query with zeros (M<P or N<Q) or switches who
is considered the "template" and who is considered the "query" (M<P and N<Q).

In order to implement correlation over shifts of the template that go over the edges of the query, the query is
padded with zeros by amounts to accomidate the template overhang for these shifts.
}
\examples{
XXXX
}
\references{
J.P. Lewis, Fast Template Matching, Vision Interface 95, Canadian Image Processing
and Pattern Recognition Society, Quebec City, Canada, May 15-19, 1995, p.  120-123.
}

